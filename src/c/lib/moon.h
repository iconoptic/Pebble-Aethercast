#pragma once

#include <stdint.h>

// Pure, host-testable moon-phase maths - see docs/design/02-moon-phase.md.
// Guarded behind PBL_SDK_3 (a real compiler define on every Pebble build,
// see docs/vendor/pebble-building-for-every-pebble.md) rather than pebble.h
// itself, so this file also builds under plain gcc for tests/.

#define MOON_SYNODIC_S 2551443       // 29.530588853 days
#define MOON_EPOCH_NEW_UTC 947182440 // 2000-01-06 18:14 UTC, a known new moon

typedef struct {
  int16_t x0; // lit-span left x, relative to the disc centre
  int16_t x1; // lit-span right x, relative to the disc centre
} MoonSpan;

// Moon age in seconds since the last new moon, wrapped into [0, MOON_SYNODIC_S).
int32_t moon_age_s(int32_t now_utc);

// Illuminated fraction as permille (0 = new, 500 = half, 1000 = full),
// integer-only so it is exactly reproducible in host tests.
uint16_t moon_lit_permille(int32_t age_s);

// Lit horizontal span [x0, x1] at scanline `y` (relative to disc centre,
// y in [-r, r]) for a disc of radius `r`. `lat_sign` < 0 mirrors the span
// for the southern hemisphere, per docs/design/02-moon-phase.md.
MoonSpan moon_scanline_span(int32_t age_s, int16_t r, int16_t y, int8_t lat_sign);
