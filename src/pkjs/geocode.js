// Place name for the watch header. Open-Meteo's timezone is a representative
// city for the whole zone (America/Chicago covers Nebraska), so it cannot
// label where the forecast coordinates actually are.

var GEOCODE_TIMEOUT_MS = 8000;

function buildUrl(lat, lon) {
  return 'https://api.bigdatacloud.net/data/reverse-geocode-client?latitude=' +
    encodeURIComponent(lat) + '&longitude=' + encodeURIComponent(lon) +
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
  buildUrl: buildUrl,
  placeFromResponse: placeFromResponse,
  reverseCity: reverseCity,
};
