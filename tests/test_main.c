// Host test runner - see docs/design/02-moon-phase.md and
// docs/design/03-barograph.md "Tests"/"Host testing".

#include <stdio.h>

void test_scale_run(void);
void test_moon_run(void);
void test_units_run(void);
void test_wmo_run(void);
void test_place_change_run(void);
void test_sample_schedule_run(void);

int main(void) {
  test_scale_run();
  test_moon_run();
  test_units_run();
  test_wmo_run();
  test_place_change_run();
  test_sample_schedule_run();
  printf("all host tests passed\n");
  return 0;
}
