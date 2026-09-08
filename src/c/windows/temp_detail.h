#pragma once

#include <pebble.h>

// Pushes the full-screen temperature + forecast detail window, animating in
// from `from_rect` (the dashboard's conditions zone). No moon glyph here -
// see docs/design/04-ui-layout.md "Detail screens".
void temp_detail_push(GRect from_rect);
