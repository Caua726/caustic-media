#!/usr/bin/env python3
# text/tools/outline_reference.py — every glyph's outline at the default
# instance, written as text for outline_test.cst to hold the outline readers to.
#
# Development tool: needs fontTools and uharfbuzz (see reference_common.py);
# its output is versioned. One file per font, <font>.outline.ref:
#
#   faces N
#   face I
#   glyph GID           the segments after it, until the next glyph:
#   M x y               a contour starts
#   L x y               a line
#   Q cx cy x y         a quadratic curve
#   C c1x c1y c2x c2y x y   a cubic curve
#   Z                   the contour closes, back to where it started
#
# TrueType contours as fontTools draws them: starting at the first on-curve
# point, the line back to it left to the close, off-curve pairs split at their
# midpoint, a contour of off-curve points alone starting at the midpoint of its
# last and first. Glyphs composed as HarfBuzz composes them, the first phantom
# point at the origin (reference_common.py). CFF and CFF2 as fontTools'
# charstring interpreter draws them. Every glyph is drawn by HarfBuzz too, and
# the two must pass through the same points.
#
#   ~/.cache/caustic-media/venv/bin/python text/tools/outline_reference.py
import glob
import os

from fontTools.ttLib import TTFont, TTCollection

from reference_common import Segments, draw_glyf, hb_font, check_against_hb

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")


def face(out, font, hbf, what):
    order = font.getGlyphOrder()
    gs = font.getGlyphSet()
    for gid, name in enumerate(order):
        out.append("glyph %d" % gid)
        lines = []
        if "glyf" in font:
            draw_glyf(font, name, {}, lines)
        else:
            gs[name].draw(Segments(lines, gs))
        check_against_hb(hbf, gid, lines, what)
        out += lines


def main():
    paths = sorted(glob.glob(os.path.join(DATA, "*.ttf")) + glob.glob(os.path.join(DATA, "*.otf"))
                   + glob.glob(os.path.join(DATA, "*.ttc")))
    for p in paths:
        # The bombs are for refusing, not drawing.
        if os.path.basename(p).startswith("bomb"):
            continue
        data = open(p, "rb").read()
        fonts = TTCollection(p).fonts if p.endswith(".ttc") else [TTFont(p)]
        out = ["faces %d" % len(fonts)]
        for i, f in enumerate(fonts):
            out.append("face %d" % i)
            face(out, f, hb_font(data, i), os.path.basename(p))
        with open(p + ".outline.ref", "w") as fh:
            fh.write("\n".join(out) + "\n")
        print(os.path.basename(p) + ".outline.ref", len(out), "lines")


if __name__ == "__main__":
    main()
