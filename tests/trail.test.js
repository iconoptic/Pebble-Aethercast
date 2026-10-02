// Host-side unit tests for the location trail and stitched pressure history.
// See docs/design/01-data-protocol.md and docs/design/03-barograph.md.
//
// Usage: NODE_PATH="$(pwd)/tests/js" node tests/trail.test.js

var assert = require('assert');
var fs = require('fs');
var path = require('path');

var trail = require('../src/pkjs/trail');
var pack = require('../src/pkjs/pack');
var openmeteo = require('../src/pkjs/openmeteo');
var fake = require('../tools/fake_payload');
var MessageKeys = require('message_keys');

var multi = JSON.parse(fs.readFileSync(path.join(__dirname, 'fixtures/openmeteo-multi.json'), 'utf8'));

function memoryStorage() {
  var m = {};
  return {
    getItem: function (k) { return Object.prototype.hasOwnProperty.call(m, k) ? m[k] : null; },
    setItem: function (k, v) { m[k] = String(v); },
    removeItem: function (k) { delete m[k]; },
  };
}

function hourlyTimes(t0) {
  var out = [];
  for (var i = 0; i < 36; i++) out.push(t0 + i * 3600);
  return out;
}

function forecast(lat, lon, pressures, t0) {
  return {
    latitude: lat,
    longitude: lon,
    hourly: { time: hourlyTimes(t0), pressure_msl: pressures },
  };
}

function series(fill) {
  var out = [];
  for (var i = 0; i < 36; i++) out.push(typeof fill === 'function' ? fill(i) : fill);
  return out;
}

function bit(mask, i) {
  return (mask[i >> 3] & (1 << (i & 7))) !== 0;
}

function testRoundingAndClustering() {
  assert.strictEqual(trail.roundCoord(39.7392), 39.74);
  assert.strictEqual(trail.roundCoord(-104.9903), -104.99);
  assert.ok(trail.distanceKm(39.74, -104.99, 39.1, -94.58) > 500, 'Denver to Kansas City is a long way');

  var places = trail.appendFix([], 39.7392, -104.9903, 1000);
  // ~3 km north, still one place, and the arrival time stays put.
  places = trail.appendFix(places, 39.77, -104.99, 5000);
  assert.strictEqual(places.length, 1);
  assert.strictEqual(places[0].t, 1000);
  assert.strictEqual(places[0].lat, 39.74);
  assert.strictEqual(places[0].lon, -104.99);

  places = trail.appendFix(places, 39.1, -94.58, 9000);
  assert.strictEqual(places.length, 2);
  assert.strictEqual(places[1].lat, 39.1);
  assert.strictEqual(places[1].lon, -94.58);
  console.log('  ok: rounding and 5 km clustering');
}

function testRetentionAndCap() {
  var now = 1000000;
  var places = [
    { t: now - 40 * 3600, lat: 39.74, lon: -104.99 },
    { t: now - 35 * 3600, lat: 39.1, lon: -94.58 },
    { t: now - 10 * 3600, lat: 38.63, lon: -90.2 },
  ];
  var pruned = trail.prunePlaces(places, now);
  // The Denver stay ended 35 h ago, before the 30 h cutoff. Kansas City was
  // the place at the cutoff, so it is kept along with St. Louis.
  assert.strictEqual(pruned.length, 2);
  assert.strictEqual(pruned[0].lat, 39.1);
  assert.strictEqual(pruned[1].lat, 38.63);

  var anchor = trail.prunePlaces([
    { t: now - 40 * 3600, lat: 39.74, lon: -104.99 },
    { t: now - 10 * 3600, lat: 39.1, lon: -94.58 },
  ], now);
  assert.strictEqual(anchor.length, 2, 'the place active at the cutoff is kept even if it started earlier');

  var many = [];
  for (var i = 0; i < 60; i++) {
    many.push({ t: now - (60 - i) * 60, lat: 10 + i, lon: -100 });
  }
  var capped = trail.prunePlaces(many, now);
  assert.strictEqual(capped.length, trail.MAX_STORED_PLACES);
  console.log('  ok: retention pruning and stored-place cap');
}

function testHourAssignment() {
  var t0 = 1000000;
  var times = hourlyTimes(t0);
  var denver = { t: times[0] - 10, lat: 39.74, lon: -104.99 };
  var midway = { t: times[10], lat: 39.0, lon: -100.0 };
  var kc = { t: times[20], lat: 39.1, lon: -94.58 };
  var current = { lat: 38.63, lon: -90.2 };
  var assigned = trail.assignHours([denver, midway, kc], times, current, 24);

  assert.strictEqual(assigned.length, 36);
  for (var i = 0; i < 10; i++) assert.ok(trail.samePlace(assigned[i], denver), 'hour ' + i);
  for (var j = 10; j < 20; j++) assert.ok(trail.samePlace(assigned[j], midway), 'hour ' + j);
  for (var k = 20; k < 24; k++) assert.ok(trail.samePlace(assigned[k], kc), 'hour ' + k);
  for (var n = 24; n < 36; n++) assert.ok(trail.samePlace(assigned[n], current), 'hour ' + n);

  // Nothing on the trail is old enough for the first hours: they use the oldest place.
  var late = trail.assignHours([{ t: times[5], lat: 39.1, lon: -94.58 }], times, current, 24);
  for (var h = 0; h < 5; h++) assert.ok(trail.samePlace(late[h], { lat: 39.1, lon: -94.58 }));
  console.log('  ok: hour → place assignment');
}

function testDownsampleAndCallCount() {
  var now = 1790924400 + 22 * 60; // 2026-10-02 07:22 UTC, matches the live fixture's current hour
  var times = trail.hourTimesFromNow(now);
  assert.strictEqual(times[0], 1790838000);
  assert.strictEqual(times[24], 1790924400);
  assert.strictEqual(times.length, 36);

  var places = [];
  for (var i = 0; i < 8; i++) {
    places.push({ t: times[i * 3], lat: 30 + i, lon: -100 });
  }
  var current = { lat: 50, lon: -90 };
  var plan = trail.planFetch(places, current, now, { hours: {} });
  assert.ok(plan.apiCalls <= trail.MAX_PLACES_PER_REQUEST);
  assert.strictEqual(plan.apiCalls, 6, '8 historical places + current downsample to the cap of 6');
  assert.strictEqual(plan.coords[0].lat, 50);
  var sawEarly = false;
  var sawCurrent = false;
  for (var s = 0; s < plan.assignments.length; s++) {
    if (plan.assignments[s].lat === 30) sawEarly = true;
    if (trail.samePlace(plan.assignments[s], current)) sawCurrent = true;
  }
  assert.strictEqual(sawEarly, false, 'the shortest early place is merged away');
  assert.strictEqual(sawCurrent, true, 'the current place is never merged away');

  var one = trail.planFetch(
    [{ t: times[0] - 100, lat: 39.74, lon: -104.99 }],
    { lat: 39.739, lon: -104.991 },
    now,
    { hours: {} });
  assert.strictEqual(one.apiCalls, 1, 'a single place is one API call, same as before');

  var denver = { t: times[0] - 100, lat: 39.74, lon: -104.99 };
  var kc = { lat: 39.1, lon: -94.58 };
  var two = trail.planFetch([denver, { t: times[14], lat: kc.lat, lon: kc.lon }], kc, now, { hours: {} });
  assert.strictEqual(two.apiCalls, 2);

  var cache = { hours: {} };
  for (var h = 0; h < 24; h++) {
    if (!trail.samePlace(two.assignments[h], kc)) {
      cache.hours[String(two.hourTimes[h])] = {
        lat: two.assignments[h].lat,
        lon: two.assignments[h].lon,
        p: 1010,
      };
    }
  }
  var cached = trail.planFetch([denver, { t: times[14], lat: kc.lat, lon: kc.lon }], kc, now, cache);
  assert.strictEqual(cached.apiCalls, 1, 'cached past hours do not cost another coordinate');
  console.log('  ok: downsample cap and per-refresh API call count');
}

function testStitchNullsMaskAndTrend() {
  // Hour-aligned, same convention as Open-Meteo's unixtime hourly grid.
  var t0 = 997200;
  var times = hourlyTimes(t0);
  var a = { lat: 39.74, lon: -104.99 };
  var b = { lat: 39.1, lon: -94.58 };
  var aSeries = series(function (i) { return i < 3 ? null : 1000 + i; });
  var bSeries = series(function (i) { return 1100 + i; });
  var places = [
    { t: times[0] - 5, lat: a.lat, lon: a.lon },
    { t: times[10], lat: b.lat, lon: b.lon },
  ];
  var plan = trail.planFetch(places, b, t0 + 24 * 3600 + 600, { hours: {} });
  var out = trail.integrate(plan, [
    forecast(b.lat, b.lon, bSeries, t0),
    forecast(a.lat, a.lon, aSeries, t0),
  ], { hours: {} });
  assert.ok(!out.error);
  // Leading nulls back-fill from the first real Denver sample (index 3).
  assert.strictEqual(out.pressure[0], 1003);
  assert.strictEqual(out.pressure[3], 1003);
  assert.strictEqual(out.pressure[9], 1009);
  assert.strictEqual(out.pressure[10], 1110);
  assert.ok(bit(out.placeChange, 10));
  assert.strictEqual(bit(out.placeChange, 0), false);
  assert.strictEqual(out.delta3Tenths, null, 'a change 14 h ago is outside the trend window');

  // A move inside the last 3 h must not be classified off the stitched step.
  var moved = [
    { t: times[0] - 5, lat: a.lat, lon: a.lon },
    { t: times[23], lat: b.lat, lon: b.lon },
  ];
  var bTrend = series(function (i) { return 1000 + i; });
  var aFlat = series(900);
  var trendPlan = trail.planFetch(moved, b, t0 + 24 * 3600 + 600, { hours: {} });
  var trend = trail.integrate(trendPlan, [
    forecast(a.lat, a.lon, aFlat, t0),
    forecast(b.lat, b.lon, bTrend, t0),
  ], { hours: {} });
  assert.ok(bit(trend.placeChange, 23));
  assert.strictEqual(trend.delta3Tenths, 30, '3.0 hPa at the current place, not the ~100 hPa step');
  assert.ok(Math.abs(trend.pressure[24] - trend.pressure[21]) > 50, 'the drawn series still contains the step');

  var blank = trail.integrate(plan, [
    forecast(a.lat, a.lon, series(null), t0),
    forecast(b.lat, b.lon, series(null), t0),
  ], { hours: {} });
  assert.strictEqual(blank.error, 4);
  console.log('  ok: stitch nulls, place-change mask, trend window');
}

function testCacheFreezesPastHours() {
  var t0 = multi[0].hourly.time[0];
  var now = multi[0].hourly.time[24] + 600;
  var denver = { t: t0 - 10, lat: 39.74, lon: -104.99 };
  var kc = { lat: 39.1, lon: -94.58 };
  var places = [denver, { t: multi[0].hourly.time[14], lat: kc.lat, lon: kc.lon }];
  var plan = trail.planFetch(places, kc, now, { hours: {} });
  assert.strictEqual(plan.apiCalls, 2);

  var first = trail.integrate(plan, multi, { hours: {} });
  assert.ok(!first.error);
  assert.ok(Math.abs(first.pressure[0] - multi[0].hourly.pressure_msl[0]) < 0.05);
  assert.ok(Math.abs(first.pressure[20] - multi[1].hourly.pressure_msl[20]) < 0.05);
  assert.ok(bit(first.placeChange, 14));
  assert.strictEqual(first.currentJson.latitude, multi[1].latitude, 'current conditions follow Kansas City, not array order');

  var swapped = trail.integrate(plan, [multi[1], multi[0]], { hours: {} });
  assert.ok(Math.abs(swapped.pressure[0] - first.pressure[0]) < 0.05);
  assert.ok(Math.abs(swapped.pressure[20] - first.pressure[20]) < 0.05);

  var revised = JSON.parse(JSON.stringify(multi));
  for (var i = 0; i < 36; i++) {
    revised[0].hourly.pressure_msl[i] += 40;
    revised[1].hourly.pressure_msl[i] += 40;
  }
  var second = trail.integrate(plan, revised, first.cache);
  assert.ok(Math.abs(second.pressure[0] - first.pressure[0]) < 0.05, 'a resolved past hour does not move');
  assert.ok(Math.abs(second.pressure[20] - first.pressure[20]) < 0.05);
  assert.ok(Math.abs(second.pressure[24] - revised[1].hourly.pressure_msl[24]) < 0.05,
            'the current hour stays live');
  var again = trail.planFetch(places, kc, now, second.cache);
  assert.strictEqual(again.apiCalls, 1, 'both places are resolved, so only the current fix is fetched');
  console.log('  ok: multi-location fixture, cache freeze, call count 2 → 1');
}

function testManualOverrideBypass() {
  assert.strictEqual(trail.trailActive({ manualLocationEnabled: true, pressureHistory: 0 }), false);
  assert.strictEqual(trail.trailActive({ manualLocationEnabled: false, pressureHistory: 1 }), false);
  assert.strictEqual(trail.trailActive({ manualLocationEnabled: false, pressureHistory: 0 }), true);
  assert.strictEqual(trail.trailActive({}), true);

  var store = trail.createStore(memoryStorage());
  var settings = { manualLocationEnabled: true, manualLat: 1, manualLon: 2, pressureHistory: 0 };
  // index.js records a fix only when trailActive(settings) is true.
  if (trail.trailActive(settings)) store.recordFix(39.74, -104.99, 1000);
  assert.strictEqual(store.loadPlaces().length, 0);

  settings = { manualLocationEnabled: false, pressureHistory: 0 };
  if (trail.trailActive(settings)) store.recordFix(39.7392, -104.9903, 1000);
  if (trail.trailActive(settings)) store.recordFix(39.77, -104.99, 2000);
  assert.strictEqual(store.loadPlaces().length, 1);
  store.clear();
  assert.strictEqual(store.loadPlaces().length, 0);
  assert.deepStrictEqual(store.loadCache(), { hours: {} });

  assert.strictEqual(trail.configRequestsClear({ clearLocationHistory: { value: true } }), true);
  assert.strictEqual(trail.configRequestsClear({ clearLocationHistory: true }), true);
  assert.strictEqual(trail.configRequestsClear({ UNIT_SYSTEM: { value: 0 } }), false);
  console.log('  ok: manual override and this-location-only bypass the trail');
}

function testWireKeys() {
  var denver = JSON.parse(fs.readFileSync(path.join(__dirname, 'fixtures/openmeteo-denver.json'), 'utf8'));
  var now = denver.daily.time[1] + 12 * 3600;
  var plain = pack.packPayload(denver, now, 1);
  assert.strictEqual(plain[MessageKeys.PLACE_CHANGE], undefined);
  assert.strictEqual(plain[MessageKeys.PRESS_DELTA3], undefined);
  assert.strictEqual(plain[MessageKeys.SCHEMA], 3);

  var override = [];
  for (var i = 0; i < 36; i++) override.push(1000 + i);
  var dict = pack.packPayload(denver, now, 1, {
    pressure: override,
    placeChange: [0, 64, 0, 0, 0],
    delta3Tenths: 0,
  });
  assert.strictEqual(dict[MessageKeys.MSG_TYPE], 1);
  assert.deepStrictEqual(dict[MessageKeys.PLACE_CHANGE], [0, 64, 0, 0, 0]);
  assert.strictEqual(dict[MessageKeys.PRESS_DELTA3], 0, 'a steady override must still be sent');
  var view = new DataView(new Uint8Array(dict[MessageKeys.PRESS_SERIES]).buffer);
  assert.strictEqual(view.getInt16(0, true), 10000);
  assert.strictEqual(view.getInt16(24 * 2, true), 10240);

  var trailJson = fake.buildFakeJson('trail', now);
  var packed = pack.packPayload(trailJson, now, 0);
  assert.deepStrictEqual(packed[MessageKeys.PLACE_CHANGE], [0, 64, 0, 0, 0]);
  assert.strictEqual(packed[MessageKeys.LOC_NAME], 'CHICAGO');
  console.log('  ok: wire keys stay optional and the trail preset carries the mask');
}

function testStoreWriteDoesNotThrow() {
  var store = trail.createStore({
    getItem: function () { return null; },
    setItem: function () { throw new Error('quota'); },
    removeItem: function () { throw new Error('quota'); },
  });
  var places;
  assert.doesNotThrow(function () {
    places = store.recordFix(39.7392, -104.9903, 1000);
  });
  assert.strictEqual(places.length, 1, 'the in-memory trail is returned when the write fails');
  assert.doesNotThrow(function () {
    store.saveCache({ hours: { '1': { lat: 39.74, lon: -104.99, p: 1010 } } });
  });
  assert.doesNotThrow(function () {
    store.clear();
  });
  assert.deepStrictEqual(store.loadPlaces(), []);
  console.log('  ok: storage failures do not throw');
}

function testStitchFallbackToCurrent() {
  var t0 = 997200;
  var times = hourlyTimes(t0);
  var denver = { lat: 39.74, lon: -104.99 };
  var kc = { lat: 39.1, lon: -94.58 };
  var places = [
    { t: times[0] - 5, lat: denver.lat, lon: denver.lon },
    { t: times[10], lat: kc.lat, lon: kc.lon },
  ];
  var plan = trail.planFetch(places, kc, t0 + 24 * 3600 + 600, { hours: {} });
  assert.strictEqual(plan.coords[0].lat, kc.lat);
  assert.ok(plan.coords.length >= 2);

  // Denver's grid point is missing; a far-away body must not be paired with it.
  var kcJson = forecast(kc.lat, kc.lon, series(function (i) { return 1010 + i; }), t0);
  var far = forecast(0, 0, series(900), t0);
  var unmatched = [kcJson, far];
  var unmatchedStitch = trail.integrate(plan, unmatched, { hours: {} });
  assert.strictEqual(unmatchedStitch.error, 4);
  var current = trail.fallbackCurrent(plan, unmatched);
  assert.strictEqual(current, kcJson);
  assert.ok(trail.distanceKm(current.latitude, current.longitude, kc.lat, kc.lon) <= 50);

  // One place's series is not an array. The current location's object is still usable.
  var broken = forecast(denver.lat, denver.lon, series(1000), t0);
  broken.hourly.pressure_msl = { not: 'an array' };
  var badSeries = [kcJson, broken];
  var badStitch = trail.integrate(plan, badSeries, { hours: {} });
  assert.strictEqual(badStitch.error, 4);
  assert.strictEqual(trail.fallbackCurrent(plan, badSeries), kcJson);
  assert.ok(Array.isArray(trail.fallbackCurrent(plan, badSeries).hourly.pressure_msl));

  var only = forecast(kc.lat, kc.lon, series(1010), t0);
  only.hourly.pressure_msl = null;
  assert.strictEqual(trail.fallbackCurrent(plan, [only]), only);

  assert.strictEqual(trail.fallbackCurrent(plan, [far]), null, 'nothing near the current fix');
  console.log('  ok: stitch failure falls back to the current location');
}

function testUrlShape() {
  var url = openmeteo.buildUrlForCoords([
    { lat: 39.1, lon: -94.58 },
    { lat: 39.74, lon: -104.99 },
  ]);
  assert.ok(url.indexOf('latitude=39.1,39.74') !== -1);
  assert.ok(url.indexOf('longitude=-94.58,-104.99') !== -1);
  assert.ok(url.indexOf('past_hours=24') !== -1);
  var single = openmeteo.normalizeResults({ latitude: 1, hourly: {} });
  assert.strictEqual(single.length, 1);
  var many = openmeteo.normalizeResults(multi);
  assert.strictEqual(many.length, 2);
  assert.strictEqual(many[0].hourly.time.length, 36);
  assert.deepStrictEqual(many[0].hourly.time, many[1].hourly.time);
  console.log('  ok: multi-location URL and response normalisation');
}

testRoundingAndClustering();
testRetentionAndCap();
testHourAssignment();
testDownsampleAndCallCount();
testStitchNullsMaskAndTrend();
testCacheFreezesPastHours();
testManualOverrideBypass();
testStoreWriteDoesNotThrow();
testStitchFallbackToCurrent();
testWireKeys();
testUrlShape();
console.log('all trail tests passed');
