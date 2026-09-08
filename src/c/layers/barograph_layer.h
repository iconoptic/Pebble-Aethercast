#pragma once

#include <pebble.h>

#include "../model.h"
#include "../lib/scale.h"

// Draws the pressure label row: current value, 3h delta, and trend word.
void barograph_draw_label(GContext *ctx, GRect rect, const WeatherPayload *payload);

// Draws the pressure plot: grid, fill under the past curve, solid past
// segment, dashed forecast segment, now-divider and current-value dot.
// See docs/design/03-barograph.md.
void barograph_draw_plot(GContext *ctx, GRect rect, const WeatherPayload *payload);

// Shared formatting/classification helpers, reused by the full-screen
// barograph detail (windows/baro_detail.c) so the two screens agree on
// trend words, colours, and value formatting.
void barograph_format_hpa(int16_t tenths, char *buf, size_t len);
void barograph_format_delta(int16_t tenths, char *buf, size_t len);
const char *barograph_trend_word(ScaleTrend t);
GColor barograph_trend_color(ScaleTrend t);
uint8_t barograph_trend_width(ScaleTrend t);

