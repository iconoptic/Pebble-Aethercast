// PebbleKit JS entry point: wait for REQUEST from the watch, geolocate,
// fetch Open-Meteo, pack, send back. See docs/design/01-data-protocol.md.
//
// With "Pressure history: Follow me" (the default) each GPS fix is appended
// to a phone-local trail and the past 24 h of PRESS_SERIES is stitched from
// the places on that trail. The forecast half stays the current location.
// Manual location and "This location only" take the original single-place path.

var MessageKeys = require('message_keys');
var openmeteo = require('./openmeteo');
var pack = require('./pack');
var trail = require('./trail');
var Clay = require('@rebble/clay');
var clayConfig = require('./config');

// Runs inside the Clay config page, not in PKJS. The clear button has no
// messageKey (it must not stick in clay-settings); this closes the page
// with a one-shot flag that webviewclosed handles, alongside whatever else
// the user had set.
function clayCustom() {
  var clayConfig = this;
  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function () {
    var button = clayConfig.getItemById('clearLocationHistory');
    if (!button) {
      return;
    }
    button.on('click', function () {
      var settings = clayConfig.serialize();
      settings.clearLocationHistory = { value: true };
      var returnTo = (typeof window !== 'undefined' && window.returnTo)
        ? window.returnTo
        : 'pebblejs://close#';
      window.location.href = returnTo + encodeURIComponent(JSON.stringify(settings));
    });
  });
}

// autoHandleEvents: false - we decide which settings actually reach the
// watch (only UNIT_SYSTEM, via a normal refresh) and keep the manual
// location override and the pressure-history trail JS-only. See
// src/pkjs/config.js and PLAN.md's Clay risk mitigation.
var clay = new Clay(clayConfig, clayCustom, { autoHandleEvents: false });
var trailStore = trail.createStore(localStorage);

// Dev-only: set to one of tools/fake_payload.js's PRESETS ('rising',
// 'falling', 'flat', 'sawtooth', 'missing', 'trail') to exercise the
// barograph without geolocation/network, per PLAN.md's M4 exit criteria.
// 'trail' is Denver → Kansas City over 10 h, with place-change ticks.
// Leave null for normal operation - never ship a build with this set.
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
    manualLon: parseFloat(raw.manualLon),
    // 0 = follow me (default), 1 = this location only.
    pressureHistory: raw.pressureHistory === 1 ? 1 : 0,
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

function sendForecast(json, settings, opts) {
  sendDict(pack.packPayload(json, Math.floor(Date.now() / 1000), settings.unitSystem, opts));
}

function fetchSingle(lat, lon, settings) {
  openmeteo.fetchForecast(lat, lon, function (json) {
    sendForecast(json, settings);
  }, function (errCode) {
    sendDict(pack.packError(errCode));
  });
}

// The multi-place body could not be stitched. Send the current location's
// forecast with no pressure/placeChange/delta3Tenths opts — the same dict
// fetchSingle would build — and do not write a stitched cache. A second
// HTTP request happens only when that object is not already in hand.
function sendTrailFallback(plan, results, lat, lon, settings) {
  var current = trail.fallbackCurrent(plan, results);
  if (current) {
    console.log('AetherCast: trail stitch failed, sending the current location only');
    sendForecast(current, settings);
    return;
  }
  console.log('AetherCast: trail stitch failed and the current result was unusable');
  fetchSingle(lat, lon, settings);
}

function fetchTrail(lat, lon, settings) {
  var now = Math.floor(Date.now() / 1000);
  var plan;
  try {
    // recordFix returns the in-memory trail even when localStorage refuses
    // the write, so this refresh still names the place we are in.
    var places = trailStore.recordFix(lat, lon, now);
    plan = trail.planFetch(places, { lat: lat, lon: lon }, now, trailStore.loadCache());
  } catch (e) {
    console.log('AetherCast: trail plan failed: ' + (e && e.message ? e.message : e));
    fetchSingle(lat, lon, settings);
    return;
  }
  console.log('AetherCast: trail places=' + plan.places.length +
              ' distinct=' + plan.coords.length);
  openmeteo.fetchForecasts(plan.coords, function (results) {
    try {
      var integrated = trail.integrate(plan, results, trailStore.loadCache());
      if (!integrated || integrated.error) {
        sendTrailFallback(plan, results, lat, lon, settings);
        return;
      }
      trailStore.saveCache(integrated.cache);
      var opts = { pressure: integrated.pressure };
      if (integrated.placeChange) opts.placeChange = integrated.placeChange;
      if (integrated.delta3Tenths !== null && integrated.delta3Tenths !== undefined) {
        opts.delta3Tenths = integrated.delta3Tenths;
      }
      sendForecast(integrated.currentJson, settings, opts);
    } catch (e) {
      console.log('AetherCast: trail stitch failed: ' + (e && e.message ? e.message : e));
      sendTrailFallback(plan, results, lat, lon, settings);
    }
  }, function (errCode) {
    sendDict(pack.packError(errCode));
  });
}

function fetchAndSend() {
  var settings = loadSettings();

  if (fakePayload) {
    var nowUtc = Math.floor(Date.now() / 1000);
    var opts = {};
    if (FAKE_WX_CODE !== null) opts.wxCode = FAKE_WX_CODE;
    if (FAKE_IS_DAY !== null) opts.isDay = FAKE_IS_DAY;
    sendForecast(fakePayload.buildFakeJson(FAKE_PAYLOAD_PRESET, nowUtc, opts), settings);
    return;
  }

  function onCoords(lat, lon) {
    // Manual location and "this location only" are the original one-place
    // fetch: no trail write, no extra coordinates, no place-change mask.
    if (!trail.trailActive(settings)) {
      fetchSingle(lat, lon, settings);
      return;
    }
    fetchTrail(lat, lon, settings);
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
  var settings = clay.getSettings(e.response, false);
  if (trail.configRequestsClear(settings)) {
    trailStore.clear();
    // The flag is one-shot. getSettings just wrote it into clay-settings;
    // drop it so the next save doesn't look like another clear.
    try {
      var stored = JSON.parse(localStorage.getItem('clay-settings')) || {};
      delete stored.clearLocationHistory;
      localStorage.setItem('clay-settings', JSON.stringify(stored));
    } catch (err) {
      console.log('AetherCast: could not drop clear-history flag');
    }
  }
  // Push the updated UNIT_SYSTEM (and/or location) to the watch now,
  // rather than waiting for its next launch refresh.
  fetchAndSend();
});
