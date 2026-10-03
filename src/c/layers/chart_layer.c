#include "chart_layer.h"

// Darkens `c` one step per channel for the "fill under the curve" tint.
// Relies only on the documented packed `.argb` field (2 bits/channel, see
// docs/vendor/capi-graphics-types.md) rather than undocumented union members.
static GColor prv_tint(GColor c) {
#if defined(PBL_COLOR)
  uint8_t a = (uint8_t)((c.argb >> 6) & 0x3);
  uint8_t r = (uint8_t)((c.argb >> 4) & 0x3);
  uint8_t g = (uint8_t)((c.argb >> 2) & 0x3);
  uint8_t b = (uint8_t)(c.argb & 0x3);
  r = r > 0 ? (uint8_t)(r - 1) : 0;
  g = g > 0 ? (uint8_t)(g - 1) : 0;
  b = b > 0 ? (uint8_t)(b - 1) : 0;
  GColor out;
  out.argb = (uint8_t)((a << 6) | (r << 4) | (g << 2) | b);
  return out;
#else
  return GColorDarkGray;
#endif
}

void chart_layer_draw(GContext *ctx, GRect rect, const ChartSpec *spec) {
  ScaleRect srect = { rect.origin.x, rect.origin.y, rect.size.w, rect.size.h };
  ScalePoint pts[SCALE_N_SAMPLES];
  scale_series_ex(spec->series, srect, spec->min_span, pts);

  int16_t bottom = (int16_t)(rect.origin.y + rect.size.h - 1);
  // -1 (or anything else out of range below zero) omits the divider and
  // treats every sample as already in the past. A too-large index is
  // clamped so the loops below never read past the last point.
  bool show_now = spec->now_idx >= 0 && spec->now_idx < SCALE_N_SAMPLES;
  uint8_t now_idx = show_now ? (uint8_t)spec->now_idx : (uint8_t)(SCALE_N_SAMPLES - 1);

  if (spec->show_grid) {
    int16_t lo, hi;
    scale_bounds_ex(spec->series, spec->min_span, &lo, &hi);
    graphics_context_set_stroke_color(ctx, GColorDarkGray);
    int16_t first_line = (int16_t)(((lo / spec->grid_step) + 1) * spec->grid_step);
    for (int16_t v = first_line; v < hi; v += spec->grid_step) {
      int16_t y = (int16_t)(rect.origin.y + rect.size.h - 1 -
                            ((int32_t)(v - lo) * (rect.size.h - 1)) / (hi - lo));
      for (int16_t x = rect.origin.x; x < rect.origin.x + rect.size.w; x += 3) {
        graphics_draw_pixel(ctx, GPoint(x, y));
      }
    }
  }

  // Fill under the past curve (tinted), one vertical line per pixel column
  // so the fill has no gaps between the sample points.
  graphics_context_set_stroke_width(ctx, 1);
  graphics_context_set_stroke_color(ctx, prv_tint(spec->line_color));
  for (uint8_t i = 0; i < now_idx; i++) {
    int16_t x0 = pts[i].x, x1 = pts[i + 1].x;
    int16_t y0 = pts[i].y, y1 = pts[i + 1].y;
    if (x1 <= x0) {
      graphics_draw_line(ctx, GPoint(x0, y0), GPoint(x0, bottom));
      continue;
    }
    for (int16_t x = x0; x <= x1; x++) {
      int16_t y = (int16_t)(y0 + (int32_t)(y1 - y0) * (x - x0) / (x1 - x0));
      graphics_draw_line(ctx, GPoint(x, y), GPoint(x, bottom));
    }
  }

  // Past segment: solid polyline; width also conveys trend on B/W screens.
  graphics_context_set_stroke_color(ctx, spec->line_color);
  graphics_context_set_stroke_width(ctx, spec->past_width);
  for (uint8_t i = 0; i < now_idx; i++) {
    graphics_draw_line(ctx, GPoint(pts[i].x, pts[i].y), GPoint(pts[i + 1].x, pts[i + 1].y));
  }

  // Forecast segment: 1px, dashed by skipping every other sample-to-sample
  // segment - the SDK has no dashed-line primitive.
  graphics_context_set_stroke_width(ctx, 1);
  for (uint8_t i = now_idx; i < SCALE_N_SAMPLES - 1; i++) {
    if ((i - now_idx) % 2 == 0) {
      graphics_draw_line(ctx, GPoint(pts[i].x, pts[i].y), GPoint(pts[i + 1].x, pts[i + 1].y));
    }
  }

  if (show_now) {
    // Now divider: full plot height.
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_line(ctx, GPoint(pts[now_idx].x, rect.origin.y), GPoint(pts[now_idx].x, bottom));

    // Current-value dot.
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_circle(ctx, GPoint(pts[now_idx].x, pts[now_idx].y), 3);
  }

  // Scrub cursor: a secondary marker so a detail screen can highlight a
  // scanned-to sample without disturbing the now-divider/dot above.
  if (spec->cursor_idx >= 0 && spec->cursor_idx < SCALE_N_SAMPLES) {
    int16_t cx = pts[spec->cursor_idx].x;
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx, 1);
    for (int16_t y = rect.origin.y; y <= bottom; y += 4) {
      graphics_draw_line(ctx, GPoint(cx, y), GPoint(cx, (int16_t)(y + 2 < bottom ? y + 2 : bottom)));
    }
    graphics_context_set_fill_color(ctx, spec->line_color);
    graphics_fill_circle(ctx, GPoint(cx, pts[spec->cursor_idx].y), 3);
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_circle(ctx, GPoint(cx, pts[spec->cursor_idx].y), 3);
  }
}
