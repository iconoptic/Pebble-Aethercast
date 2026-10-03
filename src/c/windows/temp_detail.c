#include "temp_detail.h"
#include "detail_window.h"
#include "../model.h"
#include "../layers/barograph_layer.h"
#include "../layers/chart_layer.h"
#include "../layers/icon_layer.h"
#include "../lib/scale.h"
#include "../lib/units.h"
#include "../lib/wmo.h"

#include <string.h>

// -1 = follow "now"; see baro_detail.c for the same convention.
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

// Same graph-view anchor as the barograph: re-anchored when the cache is
// misleading, last sample when the series no longer covers "now".
static int prv_anchor_idx(const ModelGraphView *view) {
  return view->now_idx >= 0 ? view->now_idx : SCALE_N_SAMPLES - 1;
}

static void prv_fmt_time(int32_t utc, char *buf, size_t len) {
  time_t t = (time_t)utc;
  struct tm *lt = localtime(&t);
  strftime(buf, len, clock_is_24h_style() ? "%H:%M" : "%I:%M%p", lt);
}

// Maps a UTC timestamp onto the chart's x-axis using the same linear
// index-to-pixel mapping as scale_series_ex, so sunrise/sunset ticks line
// up exactly with the hourly samples they fall between.
static int16_t prv_x_for_utc(GRect rect, int32_t t0_utc, int32_t utc) {
  int32_t idx256 = (int32_t)((utc - t0_utc) * 256 / 3600);
  int32_t x = rect.origin.x + (idx256 * (rect.size.w - 1) / (SCALE_N_SAMPLES - 1) + 128) / 256;
  if (x < rect.origin.x) {
    x = rect.origin.x;
  }
  if (x > rect.origin.x + rect.size.w - 1) {
    x = rect.origin.x + rect.size.w - 1;
  }
  return (int16_t)x;
}

static void prv_draw_sun_tick(GContext *ctx, GRect chart_rect, int32_t t0_utc, int32_t utc) {
  int16_t x = prv_x_for_utc(chart_rect, t0_utc, utc);
  int16_t bottom = (int16_t)(chart_rect.origin.y + chart_rect.size.h - 1);
  graphics_context_set_stroke_color(ctx, GColorYellow);
  graphics_draw_line(ctx, GPoint(x, bottom), GPoint(x, (int16_t)(bottom - 4)));
}

static void prv_draw_outlook(GContext *ctx, GRect rect, const ForecastPayload *forecast) {
  int16_t col_w = (int16_t)(rect.size.w / MODEL_OUTLOOK_DAYS);
  for (int i = 0; i < MODEL_OUTLOOK_DAYS; i++) {
    GRect col = GRect((int16_t)(rect.origin.x + i * col_w), rect.origin.y, col_w, rect.size.h);

    char day_buf[4] = "--";
    time_t day_t = (time_t)(forecast->daily_t0_utc + (int32_t)i * 86400);
    struct tm *lt = localtime(&day_t);
    strftime(day_buf, sizeof(day_buf), "%a", lt);
    graphics_context_set_text_color(ctx, GColorLightGray);
    GRect day_rect = GRect(col.origin.x, col.origin.y, col.size.w, 16);
    graphics_draw_text(ctx, day_buf, fonts_get_system_font(FONT_KEY_GOTHIC_14), day_rect,
                        GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);

    int16_t icon_size = (int16_t)(col.size.w < col.size.h - 34 ? col.size.w - 8 : col.size.h - 34);
    if (icon_size > 28) {
      icon_size = 28;
    }
    GRect icon_rect = GRect((int16_t)(col.origin.x + (col.size.w - icon_size) / 2),
                            (int16_t)(col.origin.y + 16), icon_size, icon_size);
    icon_layer_draw(ctx, icon_rect, wmo_bucket(forecast->daily_code[i]), true);

    char hi_buf[8], lo_buf[8], hilo_buf[20];
    units_format_temp_c10(forecast->daily_hi[i], hi_buf, sizeof(hi_buf));
    units_format_temp_c10(forecast->daily_lo[i], lo_buf, sizeof(lo_buf));
    snprintf(hilo_buf, sizeof(hilo_buf), "%s/%s", hi_buf, lo_buf);
    graphics_context_set_text_color(ctx, GColorWhite);
    GRect hilo_rect = GRect(col.origin.x, (int16_t)(col.origin.y + col.size.h - 16), col.size.w, 16);
    graphics_draw_text(ctx, hilo_buf, fonts_get_system_font(FONT_KEY_GOTHIC_14), hilo_rect,
                        GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
  }
}

static void prv_draw(GContext *ctx, GRect bounds, void *context) {
  const WeatherPayload *payload = model_get_payload();
  const ForecastPayload *forecast = model_get_forecast();
  ModelGraphView view;
  model_get_graph_view(&view);
  if (!payload || !forecast || view.mode == MODEL_GRAPH_ABSENT) {
    graphics_context_set_text_color(ctx, GColorWhite);
    graphics_draw_text(ctx, "No forecast yet.", fonts_get_system_font(FONT_KEY_GOTHIC_18),
                        bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    return;
  }

  int16_t series[SCALE_N_SAMPLES];
  memcpy(series, forecast->temp_series, sizeof(series));
  int anchor = prv_anchor_idx(&view);
  int cursor = s_cursor_idx < 0 ? anchor : s_cursor_idx;

  int16_t title_h = (int16_t)(bounds.size.h * 14 / 100);
  int16_t outlook_h = (int16_t)(bounds.size.h * 26 / 100);
  int16_t chart_h = (int16_t)(bounds.size.h - title_h - outlook_h);

  GRect title_rect = GRect(bounds.origin.x + 6, bounds.origin.y, bounds.size.w - 12, title_h);
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_draw_text(ctx, "TEMPERATURE", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), title_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);

  GRect chart_rect = GRect(bounds.origin.x + 8, (int16_t)(bounds.origin.y + title_h),
                           bounds.size.w - 16, chart_h);

  // Same rule as the barograph: a misleading cache with a refresh in
  // flight shows the placeholder; otherwise the divider follows the
  // graph-view now_idx (re-anchored when the recorded hour is stale).
  if (view.mode == MODEL_GRAPH_LOADING) {
    barograph_draw_loading(ctx, chart_rect);
  } else {
    char cursor_buf[24], time_buf[8], val_buf[8];
    prv_fmt_time(payload->press_t0_utc + (int32_t)cursor * 3600, time_buf, sizeof(time_buf));
    units_format_temp_c10(series[cursor], val_buf, sizeof(val_buf));
    snprintf(cursor_buf, sizeof(cursor_buf), "%s %s", time_buf, val_buf);
    graphics_draw_text(ctx, cursor_buf, fonts_get_system_font(FONT_KEY_GOTHIC_18), title_rect,
                        GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);

    int16_t lo, hi;
    scale_bounds_ex(series, SCALE_MIN_SPAN_TEMP, &lo, &hi);
    ChartSpec spec = {
      .series = series,
      .now_idx = view.now_idx,
      .min_span = SCALE_MIN_SPAN_TEMP,
      .line_color = GColorOrange,
      .past_width = 2,
      .show_grid = true,
      .grid_step = scale_grid_step(lo, hi),
      .cursor_idx = s_cursor_idx,
    };
    chart_layer_draw(ctx, chart_rect, &spec);

    prv_draw_sun_tick(ctx, chart_rect, payload->press_t0_utc, payload->sunrise_utc);
    prv_draw_sun_tick(ctx, chart_rect, payload->press_t0_utc, payload->sunset_utc);
  }

  GRect outlook_rect = GRect(bounds.origin.x, (int16_t)(bounds.origin.y + title_h + chart_h),
                             bounds.size.w, outlook_h);
  prv_draw_outlook(ctx, outlook_rect, forecast);
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

void temp_detail_push(GRect from_rect) {
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
