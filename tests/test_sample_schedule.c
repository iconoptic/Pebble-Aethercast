// Host tests for src/c/sample_schedule.c. Run via `make -C tests && ./tests/run`.

#include <assert.h>

#include "../src/c/sample_schedule.h"

static void test_normalize(void) {
  assert(sample_hours_normalize(0) == 0);
  assert(sample_hours_normalize(1) == 1);
  assert(sample_hours_normalize(2) == 2);
  assert(sample_hours_normalize(4) == 4);
  assert(sample_hours_normalize(3) == 0);
  assert(sample_hours_normalize(8) == 0);
  assert(sample_hours_normalize(-1) == 0);
}

static void test_timestamp(void) {
  time_t now = 1700000000;
  assert(sample_wakeup_timestamp(now, 1, 0) == now + 3600);
  assert(sample_wakeup_timestamp(now, 2, 0) == now + 2 * 3600);
  assert(sample_wakeup_timestamp(now, 4, 0) == now + 4 * 3600);
  assert(sample_wakeup_timestamp(now, 1, 1) == now + 3600 + SAMPLE_WAKEUP_NUDGE_S);
  assert(sample_wakeup_timestamp(now, 4, SAMPLE_WAKEUP_MAX_NUDGE) ==
         now + 4 * 3600 + SAMPLE_WAKEUP_MAX_NUDGE * SAMPLE_WAKEUP_NUDGE_S);
  // A negative attempt is the first try, not a time in the past.
  assert(sample_wakeup_timestamp(now, 1, -3) == now + 3600);
  // Past the cap the nudge stops growing, so a bug in the caller cannot
  // walk the wakeup hours away from the interval the user picked.
  assert(sample_wakeup_timestamp(now, 1, SAMPLE_WAKEUP_MAX_NUDGE + 4) ==
         sample_wakeup_timestamp(now, 1, SAMPLE_WAKEUP_MAX_NUDGE));
}

void test_sample_schedule_run(void) {
  test_normalize();
  test_timestamp();
}
