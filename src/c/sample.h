#pragma once

#include <stdbool.h>

// Loads the persisted interval, schedules or cancels the next wakeup, and
// remembers whether this process was started by one. Call once from main,
// before the window goes up.
void sample_init(void);

// True only for APP_LAUNCH_WAKEUP. The dashboard stays off this launch;
// sample_window exits when the phone answers or the watch gives up.
bool sample_is_quiet_launch(void);

// Persist a Clay interval (0/1/2/4; anything else is stored as off) and
// re-arm. Safe to call on every inbox, including a repeat of the same value.
void sample_set_hours(int hours);

// Cancel whatever we scheduled and arm now+interval from the persisted
// hours. A no-op schedule (hours == 0) still cancels, so turning the
// feature off cannot leave a wakeup behind.
void sample_rearm(void);
