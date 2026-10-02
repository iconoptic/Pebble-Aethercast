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
static bool s_quiet_abort;

static int prv_stored_hours(void) {
  if (!persist_exists(PERSIST_KEY_SAMPLE_HRS)) {
    return 0;
  }
  return sample_hours_normalize((int)persist_read_int(PERSIST_KEY_SAMPLE_HRS));
}

static bool prv_wakeup_pending(void) {
  if (!persist_exists(PERSIST_KEY_WAKEUP_ID)) {
    return false;
  }
  WakeupId id = (WakeupId)persist_read_int(PERSIST_KEY_WAKEUP_ID);
  return wakeup_query(id, NULL);
}

// Quiet Time and a missing phone both make a sample worthless (or rude).
// connection_service_peek_pebble_app_connection is on every SDK 4 target.
// quiet_time_is_active is Preferences API: when the symbol is absent from
// the SDK headers (PBL_API_EXISTS false), that platform skips the Quiet
// Time check and only the phone-connection guard applies.
static bool prv_should_abort_quiet_launch(void) {
  if (!connection_service_peek_pebble_app_connection()) {
    APP_LOG(APP_LOG_LEVEL_INFO, "sample: abort quiet launch, phone disconnected");
    return true;
  }
#ifdef _PBL_API_EXISTS_quiet_time_is_active
  if (quiet_time_is_active()) {
    APP_LOG(APP_LOG_LEVEL_INFO, "sample: abort quiet launch, quiet time");
    return true;
  }
#else
  // Aplite's SDK headers stub quiet_time_is_active as (false) but omit
  // _PBL_API_EXISTS_quiet_time_is_active, so this branch skips the call
  // (same effect: never treat Quiet Time as active). Phone-connection
  // still runs on every platform.
#endif
  return false;
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
  int normalized = sample_hours_normalize(hours);
  int stored = prv_stored_hours();
  // Inbox traffic repeats SAMPLE_HRS on every weather/error dict. Skip the
  // flash rewrite and schedule churn when nothing changed and a wakeup is
  // already pending. Key 4 + wakeup_query make "pending" real.
  if (normalized == stored && prv_wakeup_pending()) {
    return;
  }
  if (normalized != stored) {
    persist_write_int(PERSIST_KEY_SAMPLE_HRS, normalized);
  }
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
  s_quiet_abort = false;
  if (s_quiet) {
    WakeupId id = 0;
    int32_t cookie = 0;
    wakeup_get_launch_event(&id, &cookie);
    APP_LOG(APP_LOG_LEVEL_INFO, "sample: quiet launch, id=%ld cookie=%ld", (long)id, (long)cookie);
  }
  wakeup_service_subscribe(prv_wakeup_handler);
  // Every launch (user or wakeup) pushes the next wakeup out from now, so
  // opening the app resets the interval. See the Clay description.
  sample_rearm();
  if (s_quiet && prv_should_abort_quiet_launch()) {
    s_quiet_abort = true;
  }
}

bool sample_is_quiet_launch(void) {
  return s_quiet;
}

bool sample_quiet_launch_aborted(void) {
  return s_quiet_abort;
}
