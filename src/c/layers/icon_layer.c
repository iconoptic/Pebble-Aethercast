#include "icon_layer.h"

static void prv_draw_sun(GContext *ctx, GPoint c, int16_t r, GColor color) {
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_circle(ctx, c, r);
  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_stroke_width(ctx, 1);
  for (int i = 0; i < 8; i++) {
    int32_t angle = (int32_t)i * TRIG_MAX_ANGLE / 8;
    int16_t sx = (int16_t)(c.x + (r + 2) * sin_lookup(angle) / TRIG_MAX_RATIO);
    int16_t sy = (int16_t)(c.y - (r + 2) * cos_lookup(angle) / TRIG_MAX_RATIO);
    int16_t ex = (int16_t)(c.x + (r + 5) * sin_lookup(angle) / TRIG_MAX_RATIO);
    int16_t ey = (int16_t)(c.y - (r + 5) * cos_lookup(angle) / TRIG_MAX_RATIO);
    graphics_draw_line(ctx, GPoint(sx, sy), GPoint(ex, ey));
  }
}

// Carves a crescent by overpainting in black; relies on the dashboard's
// content layer always filling GColorBlack behind every zone.
static void prv_draw_crescent(GContext *ctx, GPoint c, int16_t r, GColor color) {
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_circle(ctx, c, r);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint((int16_t)(c.x + r / 2), (int16_t)(c.y - r / 3)), r);
}

static void prv_draw_cloud(GContext *ctx, GPoint c, int16_t r, GColor color) {
  graphics_context_set_fill_color(ctx, color);
  int16_t side_r = (int16_t)(r * 3 / 5);
  int16_t side_dx = (int16_t)(r * 4 / 5);
  int16_t side_dy = (int16_t)(r / 3);
  graphics_fill_circle(ctx, GPoint((int16_t)(c.x - side_dx), (int16_t)(c.y + side_dy)), side_r);
  graphics_fill_circle(ctx, GPoint((int16_t)(c.x + side_dx), (int16_t)(c.y + side_dy)), side_r);
  graphics_fill_circle(ctx, c, r);
  // Fills the gap between the top and side puffs so the cloud reads as one
  // solid shape instead of three overlapping circles.
  GRect base = GRect((int16_t)(c.x - side_dx - side_r), c.y,
                      (int16_t)(2 * (side_dx + side_r)), (int16_t)(side_dy + side_r));
  graphics_fill_rect(ctx, base, 0, GCornerNone);
}

static void prv_draw_bolt(GContext *ctx, GPoint c, int16_t r, GColor color) {
  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_stroke_width(ctx, 2);
  GPoint p0 = GPoint((int16_t)(c.x + r / 4), (int16_t)(c.y + r));
  GPoint p1 = GPoint((int16_t)(c.x - r / 6), (int16_t)(c.y + r * 3 / 2));
  GPoint p2 = GPoint((int16_t)(c.x + r / 6), (int16_t)(c.y + r * 3 / 2));
  GPoint p3 = GPoint((int16_t)(c.x - r / 4), (int16_t)(c.y + r * 2));
  graphics_draw_line(ctx, p0, p1);
  graphics_draw_line(ctx, p1, p2);
  graphics_draw_line(ctx, p2, p3);
}

static void prv_draw_rain(GContext *ctx, GPoint c, int16_t r, GColor color, int count) {
  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_stroke_width(ctx, 1);
  int16_t base_y = (int16_t)(c.y + r + 2);
  for (int i = 0; i < count; i++) {
    int16_t x = (int16_t)(c.x - r + (2 * r * i) / (count > 1 ? count - 1 : 1));
    graphics_draw_line(ctx, GPoint(x, base_y), GPoint((int16_t)(x - 2), (int16_t)(base_y + r / 2)));
  }
}

static void prv_draw_snow(GContext *ctx, GPoint c, int16_t r, GColor color, int count) {
  graphics_context_set_fill_color(ctx, color);
  int16_t base_y = (int16_t)(c.y + r + 3);
  for (int i = 0; i < count; i++) {
    int16_t x = (int16_t)(c.x - r + (2 * r * i) / (count > 1 ? count - 1 : 1));
    graphics_fill_circle(ctx, GPoint(x, base_y), 1);
  }
}

static void prv_draw_fog_lines(GContext *ctx, GPoint c, int16_t r, GColor color) {
  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_stroke_width(ctx, 1);
  for (int i = 0; i < 3; i++) {
    int16_t y = (int16_t)(c.y + r / 2 + i * 3);
    graphics_draw_line(ctx, GPoint((int16_t)(c.x - r), y), GPoint((int16_t)(c.x + r), y));
  }
}

void icon_layer_draw(GContext *ctx, GRect rect, WxIconBucket bucket, bool is_day) {
  int16_t r = (int16_t)((rect.size.w < rect.size.h ? rect.size.w : rect.size.h) / 2 - 4);
  if (r < 6) {
    r = 6;
  }
  GPoint c = GPoint((int16_t)(rect.origin.x + rect.size.w / 2),
                     (int16_t)(rect.origin.y + rect.size.h / 2));

  GColor sun_color = PBL_IF_COLOR_ELSE(GColorPastelYellow, GColorWhite);
  GColor cloud_color = PBL_IF_COLOR_ELSE(GColorLightGray, GColorWhite);
  GColor bolt_color = PBL_IF_COLOR_ELSE(GColorChromeYellow, GColorWhite);

  switch (bucket) {
    case WX_ICON_CLEAR:
      if (is_day) {
        prv_draw_sun(ctx, c, r, sun_color);
      } else {
        prv_draw_crescent(ctx, c, r, sun_color);
      }
      break;

    case WX_ICON_PARTLY_CLOUDY: {
      GPoint sun_c = GPoint((int16_t)(c.x - r / 2), (int16_t)(c.y - r / 2));
      int16_t sun_r = (int16_t)(r * 3 / 5);
      if (is_day) {
        prv_draw_sun(ctx, sun_c, sun_r, sun_color);
      } else {
        prv_draw_crescent(ctx, sun_c, sun_r, sun_color);
      }
      GPoint cloud_c = GPoint((int16_t)(c.x + r / 6), (int16_t)(c.y + r / 4));
      prv_draw_cloud(ctx, cloud_c, (int16_t)(r * 4 / 5), cloud_color);
      break;
    }

    case WX_ICON_OVERCAST:
      prv_draw_cloud(ctx, c, r, cloud_color);
      break;

    case WX_ICON_FOG:
      prv_draw_cloud(ctx, GPoint(c.x, (int16_t)(c.y - r / 4)), (int16_t)(r * 4 / 5), cloud_color);
      prv_draw_fog_lines(ctx, c, r, cloud_color);
      break;

    case WX_ICON_DRIZZLE:
      prv_draw_cloud(ctx, GPoint(c.x, (int16_t)(c.y - r / 4)), (int16_t)(r * 4 / 5), cloud_color);
      prv_draw_rain(ctx, c, r, cloud_color, 2);
      break;

    case WX_ICON_RAIN:
      prv_draw_cloud(ctx, GPoint(c.x, (int16_t)(c.y - r / 4)), (int16_t)(r * 4 / 5), cloud_color);
      prv_draw_rain(ctx, c, r, cloud_color, 4);
      break;

    case WX_ICON_SNOW:
      prv_draw_cloud(ctx, GPoint(c.x, (int16_t)(c.y - r / 4)), (int16_t)(r * 4 / 5), cloud_color);
      prv_draw_snow(ctx, c, r, GColorWhite, 4);
      break;

    case WX_ICON_THUNDERSTORM:
      prv_draw_cloud(ctx, GPoint(c.x, (int16_t)(c.y - r / 3)), (int16_t)(r * 4 / 5), cloud_color);
      prv_draw_bolt(ctx, GPoint(c.x, (int16_t)(c.y - r / 3)), r, bolt_color);
      break;

    case WX_ICON_THUNDERSTORM_HAIL:
      prv_draw_cloud(ctx, GPoint(c.x, (int16_t)(c.y - r / 3)), (int16_t)(r * 4 / 5), cloud_color);
      prv_draw_bolt(ctx, GPoint(c.x, (int16_t)(c.y - r / 3)), r, bolt_color);
      prv_draw_snow(ctx, c, r, GColorWhite, 3);
      break;
  }
}
