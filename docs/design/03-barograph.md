# Barograph

The reason this app exists.

## Input

36 samples of MSL pressure in tenths of hPa, hourly, from `PRESS_SERIES`:

- indices `0 … 23` — the last 24 hours (index 23 is the most recent past hour)
- index `24` = `PRESS_NOW_IDX` — now
- indices `25 … 35` — the next 11 hours of forecast

`PRESS_T0_UTC` is the epoch of index 0, so hour labels are derivable without
sending any extra time data.

## Vertical scaling

Naive min/max scaling makes a dead-calm day look like a hurricane. Clamp the span:

```
lo_raw = min(series)      hi_raw = max(series)
span   = hi_raw − lo_raw
if span < MIN_SPAN:                     // MIN_SPAN = 80  (= 8.0 hPa)
    mid = (hi_raw + lo_raw) / 2
    lo  = mid − MIN_SPAN/2
    hi  = mid + MIN_SPAN/2
else:
    pad = span / 10                     // 10% headroom
    lo  = lo_raw − pad
    hi  = hi_raw + pad

y(v) = plot.y + plot.h − 1 − ((v − lo) * (plot.h − 1)) / (hi − lo)
```

Integer throughout. `hi > lo` is guaranteed by the `MIN_SPAN` clamp, so there is no
divide-by-zero branch to write. Note the `plot.h − 1` (not `plot.h`): dividing by
`plot.h` would let `y` land exactly one row past the bottom of the rect when
`v == lo`, contradicting the `y < plot.y + plot.h` assertion below - see
`src/c/lib/scale.c`.

8.0 hPa is chosen because it is roughly the range that distinguishes "settled" from
"a front is coming" — narrower and normal noise dominates, wider and real changes
flatten out.

## Horizontal mapping

Plot rect on `emery`: `x = 8 … 191` (184 px), `y = 116 … 190` (74 px) — the zone
rect inset by 8 px horizontally.

184 px / 35 intervals = 5.257 px per sample. Use fixed point:

```
x(i) = plot.x + (i * (plot.w - 1) * 256 / (N - 1) + 128) / 256
```

so rounding error never accumulates and the last sample lands exactly on `plot.x + plot.w - 1`.

## Rendering

```
      hPa
 1010 ┊·······································┊   ← grid line at a round hPa value
      ┊         ╭──╮                 ┃         ┊
      ┊    ╭────╯  ╰───╮             ┃         ┊
 1005 ┊···─╯···········╰────╌╌╌╌╌╌╌╌╌┃╌╌╌╌╌╌╌··┊
      ┊                              ┃         ┊
      └──────────────────────────────┸─────────┘
      −24h            −12h          now      +12h
```

Draw order:

1. **Grid.** Horizontal dotted lines at the round hPa values inside `[lo, hi]`
   (step chosen from 2 / 5 / 10 hPa — 20 / 50 / 100 in the tenths the code works
   in — so that 2–4 lines appear). `GColorDarkGray`, drawn with
   `graphics_draw_pixel` every 3 px — the SDK has no dashed-line primitive.
2. **Fill under the past curve.** Vertical `graphics_draw_line` per x column, in a
   tint of the trend colour (each RGB channel of the packed `.argb` darkened one
   step). Gives the graph weight on a 200 px display without needing
   anti-aliasing.
3. **Past segment** (`i = 0 … now_idx`): solid polyline in the trend colour, 2 px on
   colour platforms. On B/W platforms the width itself carries the trend — 1 px
   for `STEADY`, 2 px for any rise or fall — since there is no hue to use.
   When the divider is omitted (`now_idx < 0`) every sample is past.
4. **Forecast segment** (`i = now_idx … 35`): 1 px, same trend colour, dashed by drawing
   every other sample-to-sample segment. Visually subordinate — it is a model, not
   a measurement. Empty when the divider is omitted or sits on the last sample.
5. **Now divider** at `x(now_idx)`: vertical 1 px line, `GColorWhite`, full plot height.
   On a live payload `now_idx` is `PRESS_NOW_IDX` (24). After a failed refresh it
   is the re-anchored index below, and it is omitted entirely when that index
   falls off the end of the series.
6. **Current-value dot**: 3 px filled circle at `(x(now_idx), y(series[now_idx]))`.
   Omitted together with the divider.

## Stale cache on launch, and a failed refresh

The cached series is what the watch can draw before the phone answers. The
now-divider in that cache is wherever it was at the last fetch (`PRESS_NOW_IDX`,
normally 24), not wherever the wall clock is now. Drawn unchanged, a cache from
this morning looks like a live barograph.

The divider is one hourly sample wide. It is on the wrong hour once

```
now - (press_t0_utc + press_now_idx * 3600) >= MODEL_GRAPH_MISLEADING_AFTER_S
```

`MODEL_GRAPH_MISLEADING_AFTER_S` is 3600, one `SCALE_SAMPLE_PERIOD_S`. Anything
younger still lands on the correct sample, so it renders immediately — that is
the common case, opening the app again a few minutes later. The 20-minute
header-dot threshold (`MODEL_STALE_AFTER_S`) is deliberately shorter: the dot
may already be yellow while the curve is still honest.

While that inequality holds **and** a refresh is in flight
(`model_is_refreshing()`), the dashboard plot zone and the barograph detail
chart draw a placeholder instead of the curve: a dotted midline in the same
3 px cadence as the grid, and the word `UPDATING`. On black-and-white platforms
both are white; the word is what carries it, not the colour. The pressure value
and the 3 h trend word are omitted for the same reason. They are computed from
the recorded now-sample (`press_hpa10` is the reading at fetch time; the trend
is `series[now] − series[now−3]` there), so printing them next to the
placeholder would still assert the stale hour. `model_note_request_sent()`
notifies listeners, so the placeholder replaces the curve as soon as the launch
`REQUEST` goes out, not only when the payload comes back.

If the refresh ends in `MODEL_STATUS_ERROR` (no phone, no fix, watchdog, …) the
placeholder comes down and the cached curve is shown. The divider is moved to
the sample that actually contains the wall clock:

```
now_idx = (now - press_t0_utc) / 3600
```

implemented by `scale_reanchor_now_idx()`:

| `now` vs the series | Divider |
|---|---|
| `now < press_t0_utc` (clock skew) | Clamped to sample 0 |
| Inside the series | That sample. Past/forecast split follows it, so hours that were forecast at fetch time and are now behind the divider draw solid |
| At or past the end of sample 35 | No divider and no current-value dot. The whole curve draws as the past segment |

The series holds 12 h of forecast past the original now (index 24 through 35),
so a cache up to 11 h old still places the divider on a real sample, and it
stays on sample 35 until the clock reaches 12 h past the recorded now. Past
that, pinning the divider on sample 35 would call a forecast hour "now" when
the clock is already beyond it, so the divider is left off and the red header
text (`NO PHONE`, `NO FIX`, …) is the explanation. The label then prints the
last sample and the 3 h trend into it, which is the newest hour the cache has,
not a claim about the present.

A live payload (`MODEL_GRAPH_LIVE`) is unchanged: divider at `press_now_idx`,
label value `press_hpa10`.

## Trend

Computed from the 3-hour change, which is the standard meteorological convention:

```
delta3 = series[NOW_IDX] − series[NOW_IDX − 3]      // tenths hPa
```

| |Δ₃| (hPa) | Word | Colour |
|---|---|---|
| ≥ 3.0 falling | `FALLING FAST` | `GColorRed` |
| 1.0 – 3.0 falling | `FALLING` | `GColorOrange` |
| < 1.0 either way | `STEADY` | `GColorLightGray` |
| 1.0 – 3.0 rising | `RISING` | `GColorPictonBlue` |
| ≥ 3.0 rising | `RISING FAST` | `GColorBlueMoon` |

Label row above the plot: the current value is left-aligned in white, the delta and
trend word right-aligned in the trend colour — `1006.6 hPa` … `-2.1/3h FALLING`.
No ▲/▼ glyph: the sign of the delta already carries it, and the repo avoids
non-ASCII glyphs whose coverage in the system fonts is unverified.

Falling pressure gets warm colours because a falling barometer is the thing you
want to notice.

## Units

Pressure is always rendered in hPa — `v / 10`, e.g. `1006.6 hPa`. The Clay unit
setting (`UNIT_SYSTEM`) covers temperature and wind only; the plot itself is
unitless, so adding inHg (`v * 0.02953 / 10` → `29.72 inHg`) or mmHg
(`v * 0.750062 / 10` → `755 mmHg`) later is a label-formatting change in
`lib/units.c` plus one more radio group, and nothing else.

## Degenerate cases and what they must do

| Case | Required behaviour |
|---|---|
| All 36 samples identical | Flat line dead centre, `STEADY`, no divide-by-zero |
| Series contains a forward-filled gap | Renders as a flat run; no marker (JS already handled it) |
| Extreme range (e.g. 40 hPa over 24 h) | 10% padding, curve stays inside the rect |
| Cached data, divider still on the right hour | Plot is drawn from the cached series. Staleness is the header dot and age. A cache younger than one sample (`MODEL_GRAPH_MISLEADING_AFTER_S`) always takes this path, including while a refresh is in flight |
| Cached now is ≥ 1 sample behind, refresh in flight | Loading placeholder in the plot zone (dotted midline + `UPDATING`). The value/trend label is omitted — it is computed from that same sample |
| Refresh failed, series still covers now | Cached curve, now-divider re-anchored (see below). Label value is the sample under the divider, so it matches the dot |
| Refresh failed, series does not cover now | Whole curve drawn as the past segment, no now-divider and no current-value dot. Label shows the last sample. The header's red `model_error_text()` says why |
| No payload at all | Empty plot rect outline in `GColorDarkGray`, no label row |
| `PRESS_NOW_IDX` out of range | Clamped to `SCALE_N_SAMPLES - 1` before use |
| Non-`emery` platform | Plot rect derived from unobstructed bounds; sample count unchanged |

The first three are cases in `tests/test_scale.c`, and each has a matching preset
in `tools/fake_payload.js` (`flat`, `missing`, `sawtooth`, plus `rising`/`falling`
for the trend thresholds).

## Shared chart layer

Everything in "Rendering" above is implemented once, in `src/c/layers/chart_layer.c`,
as `chart_layer_draw(ctx, rect, ChartSpec)` — not duplicated between the
dashboard's compact plot and the barograph/temperature detail screens
(`docs/design/04-ui-layout.md`). `barograph_layer.c` builds a `ChartSpec` from the
trend classification (colour, line width, grid step) and calls it; `baro_detail.c`
and `temp_detail.c` do the same at full-screen size, `temp_detail.c` passing a
fixed `GColorOrange` instead of a trend colour (temperature has no equivalent
"trend" concept in this app) and `SCALE_MIN_SPAN_TEMP` instead of `SCALE_MIN_SPAN`.
A `ChartSpec.cursor_idx >= 0` draws an extra dashed vertical tick + ringed dot at
that sample, used by both detail screens' scrub-cursor feature; the dashboard's
compact plot never sets it (`cursor_idx = -1`).

`lib/scale.c` generalizes the same way: `scale_bounds`/`scale_series`/
`scale_grid_step` are unchanged thin wrappers (`MIN_SPAN = 80`, grid step
candidates `{20, 50, 100}`) kept for the barograph's existing call sites and
tests; `scale_bounds_ex`/`scale_series_ex`/`scale_grid_step_ex` take an explicit
min-span (or grid-step candidate array) parameter so `temp_detail.c` can reuse the
identical clamping/mapping maths with `SCALE_MIN_SPAN_TEMP = 50` (5.0°C) instead of
8.0 hPa — a temperature series has a different natural "is this actually varying"
threshold than a pressure series, but the same divide-by-zero-safe clamp shape.

## Host testing

`lib/scale.c` has no `pebble.h` dependency — it takes the series, the plot
rectangle as four ints, and fills an output array of `(x, y)` pairs. So the entire
scaling and mapping layer is verifiable with `gcc` and `assert()` on this machine,
and the Pebble-side layer code reduces to "call scale, then draw lines".

Assertions:

- every output `y` satisfies `plot.y <= y < plot.y + plot.h`, for all five preset
  series and for 10 000 random series
- `x[0] == plot.x` and `x[35] == plot.x + plot.w − 1`
- `x` is non-decreasing
- flat input → all `y` equal, and equal to the vertical centre ±1
- `MIN_SPAN` clamp engages exactly when `hi_raw − lo_raw < 80`
- `scale_reanchor_now_idx`: a fresh cache (now on sample 24) stays at 24,
  including 59 minutes into that hour; 3 h later is 27; 11 h later is 35;
  12 h later (and anything past the last sample) is −1 so the divider is
  omitted; `now < press_t0_utc` clamps to 0
