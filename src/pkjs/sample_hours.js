// Clay's "Background location" value. Only 1, 2 and 4 hours are real
// intervals; everything else (including a missing key) is off. Kept out of
// index.js so the host tests can load it without PebbleKit JS.

function normalizeSampleHours(value) {
  var n = Number(value);
  if (n === 1 || n === 2 || n === 4) return n;
  return 0;
}

module.exports = {
  normalizeSampleHours: normalizeSampleHours,
};
