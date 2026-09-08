#pragma once

#include <stdint.h>

// Pure C, no pebble.h - host-testable per PLAN.md #8.

typedef enum {
  WX_ICON_CLEAR = 0,
  WX_ICON_PARTLY_CLOUDY,
  WX_ICON_OVERCAST,
  WX_ICON_FOG,
  WX_ICON_DRIZZLE,
  WX_ICON_RAIN,
  WX_ICON_SNOW,
  WX_ICON_THUNDERSTORM,
  WX_ICON_THUNDERSTORM_HAIL,
} WxIconBucket;

// Maps an Open-Meteo WMO weather code to one of nine icon buckets, per
// docs/design/01-data-protocol.md "WMO weather codes -> icon buckets".
// Unmapped codes fall back to overcast rather than showing nothing.
WxIconBucket wmo_bucket(uint8_t code);
