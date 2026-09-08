#include <pebble.h>

#include "comm.h"
#include "model.h"
#include "windows/dashboard.h"

int main(void) {
  model_init();
  comm_init();
  dashboard_window_push();
  app_event_loop();
}
