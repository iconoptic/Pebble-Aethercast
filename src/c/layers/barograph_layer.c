#include "barograph_layer.h"
#include "chart_layer.h"

#include <stdlib.h>
#include <string.h>

void barograph_format_hpa(int16_t tenths, char *buf, size_t len) {
  snprintf(buf, len, "%d.%d hPa", tenths / 10, abs(tenths % 10));
}

void barograph_format_delta(int16_t tenths, char *buf, size_t len) {
  int16_t a = (int16_t)abs(tenths);
  snprintf(buf, len, "%s%d.%d", tenths < 0 ? "-" : "+", (int)(a / 10), (int)(a % 10));
}

const char *barograph_trend_word(ScaleTrend t) {
  switch (t) {
    case SCALE_TREND_FALLING_FAST: return "FALLING FAST";
    case SCALE_TREND_FALLING:      return "FALLING";
    case SCALE_TREND_RISING:       return "RISING";
    case SCALE_TREND_RISING_FAST:  return "RISING FAST";
    default:                       return "STEADY";
  }
}

GColor barograph_trend_color(ScaleTrend t) {
#if defined(PBL_COLOR)
  switch (t) {
    case SCALE_TREND_FALLING_FAST: return GColorRed;
    case SCALE_TREND_FALLING:      return GColorOrange;
    case SCALE_TREND_RISING:       return GColorPictonBlue;
    case SCALE_TREND_RISING_FAST:  return GColorBlueMoon;
    default:                       return GColorLightGray;
  }
#else
  return GColorWhite;
#endif
}

// B/W platforms have no hue to distinguish trend, so the past-segment line
// weight carries it instead - steady stays thin, any rise/fall gets bolder.
uint8_t barograph_trend_width(ScaleTrend t) {
#if defined(PBL_COLOR)
  return 2;
#else
  return t == SCALE_TREND_STEADY ? 1 : 2;
#endif
}

static uint8_t prv_now_idx(const WeatherPayload *payload) {
  return payload->press_now_idx < SCALE_N_SAMPLES ? payload->press_now_idx : SCALE_N_SAMPLES - 1;
}

void barograph_draw_label(GContext *ctx, GRect rect, const WeatherPayload *payload) {
  if (!payload) {
    return;
  }

  // press_series lives in a packed struct; copy out before passing its
  // address to scale.c to avoid an unaligned-pointer warning/UB risk.
  int16_t series[SCALE_N_SAMPLES];
  memcpy(series, payload->press_series, sizeof(series));

  uint8_t now_idx = prv_now_idx(payload);
  int16_t delta3 = scale_trend_delta3(series, now_idx);
  ScaleTrend trend = scale_trend_from_delta3(delta3);

  // Value text ("1000.0 hPa") is a bounded length, but the trend text can
  // run to "-99.9/3h FALLING FAST" - give it most of the row so two-digit
  // deltas don't get ellipsised (was a 50/50 split before).
  int16_t value_w = rect.size.w * 2 / 5;

  char value_buf[16];
  barograph_format_hpa(payload->press_hpa10, value_buf, sizeof(value_buf));
  graphics_context_set_text_color(ctx, GColorWhite);
  GRect value_rect = GRect(rect.origin.x, rect.origin.y, value_w, rect.size.h);
  graphics_draw_text(ctx, value_buf, fonts_get_system_font(FONT_KEY_GOTHIC_14), value_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);

  char delta_buf[12];
  barograph_format_delta(delta3, delta_buf, sizeof(delta_buf));
  char trend_buf[32];
  snprintf(trend_buf, sizeof(trend_buf), "%s/3h %s", delta_buf, barograph_trend_word(trend));
  graphics_context_set_text_color(ctx, barograph_trend_color(trend));
  GRect trend_rect = GRect(rect.origin.x + value_w, rect.origin.y, rect.size.w - value_w, rect.size.h);
  graphics_draw_text(ctx, trend_buf, fonts_get_system_font(FONT_KEY_GOTHIC_14), trend_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
}

void barograph_draw_plot(GContext *ctx, GRect rect, const WeatherPayload *payload) {
  if (!payload) {
    graphics_context_set_stroke_color(ctx, GColorDarkGray);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_draw_rect(ctx, rect);
    return;
  }

  // press_series lives in a packed struct; copy out before passing its
  // address to scale.c to avoid an unaligned-pointer warning/UB risk.
  int16_t series[SCALE_N_SAMPLES];
  memcpy(series, payload->press_series, sizeof(series));

  uint8_t now_idx = prv_now_idx(payload);
  int16_t delta3 = scale_trend_delta3(series, now_idx);
  ScaleTrend trend = scale_trend_from_delta3(delta3);

  int16_t lo, hi;
  scale_bounds(series, &lo, &hi);

  ChartSpec spec = {
    .series = series,
    .now_idx = now_idx,
    .min_span = SCALE_MIN_SPAN,
    .line_color = barograph_trend_color(trend),
    .past_width = barograph_trend_width(trend),
    .show_grid = true,
    .grid_step = scale_grid_step(lo, hi),
    .cursor_idx = -1,
  };
  chart_layer_draw(ctx, rect, &spec);
}
