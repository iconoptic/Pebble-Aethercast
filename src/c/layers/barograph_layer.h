#pragma once

#include <pebble.h>

#include "../model.h"
#include "../lib/scale.h"

// Draws the pressure label row: value, 3h delta, and trend word.
// `now_idx` selects the sample the trend is computed from (the divider).
// -1 uses the last sample. `value_from_sample` prints that sample instead
// of press_hpa10 — used when the divider has been re-anchored, so the
// number and the dot agree. A live reading passes false and keeps the
// instantaneous press_hpa10.
void barograph_draw_label(GContext *ctx, GRect rect, const WeatherPayload *payload,
                          int now_idx, bool value_from_sample);

// Draws the pressure plot: grid, fill under the past curve, solid past
// segment, dashed forecast segment, now-divider and current-value dot.
// `now_idx` < 0 omits the divider and draws the whole series as past.
// See docs/design/03-barograph.md.
void barograph_draw_plot(GContext *ctx, GRect rect, const WeatherPayload *payload, int now_idx);

// Placeholder for the plot zone while a refresh will replace a misleading
// cache: a dotted midline (same cadence as the chart grid) and the word
// UPDATING. The word is the signal; colour is not required to read it.
void barograph_draw_loading(GContext *ctx, GRect rect);

// Shared formatting/classification helpers, reused by the full-screen
// barograph detail (windows/baro_detail.c) so the two screens agree on
// trend words, colours, and value formatting.
void barograph_format_hpa(int16_t tenths, char *buf, size_t len);
void barograph_format_delta(int16_t tenths, char *buf, size_t len);
const char *barograph_trend_word(ScaleTrend t);
GColor barograph_trend_color(ScaleTrend t);
uint8_t barograph_trend_width(ScaleTrend t);

