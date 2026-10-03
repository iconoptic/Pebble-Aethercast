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

// -1 keeps the "no divider" sentinel. Anything else is clamped into the series.
static int prv_clamp_now_idx(int now_idx) {
  if (now_idx < 0) {
    return -1;
  }
  if (now_idx >= SCALE_N_SAMPLES) {
    return SCALE_N_SAMPLES - 1;
  }
  return now_idx;
}

// Trend (and, when the divider is omitted, the printed value) needs a real
// sample. The last hour is the newest one the cache still has.
static uint8_t prv_trend_idx(int now_idx) {
  int clamped = prv_clamp_now_idx(now_idx);
  return clamped < 0 ? (uint8_t)(SCALE_N_SAMPLES - 1) : (uint8_t)clamped;
}

void barograph_draw_label(GContext *ctx, GRect rect, const WeatherPayload *payload,
                          int now_idx, bool value_from_sample) {
  if (!payload) {
    return;
  }

  // press_series lives in a packed struct; copy out before passing its
  // address to scale.c to avoid an unaligned-pointer warning/UB risk.
  int16_t series[SCALE_N_SAMPLES];
  memcpy(series, payload->press_series, sizeof(series));

  uint8_t trend_idx = prv_trend_idx(now_idx);
  // A live divider uses the phone's place-aware delta. A re-anchored
  // divider (or an omitted one) is a later sample, so the stored delta
  // would describe the wrong hour.
  int16_t delta3 = value_from_sample
      ? scale_trend_delta3(series, trend_idx)
      : payload->press_delta3;
  ScaleTrend trend = scale_trend_from_delta3(delta3);
  int16_t value = value_from_sample ? series[trend_idx] : payload->press_hpa10;

  // Value text ("1000.0 hPa") is a bounded length, but the trend text can
  // run to "-99.9/3h FALLING FAST" - give it most of the row so two-digit
  // deltas don't get ellipsised (was a 50/50 split before).
  int16_t value_w = rect.size.w * 2 / 5;

  char value_buf[16];
  barograph_format_hpa(value, value_buf, sizeof(value_buf));
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

void barograph_draw_loading(GContext *ctx, GRect rect) {
  if (rect.size.w <= 0 || rect.size.h <= 0) {
    return;
  }

  // Same 3px dotted cadence as the chart grid, through the vertical middle
  // of whatever rect the caller already computed from unobstructed bounds.
  int16_t mid_y = (int16_t)(rect.origin.y + rect.size.h / 2);
  graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorDarkGray, GColorWhite));
  for (int16_t x = rect.origin.x; x < rect.origin.x + rect.size.w; x += 3) {
    graphics_draw_pixel(ctx, GPoint(x, mid_y));
  }

  int16_t band_h = (int16_t)(rect.size.h / 3);
  if (band_h < 1) {
    return;
  }
  GRect text_rect = GRect(rect.origin.x, (int16_t)(rect.origin.y + (rect.size.h - band_h) / 2),
                          rect.size.w, band_h);
  graphics_context_set_text_color(ctx, PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite));
  graphics_draw_text(ctx, "UPDATING", fonts_get_system_font(FONT_KEY_GOTHIC_14), text_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
}

void barograph_draw_plot(GContext *ctx, GRect rect, const WeatherPayload *payload, int now_idx) {
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

  int split = prv_clamp_now_idx(now_idx);
  uint8_t trend_idx = prv_trend_idx(split);
  // Same rule as the label: the stored delta belongs to the recorded now.
  int16_t delta3 = (split == (int)payload->press_now_idx)
      ? payload->press_delta3
      : scale_trend_delta3(series, trend_idx);
  ScaleTrend trend = scale_trend_from_delta3(delta3);

  int16_t lo, hi;
  scale_bounds(series, &lo, &hi);

  ChartSpec spec = {
    .series = series,
    .now_idx = split,
    .min_span = SCALE_MIN_SPAN,
    .line_color = barograph_trend_color(trend),
    .past_width = barograph_trend_width(trend),
    .show_grid = true,
    .grid_step = scale_grid_step(lo, hi),
    .cursor_idx = -1,
    .place_change = payload->place_change,
  };
  chart_layer_draw(ctx, rect, &spec);
}
