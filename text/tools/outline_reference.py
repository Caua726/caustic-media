#!/usr/bin/env python3
# text/tools/outline_reference.py — every glyph's outline as fontTools draws
# it, written as text for outline_test.cst to hold the outline readers to.
#
# Development tool: needs fontTools; its output is versioned. One file per
# font, <font>.outline.ref:
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
# last and first. Composites are expanded by fontTools' own getCoordinates —
# offsets, scales, point matching — and drawn the same way. CFF and CFF2 at the
# default instance, as fontTools' charstring interpreter draws them.
#
#   python3 text/tools/outline_reference.py
import glob
import os

from fontTools.pens.basePen import BasePen
from fontTools.ttLib import TTFont, TTCollection
from fontTools.ttLib.tables._g_l_y_f import Glyph

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")


def num(v):
    if v == int(v):
        return "%d" % int(v)
    return repr(float(v))


class Segments(BasePen):
    # Every segment explicit: BasePen splits quadratic runs at their implied
    # on-curve points before they reach _qCurveToOne.
    def __init__(self, out, glyphs=None):
        # The glyph set draws components: endchar's accent building.
        super().__init__(glyphs)
        self.out = out

    def _moveTo(self, p):
        self.out.append("M %s %s" % (num(p[0]), num(p[1])))

    def _lineTo(self, p):
        self.out.append("L %s %s" % (num(p[0]), num(p[1])))

    def _qCurveToOne(self, p1, p2):
        self.out.append("Q %s %s %s %s" % (num(p1[0]), num(p1[1]), num(p2[0]), num(p2[1])))

    def _curveToOne(self, p1, p2, p3):
        self.out.append("C %s %s %s %s %s %s" % tuple(num(v) for v in (p1[0], p1[1], p2[0], p2[1], p3[0], p3[1])))

    def _closePath(self):
        self.out.append("Z")

    def _endPath(self):
        self.out.append("Z")


def glyf_glyph(font, name, out):
    glyf = font["glyf"]
    g = glyf[name]
    if g.numberOfContours == 0:
        return
    coords, ends, flags = g.getCoordinates(glyf)
    simple = Glyph()
    simple.numberOfContours = len(ends)
    simple.coordinates = coords
    simple.endPtsOfContours = list(ends)
    simple.flags = flags
    simple.program = None
    simple.draw(Segments(out), glyf)


def face(out, font):
    order = font.getGlyphOrder()
    gs = font.getGlyphSet()
    for gid, name in enumerate(order):
        out.append("glyph %d" % gid)
        if "glyf" in font:
            glyf_glyph(font, name, out)
        else:
            gs[name].draw(Segments(out, gs))


def main():
    paths = sorted(glob.glob(os.path.join(DATA, "*.ttf")) + glob.glob(os.path.join(DATA, "*.otf"))
                   + glob.glob(os.path.join(DATA, "*.ttc")))
    for p in paths:
        # The bombs are for refusing, not drawing.
        if os.path.basename(p).startswith("bomb"):
            continue
        fonts = TTCollection(p).fonts if p.endswith(".ttc") else [TTFont(p)]
        out = ["faces %d" % len(fonts)]
        for i, f in enumerate(fonts):
            out.append("face %d" % i)
            face(out, f)
        with open(p + ".outline.ref", "w") as fh:
            fh.write("\n".join(out) + "\n")
        print(os.path.basename(p) + ".outline.ref", len(out), "lines")


if __name__ == "__main__":
    main()
