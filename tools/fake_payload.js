// Synthetic Open-Meteo-shaped pressure series for exercising every barograph
// render branch without a live network fetch. See docs/design/03-barograph.md
// and PLAN.md's M4 exit criteria.
//
// Dev usage from src/pkjs/index.js: set FAKE_PAYLOAD_PRESET to one of
// PRESETS below to short-circuit the real fetch with one of these series.
// 'trail' is a Denver → Kansas City drive over the 10 h before now, with
// the place-change bit set so the watch draws a tick at the step.
//
// Standalone: NODE_PATH="$(pwd)/build/js" node tools/fake_payload.js <preset>
// prints the packed wire dict (via src/pkjs/pack.js) as JSON. NODE_PATH must
// point at the `pebble build` output so pack.js's require('message_keys')
// resolves outside the webpack/Pebble-SDK build (run `pebble build` first).

var HOURLY_SAMPLES = 36;
var NOW_IDX = 24; // past_hours=24 -> sample 24 is the first forecast hour

function series_flat() {
  var out = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) out.push(1013.0);
  return out;
}

function series_rising() {
  // ~17.5 hPa climb across the full span -> RISING FAST at now.
  var out = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) out.push(996 + i * 0.5);
  return out;
}

function series_falling() {
  // Mirror of rising -> FALLING FAST at now.
  var out = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) out.push(1022 - i * 0.5);
  return out;
}

function series_sawtooth() {
  // ~20 hPa swings, well beyond MIN_SPAN, exercises the 10% padding path
  // and the "curve never clips the plot rect" assertion.
  var out = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) {
    var phase = i % 12;
    out.push(1000 + (phase < 6 ? phase : 12 - phase) * 4);
  }
  return out;
}

function series_missing() {
  // A forward-filled gap (hours 10-15 held flat) inside otherwise normal
  // variation - what pack.js's fillNulls() already produces upstream.
  var out = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) out.push(1008 + Math.sin(i / 6) * 3);
  for (var i = 10; i <= 15; i++) out[i] = out[9];
  return out;
}

// Change at index 14, which is 10 h before the current hour (index 24).
// Denver sits near 1006 hPa; Kansas City steps up to ~1018 and eases off.
// The 3 h window (indices 21-24) is entirely Kansas City, so the trend word
// stays STEADY instead of reporting the step as a front. Bit 14 of the
// place-change mask is byte 1, bit 6 (1 << 6 = 64).
var TRAIL_CHANGE_IDX = 14;

function series_trail() {
  var out = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) {
    if (i < TRAIL_CHANGE_IDX) out.push(1006.0 + i * 0.04);
    else out.push(1018.0 - (i - TRAIL_CHANGE_IDX) * 0.06);
  }
  return out;
}

var PRESETS = {
  rising: series_rising,
  falling: series_falling,
  flat: series_flat,
  sawtooth: series_sawtooth,
  missing: series_missing,
  trail: series_trail,
};

// Builds an Open-Meteo `/v1/forecast`-shaped JSON object carrying one of the
// PRESETS in `hourly.pressure_msl`, so it can be handed straight to
// src/pkjs/pack.js's packPayload() unchanged. `opts.wxCode`/`opts.isDay`
// override the default weather_code=2 (partly cloudy) / is_day=1, for
// spot-checking icon_layer.c's WMO buckets (docs/design/01-data-protocol.md).
function buildFakeJson(presetName, nowUtc, opts) {
  opts = opts || {};
  var gen = PRESETS[presetName];
  if (!gen) {
    throw new Error('unknown fake payload preset "' + presetName + '" (want one of ' +
                     Object.keys(PRESETS).join(', ') + ')');
  }
  var pressure = gen();
  var t0 = nowUtc - NOW_IDX * 3600;
  var hourlyTime = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) hourlyTime.push(t0 + i * 3600);

  var dayStart = nowUtc - (nowUtc % 86400);
  // yesterday, today, +1, +2, +3 - matches forecast_days=4 (past_days=1) so
  // OUTLOOK_DAYS=4 in pack.js never has to pad by repeating the last entry.
  var dayOffsets = [-1, 0, 1, 2, 3];
  var dailyTime = dayOffsets.map(function (d) { return dayStart + d * 86400; });
  var dailyHi = [22.0, 23.0, 21.0, 24.0, 20.0];
  var dailyLo = [12.0, 13.0, 11.0, 14.0, 10.0];
  var dailyCode = [2, 3, 61, 2, 1];
  var sunrise = dailyTime.map(function (t) { return t + 6 * 3600; });
  var sunset = dailyTime.map(function (t) { return t + 19 * 3600; });

  var temperature = [];
  var wxHourly = [];
  for (var i = 0; i < HOURLY_SAMPLES; i++) {
    temperature.push(18.0 + Math.sin(i / 6) * 4);
    wxHourly.push(opts.wxCode !== undefined ? opts.wxCode : 2);
  }

  var json = {
    latitude: 42.33,
    longitude: -83.05,
    timezone: 'America/Detroit',
    utc_offset_seconds: -14400,
    current: {
      temperature_2m: 21.0,
      apparent_temperature: 20.0,
      relative_humidity_2m: 55,
      wind_speed_10m: 12.0,
      wind_direction_10m: 270,
      weather_code: opts.wxCode !== undefined ? opts.wxCode : 2,
      is_day: opts.isDay !== undefined ? opts.isDay : 1,
      pressure_msl: pressure[NOW_IDX],
    },
    daily: {
      time: dailyTime,
      temperature_2m_max: dailyHi,
      temperature_2m_min: dailyLo,
      weather_code: dailyCode,
      sunrise: sunrise,
      sunset: sunset,
    },
    hourly: {
      time: hourlyTime,
      pressure_msl: pressure,
      temperature_2m: temperature,
      weather_code: wxHourly,
    },
  };
  if (presetName === 'trail') {
    json.timezone = 'America/Chicago';
    json.latitude = 39.1;
    json.longitude = -94.58;
    // Bit 14: the Denver → Kansas City step. See series_trail.
    json.place_change = [0, 64, 0, 0, 0];
  }
  return json;
}

module.exports = {
  PRESETS: Object.keys(PRESETS),
  seriesFor: function (name) {
    if (!PRESETS[name]) throw new Error('unknown preset "' + name + '"');
    return PRESETS[name]();
  },
  buildFakeJson: buildFakeJson,
};

if (require.main === module) {
  var preset = process.argv[2];
  if (!preset || !PRESETS[preset]) {
    console.error('usage: node tools/fake_payload.js <' + Object.keys(PRESETS).join('|') + '> [wxCode] [isDay]');
    process.exit(1);
  }
  var pack = require('../src/pkjs/pack');
  var nowUtc = Math.floor(Date.now() / 1000);
  var opts = {};
  if (process.argv[3] !== undefined) opts.wxCode = parseInt(process.argv[3], 10);
  if (process.argv[4] !== undefined) opts.isDay = parseInt(process.argv[4], 10);
  console.log(JSON.stringify(pack.packPayload(buildFakeJson(preset, nowUtc, opts), nowUtc), null, 2));
}
