// Open-Meteo forecast request, per docs/design/01-data-protocol.md.
//
// One coordinate returns a JSON object. Several coordinates (comma-separated
// latitude and longitude, same length) return a JSON array in an unspecified
// order — callers match on coordinates. Each coordinate counts as one API call.

function buildUrlForCoords(coords) {
  var lats = [];
  var lons = [];
  for (var i = 0; i < coords.length; i++) {
    lats.push(coords[i].lat);
    lons.push(coords[i].lon);
  }
  var params = [
    'latitude=' + lats.join(','),
    'longitude=' + lons.join(','),
    'current=temperature_2m,apparent_temperature,relative_humidity_2m,is_day,' +
      'weather_code,surface_pressure,pressure_msl,wind_speed_10m,wind_direction_10m',
    'hourly=pressure_msl,temperature_2m,weather_code',
    'daily=temperature_2m_max,temperature_2m_min,sunrise,sunset,weather_code',
    'past_hours=24',
    'forecast_hours=12',
    'past_days=1',
    'forecast_days=4',
    'timezone=auto',
    'timeformat=unixtime',
  ];
  return 'https://api.open-meteo.com/v1/forecast?' + params.join('&');
}

function buildUrl(lat, lon) {
  return buildUrlForCoords([{ lat: lat, lon: lon }]);
}

// A single-location response is an object; a multi-location response is an
// array. Both become an array. null means the body wasn't a forecast.
function normalizeResults(parsed) {
  if (Array.isArray(parsed)) return parsed;
  if (parsed && typeof parsed === 'object') return [parsed];
  return null;
}

// onError receives one of the ERR_CODE values from the data protocol doc:
// 3 = HTTP/network failure, 4 = bad response (parse failure).
// onSuccess receives an array of forecast objects, one per coordinate.
function fetchForecasts(coords, onSuccess, onError) {
  // One HTTP request. Open-Meteo's free tier counts each coordinate.
  console.log('AetherCast: Open-Meteo ' + coords.length + ' location(s), ' +
              coords.length + ' API call(s)');
  var xhr = new XMLHttpRequest();
  xhr.timeout = 15000;
  xhr.onload = function () {
    if (xhr.status < 200 || xhr.status >= 300) {
      onError(3);
      return;
    }
    try {
      var parsed = JSON.parse(xhr.responseText);
      if (parsed && parsed.error && !Array.isArray(parsed)) {
        onError(4);
        return;
      }
      var results = normalizeResults(parsed);
      if (!results) {
        onError(4);
        return;
      }
      onSuccess(results);
    } catch (e) {
      onError(4);
    }
  };
  xhr.onerror = function () { onError(3); };
  xhr.ontimeout = function () { onError(3); };
  xhr.open('GET', buildUrlForCoords(coords));
  xhr.send();
}

function fetchForecast(lat, lon, onSuccess, onError) {
  fetchForecasts([{ lat: lat, lon: lon }], function (results) {
    onSuccess(results[0]);
  }, onError);
}

module.exports = {
  buildUrl: buildUrl,
  buildUrlForCoords: buildUrlForCoords,
  normalizeResults: normalizeResults,
  fetchForecast: fetchForecast,
  fetchForecasts: fetchForecasts,
};
