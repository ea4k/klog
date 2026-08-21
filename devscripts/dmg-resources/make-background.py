#!/usr/bin/env python3
"""Regenerate background.png, the KLog DMG background image.

Run from anywhere with Pillow installed (pip install pillow):
    python3 devscripts/dmg-resources/make-background.py

Logical window size is 660x400 (must match --window-size in the
macos-create-*.sh scripts). This image is rendered at 2x (1320x800)
for a crisp look on Retina displays; --icon-size / --icon / --app-drop-link
coordinates in those scripts are logical coordinates and must line up with
LEFT_X / RIGHT_X / ICON_Y below.
"""
import os
from PIL import Image, ImageDraw, ImageFont, ImageFilter

SCALE = 2
W, H = 660 * SCALE, 400 * SCALE

FONT_BOLD = "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf"
FONT_REG = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"

TEAL = (79, 199, 195)
TEAL_DARK = (46, 134, 130)
INK = (45, 55, 60)
FAINT = (150, 158, 162)

# Icon centers in logical coordinates - keep in sync with the --icon and
# --app-drop-link positions passed to create-dmg in macos-create-*.sh.
LEFT_X, RIGHT_X, ICON_Y = 170, 490, 210

# --- base canvas: soft vertical gradient, off-white to a whisper of teal ---
img = Image.new("RGB", (W, H), (255, 255, 255))
top = (250, 252, 252)
bottom = (231, 244, 243)
for y in range(H):
    t = y / H
    r = round(top[0] + (bottom[0] - top[0]) * t)
    g = round(top[1] + (bottom[1] - top[1]) * t)
    b = round(top[2] + (bottom[2] - top[2]) * t)
    ImageDraw.Draw(img).line([(0, y), (W, y)], fill=(r, g, b))

draw = ImageDraw.Draw(img, "RGBA")

# subtle top accent bar
draw.rectangle([0, 0, W, 10 * SCALE], fill=(*TEAL, 255))

# --- title ---
title_font = ImageFont.truetype(FONT_BOLD, 30 * SCALE)
subtitle_font = ImageFont.truetype(FONT_REG, 15 * SCALE)
small_font = ImageFont.truetype(FONT_REG, 11 * SCALE)

title = "Install KLog"
tb = draw.textbbox((0, 0), title, font=title_font)
tw = tb[2] - tb[0]
draw.text(((W - tw) / 2, 34 * SCALE), title, font=title_font, fill=INK)

subtitle = "Drag KLog into the Applications folder to install it"
sb = draw.textbbox((0, 0), subtitle, font=subtitle_font)
sw = sb[2] - sb[0]
draw.text(((W - sw) / 2, 74 * SCALE), subtitle, font=subtitle_font, fill=(90, 100, 105))

# --- arrow between the two icon slots ---
lx, rx, iy = LEFT_X * SCALE, RIGHT_X * SCALE, ICON_Y * SCALE
icon_half = 64 * SCALE  # half of a 128px icon

ax0 = lx + icon_half + 14 * SCALE
ax1 = rx - icon_half - 14 * SCALE
ay = iy

shaft_h = 6 * SCALE
head_len = 26 * SCALE
head_h = 22 * SCALE

draw.rounded_rectangle(
    [ax0, ay - shaft_h / 2, ax1 - head_len, ay + shaft_h / 2],
    radius=shaft_h / 2,
    fill=(*TEAL_DARK, 200),
)
draw.polygon(
    [
        (ax1 - head_len, ay - head_h / 2),
        (ax1, ay),
        (ax1 - head_len, ay + head_h / 2),
    ],
    fill=(*TEAL_DARK, 200),
)

# --- soft drop shadows where the icons will sit (Finder draws icons on top) ---
shadow = Image.new("RGBA", (W, H), (0, 0, 0, 0))
sdraw = ImageDraw.Draw(shadow)
for cx in (lx, rx):
    sdraw.ellipse(
        [cx - 46 * SCALE, iy + 40 * SCALE, cx + 46 * SCALE, iy + 54 * SCALE],
        fill=(20, 30, 32, 60),
    )
shadow = shadow.filter(ImageFilter.GaussianBlur(6 * SCALE))
img.paste(Image.alpha_composite(img.convert("RGBA"), shadow).convert("RGB"), (0, 0))

# --- footer ---
draw = ImageDraw.Draw(img, "RGBA")
footer = "KLog — free Ham Radio logging software · GPLv3 · ea4k.es"
fb = draw.textbbox((0, 0), footer, font=small_font)
fw = fb[2] - fb[0]
draw.text(((W - fw) / 2, H - 26 * SCALE), footer, font=small_font, fill=FAINT)

out = os.path.join(os.path.dirname(os.path.abspath(__file__)), "background.png")
img.save(out)
print("wrote", out, img.size)
