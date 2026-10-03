#pragma once

#include <pebble.h>

#include "../lib/scale.h"

// Data-type-agnostic time-series chart: past segment solid, forecast segment
// dashed, now-divider, current-value dot, and an optional scrub cursor. Used
// by both the dashboard barograph and the full-screen detail charts - see
// docs/design/03-barograph.md "Rendering".
typedef struct {
  const int16_t *series;  // SCALE_N_SAMPLES samples
  // 0 .. SCALE_N_SAMPLES-1 splits past (solid) from forecast (dashed) and
  // places the now-divider. -1 means the series does not cover the present:
  // the whole curve is drawn as the past segment, with no divider and no
  // current-value dot. See docs/design/03-barograph.md.
  int now_idx;
  int16_t min_span;        // SCALE_MIN_SPAN or SCALE_MIN_SPAN_TEMP
  GColor line_color;       // past segment, forecast segment, tint and dot
  uint8_t past_width;      // past-segment stroke width (trend width on B/W)
  bool show_grid;          // horizontal dotted gridlines at round values
  int16_t grid_step;       // ignored if show_grid is false
  int cursor_idx;          // -1 = no cursor marker
} ChartSpec;

// Draws grid + tinted fill + solid past polyline + dashed forecast polyline +
// now-divider + current-value dot (+ cursor marker, if enabled) into `rect`.
void chart_layer_draw(GContext *ctx, GRect rect, const ChartSpec *spec);
