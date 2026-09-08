#pragma once

#include <pebble.h>

// Draws the moon phase disc centred within `rect` (sized off rect.size.h,
// per docs/design/02-moon-phase.md). `lat_sign` < 0 mirrors it for the
// southern hemisphere. `compact` shrinks the disc further on 144x168
// screens (r=12 per docs/design/04-ui-layout.md), where the proportional
// radius alone leaves too little room for the temperature text beside it.
void moon_layer_draw(GContext *ctx, GRect rect, int8_t lat_sign, bool compact);
