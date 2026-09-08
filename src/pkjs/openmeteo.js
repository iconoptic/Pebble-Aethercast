// Open-Meteo forecast request, per docs/design/01-data-protocol.md.

function buildUrl(lat, lon) {
  var params = [
    'latitude=' + lat,
    'longitude=' + lon,
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

// onError receives one of the ERR_CODE values from the data protocol doc:
// 3 = HTTP/network failure, 4 = bad response (parse failure).
function fetchForecast(lat, lon, onSuccess, onError) {
  var xhr = new XMLHttpRequest();
  xhr.timeout = 15000;
  xhr.onload = function () {
    if (xhr.status < 200 || xhr.status >= 300) {
      onError(3);
      return;
    }
    try {
      onSuccess(JSON.parse(xhr.responseText));
    } catch (e) {
      onError(4);
    }
  };
  xhr.onerror = function () { onError(3); };
  xhr.ontimeout = function () { onError(3); };
  xhr.open('GET', buildUrl(lat, lon));
  xhr.send();
}

module.exports = {
  buildUrl: buildUrl,
  fetchForecast: fetchForecast,
};
