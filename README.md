# AetherCast

A Pebble Time 2 watchapp: current conditions, a **barometric pressure graph**
(24 h back, 12 h ahead), and a **minimalist moon phase disc**.

> **Status: M10 done** — wire protocol v3 (36-sample temperature + WMO series,
> 4-day outlook) and the animated barograph/temperature detail screens are
> code-complete, `emery`-verified on **both emulator and real Pebble Time 2
> hardware** (side-loaded over the phone's Developer Connection): dashboard,
> both detail screens, cursor scrub, all five synthetic data presets, and all
> five `ERR_CODE` states all screenshotted. M1-M9 also done except M9 itself
> (publish to apps.repebble.com — not started). Low-priority/best-effort items
> still open: `gabbro`/`chalk` bezel-fit polish, a full `ERR_CODE` sweep on the
> B/W platforms (`aplite`/`diorite`/`flint`) beyond the one already-confirmed
> status-dot-shape check — `emery` is the only platform the project actually
> targets.
> Start at **[PLAN.md](PLAN.md)**.

## Privacy

The "follow me" pressure history is a trail of where the phone has been,
stored only in PebbleKit JS `localStorage` (rounded to about 0.01°, kept
about 30 h). It is only as fine-grained as how often the app is opened: a
place is recorded when a refresh runs, and the tick marks when the new
place was first seen, not when you arrived. It never goes to the watch and
it never goes to a server of ours. Open-Meteo receives coordinates, which it already did for the current
fix; the watch receives the weather and a bit mask of *when* the place
changed, not the coordinates. "This location only", manual location, and
"Clear location history" in settings stop or delete that trail.
**Background location** (off unless you turn it on) is the same refresh on a
timer: the watch opens, the phone records a fix, and the watch closes. The
trail still stays on the phone.

## Why this exists

Off-the-shelf Pebble weather apps show you a temperature. This one is built around
the barograph — the pressure trend is the part that actually tells you what the
weather is about to do — with the moon phase as a quiet second signal.

## The finding that shaped the design

The Pebble Time 2 **has no barometer** (that sensor shipped in the Pebble 2 Duo).
So pressure comes from Open-Meteo via the phone, which turns out to be better: you
get a full 24 h of history the moment you install the app, no elevation artefacts,
and a 12 h forward projection an on-wrist sensor could never produce.

Details and sources: [docs/research/00-platform-findings.md](docs/research/00-platform-findings.md).

## Documents

| | |
|---|---|
| [PLAN.md](PLAN.md) | Architecture, diagrams, milestones, risks — **read this first** |
| [docs/research/00-platform-findings.md](docs/research/00-platform-findings.md) | Verified hardware/SDK facts, with sources |
| [docs/design/01-data-protocol.md](docs/design/01-data-protocol.md) | Open-Meteo query + AppMessage wire format |
| [docs/design/02-moon-phase.md](docs/design/02-moon-phase.md) | Phase maths and scanline rendering |
| [docs/design/03-barograph.md](docs/design/03-barograph.md) | Scaling, trend classification, edge cases |
| [docs/design/04-ui-layout.md](docs/design/04-ui-layout.md) | Wireframes, palette, input model |

## Offline references

- `docs/vendor/` — 43 upstream pages (Pebble SDK guides, C API, Open-Meteo docs)
  mirrored as Markdown. Refresh with `./tools/fetch-docs.sh`.
- `docs/reference/` — shallow clones of `coredevices/example-apps` and
  `pebble-examples/pebblekit-js-weather`.

## Getting started

```bash
# Arch — emulator dependencies (see PLAN.md §10 for the caveat)
sudo pacman -S --needed nodejs npm sdl2-compat glib2 pixman zlib

# Toolchain — pebble-tool v5.0.40 is already installed via uv
uv tool install pebble-tool
pebble sdk install 4.33.1

pebble build
pebble install --emulator emery
```

## Licence / attribution

Weather data by [Open-Meteo.com](https://open-meteo.com/), CC BY 4.0, free
non-commercial tier. Attribution is rendered on the barograph detail screen.
