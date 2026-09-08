#!/usr/bin/env python3
"""Render static PNG mockups of AetherCast dashboard layout variants.

Draws at native `emery` resolution (200x228) then upscales with nearest-
neighbour so the pixel grid stays crisp for review. Not part of the watch
build -- purely a design-review aid, see docs/design/mockups/.

Usage: python3 tools/render_mockups.py
"""
import math
import os

from PIL import Image, ImageDraw, ImageFont

W, H = 200, 228
SCALE = 4
OUT_DIR = os.path.join(os.path.dirname(__file__), "..", "docs", "design", "mockups")

# Pebble 64-colour palette (approximate RGB for named GColors used in the design doc)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
LIGHT_GRAY = (170, 170, 170)
DARK_GRAY = (85, 85, 85)
ORANGE = (255, 85, 0)
RED = (255, 0, 0)
PICTON_BLUE = (85, 170, 255)
BLUE_MOON = (85, 85, 255)
PASTEL_YELLOW = (255, 255, 170)
OXFORD_BLUE = (0, 0, 60)
GREEN = (0, 255, 0)
YELLOW = (255, 255, 0)

FONT_DIR = "/usr/share/fonts/TTF"


def font(size, bold=True):
    name = "DejaVuSans-Bold.ttf" if bold else "DejaVuSans.ttf"
    return ImageFont.truetype(os.path.join(FONT_DIR, name), size)


F_TEMP = font(30)
F_BOLD18 = font(15)
F_REG18 = font(15, bold=False)
F_REG14 = font(11, bold=False)
F_REG12 = font(10, bold=False)


def text_r(draw, xy, s, fnt, fill, anchor="ra"):
    """Right/given-anchor text helper (Pillow anchor semantics)."""
    draw.text(xy, s, font=fnt, fill=fill, anchor=anchor)


def draw_moon(draw, cx, cy, r, phase, lit=PASTEL_YELLOW, dark=OXFORD_BLUE):
    """Scanline moon per docs/design/02-moon-phase.md algorithm."""
    c = math.cos(2 * math.pi * phase)
    draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=dark, outline=LIGHT_GRAY)
    for dy in range(-r, r + 1):
        half = math.isqrt(max(0, r * r - dy * dy))
        if half == 0:
            continue
        if phase < 0.5:
            x0, x1 = half * c, half
        else:
            x0, x1 = -half, -half * c
        if x1 <= x0:
            continue
        draw.line([cx + x0, cy + dy, cx + x1, cy + dy], fill=lit)


def draw_sun_icon(draw, cx, cy, r):
    draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=(255, 200, 40))
    for a in range(0, 360, 45):
        rad = math.radians(a)
        x0, y0 = cx + (r + 2) * math.cos(rad), cy + (r + 2) * math.sin(rad)
        x1, y1 = cx + (r + 6) * math.cos(rad), cy + (r + 6) * math.sin(rad)
        draw.line([x0, y0, x1, y1], fill=(255, 200, 40), width=2)


def dotted_hline(draw, x0, x1, y, fill, step=4, length=2):
    x = x0
    while x < x1:
        draw.line([x, y, min(x + length, x1), y], fill=fill)
        x += step


def dotted_vline(draw, x, y0, y1, fill, step=4, length=2):
    y = y0
    while y < y1:
        draw.line([x, y, x, min(y + length, y1)], fill=fill)
        y += step


# Synthetic pressure series: 24h past (solid, falling) + 12h future (dotted, continues falling)
def pressure_series():
    past = [1010 - i * 0.15 - (2 if 10 < i < 16 else 0) for i in range(24)]
    future = [past[-1] - i * 0.2 for i in range(1, 13)]
    return past, future


def plot_barograph(draw, rect, trend_color, dotted_future=True):
    x0, y0, x1, y1 = rect
    past, future = pressure_series()
    series = past + future
    lo, hi = min(series), max(series)
    span = max(hi - lo, 8)
    mid = (lo + hi) / 2
    lo, hi = mid - span / 2, mid + span / 2

    n = len(series)
    now_idx = len(past) - 1

    def pt(i, v):
        x = x0 + (x1 - x0) * i / (n - 1)
        y = y1 - (y1 - y0) * (v - lo) / (hi - lo)
        return x, y

    # gridlines
    dotted_hline(draw, x0, x1, y0, DARK_GRAY)
    dotted_hline(draw, x0, x1, y1 - 1, DARK_GRAY)

    pts = [pt(i, v) for i, v in enumerate(series)]
    # solid past
    for i in range(now_idx):
        draw.line([pts[i], pts[i + 1]], fill=trend_color, width=2)
    # dotted future
    for i in range(now_idx, n - 1):
        x_a, y_a = pts[i]
        x_b, y_b = pts[i + 1]
        if dotted_future:
            steps = 4
            for s in range(steps):
                t0, t1 = s / steps, (s + 0.5) / steps
                draw.line(
                    [x_a + (x_b - x_a) * t0, y_a + (y_b - y_a) * t0,
                     x_a + (x_b - x_a) * t1, y_a + (y_b - y_a) * t1],
                    fill=trend_color, width=2,
                )
        else:
            draw.line([pts[i], pts[i + 1]], fill=trend_color, width=2)
    # now divider
    now_x, _ = pts[now_idx]
    dotted_vline(draw, int(now_x), y0, y1, WHITE, step=3, length=2)


def status_dot(draw, cx, cy, r, color):
    draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=color)


def base_canvas():
    img = Image.new("RGB", (W, H), BLACK)
    return img, ImageDraw.Draw(img)


def save(img, name):
    os.makedirs(OUT_DIR, exist_ok=True)
    big = img.resize((W * SCALE, H * SCALE), Image.NEAREST)
    path = os.path.join(OUT_DIR, name)
    big.save(path)
    print("wrote", path)


# ---------------------------------------------------------------------------
# Variant A -- baseline, as specced in docs/design/04-ui-layout.md
# ---------------------------------------------------------------------------
def variant_a():
    img, d = base_canvas()

    # header
    d.text((6, 5), "DENVER", font=F_BOLD18, fill=WHITE)
    status_dot(d, W - 26, 11, 4, GREEN)
    text_r(d, (W - 6, 4), "4m", F_REG14, LIGHT_GRAY, anchor="ra")
    d.line([0, 22, W, 22], fill=DARK_GRAY)

    # conditions
    draw_sun_icon(d, 40, 55, 14)
    text_r(d, (72, 62), "72°", F_TEMP, WHITE, anchor="ma")
    d.line([100, 30, 100, 90], fill=DARK_GRAY)
    text_r(d, (194, 40), "feels 70°", F_REG14, LIGHT_GRAY, anchor="ra")
    text_r(d, (194, 60), "H 81°  L 54°", F_REG14, LIGHT_GRAY, anchor="ra")
    d.line([0, 98, W, 98], fill=DARK_GRAY)

    # baro label
    d.text((6, 101), "1006.6 hPa", font=F_REG12, fill=WHITE)
    text_r(d, (100, 101), "▼ -2.1/3h", F_REG12, ORANGE, anchor="ma")
    text_r(d, (194, 101), "FALLING", F_REG12, ORANGE, anchor="ra")

    # plot
    plot_barograph(d, (4, 122, 196, 188), ORANGE)

    d.line([0, 190, W, 190], fill=DARK_GRAY)
    # footer
    d.text((6, 200), "↖ 8 mph", font=F_REG14, fill=LIGHT_GRAY)
    d.text((80, 200), "46%", font=F_REG14, fill=LIGHT_GRAY)
    draw_moon(d, 172, 209, 14, 0.28)

    save(img, "dashboard-a-baseline.png")


# ---------------------------------------------------------------------------
# Variant B -- moon-prominent: big disc as a quiet background anchor,
# condensed text so the moon reads as a second focal point, not an afterthought.
# ---------------------------------------------------------------------------
def variant_b():
    img, d = base_canvas()

    d.text((6, 5), "DENVER", font=F_BOLD18, fill=WHITE)
    status_dot(d, W - 26, 11, 4, GREEN)
    text_r(d, (W - 6, 4), "4m", F_REG14, LIGHT_GRAY, anchor="ra")
    d.line([0, 22, W, 22], fill=DARK_GRAY)

    # large moon disc watermark, upper right, behind the temperature
    draw_moon(d, 158, 60, 30, 0.28)

    draw_sun_icon(d, 36, 55, 13)
    text_r(d, (70, 62), "72°", F_TEMP, WHITE, anchor="ma")
    text_r(d, (70, 88), "feels 70° · H81° L54°", F_REG12, LIGHT_GRAY, anchor="ma")

    d.line([0, 98, W, 98], fill=DARK_GRAY)
    d.text((6, 100), "1006.6 hPa", font=F_REG14, fill=WHITE)
    text_r(d, (194, 100), "▼ FALLING", F_REG14, PICTON_BLUE, anchor="ra")

    plot_barograph(d, (4, 122, 196, 188), PICTON_BLUE)

    d.line([0, 190, W, 190], fill=DARK_GRAY)
    d.text((6, 200), "↖ 8 mph   46%", font=F_REG14, fill=LIGHT_GRAY)

    save(img, "dashboard-b-moon-prominent.png")


# ---------------------------------------------------------------------------
# Variant C -- graph-dominant: current conditions condensed to one line,
# the barograph grows to be the visual anchor of the screen.
# ---------------------------------------------------------------------------
def variant_c():
    img, d = base_canvas()

    d.text((6, 5), "DENVER", font=F_BOLD18, fill=WHITE)
    status_dot(d, W - 26, 11, 4, GREEN)
    text_r(d, (W - 6, 4), "4m", F_REG14, LIGHT_GRAY, anchor="ra")
    d.line([0, 22, W, 22], fill=DARK_GRAY)

    # single condensed conditions row
    draw_sun_icon(d, 18, 36, 10)
    d.text((32, 26), "72°", font=F_BOLD18, fill=WHITE)
    text_r(d, (194, 26), "feels 70°  H81° L54°", F_REG12, LIGHT_GRAY, anchor="ra")
    d.line([0, 50, W, 50], fill=DARK_GRAY)

    # baro label, tight
    d.text((6, 54), "1006.6 hPa   ▼ -2.1/3h", font=F_REG14, fill=WHITE)
    text_r(d, (194, 54), "FALLING", F_REG14, ORANGE, anchor="ra")

    # big plot, ~45% of screen height
    plot_barograph(d, (4, 76, 196, 188), ORANGE)

    d.line([0, 190, W, 190], fill=DARK_GRAY)
    d.text((6, 200), "↖ 8 mph", font=F_REG14, fill=LIGHT_GRAY)
    d.text((80, 200), "46%", font=F_REG14, fill=LIGHT_GRAY)
    draw_moon(d, 172, 209, 14, 0.28)

    save(img, "dashboard-c-graph-dominant.png")


# ---------------------------------------------------------------------------
# Variant D -- high-contrast trend banner: a bold colour band carries the
# trend, large glyph, favours legibility over density (also previews how the
# B/W platforms would read the same information via shape/weight not hue).
# ---------------------------------------------------------------------------
def variant_d():
    img, d = base_canvas()

    d.text((6, 5), "DENVER", font=F_BOLD18, fill=WHITE)
    status_dot(d, W - 26, 11, 4, GREEN)
    text_r(d, (W - 6, 4), "4m", F_REG14, LIGHT_GRAY, anchor="ra")
    d.line([0, 22, W, 22], fill=DARK_GRAY)

    draw_sun_icon(d, 40, 55, 14)
    text_r(d, (72, 62), "72°", F_TEMP, WHITE, anchor="ma")
    d.line([100, 30, 100, 90], fill=DARK_GRAY)
    text_r(d, (194, 40), "feels 70°", F_REG14, LIGHT_GRAY, anchor="ra")
    text_r(d, (194, 60), "H 81°  L 54°", F_REG14, LIGHT_GRAY, anchor="ra")

    # bold trend banner replaces the plain baro label row
    d.rectangle([0, 98, W, 118], fill=(60, 20, 0))
    d.text((6, 101), "1006.6 hPa", font=F_REG14, fill=WHITE)
    text_r(d, (194, 100), "▼▼ FALLING", F_BOLD18, ORANGE, anchor="ra")

    plot_barograph(d, (4, 122, 196, 188), ORANGE)

    d.line([0, 190, W, 190], fill=DARK_GRAY)
    d.text((6, 200), "WIND 8mph", font=F_REG12, fill=LIGHT_GRAY)
    d.text((80, 200), "HUMID 46%", font=F_REG12, fill=LIGHT_GRAY)
    draw_moon(d, 172, 209, 14, 0.28)

    save(img, "dashboard-d-trend-banner.png")


if __name__ == "__main__":
    variant_a()
    variant_b()
    variant_c()
    variant_d()
