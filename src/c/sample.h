#pragma once

#include <stdbool.h>

// Loads the persisted interval, schedules or cancels the next wakeup, and
// remembers whether this process was started by one. Call once from main,
// before the window goes up.
void sample_init(void);

// True only for APP_LAUNCH_WAKEUP. The dashboard stays off this launch;
// sample_window exits when the phone answers or the watch gives up.
bool sample_is_quiet_launch(void);

// True when a wakeup launch should exit without taking the screen: Quiet
// Time is on, or the phone app is disconnected. sample_init already re-armed.
bool sample_quiet_launch_aborted(void);

// Persist a Clay interval (0/1/2/4; anything else is stored as off) and
// re-arm when the value changed or there is no pending wakeup. Safe on
// every inbox, including a repeat of the same value.
void sample_set_hours(int hours);

// Cancel whatever we scheduled and arm now+interval from the persisted
// hours. A no-op schedule (hours == 0) still cancels, so turning the
// feature off cannot leave a wakeup behind. Called on every app open so
// a user launch resets the timer from now.
void sample_rearm(void);
