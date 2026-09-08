#pragma once

#include <pebble.h>

#include "../model.h"

// Draws the icon + temperature + "feels/hi/lo" sub-line in the left portion
// of `rect` (the moon phase disc occupies the right portion separately, see
// moon_layer.h). Layout per docs/design/04-ui-layout.md. `compact` drops the
// feels/hi/lo sub-line entirely on 144x168 screens, where there isn't room.
// Separately, the temperature's font size is chosen from the actual space
// available (not from `compact`), since round platforms' bezel insets can
// leave too little width for the big numeric font even when not compact.
void conditions_layer_draw(GContext *ctx, GRect rect, const WeatherPayload *payload, bool compact);
