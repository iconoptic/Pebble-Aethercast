#pragma once

#include <pebble.h>

#include "../model.h"

// Draws the footer: wind direction arrow + speed on the left, humidity on
// the right. Layout per docs/design/04-ui-layout.md.
void footer_layer_draw(GContext *ctx, GRect rect, const WeatherPayload *payload);
