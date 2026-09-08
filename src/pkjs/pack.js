// Open-Meteo JSON -> AppMessage wire dict, per docs/design/01-data-protocol.md.

var MessageKeys = require('message_keys');

var SCHEMA_VERSION = 3; // bumped for the temperature/outlook forecast fields
var HOURLY_SAMPLES = 36;
var NOW_IDX = 24; // past_hours=24 -> sample 24 is the first forecast hour
var OUTLOOK_DAYS = 4;

function locNameFromTimezone(tz) {
  var parts = String(tz).split('/');
  var name = parts[parts.length - 1].replace(/_/g, ' ').toUpperCase();
  return name.slice(0, 23); // LOC_NAME wire limit is 24 bytes incl. NUL
}

// Open-Meteo's response shape isn't schema-validated by anything upstream of
// this module, so any numeric field could in principle be missing/non-numeric
// (malformed/unexpected server response). Throw rather than let a NaN/undefined
// silently ride the wire - packPayload's try/catch turns that into ERR_CODE 4.
function num(value) {
  var n = Number(value);
  if (!isFinite(n)) {
    throw new Error('non-finite numeric field: ' + value);
  }
  return n;
}

// Forward-fill nulls; back-fill from the first non-null if index 0 is null.
// Returns null if the entire series is null/missing (caller sends ERR_CODE 4).
function fillNulls(arr) {
  if (!Array.isArray(arr)) return null;
  var out = arr.slice();
  var firstNonNull = -1;
  for (var i = 0; i < out.length; i++) {
    if (out[i] !== null && out[i] !== undefined) {
      firstNonNull = i;
      break;
    }
  }
  if (firstNonNull === -1) return null;
  out[firstNonNull] = num(out[firstNonNull]);
  for (var i = 0; i < firstNonNull; i++) out[i] = out[firstNonNull];
  for (var i = 1; i < out.length; i++) {
    out[i] = (out[i] === null || out[i] === undefined) ? out[i - 1] : num(out[i]);
  }
  return out;
}

// Pad (repeat last sample) or truncate to exactly HOURLY_SAMPLES so the C
// side never has to handle a variable-length series.
function normalizeLength(arr) {
  var out = arr.slice(0, HOURLY_SAMPLES);
  while (out.length < HOURLY_SAMPLES) out.push(out[out.length - 1]);
  return out;
}

// N x int16 little-endian, values pre-multiplied by `scale` -> byte array.
function packInt16Series(values, scale, count) {
  var buffer = new ArrayBuffer(count * 2);
  var view = new DataView(buffer);
  for (var i = 0; i < count; i++) {
    view.setInt16(i * 2, Math.round(values[i] * scale), true);
  }
  return Array.prototype.slice.call(new Uint8Array(buffer));
}

// N x uint8 -> plain byte array (weather codes top out at 99, so no scaling).
function packUint8Series(values, count) {
  var out = new Array(count);
  for (var i = 0; i < count; i++) {
    out[i] = Math.round(values[i]) & 0xff;
  }
  return out;
}

// daily arrays hold [yesterday, today, ...forecast_days] (past_days=1); find
// "today" by local-time window rather than assuming a fixed index.
function pickTodayIndex(daily, nowUtc, utcOffsetSeconds) {
  var localNow = nowUtc + utcOffsetSeconds;
  for (var i = 0; i < daily.time.length; i++) {
    if (localNow >= daily.time[i] && localNow < daily.time[i] + 86400) return i;
  }
  return Math.min(1, daily.time.length - 1);
}

// Slice OUTLOOK_DAYS entries starting at todayIdx, padding by repeating the
// last entry if the daily arrays are shorter than requested.
function sliceOutlook(arr, todayIdx) {
  var out = [];
  for (var i = 0; i < OUTLOOK_DAYS; i++) {
    var idx = Math.min(todayIdx + i, arr.length - 1);
    out.push(num(arr[idx]));
  }
  return out;
}

function packError(errCode) {
  var dict = {};
  dict[MessageKeys.MSG_TYPE] = 2;
  dict[MessageKeys.ERR_CODE] = errCode;
  return dict;
}

function packPayload(json, nowUtc, unitSystem) {
  try {
    return buildPayloadDict(json, nowUtc, unitSystem);
  } catch (e) {
    console.log('AetherCast: malformed Open-Meteo response: ' + e.message);
    return packError(4);
  }
}

function buildPayloadDict(json, nowUtc, unitSystem) {
  var pressure = fillNulls(json.hourly.pressure_msl);
  if (!pressure) return packError(4);
  pressure = normalizeLength(pressure);

  var temperature = fillNulls(json.hourly.temperature_2m);
  if (!temperature) return packError(4);
  temperature = normalizeLength(temperature);

  var wxHourly = fillNulls(json.hourly.weather_code);
  if (!wxHourly) return packError(4);
  wxHourly = normalizeLength(wxHourly);

  if (!Array.isArray(json.daily.time) || json.daily.time.length === 0) {
    return packError(4);
  }
  var todayIdx = pickTodayIndex(json.daily, nowUtc, num(json.utc_offset_seconds));

  var dict = {};
  dict[MessageKeys.MSG_TYPE] = 1;
  dict[MessageKeys.SCHEMA] = SCHEMA_VERSION;
  dict[MessageKeys.UNIT_SYSTEM] = unitSystem === 1 ? 1 : 0;
  dict[MessageKeys.TEMP_C10] = Math.round(num(json.current.temperature_2m) * 10);
  dict[MessageKeys.FEELS_C10] = Math.round(num(json.current.apparent_temperature) * 10);
  dict[MessageKeys.HI_C10] = Math.round(num(json.daily.temperature_2m_max[todayIdx]) * 10);
  dict[MessageKeys.LO_C10] = Math.round(num(json.daily.temperature_2m_min[todayIdx]) * 10);
  dict[MessageKeys.HUMIDITY] = Math.round(num(json.current.relative_humidity_2m));
  dict[MessageKeys.WIND_KMH10] = Math.round(num(json.current.wind_speed_10m) * 10);
  dict[MessageKeys.WIND_DIR] = Math.round(num(json.current.wind_direction_10m));
  dict[MessageKeys.WX_CODE] = Math.round(num(json.current.weather_code));
  dict[MessageKeys.IS_DAY] = Math.round(num(json.current.is_day));
  dict[MessageKeys.PRESS_HPA10] = Math.round(num(json.current.pressure_msl) * 10);
  dict[MessageKeys.PRESS_SERIES] = packInt16Series(pressure, 10, HOURLY_SAMPLES);
  dict[MessageKeys.PRESS_NOW_IDX] = NOW_IDX;
  dict[MessageKeys.PRESS_T0_UTC] = Math.round(num(json.hourly.time[0]));
  dict[MessageKeys.TEMP_SERIES] = packInt16Series(temperature, 10, HOURLY_SAMPLES);
  dict[MessageKeys.WX_SERIES] = packUint8Series(wxHourly, HOURLY_SAMPLES);
  dict[MessageKeys.DAILY_HI] = packInt16Series(sliceOutlook(json.daily.temperature_2m_max, todayIdx), 10, OUTLOOK_DAYS);
  dict[MessageKeys.DAILY_LO] = packInt16Series(sliceOutlook(json.daily.temperature_2m_min, todayIdx), 10, OUTLOOK_DAYS);
  dict[MessageKeys.DAILY_CODE] = packUint8Series(sliceOutlook(json.daily.weather_code, todayIdx), OUTLOOK_DAYS);
  dict[MessageKeys.DAILY_T0_UTC] = Math.round(num(json.daily.time[todayIdx]));
  dict[MessageKeys.SUNRISE_UTC] = Math.round(num(json.daily.sunrise[todayIdx]));
  dict[MessageKeys.SUNSET_UTC] = Math.round(num(json.daily.sunset[todayIdx]));
  dict[MessageKeys.UPDATED_UTC] = nowUtc;
  dict[MessageKeys.LOC_NAME] = locNameFromTimezone(json.timezone);
  dict[MessageKeys.LAT_SIGN] = num(json.latitude) < 0 ? -1 : 1;
  return dict;
}

module.exports = {
  packPayload: packPayload,
  packError: packError,
  SCHEMA_VERSION: SCHEMA_VERSION,
};
