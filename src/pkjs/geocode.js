// Place name for the watch header. Open-Meteo's timezone is a representative
// city for the whole zone (America/Chicago covers Nebraska), so it cannot
// label where the forecast coordinates actually are.
//
// BigDataCloud's free reverse-geocode-client endpoint is client-side only and
// may only receive the calling device's current location. See
// docs/vendor/bigdatacloud-reverse-geocode-client.md. Manual coordinates must
// never hit this API.

var GEOCODE_TIMEOUT_MS = 8000;
// After the forecast lands, wait at most this long for a name before sending.
var NAME_WAIT_MS = 1500;
// Reuse a cached name when the new fix is within this distance.
var CACHE_KM = 2;
var CACHE_KEY = 'aethercast-place-name';
// Town names do not need more than ~1 km precision.
var COORD_DECIMALS = 2;

function roundCoord(v) {
  var step = Math.pow(10, COORD_DECIMALS);
  return Math.round(Number(v) * step) / step;
}

function distanceKm(lat1, lon1, lat2, lon2) {
  var r = 6371;
  var dLat = (lat2 - lat1) * Math.PI / 180;
  var dLon = (lon2 - lon1) * Math.PI / 180;
  var a = Math.sin(dLat / 2) * Math.sin(dLat / 2) +
    Math.cos(lat1 * Math.PI / 180) * Math.cos(lat2 * Math.PI / 180) *
    Math.sin(dLon / 2) * Math.sin(dLon / 2);
  return 2 * r * Math.asin(Math.min(1, Math.sqrt(a)));
}

function buildUrl(lat, lon) {
  var rLat = roundCoord(lat);
  var rLon = roundCoord(lon);
  return 'https://api.bigdatacloud.net/data/reverse-geocode-client?latitude=' +
    encodeURIComponent(rLat) + '&longitude=' + encodeURIComponent(rLon) +
    '&localityLanguage=en';
}

// Prefer the populated place, then the finer locality, then the state.
// Returns the API's spelling, or null when nothing usable came back.
function placeFromResponse(json) {
  if (!json || typeof json !== 'object') return null;
  var fields = [json.city, json.locality, json.principalSubdivision];
  for (var i = 0; i < fields.length; i++) {
    if (fields[i] == null) continue;
    var raw = String(fields[i]).trim();
    if (raw) return raw;
  }
  return null;
}

function loadCache(storage) {
  try {
    var raw = storage.getItem(CACHE_KEY);
    if (!raw) return null;
    var data = JSON.parse(raw);
    if (!data || typeof data !== 'object') return null;
    if (!isFinite(data.lat) || !isFinite(data.lon)) return null;
    if (data.name == null || !String(data.name).trim()) return null;
    return {
      lat: Number(data.lat),
      lon: Number(data.lon),
      name: String(data.name).trim(),
      t: isFinite(data.t) ? Number(data.t) : 0
    };
  } catch (e) {
    return null;
  }
}

// Quota errors and a disabled localStorage must not escape into the
// geolocation callback: the watch would then wait out its 30 s watchdog
// and show "NO PHONE" with a good forecast sitting unused.
function saveCache(storage, lat, lon, name, t) {
  if (name == null || !String(name).trim()) return;
  try {
    storage.setItem(CACHE_KEY, JSON.stringify({
      lat: roundCoord(lat),
      lon: roundCoord(lon),
      name: String(name).trim(),
      t: t || 0
    }));
  } catch (e) {
    console.log('AetherCast: could not store place name');
  }
}

// Pure: cached name if the fix is within CACHE_KM of the stored point.
function nameFromCache(cache, lat, lon) {
  if (!cache || !cache.name) return null;
  if (!isFinite(cache.lat) || !isFinite(cache.lon)) return null;
  if (!isFinite(lat) || !isFinite(lon)) return null;
  if (distanceKm(cache.lat, cache.lon, lat, lon) <= CACHE_KM) return cache.name;
  return null;
}

// Pure decision after the forecast has arrived.
// - geocodePending: we started a lookup that has not finished
// - placeName: name already in hand (cache hit or early geocode)
// - waitedMs: time already spent waiting after the forecast landed
// Returns 'send' or 'wait'.
function decideAfterForecast(placeName, geocodePending, waitedMs, maxWaitMs) {
  if (!geocodePending) return 'send';
  if (placeName != null && String(placeName).trim()) return 'send';
  if (waitedMs >= maxWaitMs) return 'send';
  return 'wait';
}

// onName is always called once with a string or null. A miss must not fail
// the forecast; pack.js falls back to the timezone city.
function reverseCity(lat, lon, onName) {
  var xhr = new XMLHttpRequest();
  var done = false;
  function finish(name) {
    if (done) return;
    done = true;
    onName(name);
  }
  xhr.timeout = GEOCODE_TIMEOUT_MS;
  xhr.onload = function () {
    if (xhr.status < 200 || xhr.status >= 300) {
      finish(null);
      return;
    }
    try {
      finish(placeFromResponse(JSON.parse(xhr.responseText)));
    } catch (e) {
      finish(null);
    }
  };
  xhr.onerror = function () { finish(null); };
  xhr.ontimeout = function () { finish(null); };
  xhr.open('GET', buildUrl(lat, lon));
  xhr.send();
}

module.exports = {
  GEOCODE_TIMEOUT_MS: GEOCODE_TIMEOUT_MS,
  NAME_WAIT_MS: NAME_WAIT_MS,
  CACHE_KM: CACHE_KM,
  CACHE_KEY: CACHE_KEY,
  roundCoord: roundCoord,
  distanceKm: distanceKm,
  buildUrl: buildUrl,
  placeFromResponse: placeFromResponse,
  loadCache: loadCache,
  saveCache: saveCache,
  nameFromCache: nameFromCache,
  decideAfterForecast: decideAfterForecast,
  reverseCity: reverseCity,
};
