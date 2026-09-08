# Platform findings

Everything here was verified on **2026-09-05** against a live source or by running
a command on this machine. Anything unverified is marked ⚠️.

## Pebble Time 2 = the `emery` platform

`PBL_PLATFORM_EMERY` is documented as "Built for Pebble Time 2".
Source: `../vendor/pebble-building-for-every-pebble.md`.

Note the naming history: `emery` was originally defined in 2016 for the *cancelled*
Pebble Time 2. Core Devices reused the platform for the 2025/26 watch because the
display spec (200×228, 64 colour) is the same. Existing pre-2016 apps built for
`aplite`/`basalt` therefore letterbox on it until rebuilt.

Current platform roster:

| Define | Watch |
|---|---|
| `PBL_PLATFORM_APLITE` | Pebble / Pebble Steel |
| `PBL_PLATFORM_BASALT` | Pebble Time / Time Steel |
| `PBL_PLATFORM_CHALK` | Pebble Time Round |
| `PBL_PLATFORM_DIORITE` | Pebble 2 |
| `PBL_PLATFORM_EMERY` | **Pebble Time 2** |
| `PBL_PLATFORM_FLINT` | Pebble 2 Duo |
| `PBL_PLATFORM_GABBRO` | Pebble Round 2 |

## ⚠️→✅ No barometer on Pebble Time 2

This was the assumption most worth checking, and it did not survive.

From Core Devices' launch post comparison table:

| | Pebble 2 Duo (`flint`) | **Pebble Time 2** (`emery`) |
|---|---|---|
| Sensors | 6-axis IMU, compass, **barometer** | 6-axis IMU, **heart rate**, compass |

The hardware-information matrix agrees: the row listing
`6-axis IMU, Compass, Barometer` belongs to the Pebble 2 Duo column.

There is no `BarometerService` in the SDK, and no `PBL_BAROMETER` define in the
capability table.

**Consequence:** the barograph must be sourced from the network. See
[01-data-protocol.md](../design/01-data-protocol.md).

## Emery display and limits

- 200 × 228 px, 1.5", 202 PPI, 64 colours, JDI LPM015M135A
- Flat hardened glass, **touchscreen**, RGB backlight
- Max app size (code + heap): **128 kB**
- Max resource size: **256 kB**
- Star-MC1 (Cortex-M33-like) @ 240 MHz, SiFli SF32LB52J
- ~30 day rated battery life — so a chatty refresh loop is genuinely rude here

## Touch

From `../vendor/pebble-touch.md`:

- **Touch is not supported in watchfaces.** Watchapps only. This alone settles the
  watchapp-vs-watchface question for AetherCast.
- `touch_service_is_enabled()` returns `false` both when there is no touch hardware
  *and* when the user disabled touch in Settings → Display → Touch. One check covers
  both; call it from the window's `appear` handler.
- Raw stream: `touch_service_subscribe(handler, ctx)` →
  `TouchEvent_Touchdown` / `TouchEvent_PositionUpdate` / `TouchEvent_Liftoff`,
  each with `x` / `y`. `touch_service_unsubscribe()` when the window disappears —
  the sensor powers down when the last subscriber drops.
- Gesture recognizers (preferred over hand-rolling):
  `tap_recognizer_create()`, `pan_recognizer_create(cb, ctx, PanAxis_Vertical)`,
  `swipe_recognizer_create()`. Attach with `window_attach_recognizer()`; the window
  takes ownership and destroys them on unload.
- Recognizer callbacks get `RecognizerEvent_Started/Updated/Completed/Cancelled`.
- **Gotcha:** you must call `window_set_touch_bridge_disabled(window, true)` or the
  system consumes the touches before your recognizers ever see them.
- Multiple recognizers on one window are exclusive by default;
  `recognizer_set_simultaneous_with()` / `recognizer_set_fail_after()` to compose.

## Persistent storage

`../vendor/pebble-persistent-storage.md` states 4 kB per app,
`PERSIST_DATA_MAX_LENGTH` = 256 bytes per value.

⚠️ The Aug 2026 Core Devices blog post says "Apps can now use up to 1 MB of
persistent storage". The guide page has not been updated. **Design to the
documented 4 kB / 256 B** — AetherCast needs ~140 bytes, so the discrepancy is
irrelevant to us and not worth resolving.

## Toolchain (verified on this machine)

```
$ uv tool install pebble-tool
$ pebble --version
Pebble Tool v5.0.40

$ pebble sdk list
No SDKs installed yet.
Available SDKs: 4.4  4.5  4.9.127  4.9.148  4.9.169  4.17  4.33.1
```

So: `pebble-tool` installs and runs fine on Arch; latest SDK is **4.33.1**.

Upstream documents dependencies for Ubuntu and Fedora only:

- Ubuntu: `nodejs npm libsdl2-2.0-0 libglib2.0-0 libpixman-1-0 zlib1g libsndio7.0`
- Fedora: `nodejs SDL2 glib2 pixman zlib`

⚠️ Arch equivalent, **not yet verified by running the emulator**:
`sudo pacman -S --needed nodejs npm sdl2-compat glib2 pixman zlib`
plus `sndio` from the AUR if the emulator complains about libsndio.
Fallback if the emulator refuses to run: CloudPebble at
<https://cloudpebble.repebble.com/>.

## Project manifest shape (from a current official example)

`docs/reference/coredevices-example-apps/touch-thing/package.json` is a live,
PT2-era project file — more trustworthy than the older docs:

```json
{
  "name": "touch-thing",
  "keywords": ["pebble-app"],
  "pebble": {
    "displayName": "touch-thing",
    "uuid": "…",
    "sdkVersion": "3",
    "enableMultiJS": true,
    "targetPlatforms": ["gabbro", "emery"],
    "watchapp": { "watchface": false },
    "messageKeys": ["dummy"],
    "resources": { "media": [] }
  }
}
```

Note `sdkVersion` is still `"3"` even on SDK 4.x — `PBL_SDK_3` covers 3.x *and* 4.x.

## Deploying to a real watch

1. Pebble mobile app → Devices → ⋯ → **Enable Dev Connect** → sign in with GitHub
2. `pebble login`
3. `pebble install --cloudpebble --logs`

Local emulator alternative: `pebble install --emulator emery`.
