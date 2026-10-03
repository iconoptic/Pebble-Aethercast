#pragma once

#include <pebble.h>

// Cache is considered stale (yellow dot, refresh requested) past this age.
#define MODEL_STALE_AFTER_S (20 * 60)

// The barograph now-divider sits on one hourly sample. Once the wall clock
// is at least this far past the sample the cache calls "now"
// (press_t0_utc + press_now_idx * SCALE_SAMPLE_PERIOD_S), that divider is
// on the wrong hour and the curve reads as current data. While a refresh
// is in flight the plot — and the value/trend label, which is computed
// from the same sample — is replaced with a loading placeholder. One hour
// is the sample spacing: a younger cache still lands on the correct
// sample, so it draws immediately. Must stay equal to SCALE_SAMPLE_PERIOD_S
// (model.c static-asserts it). See docs/design/03-barograph.md.
#define MODEL_GRAPH_MISLEADING_AFTER_S 3600

// Dashboard plus the detail window on top of it, with a spare slot so an
// appear-before-disappear overlap during a push cannot drop a registration.
#define MODEL_LISTENER_SLOTS 3

#define MODEL_OUTLOOK_DAYS 4

typedef enum {
  MODEL_STATUS_EMPTY = 0, // no cache at all yet
  MODEL_STATUS_FRESH,     // data age < MODEL_STALE_AFTER_S
  MODEL_STATUS_STALE,     // data older than that, refresh in flight
  MODEL_STATUS_ERROR,     // refresh failed/timed out; cached data (if any) still shown
} ModelStatus;

// Matches the AppMessage wire format in docs/design/01-data-protocol.md.
// Packed and persisted verbatim under a single storage key.
typedef struct __attribute__((__packed__)) {
  uint8_t schema;
  uint8_t unit_system; // UnitSystem from lib/units.h - M7 Clay settings
  int16_t temp_c10;
  int16_t feels_c10;
  int16_t hi_c10;
  int16_t lo_c10;
  uint8_t humidity;
  int16_t wind_kmh10;
  uint16_t wind_dir;
  uint8_t wx_code;
  uint8_t is_day;
  int16_t press_hpa10;
  int16_t press_series[36];
  uint8_t press_now_idx;
  int32_t press_t0_utc;
  int32_t sunrise_utc;
  int32_t sunset_utc;
  int32_t updated_utc;
  char loc_name[24];
  int8_t lat_sign;
  // 36 bits, little-endian: bit i set when slot i's place differs from slot
  // i-1. All zeros when PLACE_CHANGE is absent (single place, or an older
  // phone build). 5 bytes keeps press_delta3 2-byte aligned in this packed
  // struct (offset 138). See docs/design/01-data-protocol.md.
  uint8_t place_change[5];
  // Authoritative 3 h trend, tenths of hPa. Equals the stitched series when
  // the user stayed put; the current place's own delta when a move falls
  // inside the trend window.
  int16_t press_delta3;
} WeatherPayload;

// Forecast-detail-screen data: kept in a second persist key rather than
// folded into WeatherPayload, since PERSIST_DATA_MAX_LENGTH is 256 B/key and
// the two structs together would exceed it. Optional - the temperature
// detail screen works without it ("No forecast yet").
typedef struct __attribute__((__packed__)) {
  uint8_t schema;
  int16_t temp_series[36];
  uint8_t wx_series[36];
  int16_t daily_hi[MODEL_OUTLOOK_DAYS];
  int16_t daily_lo[MODEL_OUTLOOK_DAYS];
  uint8_t daily_code[MODEL_OUTLOOK_DAYS];
  int32_t daily_t0_utc;
} ForecastPayload;

typedef void (*ModelListener)(void);

// Loads any cached payload from persistent storage and computes initial status.
void model_init(void);

bool model_has_data(void);
const WeatherPayload *model_get_payload(void);
// NULL if no forecast has ever been received (schema mismatch clears it).
const ForecastPayload *model_get_forecast(void);
ModelStatus model_get_status(void);
uint8_t model_get_error_code(void);
// Short human-readable reason for MODEL_STATUS_ERROR, e.g. "NO PHONE".
const char *model_error_text(void);
// True while a REQUEST is outstanding (watchdog armed, no response yet).
bool model_is_refreshing(void);
// Minutes since UPDATED_UTC; 0 if there is no data yet.
uint16_t model_age_minutes(void);

// How the barograph should draw the cached series right now. The policy
// lives here so the dashboard and the barograph detail agree.
typedef enum {
  MODEL_GRAPH_ABSENT = 0, // no payload; caller draws the empty outline
  MODEL_GRAPH_LOADING,    // refresh in flight and the recorded now is >= 1 sample behind
  MODEL_GRAPH_LIVE,       // divider at the payload's press_now_idx
  MODEL_GRAPH_REANCHORED, // recorded now is >= 1 sample behind and nothing is in flight; now_idx from the wall clock
} ModelGraphMode;

typedef struct {
  ModelGraphMode mode;
  // Sample index in [0, SCALE_N_SAMPLES), or -1 when the series no longer
  // covers "now" and the divider must be omitted. Meaningful for LIVE
  // (always a real index) and REANCHORED.
  int now_idx;
} ModelGraphView;

void model_get_graph_view(ModelGraphView *out);

// Parses an inbound dictionary (MSG_TYPE 1 or 2), updates status, and
// persists successful payloads. Cancels any pending watchdog.
void model_apply_inbox(DictionaryIterator *iter);

// Arms a watchdog timer; if no response arrives before it fires, status
// becomes MODEL_STATUS_ERROR (per PLAN.md's refresh sequence).
void model_note_request_sent(void);

// Fires whenever status/data changes so the UI can redraw. Fixed-size list,
// no allocation. Each on-screen window adds itself from its appear handler
// and removes only itself from disappear, so a covered window cannot clear
// the visible window's listener. Popping a detail window restores the
// dashboard because the dashboard's appear runs again and re-adds it.
void model_add_listener(ModelListener listener);
void model_remove_listener(ModelListener listener);
