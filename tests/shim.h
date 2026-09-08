#pragma once

// Host-only stand-ins for the Pebble SDK trig primitives, so lib/moon.c
// (guarded behind #ifdef PBL_SDK_3) builds and runs under plain gcc here.
// See docs/design/02-moon-phase.md "Tests".

#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TRIG_MAX_ANGLE 0x10000
#define TRIG_MAX_RATIO 0x10000

static inline int32_t cos_lookup(int32_t angle) {
  double radians = (double)angle * 2.0 * M_PI / TRIG_MAX_ANGLE;
  return (int32_t)(cos(radians) * TRIG_MAX_RATIO);
}
