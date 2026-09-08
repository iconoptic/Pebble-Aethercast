// Host-side unit tests for src/pkjs/pack.js - see docs/design/01-data-protocol.md
// and PLAN.md's "M10" testing section.
//
// Usage: NODE_PATH="$(pwd)/tests/js" node tests/pack.test.js
// (the Makefile's `js` target wires this up - see tests/Makefile)

var assert = require('assert');
var fs = require('fs');
var path = require('path');

var pack = require('../src/pkjs/pack');
var MessageKeys = require('message_keys');

var fixture = JSON.parse(fs.readFileSync(path.join(__dirname, 'fixtures/openmeteo-denver.json'), 'utf8'));

function nowUtcForFixture() {
  // Any timestamp inside the fixture's "today" daily window (index 1).
  return fixture.daily.time[1] + 12 * 3600;
}

function unpackInt16Series(bytes, scale, count) {
  var buffer = new Uint8Array(bytes).buffer;
  var view = new DataView(buffer);
  var out = [];
  for (var i = 0; i < count; i++) {
    out.push(view.getInt16(i * 2, true) / scale);
  }
  return out;
}

function testHappyPath() {
  var nowUtc = nowUtcForFixture();
  var dict = pack.packPayload(fixture, nowUtc, 1 /* metric */);

  assert.strictEqual(dict[MessageKeys.MSG_TYPE], 1, 'MSG_TYPE should be the success type, not an error');
  assert.strictEqual(dict[MessageKeys.SCHEMA], pack.SCHEMA_VERSION);
  assert.strictEqual(dict[MessageKeys.PRESS_SERIES].length, 36 * 2, 'PRESS_SERIES must be 36 int16s');
  assert.strictEqual(dict[MessageKeys.TEMP_SERIES].length, 36 * 2, 'TEMP_SERIES must be 36 int16s');
  assert.strictEqual(dict[MessageKeys.WX_SERIES].length, 36, 'WX_SERIES must be 36 uint8s');
  assert.strictEqual(dict[MessageKeys.DAILY_HI].length, 4 * 2, 'DAILY_HI must be 4 int16s');
  assert.strictEqual(dict[MessageKeys.DAILY_LO].length, 4 * 2, 'DAILY_LO must be 4 int16s');
  assert.strictEqual(dict[MessageKeys.DAILY_CODE].length, 4, 'DAILY_CODE must be 4 uint8s');

  // DAILY_T0_UTC is a direct passthrough of daily.time[todayIdx] - no
  // utc_offset_seconds subtraction, matching SUNRISE_UTC/SUNSET_UTC/
  // PRESS_T0_UTC's existing convention (see PLAN.md's "Progress log" note
  // on this correction).
  assert.strictEqual(dict[MessageKeys.DAILY_T0_UTC], fixture.daily.time[1]);
  assert.strictEqual(dict[MessageKeys.SUNRISE_UTC], fixture.daily.sunrise[1]);
  assert.strictEqual(dict[MessageKeys.SUNSET_UTC], fixture.daily.sunset[1]);
  assert.strictEqual(dict[MessageKeys.PRESS_T0_UTC], fixture.hourly.time[0]);

  var temp = unpackInt16Series(dict[MessageKeys.TEMP_SERIES], 10, 36);
  for (var i = 0; i < 36; i++) {
    assert.ok(Math.abs(temp[i] - fixture.hourly.temperature_2m[i]) <= 0.05,
              'TEMP_SERIES[' + i + '] round-trips within 0.1 degC precision');
  }
  console.log('  ok: happy path (schema v3 fields present and correct)');
}

function testMissingHourlyTemperature() {
  var broken = JSON.parse(JSON.stringify(fixture));
  broken.hourly.temperature_2m = broken.hourly.temperature_2m.map(function () { return null; });
  var dict = pack.packPayload(broken, nowUtcForFixture(), 1);
  assert.deepStrictEqual(dict, pack.packError(4), 'all-null TEMP_SERIES input must send ERR_CODE 4');
  console.log('  ok: all-null hourly.temperature_2m -> ERR_CODE 4');
}

function testMissingDailyWeatherCode() {
  var broken = JSON.parse(JSON.stringify(fixture));
  delete broken.daily.weather_code;
  // daily.weather_code is optional-ish upstream in the sense that
  // sliceOutlook/num() will throw on the first undefined entry, which
  // packPayload's try/catch turns into ERR_CODE 4 - same as any other
  // malformed-response case.
  var dict = pack.packPayload(broken, nowUtcForFixture(), 1);
  assert.deepStrictEqual(dict, pack.packError(4), 'missing daily.weather_code must send ERR_CODE 4');
  console.log('  ok: missing daily.weather_code -> ERR_CODE 4');
}

function testOutlookPadsWhenDailyArraysAreShort() {
  var short = JSON.parse(JSON.stringify(fixture));
  // Only "yesterday" and "today" present - no forecast days at all.
  short.daily.time = short.daily.time.slice(0, 2);
  short.daily.temperature_2m_max = short.daily.temperature_2m_max.slice(0, 2);
  short.daily.temperature_2m_min = short.daily.temperature_2m_min.slice(0, 2);
  short.daily.weather_code = short.daily.weather_code.slice(0, 2);
  short.daily.sunrise = short.daily.sunrise.slice(0, 2);
  short.daily.sunset = short.daily.sunset.slice(0, 2);

  var dict = pack.packPayload(short, nowUtcForFixture(), 1);
  assert.strictEqual(dict[MessageKeys.MSG_TYPE], 1, 'short daily arrays should still succeed via padding');
  var hi = unpackInt16Series(dict[MessageKeys.DAILY_HI], 10, 4);
  // sliceOutlook pads by repeating the last available entry (index 1, "today").
  for (var i = 0; i < 4; i++) {
    assert.ok(Math.abs(hi[i] - short.daily.temperature_2m_max[1]) <= 0.05,
              'DAILY_HI[' + i + '] should repeat the last available day when padding');
  }
  console.log('  ok: short daily arrays pad by repeating the last day');
}

testHappyPath();
testMissingHourlyTemperature();
testMissingDailyWeatherCode();
testOutlookPadsWhenDailyArraysAreShort();
console.log('all pack.js tests passed');
