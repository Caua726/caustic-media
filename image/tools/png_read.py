#!/usr/bin/env python3
# image/tools/png_read.py — a PNG image/save.cst wrote, read by another
# reader (Pillow) as a black box: its size and every pixel as the pattern
# image/save_test.cst made, byte (x*37 + y*101 + x*y) % 256 of each row.
#
#   uv run --with pillow python3 image/tools/png_read.py build/save_test.png 37 23
import sys

from PIL import Image

path, w, h = sys.argv[1], int(sys.argv[2]), int(sys.argv[3])
im = Image.open(path)
im.load()
if im.size != (w, h) or im.mode != "RGBA":
    sys.exit(f"png_read: {im.size} {im.mode}, want ({w}, {h}) RGBA")
raw = im.tobytes()
row = w * 4
for y in range(h):
    for x in range(row):
        want = (x * 37 + y * 101 + x * y) % 256
        if raw[y * row + x] != want:
            sys.exit(f"png_read: byte {x} of row {y} is {raw[y * row + x]}, want {want}")
print("png read: as written")
