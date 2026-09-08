#include "units.h"

#include <stdio.h>

static UnitSystem s_system = UNITS_IMPERIAL;

void units_set_system(UnitSystem system) {
  s_system = system;
}

UnitSystem units_get_system(void) {
  return s_system;
}

// Round-half-away-from-zero integer division.
static int16_t prv_round_div(int32_t num, int32_t den) {
  if ((num < 0) != (den < 0)) {
    return (int16_t)((num - den / 2) / den);
  }
  return (int16_t)((num + den / 2) / den);
}

void units_format_temp_c10(int16_t c10, char *buf, size_t len) {
  int16_t whole;
  if (s_system == UNITS_METRIC) {
    whole = prv_round_div(c10, 10);
  } else {
    // F = C * 9/5 + 32, done in one rounding step rather than tenths->F
    // then rounding again, which would compound error.
    whole = (int16_t)(prv_round_div((int32_t)c10 * 9, 50) + 32);
  }
  snprintf(buf, len, "%d\xC2\xB0", whole);
}

void units_format_wind_kmh10(int16_t kmh10, char *buf, size_t len) {
  if (s_system == UNITS_METRIC) {
    snprintf(buf, len, "%d km/h", prv_round_div(kmh10, 10));
  } else {
    // mph = km/h / 1.609344; 1250/2012 approximates 1/1.609344 to within
    // 0.002%, plenty of precision for a rounded whole-number display.
    snprintf(buf, len, "%d mph", prv_round_div((int32_t)kmh10 * 1250, 2012 * 10));
  }
}
