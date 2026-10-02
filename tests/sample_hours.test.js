// Host tests for src/pkjs/sample_hours.js.
// Usage: node tests/sample_hours.test.js

var assert = require('assert');
var sampleHours = require('../src/pkjs/sample_hours');

assert.strictEqual(sampleHours.normalizeSampleHours(undefined), 0);
assert.strictEqual(sampleHours.normalizeSampleHours(null), 0);
assert.strictEqual(sampleHours.normalizeSampleHours(0), 0);
assert.strictEqual(sampleHours.normalizeSampleHours(1), 1);
assert.strictEqual(sampleHours.normalizeSampleHours(2), 2);
assert.strictEqual(sampleHours.normalizeSampleHours(4), 4);
assert.strictEqual(sampleHours.normalizeSampleHours(3), 0);
assert.strictEqual(sampleHours.normalizeSampleHours('2'), 2);
assert.strictEqual(sampleHours.normalizeSampleHours('nope'), 0);

console.log('sample_hours tests passed');
