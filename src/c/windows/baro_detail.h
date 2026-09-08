#pragma once

#include <pebble.h>

// Pushes the full-screen barograph detail window, animating in from
// `from_rect` (the dashboard's pressure-label+plot zone) - see
// docs/design/04-ui-layout.md "Detail screens".
void baro_detail_push(GRect from_rect);
