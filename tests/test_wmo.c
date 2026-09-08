// Host tests for src/c/lib/wmo.c. Run via `make -C tests && ./tests/run`.

#include <assert.h>

#include "../src/c/lib/wmo.h"

static void test_known_codes(void) {
  assert(wmo_bucket(0) == WX_ICON_CLEAR);
  assert(wmo_bucket(1) == WX_ICON_PARTLY_CLOUDY);
  assert(wmo_bucket(2) == WX_ICON_PARTLY_CLOUDY);
  assert(wmo_bucket(3) == WX_ICON_OVERCAST);
  assert(wmo_bucket(45) == WX_ICON_FOG);
  assert(wmo_bucket(48) == WX_ICON_FOG);
  assert(wmo_bucket(51) == WX_ICON_DRIZZLE);
  assert(wmo_bucket(57) == WX_ICON_DRIZZLE);
  assert(wmo_bucket(61) == WX_ICON_RAIN);
  assert(wmo_bucket(82) == WX_ICON_RAIN);
  assert(wmo_bucket(71) == WX_ICON_SNOW);
  assert(wmo_bucket(86) == WX_ICON_SNOW);
  assert(wmo_bucket(95) == WX_ICON_THUNDERSTORM);
  assert(wmo_bucket(96) == WX_ICON_THUNDERSTORM_HAIL);
  assert(wmo_bucket(99) == WX_ICON_THUNDERSTORM_HAIL);
}

static void test_unmapped_falls_back_to_overcast(void) {
  assert(wmo_bucket(4) == WX_ICON_OVERCAST);
  assert(wmo_bucket(200) == WX_ICON_OVERCAST);
  assert(wmo_bucket(255) == WX_ICON_OVERCAST);
}

void test_wmo_run(void) {
  test_known_codes();
  test_unmapped_falls_back_to_overcast();
}
