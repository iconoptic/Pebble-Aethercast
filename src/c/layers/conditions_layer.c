#include "conditions_layer.h"
#include "icon_layer.h"
#include "../lib/units.h"
#include "../lib/wmo.h"

void conditions_layer_draw(GContext *ctx, GRect rect, const WeatherPayload *payload, bool compact) {
  // The moon phase disc (moon_layer.h) occupies the right portion of the
  // same zone rect independently - keep everything here inside the left
  // ~60% so the two never overlap.
  int16_t content_w = (int16_t)(rect.size.w * 3 / 5);

  int16_t icon_size = (int16_t)(rect.size.h * 4 / 10);
  GRect icon_rect = GRect(rect.origin.x, rect.origin.y, icon_size, icon_size);
  WxIconBucket bucket = payload ? wmo_bucket(payload->wx_code) : WX_ICON_CLEAR;
  bool is_day = payload ? (payload->is_day != 0) : true;
  icon_layer_draw(ctx, icon_rect, bucket, is_day);

  char temp_buf[8];
  units_format_temp_c10(payload ? payload->temp_c10 : 0, temp_buf, sizeof(temp_buf));
  graphics_context_set_text_color(ctx, GColorWhite);
  // On 144x168 screens (basalt et al.) the whole zone is too short for
  // LECO_42_NUMBERS *and* a feels/hi/lo sub-line - drop the sub-line and
  // give the temperature the full zone height instead, per
  // docs/design/04-ui-layout.md's "drop the feels like line" note.
  int16_t temp_h = compact ? rect.size.h : (int16_t)(rect.size.h * 6 / 10);
  int16_t avail_w = (int16_t)(content_w - icon_size - 4);
  GRect temp_rect = GRect((int16_t)(rect.origin.x + icon_size + 4), rect.origin.y,
                          avail_w, temp_h);
  // LECO_42_NUMBERS needs ~70px to fit a "-99\xC2\xB0"-ish string without
  // glyphs overlapping into an unreadable smear - not just on basalt's
  // short compact screens but also chalk, whose round inset leaves less
  // width than gabbro's even though chalk isn't "compact" by height.
  GFont temp_font = fonts_get_system_font(avail_w < 70 ? FONT_KEY_LECO_28_LIGHT_NUMBERS
                                                        : FONT_KEY_LECO_42_NUMBERS);
  graphics_draw_text(ctx, payload ? temp_buf : "--", temp_font,
                      temp_rect, GTextOverflowModeFill, GTextAlignmentLeft, NULL);

  if (compact) {
    return;
  }

  char feels_buf[8] = "--";
  char hi_buf[8] = "--";
  char lo_buf[8] = "--";
  if (payload) {
    units_format_temp_c10(payload->feels_c10, feels_buf, sizeof(feels_buf));
    units_format_temp_c10(payload->hi_c10, hi_buf, sizeof(hi_buf));
    units_format_temp_c10(payload->lo_c10, lo_buf, sizeof(lo_buf));
  }
  // content_w only leaves ~60% of the zone width (the rest is the moon
  // disc), too narrow for one line at GOTHIC_18 - split across two lines
  // at GOTHIC_14 instead, per the design doc's "condensed" sub-line intent.
  graphics_context_set_text_color(ctx, GColorLightGray);
  char feels_line[16];
  snprintf(feels_line, sizeof(feels_line), "feels %s", feels_buf);
  GRect feels_rect = GRect(rect.origin.x, (int16_t)(rect.origin.y + rect.size.h * 62 / 100),
                           content_w, (int16_t)(rect.size.h * 19 / 100));
  graphics_draw_text(ctx, feels_line, fonts_get_system_font(FONT_KEY_GOTHIC_14), feels_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);

  char hilo_line[16];
  snprintf(hilo_line, sizeof(hilo_line), "H%s L%s", hi_buf, lo_buf);
  GRect hilo_rect = GRect(rect.origin.x, (int16_t)(rect.origin.y + rect.size.h * 81 / 100),
                          content_w, (int16_t)(rect.size.h * 19 / 100));
  graphics_draw_text(ctx, hilo_line, fonts_get_system_font(FONT_KEY_GOTHIC_14), hilo_rect,
                      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
}
