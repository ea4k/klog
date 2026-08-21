#!/usr/bin/env python3
"""Regenerate background.png, the KLog DMG background image.

Run from anywhere with Pillow installed (pip install pillow):
    python3 devscripts/dmg-resources/make-background.py

Logical window size is 400x400 (must match --window-size in the
macos-create-*.sh scripts). This image is rendered at 2x (800x800)
for a crisp look on Retina displays; --icon-size / --icon / --app-drop-link
coordinates in those scripts are logical coordinates and must line up with
LEFT_X / RIGHT_X / ICON_Y below.

Colors are sampled straight from images/klog.png (the app icon): the
notepad teal, the pencil red, and the headphones' black/gray.
"""
import os
from PIL import Image, ImageDraw, ImageFont, ImageFilter

SCALE = 2
W = H = 400 * SCALE

FONT_BOLD = "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf"
FONT_REG = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"

TEAL = (95, 213, 212)       # klog.png notepad
TEAL_TINT = (225, 247, 246)  # pale wash of the notepad teal, for the background
RED = (191, 0, 0)           # klog.png pencil
INK = (0, 0, 0)             # klog.png headphones
GRAY = (100, 100, 100)      # klog.png headphone metal

# Icon centers in logical coordinates - keep in sync with the --icon and
# --app-drop-link positions passed to create-dmg in macos-create-*.sh.
LEFT_X, RIGHT_X, ICON_Y = 100, 300, 220

# --- base canvas: soft vertical gradient, white to a pale wash of the icon's teal ---
img = Image.new("RGB", (W, H), (255, 255, 255))
top = (255, 255, 255)
for y in range(H):
    t = y / H
    r = round(top[0] + (TEAL_TINT[0] - top[0]) * t)
    g = round(top[1] + (TEAL_TINT[1] - top[1]) * t)
    b = round(top[2] + (TEAL_TINT[2] - top[2]) * t)
    ImageDraw.Draw(img).line([(0, y), (W, y)], fill=(r, g, b))

draw = ImageDraw.Draw(img, "RGBA")

# subtle top accent bar
draw.rectangle([0, 0, W, 10 * SCALE], fill=(*TEAL, 255))

# --- title ---
title_font = ImageFont.truetype(FONT_BOLD, 28 * SCALE)
subtitle_font = ImageFont.truetype(FONT_REG, 13 * SCALE)
small_font = ImageFont.truetype(FONT_REG, 10 * SCALE)

title = "Install KLog"
tb = draw.textbbox((0, 0), title, font=title_font)
tw = tb[2] - tb[0]
draw.text(((W - tw) / 2, 32 * SCALE), title, font=title_font, fill=INK)

subtitle = "Drag to Applications to install"
sb = draw.textbbox((0, 0), subtitle, font=subtitle_font)
sw = sb[2] - sb[0]
draw.text(((W - sw) / 2, 70 * SCALE), subtitle, font=subtitle_font, fill=GRAY)

# --- arrow between the two icon slots, in the pencil's red ---
lx, rx, iy = LEFT_X * SCALE, RIGHT_X * SCALE, ICON_Y * SCALE
icon_half = 64 * SCALE  # half of a 128px icon

ax0 = lx + icon_half + 10 * SCALE
ax1 = rx - icon_half - 10 * SCALE
ay = iy

shaft_h = 5 * SCALE
head_len = 20 * SCALE
head_h = 18 * SCALE

draw.rounded_rectangle(
    [ax0, ay - shaft_h / 2, ax1 - head_len, ay + shaft_h / 2],
    radius=shaft_h / 2,
    fill=(*RED, 210),
)
draw.polygon(
    [
        (ax1 - head_len, ay - head_h / 2),
        (ax1, ay),
        (ax1 - head_len, ay + head_h / 2),
    ],
    fill=(*RED, 210),
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
footer = "KLog · GPLv3 · ea4k.es"
fb = draw.textbbox((0, 0), footer, font=small_font)
fw = fb[2] - fb[0]
draw.text(((W - fw) / 2, H - 24 * SCALE), footer, font=small_font, fill=GRAY)

out = os.path.join(os.path.dirname(os.path.abspath(__file__)), "background.png")
img.save(out)
print("wrote", out, img.size)
