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

The past 24 h is the pressure along the user's own path, not the history of
wherever they happen to be standing: the phone remembers where it was, and
each past hour is filled from Open-Meteo at that place. Because the series is
`pressure_msl` (sea-level reduced), a Denver sample and a Kansas City sample
are directly comparable, which is what makes the stitch meaningful. The
future 12 h stays the forecast for the current location — the app cannot know
where the user is going. A fixed manual location, or the "This location only"
setting, keeps the old behaviour: the whole graph is one place.

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

## Several locations in one request

The past half of the barograph is stitched from the user's trail
(`src/pkjs/trail.js`), so one refresh may ask for more than one coordinate.
Open-Meteo accepts comma-separated `latitude` and `longitude` lists of equal
length. Verified live on 2026-10-02 for Denver (`39.74,-104.99`) and Kansas
City (`39.10,-94.58`), `past_hours=24&forecast_hours=12&timeformat=unixtime`:

- One coordinate returns a JSON **object**, the shape above.
- Two coordinates return a JSON **array** of those objects. A trimmed copy is
  `tests/fixtures/openmeteo-multi.json`.
- `hourly.time` was identical in both elements (36 samples, step 3600 s) even
  though the timezones differed (`America/Denver` vs `America/Chicago`).
  Index 24 was `1790924400` (2026-10-02 07:00 UTC), the hour that contained
  the request (07:22 UTC). `floor(now / 3600) * 3600` is that index, and
  index 0 is 24 hours earlier.
- The service snaps coordinates onto its grid (`39.74` came back as
  `39.746895`). The second element carried `location_id: 1`; the first
  omitted it. Array order matched the request this time, but the app does
  not trust that: it pairs each result to the coordinate it asked for by
  distance. A coordinate with no result within 50 km fails the *stitch*,
  not the refresh.
- Each coordinate counts as one free-tier API call, even though they share a
  single HTTP request. The app sends at most 6, current location first.

The current location is always requested (conditions, daily, and the forecast
half). Another place is added only when one of its past hours is not already
in the on-phone pressure cache. Resolved past hours are stored as
`hour epoch → {lat, lon, pressure}` and reused on the next refresh, so a
later Open-Meteo revision of the same place cannot move history the graph
has already shown. The current hour and the forecast are never cached. A
null sample is not cached either, so a gap can fill in later. Cached hours
older than 30 h are dropped.

| Refresh | API calls |
|---|---|
| Before this change, every refresh | 1 |
| Follow me, one place, or every other place already cached | 1 |
| Follow me, N places with an unresolved past hour | N (2 ≤ N ≤ 6), one HTTP request |
| Manual location, or "This location only" | 1, and the trail is not read or extended |

If stitching fails for any reason — a coordinate with no result within 50 km,
a place whose `hourly.pressure_msl` is not an array, or an exception while
planning or packing — the phone sends the plain single-place payload for the
current location. That dict has no `PLACE_CHANGE` mask and no `PRESS_DELTA3`.
It is not an error, as long as the current location's forecast is usable.
`plan.coords[0]` is that fix, and a successful alignment puts its object
first; when alignment itself fails, the phone picks the result nearest the
current fix from the body it already has, so the fallback is not a second
HTTP request and does not change what was sent to Open-Meteo. Only a current
location that cannot be found in that body is refetched on its own. A
`localStorage` failure while recording the fix or the pressure cache is
logged and ignored; the in-memory trail is what this refresh uses, and the
watch still receives weather instead of waiting out the 30 s watchdog.

## Wire format (phone → watch)

`messageKeys` in `package.json`. One dictionary per update; the core weather
fields and the forecast fields (temperature/outlook) travel in the same
dictionary, not separate messages.

| Key | Type | Unit / encoding | Notes |
|---|---|---|---|
| `MSG_TYPE` | uint8 | 1 = payload, 2 = error | always present |
| `SCHEMA` | uint8 | currently 3 | left at 3 on purpose when the place-change mask was added; see below |
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
| `PLACE_CHANGE` | bytes[5] | 36 bits, little-endian | **optional.** Bit `i` is set when slot `i`'s place differs from slot `i-1`. Absent (or the wrong length) means no moves: the watch draws no ticks. All-zero masks are not sent. |
| `PRESS_DELTA3` | int16 | tenths hPa | **optional.** Sent only when a place change falls inside the 3 h trend window. It is the current location's own `pressure[now] − pressure[now−3]`, so the step is not classified as weather. Absent means "compute it from `PRESS_SERIES`", which is what a single-place series already is. `0` is a real override and is sent. |
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

### Why `SCHEMA` stayed at 3

`WeatherPayload` was 133 bytes. `PLACE_CHANGE` (5) and `press_delta3` (2, the
persisted form of `PRESS_DELTA3`) bring it to **140**. `ForecastPayload` is
unchanged at 133. Both sit well under the 256-byte key limit — the core
payload was not close to it; the two structs *together* are what used to
overflow one key, which is why they are split.

`SCHEMA` was not bumped:

- Both new keys are optional. A dictionary without them is still a valid v3
  payload, and the watch treats that as "no place changes, trend from the
  series", which is today's behaviour.
- `persist_read_data` requires the stored byte count to equal
  `sizeof(WeatherPayload)`. An older 133-byte core cache fails that check and
  is discarded (it was not a stitched series). Bumping `SCHEMA` would also
  discard the forecast cache, whose layout did not change.
- `PRESS_SERIES`, `PRESS_NOW_IDX`, and `PRESS_T0_UTC` keep their meaning and
  size. The watch-side series format did not change.

### Watch → phone

| Key | Type | Notes |
|---|---|---|
| `REQUEST` | uint8 | 1 = fetch now |

> **PKJS-side gotcha (cost a real-device bug at M10):** in
> `Pebble.addEventListener('appmessage', function (e) {...})`, `e.payload` is
> keyed by the **symbolic key name** (e.g. `"REQUEST"`), not the numeric
> `MessageKeys.REQUEST` id used everywhere on the *outbound* side
> (`Pebble.sendAppMessage`, and all C code). `e.payload[MessageKeys.REQUEST]`
> is therefore always `undefined` — read `e.payload.REQUEST` instead. This
> only breaks on a real round trip; `tools/fake_payload.js`/
> `FAKE_PAYLOAD_PRESET` sends straight from PKJS and never receives an
> inbound `appmessage`, so it gives no coverage for this — see PLAN.md §8.

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

`src/pkjs/index.js` races `navigator.geolocation.getCurrentPosition()` against
its own `GEOLOCATION_GUARD_MS` (20 s) `setTimeout`, since real-device
PebbleKit JS geolocation isn't guaranteed to honour its native `timeout`
option and can otherwise hang indefinitely with no `ERR_CODE` sent at all —
if the guard fires first it sends `ERR_CODE 2` itself.

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
