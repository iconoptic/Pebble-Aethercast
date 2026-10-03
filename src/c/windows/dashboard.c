#include "dashboard.h"
#include "../comm.h"
#include "../model.h"
#include "../layers/barograph_layer.h"
#include "../layers/moon_layer.h"
#include "../layers/conditions_layer.h"
#include "../layers/footer_layer.h"
#include "baro_detail.h"
#include "temp_detail.h"

// Zone heights as per-mille of the unobstructed bounds height, per
// docs/design/04-ui-layout.md. Never hardcode pixels: emery is the reference,
// but gabbro/basalt/chalk share this code.
#define ZONE_HEADER_PERMILLE     96
#define ZONE_CONDITIONS_PERMILLE 333
#define ZONE_BLABEL_PERMILLE     79
#define ZONE_PLOT_PERMILLE       325
#define ZONE_FOOTER_PERMILLE     167

#define STATUS_DOT_RADIUS 4

// basalt/aplite/diorite/flint are all 144x168 - the only rectangular
// platforms shorter than emery's 228. Detected by height, not a platform
// macro, so it also covers any future 168-tall platform automatically.
#define SMALL_SCREEN_H 168

#define REFRESH_INDICATOR_MS 250

static Window *s_window;
static Layer *s_content_layer;
// Zones the detail-screen animations grow out of/shrink into - cached each
// draw so button handlers can hand a real screen rect to detail_window_push
// without recomputing the whole zone layout - see
// docs/design/04-ui-layout.md "Detail screens".
static GRect s_zone_conditions;
static GRect s_zone_baro;
// Cycles ".", "..", "..." while model_is_refreshing() is true. Bounded by
// model.c's own 30s watchdog - model_is_refreshing() always goes false
// (success or watchdog timeout) so this timer can never run forever.
static AppTimer *s_refresh_indicator_timer;
static uint8_t s_refresh_dots;

static GRect prv_zone(GRect bounds, int16_t *y, uint16_t permille) {
  int16_t height = (int16_t)(bounds.size.h * permille / 1000);
  GRect rect = GRect(bounds.origin.x, *y, bounds.size.w, height);
  *y += height;
  return rect;
}

#if defined(PBL_COLOR)
static GColor prv_status_color(void) {
  switch (model_get_status()) {
    case MODEL_STATUS_FRESH: return GColorGreen;
    case MODEL_STATUS_STALE: return GColorYellow;
    case MODEL_STATUS_ERROR: return GColorRed;
    default: return GColorDarkGray;
  }
}
#endif

// B/W platforms (aplite/diorite/flint - basalt is technically B/W-capable
// too but ships in colour firmware, see docs/research/00-platform-findings.md)
// can't rely on hue to tell FRESH/STALE/ERROR apart, so the dot's fill
// pattern carries the distinction instead: solid, half (checkerboard), and
// hollow (outline only).
static void prv_draw_status_dot(GContext *ctx, GPoint center) {
#if defined(PBL_COLOR)
  graphics_context_set_fill_color(ctx, prv_status_color());
  graphics_fill_circle(ctx, center, STATUS_DOT_RADIUS);
#else
  ModelStatus status = model_get_status();
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
  if (status == MODEL_STATUS_FRESH) {
    graphics_fill_circle(ctx, center, STATUS_DOT_RADIUS);
  } else if (status == MODEL_STATUS_STALE) {
    GRect half = GRect((int16_t)(center.x - STATUS_DOT_RADIUS), (int16_t)(center.y - STATUS_DOT_RADIUS),
                        (int16_t)(STATUS_DOT_RADIUS + 1), (int16_t)(STATUS_DOT_RADIUS * 2));
    graphics_fill_rect(ctx, half, 0, GCornerNone);
    graphics_draw_circle(ctx, center, STATUS_DOT_RADIUS);
  } else {
    graphics_draw_circle(ctx, center, STATUS_DOT_RADIUS);
  }
#endif
}

static void prv_draw_header(GContext *ctx, GRect rect) {
  const WeatherPayload *payload = model_get_payload();
  graphics_context_set_text_color(ctx, GColorWhite);
  GRect loc_rect = GRect(rect.origin.x + 4, rect.origin.y, rect.size.w - 40, rect.size.h);
  graphics_draw_text(ctx, payload ? payload->loc_name : "AETHERCAST",
                      fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), loc_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);

  int16_t dot_cx = rect.origin.x + rect.size.w - 26;
  int16_t dot_cy = rect.origin.y + rect.size.h / 2;
  prv_draw_status_dot(ctx, GPoint(dot_cx, dot_cy));

  char age_buf[8];
  if (model_get_status() == MODEL_STATUS_ERROR) {
    graphics_context_set_text_color(ctx, GColorRed);
    GRect err_rect = GRect(rect.origin.x + 4, rect.origin.y, rect.size.w - 40, rect.size.h);
    graphics_draw_text(ctx, model_error_text(), fonts_get_system_font(FONT_KEY_GOTHIC_14), err_rect,
                        GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
  } else if (model_is_refreshing()) {
    static const char *const dots[] = { ".", "..", "..." };
    strcpy(age_buf, dots[s_refresh_dots % 3]);
  } else if (model_has_data()) {
    snprintf(age_buf, sizeof(age_buf), "%um", model_age_minutes());
  } else {
    strcpy(age_buf, "--");
  }
  if (model_get_status() != MODEL_STATUS_ERROR) {
    GRect age_rect = GRect(rect.origin.x + rect.size.w - 20, rect.origin.y, 20, rect.size.h);
    graphics_draw_text(ctx, age_buf, fonts_get_system_font(FONT_KEY_GOTHIC_14), age_rect,
                        GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
  }
}

static void prv_root_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_unobstructed_bounds(layer);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  // "Small screen" (basalt/aplite/diorite/flint) is decided from the raw
  // screen height before the round inset below shrinks bounds.size.h - a
  // round platform's post-inset height must never be mistaken for this.
  bool compact = bounds.size.h <= SMALL_SCREEN_H;

  // Round platforms (gabbro, chalk) need insets on all four sides so zone
  // content doesn't run into the curved bezel - the header/footer rows sit
  // near the very top/bottom of the circle where the chord width is much
  // narrower than the horizontal inset alone accounts for, so they need a
  // vertical inset too or their text gets clipped by the bezel mask.
  int16_t inset = PBL_IF_ROUND_ELSE(14, 0);
  bounds.origin.x += inset;
  bounds.size.w -= inset * 2;
  bounds.origin.y += inset;
  bounds.size.h -= inset * 2;

  int16_t y = bounds.origin.y;
  prv_draw_header(ctx, prv_zone(bounds, &y, ZONE_HEADER_PERMILLE));

  GRect cond_rect = prv_zone(bounds, &y, ZONE_CONDITIONS_PERMILLE);
  s_zone_conditions = cond_rect;
  const WeatherPayload *payload = model_get_payload();
  conditions_layer_draw(ctx, GRect(cond_rect.origin.x + 4, cond_rect.origin.y,
                                   cond_rect.size.w - 8, cond_rect.size.h),
                        payload, compact);
  moon_layer_draw(ctx, cond_rect, payload ? payload->lat_sign : 1, compact);

  GRect blabel_rect = prv_zone(bounds, &y, ZONE_BLABEL_PERMILLE);
  GRect plot_rect = prv_zone(bounds, &y, ZONE_PLOT_PERMILLE);
  // Combined blabel+plot rect is what the barograph detail animates out of,
  // so the pressure value/trend line the user was just looking at ends up
  // roughly where the expanded chart's top edge lands.
  s_zone_baro = GRect(blabel_rect.origin.x, blabel_rect.origin.y, blabel_rect.size.w,
                      (int16_t)(blabel_rect.size.h + plot_rect.size.h));

  ModelGraphView graph;
  model_get_graph_view(&graph);
  GRect plot_inset = GRect(plot_rect.origin.x + 8, plot_rect.origin.y,
                           plot_rect.size.w - 16, plot_rect.size.h);
  if (graph.mode == MODEL_GRAPH_LOADING) {
    // The hPa value and the 3h trend are both taken from the recorded
    // now-sample. Leave the label row blank rather than print them next
    // to a placeholder that exists to say that sample is not "now".
    barograph_draw_loading(ctx, plot_inset);
  } else {
    // Use the series sample only when the divider has actually moved.
    // A failed refresh of a still-honest cache keeps press_hpa10.
    int recorded = payload && payload->press_now_idx < SCALE_N_SAMPLES
                       ? (int)payload->press_now_idx
                       : SCALE_N_SAMPLES - 1;
    bool value_from_sample = !payload || graph.now_idx != recorded;
    GRect label_inset = GRect(blabel_rect.origin.x + 4, blabel_rect.origin.y,
                              blabel_rect.size.w - 8, blabel_rect.size.h);
    barograph_draw_label(ctx, label_inset, payload, graph.now_idx, value_from_sample);
    barograph_draw_plot(ctx, plot_inset, payload, graph.now_idx);
  }

  GRect footer_rect = prv_zone(bounds, &y, ZONE_FOOTER_PERMILLE);
  footer_layer_draw(ctx, GRect(footer_rect.origin.x + 4, footer_rect.origin.y,
                               footer_rect.size.w - 8, footer_rect.size.h),
                     payload);
}

static void prv_mark_dirty(void) {
  if (s_content_layer) {
    layer_mark_dirty(s_content_layer);
  }
}

static void prv_refresh_indicator_tick(void *data);

// Single place that calls app_timer_register for the header dots.
// Idempotent: a call while a tick is already pending returns immediately
// and does not reset the dot phase. app_timer_register never callbacks
// synchronously and layer_mark_dirty does not redraw synchronously, so
// the pending-timer check alone is enough — no re-entrancy flag.
// reset_phase is true only when a refresh starts; the tick reschedules
// with false so the ".", "..", "..." cycle keeps advancing.
// Bounded by model.c's 30s request watchdog: model_is_refreshing() is
// guaranteed to go false (success or timeout), and the tick then stops
// rescheduling.
static void prv_arm_refresh_indicator(bool reset_phase) {
  if (s_refresh_indicator_timer) {
    return;
  }
  if (reset_phase) {
    s_refresh_dots = 0;
  }
  s_refresh_indicator_timer = app_timer_register(
      REFRESH_INDICATOR_MS, prv_refresh_indicator_tick, NULL);
}

static void prv_start_refresh_indicator(void) {
  prv_arm_refresh_indicator(true);
}

static void prv_model_changed(void) {
  // PKJS_READY starts the launch refresh without a click. Arm the header
  // dots here so that path cycles the same way SELECT does. Safe to call
  // on every model update: prv_arm_refresh_indicator is a no-op while its
  // timer is already pending.
  if (model_is_refreshing()) {
    prv_start_refresh_indicator();
  }
  prv_mark_dirty();
}

static void prv_refresh_indicator_tick(void *data) {
  (void)data;
  s_refresh_indicator_timer = NULL;
  if (!model_is_refreshing()) {
    return;
  }
  s_refresh_dots++;
  prv_mark_dirty();
  prv_arm_refresh_indicator(false);
}

static void prv_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  (void)tick_time;
  (void)units_changed;
  // Auto-retry a stale cache once per minute rather than leaving the user
  // stuck on old data until they manually hit SELECT - model_is_refreshing()
  // guards against piling up requests while one is already outstanding.
  if (model_get_status() == MODEL_STATUS_STALE && !model_is_refreshing()) {
    comm_request_refresh();
    prv_start_refresh_indicator();
  }
  prv_model_changed();
}

// Per docs/design/04-ui-layout.md "Input": every action is reachable with
// buttons alone; touch (below) is additive.
static void prv_select_click(ClickRecognizerRef recognizer, void *context) {
  comm_request_refresh();
  prv_start_refresh_indicator();
}

static void prv_down_click(ClickRecognizerRef recognizer, void *context) {
  baro_detail_push(s_zone_baro);
}

static void prv_up_click(ClickRecognizerRef recognizer, void *context) {
  temp_detail_push(s_zone_conditions);
}

static void prv_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click);
  window_single_click_subscribe(BUTTON_ID_UP, prv_up_click);
  window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click);
}

#if defined(PBL_TOUCH)
static bool prv_point_in_header(Window *window, GPoint p) {
  GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(window));
  int16_t inset = PBL_IF_ROUND_ELSE(14, 0);
  int16_t top = (int16_t)(bounds.origin.y + inset);
  int16_t header_h = (int16_t)((bounds.size.h - inset * 2) * ZONE_HEADER_PERMILLE / 1000);
  return p.y >= top && p.y < top + header_h;
}

// Only emery and gabbro define PBL_TOUCH in this SDK (confirmed via
// build/c4che/<platform>_cache.py's DEFINES) - the recognizer API is a
// no-op stub macro on the other 5 platforms, so this whole block must not
// be compiled there.
static void prv_tap_cb(const Recognizer *recognizer, RecognizerEvent event) {
  if (event != RecognizerEvent_Completed) {
    return;
  }
  if (prv_point_in_header(s_window, tap_recognizer_get_tap_point(recognizer))) {
    comm_request_refresh();
    prv_start_refresh_indicator();
  }
}

static void prv_swipe_cb(const Recognizer *recognizer, RecognizerEvent event) {
  if (event != RecognizerEvent_Completed) {
    return;
  }
  // Swipe up (content moves up, revealing what's "below") mirrors the DOWN
  // button -> barograph; swipe down mirrors UP -> temperature/forecast.
  SwipeDirection dir = swipe_recognizer_get_direction(recognizer);
  if (dir == SwipeDirection_Up) {
    baro_detail_push(s_zone_baro);
  } else if (dir == SwipeDirection_Down) {
    temp_detail_push(s_zone_conditions);
  }
}
#endif

static void prv_window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  s_content_layer = layer_create(layer_get_bounds(root_layer));
  layer_set_update_proc(s_content_layer, prv_root_update_proc);
  layer_add_child(root_layer, s_content_layer);

  window_set_click_config_provider(window, prv_click_config_provider);

#if defined(PBL_TOUCH)
  // touch_service_is_enabled() covers both "no hardware" and "user disabled
  // touch in Settings -> Display" - see docs/vendor/pebble-touch.md.
  if (touch_service_is_enabled()) {
    window_set_touch_bridge_disabled(window, true);
    Recognizer *tap = tap_recognizer_create(prv_tap_cb, NULL);
    window_attach_recognizer(window, tap);
    Recognizer *swipe_up = swipe_recognizer_create(prv_swipe_cb, NULL, SwipeDirection_Up);
    window_attach_recognizer(window, swipe_up);
    Recognizer *swipe_down = swipe_recognizer_create(prv_swipe_cb, NULL, SwipeDirection_Down);
    window_attach_recognizer(window, swipe_down);
  }
#endif
}

static void prv_window_unload(Window *window) {
  layer_destroy(s_content_layer);
}

static void prv_window_appear(Window *window) {
  // Only subscribe while Dashboard is the visible window - e.g. not while
  // a detail screen is pushed on top - per docs/design/04-ui-layout.md's
  // battery guidance ("a real cost on a watch rated for 30 days").
  model_add_listener(prv_model_changed);
  tick_timer_service_subscribe(MINUTE_UNIT, prv_minute_tick);
  if (model_is_refreshing()) {
    // A request from before a detail screen was pushed may still be
    // outstanding - resume the indicator instead of leaving it stuck off.
    prv_start_refresh_indicator();
  }
  prv_model_changed();
}

static void prv_window_disappear(Window *window) {
  tick_timer_service_unsubscribe();
  model_remove_listener(prv_model_changed);
  if (s_refresh_indicator_timer) {
    app_timer_cancel(s_refresh_indicator_timer);
    s_refresh_indicator_timer = NULL;
  }
}

void dashboard_window_push(void) {
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
