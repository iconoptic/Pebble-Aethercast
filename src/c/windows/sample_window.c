#include "sample_window.h"

#include "../model.h"

#include <pebble.h>

// PKJS ready + the 20 s geolocation guard + the 30 s request watchdog can
// stack. 45 s is long enough for a slow phone and short enough that a
// wakeup which never hears from the phone cannot sit on the watchface's
// spot. The model watchdog still ends a request that was actually sent.
#define QUIET_DEADLINE_MS (45 * 1000)

static Window *s_window;
static TextLayer *s_title;
static TextLayer *s_detail;
static AppTimer *s_deadline;
static bool s_leaving;

static void prv_leave(void *data) {
  (void)data;
  window_stack_pop_all(false);
}

static void prv_schedule_leave(void) {
  if (s_leaving) {
    return;
  }
  s_leaving = true;
  if (s_deadline) {
    app_timer_cancel(s_deadline);
    s_deadline = NULL;
  }
  // Defer so this returns out of the inbox or watchdog callback first.
  app_timer_register(50, prv_leave, NULL);
}

// model_note_request_sent does not notify, and a cached payload is already
// fresh at launch. The only notifications during this window are the inbox
// (success or ERR_CODE) and the request watchdog, both of which clear the
// refreshing flag first. Exit on either. Do not exit from appear: refreshing
// is still false until PKJS_READY, and the cache must stay up.
static void prv_model_changed(void) {
  if (model_is_refreshing()) {
    return;
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "sample: quiet launch finished, status=%d", (int)model_get_status());
  prv_schedule_leave();
}

static void prv_deadline_fired(void *data) {
  (void)data;
  s_deadline = NULL;
  APP_LOG(APP_LOG_LEVEL_WARNING, "sample: quiet launch deadline, exiting");
  prv_schedule_leave();
}

static void prv_window_load(Window *window) {
  Layer *root = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root);
  int16_t width = bounds.size.w - 8;
  int16_t y = (int16_t)(bounds.size.h / 2 - 28);

  s_title = text_layer_create(GRect(4, y, width, 28));
  text_layer_set_text(s_title, "Sampling");
  text_layer_set_background_color(s_title, GColorClear);
  text_layer_set_text_color(s_title, GColorWhite);
  text_layer_set_font(s_title, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_title, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_title));

  s_detail = text_layer_create(GRect(4, (int16_t)(y + 28), width, 24));
  text_layer_set_text(s_detail, "location");
  text_layer_set_background_color(s_detail, GColorClear);
  text_layer_set_text_color(s_detail, GColorWhite);
  text_layer_set_font(s_detail, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_detail, GTextAlignmentCenter);
  layer_add_child(root, text_layer_get_layer(s_detail));
}

static void prv_window_unload(Window *window) {
  (void)window;
  text_layer_destroy(s_title);
  text_layer_destroy(s_detail);
  s_title = NULL;
  s_detail = NULL;
}

static void prv_window_appear(Window *window) {
  (void)window;
  model_set_listener(prv_model_changed);
  s_deadline = app_timer_register(QUIET_DEADLINE_MS, prv_deadline_fired, NULL);
}

static void prv_window_disappear(Window *window) {
  (void)window;
  model_set_listener(NULL);
  if (s_deadline) {
    app_timer_cancel(s_deadline);
    s_deadline = NULL;
  }
}

void sample_window_push(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
    .appear = prv_window_appear,
    .disappear = prv_window_disappear,
  });
  window_stack_push(s_window, true);
}
