# AetherCast

A Pebble Time 2 watchapp: current conditions, a **barometric pressure graph**
(24 h back, 12 h ahead), and a **minimalist moon phase disc**.

> **Status: M8 done** — cross-platform tuning (compact/narrow-width temperature
> font, round-platform vertical+horizontal insets, B/W trend line-weight) and
> a battery pass (the minute tick timer and model listener are only subscribed
> while the dashboard is the visible window) are live, verified with screenshots
> on all seven platforms. M7 (Clay settings, touch/gesture affordances) and
> M4/M5/M6 (barograph, moon layer + host tests, conditions zone) also done. M9
> (publish to apps.repebble.com) not started. **M10** (wire protocol v3 +
> animated barograph/temperature detail screens replacing the old plain-list
> Details screen) is code-complete and build/test-verified; emulator
> screenshots and a few manual checks are still open.
> Start at **[PLAN.md](PLAN.md)**.

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
