#pragma once

#include <pebble.h>

// Generic full-screen detail window that animates in by growing its content
// layer from a small starting rect (the dashboard zone that was tapped/
// pressed) up to the full screen, and shrinks back into that same rect on
// close - see docs/design/04-ui-layout.md "Detail screens".
typedef struct {
  GRect from_rect;                                        // dashboard zone rect to grow from/shrink into
  void (*draw)(GContext *ctx, GRect bounds, void *context); // draws chrome + chart into the current animated bounds
  void (*click)(ButtonId id, void *context);               // UP/DOWN/SELECT forwarded here; BACK always closes
  void *context;
} DetailSpec;

void detail_window_push(const DetailSpec *spec);

// Re-renders the content layer - call after mutating state referenced by
// spec->draw (e.g. a scrub cursor index) in response to a click callback.
void detail_window_mark_dirty(void);
