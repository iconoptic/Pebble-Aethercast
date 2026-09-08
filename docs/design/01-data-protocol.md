# Data protocol

## Why the network provides the pressure

Pebble Time 2 has no barometer ([findings](../research/00-platform-findings.md)).
The barograph is therefore built from Open-Meteo's mean-sea-level pressure field.

This turns out to be a feature rather than a consolation prize:

| | On-wrist sensor (hypothetical) | Open-Meteo model |
|---|---|---|
| History | only since app install, lost on reset | always 24 h, instantly, on a new watch |
| Elevation artefacts | huge — walking upstairs looks like a storm | none, MSL-reduced |
| Forecast | impossible | 12 h ahead |
| Battery | continuous sensor polling | one HTTPS call per refresh, on the phone |

The honest trade-off is that it is *regional model* pressure, not *your* pressure.
For "is a front coming through", which is what a barograph is actually for, the
model is the better signal.

## Upstream request

```
GET https://api.open-meteo.com/v1/forecast
  ?latitude={lat}
  &longitude={lon}
  &current=temperature_2m,apparent_temperature,relative_humidity_2m,is_day,
           weather_code,surface_pressure,pressure_msl,wind_speed_10m,wind_direction_10m
  &hourly=pressure_msl,temperature_2m,weather_code
  &daily=temperature_2m_max,temperature_2m_min,sunrise,sunset,weather_code
  &past_hours=24
  &forecast_hours=12
  &past_days=1
  &forecast_days=4
  &timezone=auto
  &timeformat=unixtime
```

No API key. Free tier: non-commercial, < 10 000 calls/day, no per-variable cost -
adding `temperature_2m`/`weather_code` to `hourly` and `daily` didn't change the
request count. `forecast_days=4` (was 2) feeds the temperature detail screen's
4-day outlook strip; `past_hours`/`forecast_hours` apply uniformly to every
`hourly` variable, so `hourly.temperature_2m`/`hourly.weather_code` still come
back as 36 samples each, same as `pressure_msl`. The app fetches once per
launch (when PKJS signals ready) plus on each explicit user refresh, so real usage
is a handful of calls a day.

### Verified response shape

Run live on 2026-09-05 for 39.7392, −104.9903:

```json
{
  "timezone": "America/Denver",
  "utc_offset_seconds": -21600,
  "elevation": 1599,
  "current": {
    "time": 1788667200, "interval": 900,
    "temperature_2m": 24.2, "apparent_temperature": 21.6,
    "relative_humidity_2m": 31, "is_day": 0, "weather_code": 0,
    "surface_pressure": 840.3, "pressure_msl": 1006.6,
    "wind_speed_10m": 10.9, "wind_direction_10m": 107
  },
  "hourly": {
    "time": [1788580800, …, 1788706800],
    "pressure_msl": [1003.7, 1003.9, 1004.4, …, 1009.6]
  },
  "daily": {
    "time": [1788501600, 1788588000, 1788674400],
    "temperature_2m_max": [35, 34.8, 34.2],
    "temperature_2m_min": [14.8, 15.6, 16.6],
    "sunrise": [...], "sunset": [...]
  }
}
```

Confirmed facts:
- `hourly.time.length === 36` — `past_hours` and `forecast_hours` control the hourly
  block independently of `past_days`/`forecast_days`, which control `daily`.
- Total payload ~1.8 kB.
- `surface_pressure` (840.3 hPa at 1599 m) is station pressure and is **not** what
  we graph. `pressure_msl` (1006.6) is the meteorologically comparable value and
  is the only one sent over the wire; `surface_pressure` is requested but unused.
- `timezone` string gives a free location label. `America/Denver` → `DENVER`
  (take the substring after the last `/`, replace `_` with space, uppercase).
  Zero extra requests, no reverse-geocoding provider, no key, no rate limit.

### Picking "today" from `daily`

With `past_days=1&forecast_days=2` the daily arrays have 3 entries. Today is the
index `i` where `daily.time[i] <= (now + utc_offset_seconds) < daily.time[i] + 86400`.
Do not assume index 1.

## Wire format (phone → watch)

`messageKeys` in `package.json`. One dictionary per update; the core weather
fields and the forecast fields (temperature/outlook) travel in the same
dictionary, not separate messages.

| Key | Type | Unit / encoding | Notes |
|---|---|---|---|
| `MSG_TYPE` | uint8 | 1 = payload, 2 = error | always present |
| `SCHEMA` | uint8 | currently 3 | bump to invalidate cache (both persist keys, see below) |
| `UNIT_SYSTEM` | uint8 | 0 = imperial, 1 = metric | from Clay settings, M7; applied on the watch at render time |
| `TEMP_C10` | int16 | tenths °C | |
| `FEELS_C10` | int16 | tenths °C | |
| `HI_C10` | int16 | tenths °C | today's max |
| `LO_C10` | int16 | tenths °C | today's min |
| `HUMIDITY` | uint8 | % | |
| `WIND_KMH10` | int16 | tenths km/h | |
| `WIND_DIR` | uint16 | degrees, 0–359 | direction wind is coming *from* |
| `WX_CODE` | uint8 | WMO code | see table below |
| `IS_DAY` | uint8 | 0/1 | day vs night icon |
| `PRESS_HPA10` | int16 | tenths hPa | current, e.g. 10066 |
| `PRESS_SERIES` | bytes[72] | 36 × int16 little-endian, tenths hPa | |
| `PRESS_NOW_IDX` | uint8 | 24 | index of the now-divider |
| `PRESS_T0_UTC` | int32 | epoch seconds | time of sample 0, shared by `TEMP_SERIES`/`WX_SERIES` |
| `TEMP_SERIES` | bytes[72] | 36 × int16 little-endian, tenths °C | hourly temperature, same 36-sample/1h grid as `PRESS_SERIES`; barograph detail screen |
| `WX_SERIES` | bytes[36] | 36 × uint8, WMO code | hourly weather code, same grid |
| `DAILY_HI` | bytes[8] | 4 × int16 little-endian, tenths °C | today + next 3 days' high |
| `DAILY_LO` | bytes[8] | 4 × int16 little-endian, tenths °C | today + next 3 days' low |
| `DAILY_CODE` | bytes[4] | 4 × uint8, WMO code | today + next 3 days |
| `DAILY_T0_UTC` | int32 | epoch seconds | start-of-day timestamp for `DAILY_*[0]`; direct passthrough of Open-Meteo's `daily.time[todayIdx]`, no `utc_offset_seconds` math (same convention as `SUNRISE_UTC`/`SUNSET_UTC`/`PRESS_T0_UTC`) |
| `SUNRISE_UTC` | int32 | epoch seconds | |
| `SUNSET_UTC` | int32 | epoch seconds | |
| `UPDATED_UTC` | int32 | epoch seconds | staleness reference |
| `LOC_NAME` | cstring | ≤ 24 bytes incl. NUL | |
| `LAT_SIGN` | int8 | +1 / −1 | southern hemisphere moon mirroring |
| `ERR_CODE` | uint8 | only when `MSG_TYPE=2` | |

Total ≈ 280 bytes of values (core + forecast fields) plus dictionary overhead
across ~27 tuples. `app_message_open(1024, 128)` still comfortably covers it.

The forecast fields (`TEMP_SERIES`/`WX_SERIES`/`DAILY_*`) are stored under a
separate persist key (`PERSIST_KEY_FORECAST`) from the core weather fields
(`PERSIST_KEY_PAYLOAD`) because Pebble's persistent storage caps each key at
256 bytes ([`docs/vendor/capi-storage.md`](../vendor/capi-storage.md)) and the
combined struct would exceed that. A malformed/missing forecast in an inbound
dictionary never fails the core weather update — it just leaves whichever
forecast (if any) was already cached in place, via `model_get_forecast()`
returning `NULL` until one arrives.

### Watch → phone

| Key | Type | Notes |
|---|---|---|
| `REQUEST` | uint8 | 1 = fetch now |

### Phone → watch handshake

| Key | Type | Notes |
|---|---|---|
| `PKJS_READY` | uint8 | Sent once by PKJS on its `ready` event. The watch must not send `REQUEST` before receiving this — see `docs/vendor/pebble-comm-advanced.md` "Waiting for PebbleKit JS". Receiving it triggers the watch's first `REQUEST`. |

### Error codes

| Value | Meaning | Source |
|---|---|---|
| 0 | No response from the phone before the 30 s watchdog | watch (`model.c`) |
| 1 | Location permission denied | PKJS |
| 2 | Location timeout / no fix | PKJS |
| 3 | HTTP / network failure | PKJS |
| 4 | Bad response (parse failure, or schema/length assertion failed) | PKJS or watch |

The watch stores the code (`model_get_error_code()`) and logs it. `model_error_text()`
maps it to a short display string (`"NO PHONE"`, `"LOC OFF"`, `"NO FIX"`,
`"NO NET"`, `"BAD DATA"`), which the dashboard header shows in red in place of
the age text while `model_get_status() == MODEL_STATUS_ERROR`. Cached data
stays on screen either way.

## Encoding rules

1. **The wire is always metric integers.** °C×10, km/h×10, hPa×10. Unit preference
   is applied on the watch at render time, so switching to °F/inHg in settings is
   instant and works offline.
2. **`PRESS_SERIES`/`TEMP_SERIES`/`WX_SERIES` are fixed length.** JS truncates each
   `hourly.*` array to 36 and pads by repeating the last sample if it is short. The
   C side never allocates, and rejects (keeps the cached forecast/payload) if a
   tuple isn't exactly the expected byte length. `DAILY_HI`/`DAILY_LO`/`DAILY_CODE`
   are similarly fixed at 4 entries; if Open-Meteo's `daily` arrays are shorter
   than `todayIdx + 4` (shouldn't happen at `forecast_days=4`, but not assumed),
   `sliceOutlook()` pads by repeating the last available day rather than failing.
3. **`null` handling.** Open-Meteo can emit `null` for a variable in a gap. JS
   forward-fills from the previous sample; if sample 0 is null it back-fills from
   the first non-null. If an entire hourly series (`pressure_msl`, `temperature_2m`,
   or `weather_code`) is null, send `MSG_TYPE=2, ERR_CODE=4`.
4. **int16 range check.** Tenths hPa spans roughly 8700–11000, safely inside int16.
   Temperature in tenths °C spans −900…+600. No overflow path exists, so no
   overflow handling is written.

## WMO weather codes → icon buckets

`lib/wmo.c` maps the code to one of nine icon buckets, day/night variants where
meaningful.

| Codes | Bucket |
|---|---|
| 0 | clear |
| 1, 2 | partly cloudy |
| 3 | overcast |
| 45, 48 | fog |
| 51, 53, 55, 56, 57 | drizzle |
| 61, 63, 65, 66, 67, 80, 81, 82 | rain |
| 71, 73, 75, 77, 85, 86 | snow |
| 95 | thunderstorm |
| 96, 99 | thunderstorm + hail |

Anything unmapped falls back to "overcast" rather than showing nothing.
