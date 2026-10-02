#pragma once

#include <stdint.h>

// Pure, host-testable barometric-pressure scaling maths - deliberately no
// pebble.h dependency, see docs/design/03-barograph.md "Host testing".

#define SCALE_N_SAMPLES 36
#define SCALE_MIN_SPAN 80 // tenths hPa (8.0 hPa) - docs/design/03-barograph.md
#define SCALE_MIN_SPAN_TEMP 50 // tenths degC (5.0 degC) - keeps a calm day off a full-scale flat line
// PRESS_SERIES is hourly. model.h's MODEL_GRAPH_MISLEADING_AFTER_S is one of
// these periods: the now-divider is on the wrong hour once it has slipped by
// a full sample. model.c static-asserts the two constants match.
#define SCALE_SAMPLE_PERIOD_S 3600

typedef struct {
  int16_t x;
  int16_t y;
} ScalePoint;

typedef struct {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
} ScaleRect;

typedef enum {
  SCALE_TREND_FALLING_FAST = 0,
  SCALE_TREND_FALLING,
  SCALE_TREND_STEADY,
  SCALE_TREND_RISING,
  SCALE_TREND_RISING_FAST,
} ScaleTrend;

// Computes the [lo, hi] vertical scale bounds for `series` given `min_span`,
// per the MIN_SPAN clamp in docs/design/03-barograph.md. hi > lo always, so
// callers never need a divide-by-zero branch.
void scale_bounds_ex(const int16_t series[SCALE_N_SAMPLES], int16_t min_span, int16_t *lo, int16_t *hi);

// scale_bounds_ex with min_span fixed to SCALE_MIN_SPAN (pressure).
void scale_bounds(const int16_t series[SCALE_N_SAMPLES], int16_t *lo, int16_t *hi);

// Maps `series` into `rect` given `min_span`, filling `out[SCALE_N_SAMPLES]`.
// out[i].x is non-decreasing with out[0].x == rect.x and
// out[N-1].x == rect.x + rect.w - 1. Every out[i].y satisfies
// rect.y <= y < rect.y + rect.h.
void scale_series_ex(const int16_t series[SCALE_N_SAMPLES], ScaleRect rect, int16_t min_span,
                      ScalePoint out[SCALE_N_SAMPLES]);

// scale_series_ex with min_span fixed to SCALE_MIN_SPAN (pressure).
void scale_series(const int16_t series[SCALE_N_SAMPLES], ScaleRect rect, ScalePoint out[SCALE_N_SAMPLES]);

// Picks the smallest of `steps[count]` such that 2-4 gridlines fall strictly
// inside (lo, hi); falls back to the largest step if none qualify.
int16_t scale_grid_step_ex(int16_t lo, int16_t hi, const int16_t *steps, int count);

// scale_grid_step_ex with candidates {20, 50, 100} tenths hPa.
int16_t scale_grid_step(int16_t lo, int16_t hi);

// Hourly sample that contains `now_utc` in a series whose sample 0 starts
// at `t0_utc`.
//
// Returns 0 when `now_utc` is before sample 0 (clock skew: clamp to the
// start of the series). Returns -1 when `now_utc` is at or after the end
// of the last sample, so the caller omits the now-divider — the series no
// longer covers the present, and pinning the divider on the last forecast
// hour would claim that hour is "now". Otherwise an index in
// [0, SCALE_N_SAMPLES).
int scale_reanchor_now_idx(int32_t t0_utc, int32_t now_utc);

// Seconds by which `now_utc` is ahead of sample `now_idx`
// (t0_utc + now_idx * SCALE_SAMPLE_PERIOD_S). Negative when the clock is
// behind that sample. Compared with MODEL_GRAPH_MISLEADING_AFTER_S.
int32_t scale_recorded_now_lag_s(int32_t t0_utc, uint8_t now_idx, int32_t now_utc);

// 3-hour pressure delta (tenths hPa) ending at now_idx, the standard
// meteorological trend convention. Returns 0 if now_idx < 3.
int16_t scale_trend_delta3(const int16_t series[SCALE_N_SAMPLES], uint8_t now_idx);

ScaleTrend scale_trend_from_delta3(int16_t delta3);
