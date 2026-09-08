#include "footer_layer.h"
#include "../lib/units.h"

#include <string.h>

static GPoint prv_polar(GPoint center, int16_t radius, int32_t angle) {
  return GPoint((int16_t)(center.x + radius * sin_lookup(angle) / TRIG_MAX_RATIO),
                (int16_t)(center.y - radius * cos_lookup(angle) / TRIG_MAX_RATIO));
}

// WIND_DIR is the bearing the wind comes FROM (meteorological convention).
// The arrow is drawn pointing the direction it's blowing TOWARD
// (bearing + 180) - a vector arrow instead of a Unicode glyph, per this
// repo's established caution around glyph coverage (see barograph_layer.c).
static void prv_draw_wind_arrow(GContext *ctx, GPoint center, int16_t len, uint16_t dir_from_deg) {
  int32_t angle = (int32_t)(((int32_t)dir_from_deg + 180) % 360) * TRIG_MAX_ANGLE / 360;
  int32_t back_angle = (angle + TRIG_MAX_ANGLE / 2) % TRIG_MAX_ANGLE;
  GPoint head = prv_polar(center, (int16_t)(len / 2), angle);
  GPoint tail = prv_polar(center, (int16_t)(len / 2), back_angle);

  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_stroke_width(ctx, 2);
  graphics_draw_line(ctx, tail, head);

  int32_t wing_offset = TRIG_MAX_ANGLE / 12; // 30 degrees
  GPoint wing1 = prv_polar(head, (int16_t)(len / 3),
                           (int32_t)(back_angle + wing_offset) % TRIG_MAX_ANGLE);
  GPoint wing2 = prv_polar(head, (int16_t)(len / 3),
                           (int32_t)(back_angle - wing_offset + TRIG_MAX_ANGLE) % TRIG_MAX_ANGLE);
  graphics_draw_line(ctx, head, wing1);
  graphics_draw_line(ctx, head, wing2);
}

void footer_layer_draw(GContext *ctx, GRect rect, const WeatherPayload *payload) {
  int16_t cy = (int16_t)(rect.origin.y + rect.size.h / 2);
  int16_t arrow_len = (int16_t)(rect.size.h * 6 / 10);
  GPoint arrow_c = GPoint((int16_t)(rect.origin.x + 14), cy);
  prv_draw_wind_arrow(ctx, arrow_c, arrow_len, payload ? payload->wind_dir : 0);

  char wind_buf[16];
  units_format_wind_kmh10(payload ? payload->wind_kmh10 : 0, wind_buf, sizeof(wind_buf));
  graphics_context_set_text_color(ctx, GColorWhite);
  GRect wind_rect = GRect((int16_t)(rect.origin.x + 28), rect.origin.y,
                          (int16_t)(rect.size.w / 2 - 28), rect.size.h);
  graphics_draw_text(ctx, payload ? wind_buf : "--", fonts_get_system_font(FONT_KEY_GOTHIC_18),
                      wind_rect, GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);

  char humidity_buf[8];
  if (payload) {
    snprintf(humidity_buf, sizeof(humidity_buf), "%u%%", payload->humidity);
  } else {
    strcpy(humidity_buf, "--");
  }
  GRect humidity_rect = GRect((int16_t)(rect.origin.x + rect.size.w / 2), rect.origin.y,
                              (int16_t)(rect.size.w / 2 - 4), rect.size.h);
  graphics_draw_text(ctx, humidity_buf, fonts_get_system_font(FONT_KEY_GOTHIC_18), humidity_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
}
