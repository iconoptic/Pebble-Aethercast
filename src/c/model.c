#include "model.h"
#include "wire.h"
#include "lib/scale.h"
#include "lib/units.h"

_Static_assert(MODEL_GRAPH_MISLEADING_AFTER_S == SCALE_SAMPLE_PERIOD_S,
               "barograph placeholder threshold must be one hourly sample");

#define PERSIST_KEY_PAYLOAD 1
#define PERSIST_KEY_FORECAST 2
#define SCHEMA_VERSION 3
#define WATCHDOG_TIMEOUT_MS (30 * 1000)

static WeatherPayload s_payload;
static bool s_has_data;
static ForecastPayload s_forecast;
static bool s_has_forecast;
static ModelStatus s_status;
static uint8_t s_err_code;
static bool s_refreshing;
static AppTimer *s_watchdog;
static ModelListener s_listeners[MODEL_LISTENER_SLOTS];

// Indexed by ERR_CODE (0 = watch-side watchdog, 1-4 = phone-side codes) - see
// docs/design/01-data-protocol.md "Error codes".
static const char *const s_error_text[] = {
  "NO PHONE", "LOC OFF", "NO FIX", "NO NET", "BAD DATA",
};

static void prv_notify(void) {
  // Listeners only mark a layer dirty; they don't add or remove slots.
  for (int i = 0; i < MODEL_LISTENER_SLOTS; i++) {
    if (s_listeners[i]) {
      s_listeners[i]();
    }
  }
}

static ModelStatus prv_freshness_status(void) {
  int32_t age_s = (int32_t)time(NULL) - s_payload.updated_utc;
  if (age_s < 0) {
    age_s = 0;
  }
  return age_s < MODEL_STALE_AFTER_S ? MODEL_STATUS_FRESH : MODEL_STATUS_STALE;
}

static void prv_load_cache(void) {
  if (persist_exists(PERSIST_KEY_PAYLOAD)) {
    int bytes = persist_read_data(PERSIST_KEY_PAYLOAD, &s_payload, sizeof(s_payload));
    if (bytes == sizeof(s_payload) && s_payload.schema == SCHEMA_VERSION) {
      s_has_data = true;
    } else {
      persist_delete(PERSIST_KEY_PAYLOAD);
    }
  }
  if (persist_exists(PERSIST_KEY_FORECAST)) {
    int bytes = persist_read_data(PERSIST_KEY_FORECAST, &s_forecast, sizeof(s_forecast));
    if (bytes == sizeof(s_forecast) && s_forecast.schema == SCHEMA_VERSION) {
      s_has_forecast = true;
    } else {
      persist_delete(PERSIST_KEY_FORECAST);
    }
  }
}

static void prv_watchdog_fired(void *data) {
  s_watchdog = NULL;
  s_refreshing = false;
  s_status = MODEL_STATUS_ERROR;
  s_err_code = 0; // 0 = no response from phone, distinct from the phone's own ERR_CODE values
  APP_LOG(APP_LOG_LEVEL_ERROR, "model: watchdog fired, no response from phone");
  prv_notify();
}

void model_init(void) {
  s_has_data = false;
  s_has_forecast = false;
  s_status = MODEL_STATUS_EMPTY;
  prv_load_cache();
  if (s_has_data) {
    s_status = prv_freshness_status();
    units_set_system((UnitSystem)s_payload.unit_system);
    APP_LOG(APP_LOG_LEVEL_INFO, "model: loaded cache, loc=%s age=%um status=%d",
            s_payload.loc_name, model_age_minutes(), s_status);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "model: no cache found");
  }
}

bool model_has_data(void) {
  return s_has_data;
}

const WeatherPayload *model_get_payload(void) {
  return s_has_data ? &s_payload : NULL;
}

const ForecastPayload *model_get_forecast(void) {
  return s_has_forecast ? &s_forecast : NULL;
}

ModelStatus model_get_status(void) {
  return s_status;
}

uint8_t model_get_error_code(void) {
  return s_err_code;
}

const char *model_error_text(void) {
  if (s_err_code >= sizeof(s_error_text) / sizeof(s_error_text[0])) {
    return "ERROR";
  }
  return s_error_text[s_err_code];
}

bool model_is_refreshing(void) {
  return s_refreshing;
}

uint16_t model_age_minutes(void) {
  if (!s_has_data) {
    return 0;
  }
  int32_t age_s = (int32_t)time(NULL) - s_payload.updated_utc;
  if (age_s < 0) {
    age_s = 0;
  }
  return (uint16_t)(age_s / 60);
}

void model_note_request_sent(void) {
  if (s_watchdog) {
    app_timer_cancel(s_watchdog);
  }
  s_refreshing = true;
  s_watchdog = app_timer_register(WATCHDOG_TIMEOUT_MS, prv_watchdog_fired, NULL);
  // Launch refresh starts from PKJS_READY, which does not otherwise redraw.
  // Without this the misleading cache would stay on screen until the
  // payload (or the watchdog) arrived.
  prv_notify();
}

void model_add_listener(ModelListener listener) {
  if (!listener) {
    return;
  }
  int free_slot = -1;
  for (int i = 0; i < MODEL_LISTENER_SLOTS; i++) {
    if (s_listeners[i] == listener) {
      return;
    }
    if (!s_listeners[i] && free_slot < 0) {
      free_slot = i;
    }
  }
  if (free_slot >= 0) {
    s_listeners[free_slot] = listener;
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "model: listener list full");
  }
}

void model_remove_listener(ModelListener listener) {
  for (int i = 0; i < MODEL_LISTENER_SLOTS; i++) {
    if (s_listeners[i] == listener) {
      s_listeners[i] = NULL;
    }
  }
}

static int prv_recorded_now_idx(void) {
  return s_payload.press_now_idx < SCALE_N_SAMPLES ? s_payload.press_now_idx : SCALE_N_SAMPLES - 1;
}

void model_get_graph_view(ModelGraphView *out) {
  out->mode = MODEL_GRAPH_ABSENT;
  out->now_idx = -1;
  if (!s_has_data) {
    return;
  }

  int recorded = prv_recorded_now_idx();
  int32_t now = (int32_t)time(NULL);
  bool misleading = scale_recorded_now_lag_s(s_payload.press_t0_utc, (uint8_t)recorded, now)
                    >= MODEL_GRAPH_MISLEADING_AFTER_S;
  if (s_refreshing && misleading) {
    out->mode = MODEL_GRAPH_LOADING;
    out->now_idx = recorded;
    return;
  }
  if (s_status == MODEL_STATUS_ERROR) {
    out->mode = MODEL_GRAPH_REANCHORED;
    out->now_idx = scale_reanchor_now_idx(s_payload.press_t0_utc, now);
    return;
  }
  out->mode = MODEL_GRAPH_LIVE;
  out->now_idx = recorded;
}

static void prv_apply_error(DictionaryIterator *iter) {
  s_err_code = (uint8_t)wire_read_int32(iter, MESSAGE_KEY_ERR_CODE, 0);
  s_status = MODEL_STATUS_ERROR;
  APP_LOG(APP_LOG_LEVEL_ERROR, "model: error payload, ERR_CODE=%d", s_err_code);
}

// Forecast fields ride in the same dictionary as the core payload but are
// optional: a bad/missing forecast never fails the core weather update, it
// just leaves whatever forecast (if any) was already cached in place.
static void prv_apply_forecast(DictionaryIterator *iter, uint8_t schema) {
  ForecastPayload f = {0};
  f.schema = schema;
  bool ok = wire_read_bytes(iter, MESSAGE_KEY_TEMP_SERIES, f.temp_series, sizeof(f.temp_series))
         && wire_read_bytes(iter, MESSAGE_KEY_WX_SERIES, f.wx_series, sizeof(f.wx_series))
         && wire_read_bytes(iter, MESSAGE_KEY_DAILY_HI, f.daily_hi, sizeof(f.daily_hi))
         && wire_read_bytes(iter, MESSAGE_KEY_DAILY_LO, f.daily_lo, sizeof(f.daily_lo))
         && wire_read_bytes(iter, MESSAGE_KEY_DAILY_CODE, f.daily_code, sizeof(f.daily_code));
  if (!ok) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "model: forecast fields missing/bad length, keeping cached forecast");
    return;
  }
  f.daily_t0_utc = wire_read_int32(iter, MESSAGE_KEY_DAILY_T0_UTC, 0);

  s_forecast = f;
  s_has_forecast = true;
  persist_write_data(PERSIST_KEY_FORECAST, &s_forecast, sizeof(s_forecast));
}

static void prv_apply_payload(DictionaryIterator *iter) {
  WeatherPayload p = {0};
  p.schema = (uint8_t)wire_read_int32(iter, MESSAGE_KEY_SCHEMA, 0);
  if (p.schema != SCHEMA_VERSION) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "model: schema mismatch (got %d, want %d)", p.schema, SCHEMA_VERSION);
    s_status = MODEL_STATUS_ERROR;
    s_err_code = 4; // bad data
    return;
  }

  Tuple *series = dict_find(iter, MESSAGE_KEY_PRESS_SERIES);
  if (!series || series->length != sizeof(p.press_series)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "model: PRESS_SERIES bad length %d",
            series ? (int)series->length : -1);
    s_status = MODEL_STATUS_ERROR;
    s_err_code = 4;
    return;
  }
  memcpy(p.press_series, series->value->data, sizeof(p.press_series));

  p.unit_system = (uint8_t)wire_read_int32(iter, MESSAGE_KEY_UNIT_SYSTEM, UNITS_IMPERIAL);
  units_set_system((UnitSystem)p.unit_system);
  p.temp_c10 = (int16_t)wire_read_int32(iter, MESSAGE_KEY_TEMP_C10, 0);
  p.feels_c10 = (int16_t)wire_read_int32(iter, MESSAGE_KEY_FEELS_C10, 0);
  p.hi_c10 = (int16_t)wire_read_int32(iter, MESSAGE_KEY_HI_C10, 0);
  p.lo_c10 = (int16_t)wire_read_int32(iter, MESSAGE_KEY_LO_C10, 0);
  p.humidity = (uint8_t)wire_read_int32(iter, MESSAGE_KEY_HUMIDITY, 0);
  p.wind_kmh10 = (int16_t)wire_read_int32(iter, MESSAGE_KEY_WIND_KMH10, 0);
  p.wind_dir = (uint16_t)wire_read_int32(iter, MESSAGE_KEY_WIND_DIR, 0);
  p.wx_code = (uint8_t)wire_read_int32(iter, MESSAGE_KEY_WX_CODE, 0);
  p.is_day = (uint8_t)wire_read_int32(iter, MESSAGE_KEY_IS_DAY, 0);
  p.press_hpa10 = (int16_t)wire_read_int32(iter, MESSAGE_KEY_PRESS_HPA10, 0);
  p.press_now_idx = (uint8_t)wire_read_int32(iter, MESSAGE_KEY_PRESS_NOW_IDX, 24);
  p.press_t0_utc = wire_read_int32(iter, MESSAGE_KEY_PRESS_T0_UTC, 0);
  p.sunrise_utc = wire_read_int32(iter, MESSAGE_KEY_SUNRISE_UTC, 0);
  p.sunset_utc = wire_read_int32(iter, MESSAGE_KEY_SUNSET_UTC, 0);
  p.updated_utc = wire_read_int32(iter, MESSAGE_KEY_UPDATED_UTC, (int32_t)time(NULL));
  p.lat_sign = (int8_t)wire_read_int32(iter, MESSAGE_KEY_LAT_SIGN, 1);

  Tuple *loc = dict_find(iter, MESSAGE_KEY_LOC_NAME);
  if (loc) {
    strncpy(p.loc_name, loc->value->cstring, sizeof(p.loc_name) - 1);
    p.loc_name[sizeof(p.loc_name) - 1] = '\0';
  }

  s_payload = p;
  s_has_data = true;
  s_status = MODEL_STATUS_FRESH;
  persist_write_data(PERSIST_KEY_PAYLOAD, &s_payload, sizeof(s_payload));
  prv_apply_forecast(iter, p.schema);
  APP_LOG(APP_LOG_LEVEL_INFO, "model: payload stored, loc=%s temp=%ld.%ld press=%ld.%ld",
          s_payload.loc_name,
          (long)(s_payload.temp_c10 / 10), (long)labs(s_payload.temp_c10 % 10),
          (long)(s_payload.press_hpa10 / 10), (long)labs(s_payload.press_hpa10 % 10));
}

void model_apply_inbox(DictionaryIterator *iter) {
  if (s_watchdog) {
    app_timer_cancel(s_watchdog);
    s_watchdog = NULL;
  }
  s_refreshing = false;

  int32_t msg_type = wire_read_int32(iter, MESSAGE_KEY_MSG_TYPE, 0);
  if (msg_type == 2) {
    prv_apply_error(iter);
  } else if (msg_type == 1) {
    prv_apply_payload(iter);
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "model: unexpected MSG_TYPE=%d", (int)msg_type);
    return;
  }
  prv_notify();
}
