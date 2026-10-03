#include <pebble.h>

#include "comm.h"
#include "model.h"
#include "sample.h"
#include "windows/dashboard.h"
#include "windows/sample_window.h"

int main(void) {
  model_init();
  comm_init();
  sample_init();
  // A wakeup launch records a fix and leaves. The dashboard is the
  // launch the user asked for. See docs/research/01-background-refresh.md.
  // Quiet Time or a disconnected phone: re-arm already ran; leave without
  // the sample window or a REQUEST.
  if (sample_is_quiet_launch()) {
    if (sample_quiet_launch_aborted()) {
      return 0;
    }
    sample_window_push();
  } else {
    dashboard_window_push();
  }
  app_event_loop();
}
