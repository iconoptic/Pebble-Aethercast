#include "sample.h"

#include "comm.h"
#include "model.h"
#include "sample_schedule.h"

#include <pebble.h>

// model.c owns keys 1 (weather) and 2 (forecast). These stay out of
// WeatherPayload, so the background-sample setting does not change SCHEMA
// or the 256 B/key layout. See docs/research/01-background-refresh.md.
#define PERSIST_KEY_SAMPLE_HRS 3
#define PERSIST_KEY_WAKEUP_ID 4
#define SAMPLE_COOKIE 1

static bool s_quiet;

static int prv_stored_hours(void) {
  if (!persist_exists(PERSIST_KEY_SAMPLE_HRS)) {
    return 0;
  }
  return sample_hours_normalize((int)persist_read_int(PERSIST_KEY_SAMPLE_HRS));
}

void sample_rearm(void) {
  wakeup_cancel_all();
  if (persist_exists(PERSIST_KEY_WAKEUP_ID)) {
    persist_delete(PERSIST_KEY_WAKEUP_ID);
  }

  int hours = prv_stored_hours();
  if (hours == 0) {
    APP_LOG(APP_LOG_LEVEL_INFO, "sample: background sampling off");
    return;
  }

  time_t now = time(NULL);
  for (int attempt = 0; attempt <= SAMPLE_WAKEUP_MAX_NUDGE; attempt++) {
    time_t when = sample_wakeup_timestamp(now, hours, attempt);
    // notify_if_missed is false: a watch that was powered off should not
    // buzz about a location sample the user will never see.
    WakeupId id = wakeup_schedule(when, SAMPLE_COOKIE, false);
    if (id >= 0) {
      persist_write_int(PERSIST_KEY_WAKEUP_ID, (int32_t)id);
      APP_LOG(APP_LOG_LEVEL_INFO, "sample: next wakeup in %dh, nudge %d", hours, attempt);
      return;
    }
    if (id != E_RANGE) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "sample: wakeup_schedule failed (%d)", (int)id);
      return;
    }
  }
  APP_LOG(APP_LOG_LEVEL_ERROR, "sample: wakeup window busy after %d nudges", SAMPLE_WAKEUP_MAX_NUDGE);
}

void sample_set_hours(int hours) {
  persist_write_int(PERSIST_KEY_SAMPLE_HRS, sample_hours_normalize(hours));
  sample_rearm();
}

// Fires only when a wakeup comes due while we are already running. The
// launch that woke a closed app is reported via launch_reason instead
// (docs/vendor/capi-wakeup.md) and must not take this path — that launch
// exits on its own.
static void prv_wakeup_handler(WakeupId id, int32_t cookie) {
  APP_LOG(APP_LOG_LEVEL_INFO, "sample: wakeup while open, id=%ld cookie=%ld", (long)id, (long)cookie);
  sample_rearm();
  if (!model_is_refreshing()) {
    comm_request_refresh();
  }
}

void sample_init(void) {
  s_quiet = launch_reason() == APP_LAUNCH_WAKEUP;
  if (s_quiet) {
    WakeupId id = 0;
    int32_t cookie = 0;
    wakeup_get_launch_event(&id, &cookie);
    APP_LOG(APP_LOG_LEVEL_INFO, "sample: quiet launch, id=%ld cookie=%ld", (long)id, (long)cookie);
  }
  wakeup_service_subscribe(prv_wakeup_handler);
  sample_rearm();
}

bool sample_is_quiet_launch(void) {
  return s_quiet;
}
