#pragma once

#include <pebble.h>

#include "../lib/wmo.h"

// Draws a small vector weather-condition icon (sun/cloud/rain/etc, built
// from graphics primitives, no bitmap resources) centred within `rect`.
// `is_day` selects the sun/moon variant for buckets where that matters
// (clear, partly cloudy) - see docs/design/01-data-protocol.md.
void icon_layer_draw(GContext *ctx, GRect rect, WxIconBucket bucket, bool is_day);
