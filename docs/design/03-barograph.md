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
3. **Past segment** (`i = 0 … 24`): solid polyline in the trend colour, 2 px on
   colour platforms. On B/W platforms the width itself carries the trend — 1 px
   for `STEADY`, 2 px for any rise or fall — since there is no hue to use.
4. **Forecast segment** (`i = 24 … 35`): 1 px, same trend colour, dashed by drawing
   every other sample-to-sample segment. Visually subordinate — it is a model, not
   a measurement.
5. **Now divider** at `x(24)`: vertical 1 px line, `GColorWhite`, full plot height.
6. **Current-value dot**: 3 px filled circle at `(x(24), y(series[24]))`.

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
| Only cached data | Plot is drawn unchanged from the cached series; staleness is signalled by the header dot and age, not by dimming the plot |
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
