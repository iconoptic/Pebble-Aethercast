#include "scale.h"

#include <stdlib.h>

void scale_bounds_ex(const int16_t series[SCALE_N_SAMPLES], int16_t min_span, int16_t *lo, int16_t *hi) {
  int16_t lo_raw = series[0];
  int16_t hi_raw = series[0];
  for (int i = 1; i < SCALE_N_SAMPLES; i++) {
    if (series[i] < lo_raw) {
      lo_raw = series[i];
    }
    if (series[i] > hi_raw) {
      hi_raw = series[i];
    }
  }

  int16_t span = (int16_t)(hi_raw - lo_raw);
  if (span < min_span) {
    int16_t mid = (int16_t)((hi_raw + lo_raw) / 2);
    *lo = (int16_t)(mid - min_span / 2);
    *hi = (int16_t)(mid + min_span / 2);
  } else {
    int16_t pad = (int16_t)(span / 10);
    *lo = (int16_t)(lo_raw - pad);
    *hi = (int16_t)(hi_raw + pad);
  }
}

void scale_bounds(const int16_t series[SCALE_N_SAMPLES], int16_t *lo, int16_t *hi) {
  scale_bounds_ex(series, SCALE_MIN_SPAN, lo, hi);
}

void scale_series_ex(const int16_t series[SCALE_N_SAMPLES], ScaleRect rect, int16_t min_span,
                      ScalePoint out[SCALE_N_SAMPLES]) {
  int16_t lo, hi;
  scale_bounds_ex(series, min_span, &lo, &hi);
  int32_t span = hi - lo;

  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    out[i].x = (int16_t)(rect.x + (i * (int32_t)(rect.w - 1) * 256 / (SCALE_N_SAMPLES - 1) + 128) / 256);

    // Bottom row (rect.y + rect.h - 1) maps to `lo`, top row (rect.y) to
    // `hi`; using (rect.h - 1) as the divisor numerator keeps y strictly
    // inside [rect.y, rect.y + rect.h) even when v == lo exactly.
    int32_t y = rect.y + rect.h - 1 - ((int32_t)(series[i] - lo) * (rect.h - 1)) / span;
    if (y < rect.y) {
      y = rect.y;
    }
    if (y > rect.y + rect.h - 1) {
      y = rect.y + rect.h - 1;
    }
    out[i].y = (int16_t)y;
  }
}

void scale_series(const int16_t series[SCALE_N_SAMPLES], ScaleRect rect, ScalePoint out[SCALE_N_SAMPLES]) {
  scale_series_ex(series, rect, SCALE_MIN_SPAN, out);
}

int16_t scale_grid_step_ex(int16_t lo, int16_t hi, const int16_t *steps, int count) {
  for (int i = 0; i < count; i++) {
    int16_t step = steps[i];
    int16_t first = (int16_t)(((lo / step) + 1) * step);
    int found = 0;
    for (int16_t v = first; v < hi; v += step) {
      found++;
    }
    if (found >= 2 && found <= 4) {
      return step;
    }
  }
  return steps[count - 1];
}

int16_t scale_grid_step(int16_t lo, int16_t hi) {
  static const int16_t steps[] = {20, 50, 100};
  return scale_grid_step_ex(lo, hi, steps, 3);
}

int16_t scale_trend_delta3(const int16_t series[SCALE_N_SAMPLES], uint8_t now_idx) {
  if (now_idx < 3 || now_idx >= SCALE_N_SAMPLES) {
    return 0;
  }
  return (int16_t)(series[now_idx] - series[now_idx - 3]);
}

bool scale_place_change_bit(const uint8_t mask[5], int idx) {
  if (!mask || idx < 0 || idx >= SCALE_N_SAMPLES) {
    return false;
  }
  return (mask[idx >> 3] & (uint8_t)(1u << (idx & 7))) != 0;
}

int16_t scale_effective_delta3(const int16_t *series, uint8_t now_idx,
                               bool has_override, int16_t override_tenths) {
  if (has_override) {
    return override_tenths;
  }
  uint8_t clamped = now_idx < SCALE_N_SAMPLES ? now_idx : (uint8_t)(SCALE_N_SAMPLES - 1);
  return scale_trend_delta3(series, clamped);
}

ScaleTrend scale_trend_from_delta3(int16_t delta3) {
  int16_t a = (int16_t)abs(delta3);
  if (a < 10) {
    return SCALE_TREND_STEADY;
  }
  if (delta3 < 0) {
    return a >= 30 ? SCALE_TREND_FALLING_FAST : SCALE_TREND_FALLING;
  }
  return a >= 30 ? SCALE_TREND_RISING_FAST : SCALE_TREND_RISING;
}
