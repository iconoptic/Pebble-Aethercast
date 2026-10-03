// Host tests for src/pkjs/geocode.js decision helpers. No Pebble SDK.
// Run via `make -C tests js` (NODE_PATH includes ./js for message_keys).

var assert = require('assert');
var geocode = require('../src/pkjs/geocode');

function memStorage(seed) {
  var data = seed || {};
  return {
    getItem: function (k) { return Object.prototype.hasOwnProperty.call(data, k) ? data[k] : null; },
    setItem: function (k, v) { data[k] = String(v); },
    removeItem: function (k) { delete data[k]; },
    _data: data
  };
}

function testRoundAndUrl() {
  assert.strictEqual(geocode.roundCoord(40.81361), 40.81);
  assert.strictEqual(geocode.roundCoord(-96.70261), -96.7);
  var url = geocode.buildUrl(40.81361, -96.70261);
  assert.ok(url.indexOf('latitude=40.81') !== -1, url);
  assert.ok(url.indexOf('longitude=-96.7') !== -1, url);
  assert.ok(url.indexOf('40.81361') === -1, 'must not send full-precision lat');
  console.log('  ok: coordinates rounded to 2 decimals before BigDataCloud');
}

function testPlaceFromResponse() {
  assert.strictEqual(geocode.placeFromResponse({
    city: 'Lincoln', locality: 'Lincoln', principalSubdivision: 'Nebraska'
  }), 'Lincoln');
  assert.strictEqual(geocode.placeFromResponse({
    city: '', locality: 'Kearney', principalSubdivision: 'Nebraska'
  }), 'Kearney');
  assert.strictEqual(geocode.placeFromResponse({
    city: '  ', locality: '', principalSubdivision: 'Nebraska'
  }), 'Nebraska');
  assert.strictEqual(geocode.placeFromResponse({}), null);
  assert.strictEqual(geocode.placeFromResponse(null), null);
  console.log('  ok: reverse-geocode response prefers city, then locality, then state');
}

function testCacheHitWithinTwoKm() {
  var storage = memStorage();
  geocode.saveCache(storage, 41.14, -100.76, 'North Platte', 1000);
  var cache = geocode.loadCache(storage);
  assert.strictEqual(cache.name, 'North Platte');
  // ~1 km east — still within 2 km.
  assert.strictEqual(geocode.nameFromCache(cache, 41.14, -100.748), 'North Platte');
  // ~5 km away — must refetch.
  assert.strictEqual(geocode.nameFromCache(cache, 41.18, -100.76), null);
  console.log('  ok: cache reused within 2 km and skipped farther away');
}

function testSaveCacheTryCatch() {
  var exploding = {
    getItem: function () { return null; },
    setItem: function () { throw new Error('quota'); }
  };
  // Must not throw.
  geocode.saveCache(exploding, 1, 2, 'Town', 3);
  console.log('  ok: saveCache swallows storage errors');
}

function testDecideAfterForecast() {
  assert.strictEqual(geocode.decideAfterForecast(null, false, 0, 1500), 'send');
  assert.strictEqual(geocode.decideAfterForecast('Town', true, 0, 1500), 'send');
  assert.strictEqual(geocode.decideAfterForecast(null, true, 0, 1500), 'wait');
  assert.strictEqual(geocode.decideAfterForecast(null, true, 1499, 1500), 'wait');
  assert.strictEqual(geocode.decideAfterForecast(null, true, 1500, 1500), 'send');
  assert.strictEqual(geocode.decideAfterForecast('  ', true, 0, 1500), 'wait');
  console.log('  ok: decideAfterForecast waits at most NAME_WAIT_MS');
}

function testDistanceKm() {
  // Same point.
  assert.ok(geocode.distanceKm(41.14, -100.76, 41.14, -100.76) < 0.001);
  // Roughly 1° latitude ≈ 111 km.
  var d = geocode.distanceKm(0, 0, 1, 0);
  assert.ok(d > 110 && d < 112, 'expected ~111 km, got ' + d);
  console.log('  ok: distanceKm haversine sanity');
}

testRoundAndUrl();
testPlaceFromResponse();
testCacheHitWithinTwoKm();
testSaveCacheTryCatch();
testDecideAfterForecast();
testDistanceKm();
console.log('all geocode.js tests passed');
