#include "sample_schedule.h"

int sample_hours_normalize(int hours) {
  if (hours == 1 || hours == 2 || hours == 4) {
    return hours;
  }
  return 0;
}

time_t sample_wakeup_timestamp(time_t now, int hours, int attempt) {
  if (attempt < 0) {
    attempt = 0;
  }
  if (attempt > SAMPLE_WAKEUP_MAX_NUDGE) {
    attempt = SAMPLE_WAKEUP_MAX_NUDGE;
  }
  return now + (time_t)hours * 3600 + (time_t)attempt * SAMPLE_WAKEUP_NUDGE_S;
}
