#!/usr/bin/env python3
# text/tools/color_reference.py — the colour glyphs of the test fonts as
# others read them, written as text for color_test.cst.
#
# Development tool: needs fontTools, Pillow and uharfbuzz (the virtual
# environment of text/testdata/README.md); its output is versioned. One file
# per font, <font>.color.ref:
#
#   strikes N               then for each strike:
#   strike PPEM             the larger of its two sizes, then each image in it:
#   image GID W H LEFT TOP ADVANCE BYTES PNG PIXELS
#                           its metrics in pixels of the strike, its PNG's
#                           length and hash, and the hash of its pixels —
#                           decoded by Pillow as RGBA, premultiplied (each
#                           colour times alpha over 255, rounded), row by row
#   chosen PX GID PNG       the image HarfBuzz gives the glyph at PX pixels an
#                           em (it reads index formats 1 and 3 alone)
#   colr N                  then for each base glyph:
#   base GID COUNT          then its layers, the first drawn first:
#   layer GID ENTRY         65535 for the text's colour
#   palettes P E            then each colour:
#   color P E RRGGBBAA
#
# Hashes are 64-bit FNV-1a, in hexadecimal.
#
#   ~/.cache/caustic-media/venv/bin/python text/tools/color_reference.py
import io
import os

import uharfbuzz as hb
from fontTools.ttLib import TTFont
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")
FONTS = ["cbdt.ttf", "colr.ttf", "color.ttf"]
SIZES = [8, 10, 16, 20, 32, 40, 109, 200]


def fnv(data):
    h = 0xCBF29CE484222325
    for b in data:
        h = ((h ^ b) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return "%016x" % h


def pixels(png):
    im = Image.open(io.BytesIO(png)).convert("RGBA")
    out = bytearray()
    raw = im.tobytes()
    for k in range(0, len(raw), 4):
        r, g, b, a = raw[k], raw[k + 1], raw[k + 2], raw[k + 3]
        out += bytes(((r * a + 127) // 255, (g * a + 127) // 255, (b * a + 127) // 255, a))
    return fnv(out)


def metrics_of(glyph, index):
    m = getattr(glyph, "metrics", None) or index.metrics
    if hasattr(m, "horiBearingX"):
        return m.width, m.height, m.horiBearingX, m.horiBearingY, m.horiAdvance
    return m.width, m.height, m.BearingX, m.BearingY, m.Advance


def main():
    for name in FONTS:
        path = os.path.join(DATA, name)
        font = TTFont(path)
        lines = ["# %s's colour glyphs; see text/tools/color_reference.py" % name]
        strikes = font["CBLC"].strikes if "CBLC" in font else []
        lines.append("strikes %d" % len(strikes))
        for k, st in enumerate(strikes):
            bs = st.bitmapSizeTable
            lines.append("strike %d" % max(bs.ppemX, bs.ppemY))
            data = font["CBDT"].strikeData[k]
            for index in st.indexSubTables:
                for gname in index.names:
                    g = data[gname]
                    png = g.imageData
                    w, h, left, top, adv = metrics_of(g, index)
                    lines.append("image %d %d %d %d %d %d %d %s %s" % (font.getGlyphID(gname), w, h, left, top, adv,
                                                                    len(png), fnv(png), pixels(png)))
        if strikes:
            hbfont = hb.Font(hb.Face(hb.Blob.from_file_path(path)))
            for px in SIZES:
                hbfont.ppem = (px, px)
                for gid in range(len(font.getGlyphOrder())):
                    blob = hbfont.get_glyph_color_png(gid)
                    if blob and len(blob.data) > 0:
                        lines.append("chosen %d %d %s" % (px, gid, fnv(bytes(blob.data))))
        colr = font["COLR"].ColorLayers if "COLR" in font else {}
        lines.append("colr %d" % len(colr))
        for base in sorted(colr, key=font.getGlyphID):
            layers = colr[base]
            lines.append("base %d %d" % (font.getGlyphID(base), len(layers)))
            for layer in layers:
                lines.append("layer %d %d" % (font.getGlyphID(layer.name), layer.colorID))
        if "CPAL" in font:
            cpal = font["CPAL"]
            lines.append("palettes %d %d" % (len(cpal.palettes), cpal.numPaletteEntries))
            for p, pal in enumerate(cpal.palettes):
                for e, c in enumerate(pal):
                    lines.append("color %d %d %02x%02x%02x%02x" % (p, e, c.red, c.green, c.blue, c.alpha))
        else:
            lines.append("palettes 0 0")
        with open(os.path.join(DATA, name + ".color.ref"), "w") as fh:
            fh.write("\n".join(lines) + "\n")
        print(name + ".color.ref", len(lines), "lines")


if __name__ == "__main__":
    main()
