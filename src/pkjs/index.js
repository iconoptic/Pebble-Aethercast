// PebbleKit JS entry point: wait for REQUEST from the watch, geolocate,
// fetch Open-Meteo, pack, send back. See docs/design/01-data-protocol.md.

var MessageKeys = require('message_keys');
var openmeteo = require('./openmeteo');
var pack = require('./pack');
var Clay = require('@rebble/clay');
var clayConfig = require('./config');

// autoHandleEvents: false - we decide which settings actually reach the
// watch (only UNIT_SYSTEM, via a normal refresh) and keep the manual
// location override JS-only. See src/pkjs/config.js and PLAN.md's Clay risk
// mitigation.
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

// Dev-only: set to one of tools/fake_payload.js's PRESETS ('rising',
// 'falling', 'flat', 'sawtooth', 'missing') to exercise the barograph
// without geolocation/network, per PLAN.md's M4 exit criteria. Leave null
// for normal operation - never ship a build with this set.
var FAKE_PAYLOAD_PRESET = null;
var fakePayload = FAKE_PAYLOAD_PRESET ? require('../../tools/fake_payload') : null;
// Dev-only, paired with FAKE_PAYLOAD_PRESET: overrides weather_code/is_day
// for spot-checking icon_layer.c's WMO buckets. Leave both null normally.
var FAKE_WX_CODE = null;
var FAKE_IS_DAY = null;

var LOCATION_OPTIONS = { enableHighAccuracy: false, maximumAge: 60000, timeout: 15000 };
// Belt-and-suspenders past LOCATION_OPTIONS.timeout: some real-device
// PebbleKit JS geolocation implementations don't reliably honour the native
// timeout option and can hang indefinitely, leaving the watch's own 30s
// app_message watchdog to time out with an uninformative "NO PHONE" instead
// of a real ERR_CODE. This fires first and wins if the native call never
// calls back at all.
var GEOLOCATION_GUARD_MS = 20000;
var UNITS_IMPERIAL = 0;

// Clay persists its flattened settings to localStorage under this key
// regardless of our autoHandleEvents choice - see getSettings() in
// node_modules/@rebble/clay/index.js.
function loadSettings() {
  var raw = {};
  try {
    raw = JSON.parse(localStorage.getItem('clay-settings')) || {};
  } catch (e) {
    raw = {};
  }
  return {
    unitSystem: raw.UNIT_SYSTEM === 1 ? 1 : UNITS_IMPERIAL,
    manualLocationEnabled: !!raw.manualLocationEnabled,
    manualLat: parseFloat(raw.manualLat),
    manualLon: parseFloat(raw.manualLon)
  };
}

function sendDict(dict) {
  Pebble.sendAppMessage(dict, function () {
    console.log('AetherCast: sent ' + JSON.stringify(dict));
  }, function (e) {
    console.log('AetherCast: send failed: ' + JSON.stringify(e));
  });
}

// Geolocation's own error.code (PERMISSION_DENIED/POSITION_UNAVAILABLE/TIMEOUT)
// is a different scale than our ERR_CODE wire values, so map explicitly.
function mapGeoError(err) {
  return err.code === err.PERMISSION_DENIED ? 1 /* Location permission denied */
                                             : 2 /* Location timeout / no fix */;
}

function fetchAndSend() {
  var settings = loadSettings();

  if (fakePayload) {
    var nowUtc = Math.floor(Date.now() / 1000);
    var opts = {};
    if (FAKE_WX_CODE !== null) opts.wxCode = FAKE_WX_CODE;
    if (FAKE_IS_DAY !== null) opts.isDay = FAKE_IS_DAY;
    sendDict(pack.packPayload(fakePayload.buildFakeJson(FAKE_PAYLOAD_PRESET, nowUtc, opts), nowUtc,
                               settings.unitSystem));
    return;
  }

  function onCoords(lat, lon) {
    openmeteo.fetchForecast(lat, lon, function (json) {
      sendDict(pack.packPayload(json, Math.floor(Date.now() / 1000), settings.unitSystem));
    }, function (errCode) {
      sendDict(pack.packError(errCode));
    });
  }

  // Clay manual location override bypasses navigator.geolocation entirely -
  // per PLAN.md's mitigation for "Geolocation permission denied".
  if (settings.manualLocationEnabled && isFinite(settings.manualLat) && isFinite(settings.manualLon)) {
    onCoords(settings.manualLat, settings.manualLon);
    return;
  }

  var settled = false;
  var guard = setTimeout(function () {
    settled = true;
    console.log('AetherCast: geolocation guard fired, native timeout never fired');
    sendDict(pack.packError(2 /* no fix */));
  }, GEOLOCATION_GUARD_MS);

  navigator.geolocation.getCurrentPosition(function (pos) {
    if (settled) return;
    settled = true;
    clearTimeout(guard);
    onCoords(pos.coords.latitude, pos.coords.longitude);
  }, function (err) {
    if (settled) return;
    settled = true;
    clearTimeout(guard);
    sendDict(pack.packError(mapGeoError(err)));
  }, LOCATION_OPTIONS);
}

Pebble.addEventListener('ready', function () {
  console.log('AetherCast PKJS ready');
  // Tell the watch it's now safe to send us AppMessages - see
  // docs/vendor/pebble-comm-advanced.md "Waiting for PebbleKit JS".
  var dict = {};
  dict[MessageKeys.PKJS_READY] = 1;
  sendDict(dict);
});

Pebble.addEventListener('appmessage', function (e) {
  // e.payload is keyed by the symbolic name ("REQUEST"), not the numeric
  // MessageKeys.REQUEST id - indexing by the numeric id silently missed
  // every real REQUEST (confirmed via a real-device log showing
  // e.payload == {"REQUEST":1}), leaving the watch's 30s watchdog to fire
  // with a misleading "NO PHONE" every time.
  if (e.payload.REQUEST === 1) {
    fetchAndSend();
  }
});

Pebble.addEventListener('showConfiguration', function () {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function (e) {
  if (!e || !e.response) {
    return;
  }
  // convert: false - skip Clay's AppMessage conversion (which requires every
  // settings key to be a registered watch message key); getSettings() still
  // persists the flattened values to localStorage as a side effect.
  clay.getSettings(e.response, false);
  // Push the updated UNIT_SYSTEM (and/or manual location) to the watch now,
  // rather than waiting for its next scheduled refresh.
  fetchAndSend();
});
