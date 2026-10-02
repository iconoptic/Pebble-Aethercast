#pragma once

#include <time.h>

// Hours the user can pick. Anything else, including "never set", is off.
#define SAMPLE_WAKEUP_MAX_NUDGE 5
// Another app's wakeup blocks a 1-minute window either side of its time
// (docs/vendor/capi-wakeup.md). 90 s clears that window.
#define SAMPLE_WAKEUP_NUDGE_S 90

int sample_hours_normalize(int hours);

// attempt 0 is now + hours*3600. Each later attempt steps SAMPLE_WAKEUP_NUDGE_S
// later, for an E_RANGE retry. hours must already be normalized and non-zero.
time_t sample_wakeup_timestamp(time_t now, int hours, int attempt);
