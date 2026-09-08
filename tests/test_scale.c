// Host tests for src/c/lib/scale.c - see docs/design/03-barograph.md
// "Host testing". Run via `make -C tests && ./tests/run`.

#include <assert.h>
#include <stdlib.h>

#include "../src/c/lib/scale.h"

static const ScaleRect kRect = {8, 116, 184, 74};

static void prv_check_in_rect(const ScalePoint pts[SCALE_N_SAMPLES], ScaleRect rect) {
  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    assert(pts[i].y >= rect.y);
    assert(pts[i].y < rect.y + rect.h);
  }
}

static void prv_check_x_mapping(const ScalePoint pts[SCALE_N_SAMPLES], ScaleRect rect) {
  assert(pts[0].x == rect.x);
  assert(pts[SCALE_N_SAMPLES - 1].x == rect.x + rect.w - 1);
  for (int i = 1; i < SCALE_N_SAMPLES; i++) {
    assert(pts[i].x >= pts[i - 1].x);
  }
}

static void prv_fill(int16_t *series, int16_t base, int16_t step) {
  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    series[i] = (int16_t)(base + i * step);
  }
}

static void test_presets(void) {
  int16_t flat[SCALE_N_SAMPLES];
  prv_fill(flat, 10130, 0);

  int16_t rising[SCALE_N_SAMPLES];
  prv_fill(rising, 9960, 5);

  int16_t falling[SCALE_N_SAMPLES];
  prv_fill(falling, 10220, -5);

  int16_t sawtooth[SCALE_N_SAMPLES];
  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    int phase = i % 12;
    sawtooth[i] = (int16_t)(10000 + (phase < 6 ? phase : 12 - phase) * 40);
  }

  int16_t gap[SCALE_N_SAMPLES];
  prv_fill(gap, 10080, 3);
  for (int i = 10; i <= 15; i++) {
    gap[i] = gap[9];
  }

  int16_t *presets[] = {flat, rising, falling, sawtooth, gap};
  for (int p = 0; p < 5; p++) {
    ScalePoint pts[SCALE_N_SAMPLES];
    scale_series(presets[p], kRect, pts);
    prv_check_in_rect(pts, kRect);
    prv_check_x_mapping(pts, kRect);
  }
}

static void test_random_series(void) {
  srand(1);
  for (int trial = 0; trial < 10000; trial++) {
    int16_t series[SCALE_N_SAMPLES];
    for (int i = 0; i < SCALE_N_SAMPLES; i++) {
      series[i] = (int16_t)(9000 + rand() % 2000);
    }
    ScalePoint pts[SCALE_N_SAMPLES];
    scale_series(series, kRect, pts);
    prv_check_in_rect(pts, kRect);
    prv_check_x_mapping(pts, kRect);
  }
}

static void test_flat_centred(void) {
  int16_t flat[SCALE_N_SAMPLES];
  prv_fill(flat, 10130, 0);
  ScalePoint pts[SCALE_N_SAMPLES];
  scale_series(flat, kRect, pts);

  int16_t centre = (int16_t)(kRect.y + kRect.h / 2);
  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    assert(abs(pts[i].y - centre) <= 1);
    assert(pts[i].y == pts[0].y);
  }
}

static void test_min_span_clamp(void) {
  int16_t lo, hi;

  int16_t narrow[SCALE_N_SAMPLES];
  prv_fill(narrow, 10130, 0);
  narrow[0] = (int16_t)(narrow[0] - 3);
  narrow[1] = (int16_t)(narrow[1] + 3); // span = 6 tenths, well under MIN_SPAN
  scale_bounds(narrow, &lo, &hi);
  assert(hi - lo == SCALE_MIN_SPAN);

  int16_t wide[SCALE_N_SAMPLES];
  prv_fill(wide, 10000, 10); // span = 350 tenths, over MIN_SPAN
  scale_bounds(wide, &lo, &hi);
  assert(hi - lo > SCALE_MIN_SPAN);
}

static void test_grid_step(void) {
  // Per docs/design/03-barograph.md, the step is always one of {20, 50, 100}
  // tenths-hPa (2/5/10 hPa); very wide spans fall back to the widest.
  int16_t step = scale_grid_step(10000, 10080);
  assert(step == 20);
  step = scale_grid_step(10000, 10800);
  assert(step == 20 || step == 50 || step == 100);
}

static void test_trend(void) {
  int16_t series[SCALE_N_SAMPLES];
  prv_fill(series, 10000, 0);
  assert(scale_trend_from_delta3(scale_trend_delta3(series, 24)) == SCALE_TREND_STEADY);

  series[24] = (int16_t)(series[21] - 35); // falling fast: -3.5 hPa/3h
  assert(scale_trend_from_delta3(scale_trend_delta3(series, 24)) == SCALE_TREND_FALLING_FAST);

  series[24] = (int16_t)(series[21] + 15); // rising: +1.5 hPa/3h
  assert(scale_trend_from_delta3(scale_trend_delta3(series, 24)) == SCALE_TREND_RISING);

  assert(scale_trend_delta3(series, 2) == 0); // now_idx < 3 guard
}

// scale_bounds/scale_series are thin SCALE_MIN_SPAN wrappers around the _ex
// variants added for the temperature detail screen (SCALE_MIN_SPAN_TEMP) -
// see docs/design/03-barograph.md "Rendering".
static void test_ex_variants_match_wrappers(void) {
  int16_t series[SCALE_N_SAMPLES];
  prv_fill(series, 10130, 3);

  int16_t lo_wrap, hi_wrap, lo_ex, hi_ex;
  scale_bounds(series, &lo_wrap, &hi_wrap);
  scale_bounds_ex(series, SCALE_MIN_SPAN, &lo_ex, &hi_ex);
  assert(lo_wrap == lo_ex && hi_wrap == hi_ex);

  ScalePoint pts_wrap[SCALE_N_SAMPLES], pts_ex[SCALE_N_SAMPLES];
  scale_series(series, kRect, pts_wrap);
  scale_series_ex(series, kRect, SCALE_MIN_SPAN, pts_ex);
  for (int i = 0; i < SCALE_N_SAMPLES; i++) {
    assert(pts_wrap[i].x == pts_ex[i].x && pts_wrap[i].y == pts_ex[i].y);
  }

  assert(scale_grid_step(10000, 10080) == scale_grid_step_ex(10000, 10080, (int16_t[]){20, 50, 100}, 3));
}

static void test_min_span_temp(void) {
  int16_t lo, hi;
  int16_t narrow[SCALE_N_SAMPLES];
  prv_fill(narrow, 100, 0);
  narrow[0] = (int16_t)(narrow[0] - 5);
  narrow[1] = (int16_t)(narrow[1] + 5); // span = 10 tenths degC, under SCALE_MIN_SPAN_TEMP
  scale_bounds_ex(narrow, SCALE_MIN_SPAN_TEMP, &lo, &hi);
  assert(hi - lo == SCALE_MIN_SPAN_TEMP);

  ScalePoint pts[SCALE_N_SAMPLES];
  scale_series_ex(narrow, kRect, SCALE_MIN_SPAN_TEMP, pts);
  prv_check_in_rect(pts, kRect);
  prv_check_x_mapping(pts, kRect);
}

void test_scale_run(void) {
  test_presets();
  test_random_series();
  test_flat_centred();
  test_min_span_clamp();
  test_grid_step();
  test_trend();
  test_ex_variants_match_wrappers();
  test_min_span_temp();
}
