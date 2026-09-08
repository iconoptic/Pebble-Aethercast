#pragma once

#include <stddef.h>
#include <stdint.h>

// Pure C, no pebble.h - host-testable per PLAN.md #8.

typedef enum {
  UNITS_IMPERIAL = 0, // degF, mph - matches the docs/design/04-ui-layout.md mockup
  UNITS_METRIC,       // degC, km/h
} UnitSystem;

// Process-wide unit preference. Defaults to imperial. M7 (Clay settings)
// will let the user change this at runtime; the wire format is always
// metric regardless (docs/design/01-data-protocol.md encoding rule 1).
void units_set_system(UnitSystem system);
UnitSystem units_get_system(void);

// Formatters round to the nearest whole display unit and null-terminate
// into `buf` (capacity `len`).
void units_format_temp_c10(int16_t c10, char *buf, size_t len);
void units_format_wind_kmh10(int16_t kmh10, char *buf, size_t len);
