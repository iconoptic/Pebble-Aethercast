// Host tests for src/c/lib/moon.c - see docs/design/02-moon-phase.md
// "Tests". Run via `make -C tests && ./tests/run`.

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "../src/c/lib/moon.h"

static const int16_t kR = 30;

static int prv_abs16(int16_t v) {
  return v < 0 ? -v : v;
}

// Test 1: known new/full moon points -> phase within +/-0.02 (20 permille).
// MOON_EPOCH_NEW_UTC is itself a verified new moon (docs/design/02-moon-phase.md);
// this checks the age->phase formula is faithful to that reference, including
// the quarter-phase check from the design doc's own derivation.
static void test_known_phases(void) {
  assert(moon_lit_permille(0) <= 20);
  assert(prv_abs16((int16_t)(moon_lit_permille(MOON_SYNODIC_S / 2) - 1000)) <= 20);
  assert(prv_abs16((int16_t)(moon_lit_permille(MOON_SYNODIC_S / 4) - 500)) <= 20);
}

// Test 2: lit fraction is monotonic increasing over [0, 0.5] and decreasing
// over [0.5, 1], sampled at 1000 points.
static void test_monotonic_lit_fraction(void) {
  uint16_t prev = moon_lit_permille(0);
  for (int i = 1; i <= 1000; i++) {
    int32_t age = (int32_t)((int64_t)i * MOON_SYNODIC_S / 1000);
    uint16_t lit = moon_lit_permille(age);
    if (age < MOON_SYNODIC_S / 2) {
      assert(lit >= prev);
    } else if (age > MOON_SYNODIC_S / 2) {
      assert(lit <= prev);
    }
    prev = lit;
  }
}

// Test 3: span width is 0 at phase 0 and 2h at phase 0.5, for every y.
static void test_span_width_extremes(void) {
  for (int16_t y = (int16_t)-kR; y <= kR; y++) {
    MoonSpan new_span = moon_scanline_span(0, kR, y, 1);
    assert(new_span.x1 - new_span.x0 <= 1); // rounding may give a 0-or-1px sliver

    MoonSpan full_span = moon_scanline_span(MOON_SYNODIC_S / 2, kR, y, 1);
    int32_t h_sq = (int32_t)kR * kR - (int32_t)y * y;
    int16_t h = (int16_t)(h_sq > 0 ? (int32_t)(sqrt((double)h_sq)) : 0);
    assert(prv_abs16((int16_t)((full_span.x1 - full_span.x0) - 2 * h)) <= 1);
  }
}

// Test 4: waxing and waning spans agree (are continuous) at the phi=0.5 boundary.
static void test_boundary_continuity(void) {
  int32_t just_before = MOON_SYNODIC_S / 2 - 1;
  int32_t just_after = MOON_SYNODIC_S / 2 + 1;
  for (int16_t y = (int16_t)-kR; y <= kR; y++) {
    MoonSpan before = moon_scanline_span(just_before, kR, y, 1);
    MoonSpan after = moon_scanline_span(just_after, kR, y, 1);
    assert(prv_abs16((int16_t)(before.x0 - after.x0)) <= 1);
    assert(prv_abs16((int16_t)(before.x1 - after.x1)) <= 1);
  }
}

// Test 5: LAT_SIGN = -1 output is the exact mirror of LAT_SIGN = +1.
static void test_southern_hemisphere_mirror(void) {
  int32_t ages[] = {0, MOON_SYNODIC_S / 8, MOON_SYNODIC_S / 4, MOON_SYNODIC_S / 2,
                    (MOON_SYNODIC_S * 3) / 4};
  for (unsigned a = 0; a < sizeof(ages) / sizeof(ages[0]); a++) {
    for (int16_t y = (int16_t)-kR; y <= kR; y++) {
      MoonSpan north = moon_scanline_span(ages[a], kR, y, 1);
      MoonSpan south = moon_scanline_span(ages[a], kR, y, -1);
      assert(south.x0 == (int16_t)-north.x1);
      assert(south.x1 == (int16_t)-north.x0);
    }
  }
}

// Test 6: no span endpoint ever falls outside [-r, r].
static void test_span_within_radius(void) {
  for (int32_t age = 0; age < MOON_SYNODIC_S; age += MOON_SYNODIC_S / 200) {
    for (int16_t y = (int16_t)-kR; y <= kR; y++) {
      MoonSpan span = moon_scanline_span(age, kR, y, 1);
      assert(span.x0 >= -kR && span.x0 <= kR);
      assert(span.x1 >= -kR && span.x1 <= kR);
    }
  }
}

void test_moon_run(void) {
  test_known_phases();
  test_monotonic_lit_fraction();
  test_span_width_extremes();
  test_boundary_continuity();
  test_southern_hemisphere_mirror();
  test_span_within_radius();
}
