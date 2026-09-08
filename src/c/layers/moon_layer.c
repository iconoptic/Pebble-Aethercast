#include "moon_layer.h"
#include "../lib/moon.h"

void moon_layer_draw(GContext *ctx, GRect rect, int8_t lat_sign, bool compact) {
  int16_t r = compact ? 12 : (int16_t)(rect.size.h * 2 / 5);
  if (r < 4) {
    r = 4;
  }
  GPoint center = GPoint((int16_t)(rect.origin.x + rect.size.w * 4 / 5),
                         (int16_t)(rect.origin.y + rect.size.h / 2));

  GColor dark = PBL_IF_COLOR_ELSE(GColorOxfordBlue, GColorBlack);
  GColor lit = PBL_IF_COLOR_ELSE(GColorPastelYellow, GColorWhite);

  graphics_context_set_fill_color(ctx, dark);
  graphics_fill_circle(ctx, center, r);

  int32_t age = moon_age_s((int32_t)time(NULL));
  graphics_context_set_stroke_color(ctx, lit);
  for (int16_t y = (int16_t)-r; y <= r; y++) {
    MoonSpan span = moon_scanline_span(age, r, y, lat_sign);
    if (span.x1 > span.x0) {
      graphics_draw_line(ctx, GPoint((int16_t)(center.x + span.x0), (int16_t)(center.y + y)),
                          GPoint((int16_t)(center.x + span.x1), (int16_t)(center.y + y)));
    }
  }

  // Outline so a new moon (no lit pixels at all) still reads as "a moon".
  graphics_context_set_stroke_color(ctx, dark);
  graphics_draw_circle(ctx, center, r);
}
