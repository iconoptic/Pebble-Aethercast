#include "moon.h"

#ifdef PBL_SDK_3
#include <pebble.h>
#else
#include "../../../tests/shim.h"
#endif

int32_t moon_age_s(int32_t now_utc) {
  int32_t age = (int32_t)(((int64_t)now_utc - MOON_EPOCH_NEW_UTC) % MOON_SYNODIC_S);
  if (age < 0) {
    age += MOON_SYNODIC_S;
  }
  return age;
}

static int32_t prv_angle(int32_t age_s) {
  return (int32_t)(((int64_t)age_s * TRIG_MAX_ANGLE) / MOON_SYNODIC_S);
}

uint16_t moon_lit_permille(int32_t age_s) {
  int32_t c = cos_lookup(prv_angle(age_s)); // in [-TRIG_MAX_RATIO, TRIG_MAX_RATIO]
  // f_lit = (1 - c) / 2, scaled to permille.
  return (uint16_t)(((int64_t)(TRIG_MAX_RATIO - c) * 1000) / (2 * (int64_t)TRIG_MAX_RATIO));
}

static int16_t prv_isqrt(int32_t v) {
  if (v <= 0) {
    return 0;
  }
  int32_t x = v;
  int32_t y = (x + 1) / 2;
  while (y < x) {
    x = y;
    y = (x + v / x) / 2;
  }
  return (int16_t)x;
}

MoonSpan moon_scanline_span(int32_t age_s, int16_t r, int16_t y, int8_t lat_sign) {
  int32_t angle = prv_angle(age_s);
  int32_t c = cos_lookup(angle);
  int16_t h = prv_isqrt((int32_t)r * r - (int32_t)y * y);
  int16_t t = (int16_t)(((int64_t)h * c) / TRIG_MAX_RATIO);
  int waxing = angle < TRIG_MAX_ANGLE / 2;

  int16_t x0, x1;
  if (waxing) {
    x0 = t;
    x1 = h;
  } else {
    x0 = (int16_t)-h;
    x1 = (int16_t)-t;
  }

  if (lat_sign < 0) {
    int16_t mirrored_x0 = (int16_t)-x1;
    int16_t mirrored_x1 = (int16_t)-x0;
    x0 = mirrored_x0;
    x1 = mirrored_x1;
  }

  MoonSpan span = { x0, x1 };
  return span;
}
