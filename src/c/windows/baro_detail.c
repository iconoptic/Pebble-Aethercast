#include "baro_detail.h"
#include "detail_window.h"
#include "../model.h"
#include "../layers/barograph_layer.h"
#include "../layers/chart_layer.h"
#include "../lib/scale.h"

#include <string.h>

// -1 = follow "now" (moves automatically as the model refreshes); a real
// index freezes the cursor on that sample until SELECT snaps it back.
static int s_cursor_idx = -1;

static void prv_model_changed(void) {
  detail_window_mark_dirty();
}

static void prv_appear(void *context) {
  (void)context;
  model_add_listener(prv_model_changed);
}

static void prv_disappear(void *context) {
  (void)context;
  model_remove_listener(prv_model_changed);
}

// Sample the cursor treats as "now": the re-anchored index when the series
// still covers the present, otherwise the newest cached hour.
static int prv_anchor_idx(const ModelGraphView *view) {
  return view->now_idx >= 0 ? view->now_idx : SCALE_N_SAMPLES - 1;
}

static void prv_fmt_time(int32_t utc, char *buf, size_t len) {
  time_t t = (time_t)utc;
  struct tm *lt = localtime(&t);
  strftime(buf, len, clock_is_24h_style() ? "%H:%M" : "%I:%M%p", lt);
}

static int16_t prv_delta_over(const int16_t series[SCALE_N_SAMPLES], uint8_t idx, uint8_t hours) {
  if (idx < hours) {
    return 0;
  }
  return (int16_t)(series[idx] - series[idx - hours]);
}

static void prv_draw(GContext *ctx, GRect bounds, void *context) {
  const WeatherPayload *payload = model_get_payload();
  ModelGraphView view;
  model_get_graph_view(&view);
  if (!payload || view.mode == MODEL_GRAPH_ABSENT) {
    graphics_context_set_text_color(ctx, GColorWhite);
    graphics_draw_text(ctx, "No data yet.", fonts_get_system_font(FONT_KEY_GOTHIC_18),
                        bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    return;
  }

  int16_t series[SCALE_N_SAMPLES];
  memcpy(series, payload->press_series, sizeof(series));
  int anchor = prv_anchor_idx(&view);
  int cursor = s_cursor_idx < 0 ? anchor : s_cursor_idx;

  int16_t title_h = (int16_t)(bounds.size.h * 14 / 100);
  int16_t delta_h = (int16_t)(bounds.size.h * 12 / 100);
  int16_t footer_h = (int16_t)(bounds.size.h * 14 / 100);
  int16_t chart_h = (int16_t)(bounds.size.h - title_h - delta_h - footer_h);

  GRect title_rect = GRect(bounds.origin.x + 6, bounds.origin.y, bounds.size.w - 12, title_h);
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_draw_text(ctx, "PRESSURE", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), title_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);

  GRect chart_rect = GRect(bounds.origin.x + 8, (int16_t)(bounds.origin.y + title_h),
                           bounds.size.w - 16, chart_h);

  // Same rule as the dashboard: a misleading cache with a refresh in
  // flight is not drawn. The cursor readout and the delta row come from
  // that same sample, so they stay blank until the curve does.
  if (view.mode == MODEL_GRAPH_LOADING) {
    barograph_draw_loading(ctx, chart_rect);
  } else {
    char cursor_buf[24], time_buf[8], val_buf[16];
    prv_fmt_time(payload->press_t0_utc + (int32_t)cursor * 3600, time_buf, sizeof(time_buf));
    barograph_format_hpa(series[cursor], val_buf, sizeof(val_buf));
    snprintf(cursor_buf, sizeof(cursor_buf), "%s %s", time_buf, val_buf);
    graphics_draw_text(ctx, cursor_buf, fonts_get_system_font(FONT_KEY_GOTHIC_18), title_rect,
                        GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);

    int16_t lo, hi;
    scale_bounds(series, &lo, &hi);
    int16_t delta3 = scale_trend_delta3(series, (uint8_t)anchor);
    ScaleTrend trend = scale_trend_from_delta3(delta3);
    ChartSpec spec = {
      .series = series,
      .now_idx = view.now_idx,
      .min_span = SCALE_MIN_SPAN,
      .line_color = barograph_trend_color(trend),
      .past_width = barograph_trend_width(trend),
      .show_grid = true,
      .grid_step = scale_grid_step(lo, hi),
      .cursor_idx = s_cursor_idx,
    };
    chart_layer_draw(ctx, chart_rect, &spec);

    GRect delta_rect = GRect(bounds.origin.x + 6, (int16_t)(bounds.origin.y + title_h + chart_h),
                             bounds.size.w - 12, delta_h);
    char d3[12], d6[12], d12[12], line[48];
    barograph_format_delta(prv_delta_over(series, (uint8_t)anchor, 3), d3, sizeof(d3));
    barograph_format_delta(prv_delta_over(series, (uint8_t)anchor, 6), d6, sizeof(d6));
    barograph_format_delta(prv_delta_over(series, (uint8_t)anchor, 12), d12, sizeof(d12));
    snprintf(line, sizeof(line), "3h %s  6h %s  12h %s", d3, d6, d12);
    graphics_context_set_text_color(ctx, GColorLightGray);
    graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14), delta_rect,
                        GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
  }

  // Never let the user think this is measured on-wrist - PT2 has no
  // barometer, and licence terms require attribution somewhere in the UI.
  GRect footer_rect = GRect(bounds.origin.x + 6, (int16_t)(bounds.origin.y + bounds.size.h - footer_h),
                            bounds.size.w - 12, footer_h);
  graphics_context_set_text_color(ctx, GColorDarkGray);
  graphics_draw_text(ctx, "Modelled MSL - Open-Meteo", fonts_get_system_font(FONT_KEY_GOTHIC_14),
                      footer_rect, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

static void prv_click(ButtonId id, void *context) {
  const WeatherPayload *payload = model_get_payload();
  if (!payload) {
    return;
  }
  ModelGraphView view;
  model_get_graph_view(&view);
  int anchor = prv_anchor_idx(&view);
  int cursor = s_cursor_idx < 0 ? anchor : s_cursor_idx;
  if (id == BUTTON_ID_UP && cursor > 0) {
    s_cursor_idx = cursor - 1;
  } else if (id == BUTTON_ID_DOWN && cursor < SCALE_N_SAMPLES - 1) {
    s_cursor_idx = cursor + 1;
  } else if (id == BUTTON_ID_SELECT) {
    s_cursor_idx = -1;
  } else {
    return;
  }
  detail_window_mark_dirty();
}

void baro_detail_push(GRect from_rect) {
  s_cursor_idx = -1;
  DetailSpec spec = {
    .from_rect = from_rect,
    .draw = prv_draw,
    .click = prv_click,
    .appear = prv_appear,
    .disappear = prv_disappear,
    .context = NULL,
  };
  detail_window_push(&spec);
}
