// Host tests for src/c/lib/units.c. Run via `make -C tests && ./tests/run`.

#include <assert.h>
#include <string.h>

#include "../src/c/lib/units.h"

static void test_temp_imperial(void) {
  units_set_system(UNITS_IMPERIAL);
  char buf[16];

  units_format_temp_c10(0, buf, sizeof(buf));
  assert(strcmp(buf, "32\xC2\xB0") == 0); // 0 degC == 32 degF

  units_format_temp_c10(1000, buf, sizeof(buf));
  assert(strcmp(buf, "212\xC2\xB0") == 0); // 100 degC == 212 degF (boiling point)

  units_format_temp_c10(222, buf, sizeof(buf));
  assert(strcmp(buf, "72\xC2\xB0") == 0); // 22.2 degC ~= 72 degF (mockup value)

  units_format_temp_c10(-400, buf, sizeof(buf));
  assert(strcmp(buf, "-40\xC2\xB0") == 0); // -40 is the C/F crossover point
}

static void test_temp_metric(void) {
  units_set_system(UNITS_METRIC);
  char buf[16];

  units_format_temp_c10(222, buf, sizeof(buf));
  assert(strcmp(buf, "22\xC2\xB0") == 0);

  units_format_temp_c10(-55, buf, sizeof(buf));
  assert(strcmp(buf, "-6\xC2\xB0") == 0); // -5.5 rounds away from zero to -6

  units_format_temp_c10(-54, buf, sizeof(buf));
  assert(strcmp(buf, "-5\xC2\xB0") == 0); // -5.4 rounds to -5
}

static void test_wind_imperial(void) {
  units_set_system(UNITS_IMPERIAL);
  char buf[16];

  units_format_wind_kmh10(0, buf, sizeof(buf));
  assert(strcmp(buf, "0 mph") == 0);

  units_format_wind_kmh10(129, buf, sizeof(buf)); // 12.9 km/h ~= 8 mph (mockup value)
  assert(strcmp(buf, "8 mph") == 0);
}

static void test_wind_metric(void) {
  units_set_system(UNITS_METRIC);
  char buf[16];

  units_format_wind_kmh10(155, buf, sizeof(buf));
  assert(strcmp(buf, "16 km/h") == 0); // 15.5 rounds away from zero to 16
}

void test_units_run(void) {
  test_temp_imperial();
  test_temp_metric();
  test_wind_imperial();
  test_wind_metric();
}
