# text/tools/reference_common.py — what the reference tools share: numbers
# written one way, a pen that writes every segment, TrueType glyphs composed
# as HarfBuzz composes them, and HarfBuzz itself to hold them to.
#
# fontTools reads the tables; the arithmetic is HarfBuzz's, the one most
# programs on the desktop draw with: gvar's deltas weighed by fontTools'
# region scalars and filled in by its IUP, composites varied component by
# component, and every glyph shifted so that its first phantom point — the
# end of its left side bearing — is the origin. Each glyph composed here is
# then drawn by HarfBuzz as well, and every point of each drawing must lie
# within a few thousandths of a unit of a point of the other.
import uharfbuzz as hb
from fontTools.misc import psCharStrings
from fontTools.pens.basePen import BasePen
from fontTools.ttLib.tables._g_l_y_f import Glyph, GlyphCoordinates
from fontTools.ttLib.tables import _g_l_y_f as G
from fontTools.varLib.models import supportScalar
from fontTools.varLib.iup import iup_delta


# fontTools blends a CFF2 charstring with variation data 0 until the
# charstring says vsindex, where the specification and HarfBuzz start from
# the Private DICT's vsindex.
_reset = psCharStrings.SimpleT2Decompiler.reset


def _reset_with_private_vsindex(self):
    _reset(self)
    private = getattr(self, "private", None)
    if private is not None and hasattr(private, "vsindex"):
        self.vsIndex = private.vsindex


psCharStrings.SimpleT2Decompiler.reset = _reset_with_private_vsindex


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


def compose(font, name, loc):
    # Glyph name's points at normalized location loc (axis tag to -1..1, empty
    # for the default), their on-curve flags, contour ends and four phantom
    # points — not yet shifted to the first.
    glyf = font["glyf"]
    g = glyf[name]
    adv, lsb = font["hmtx"].metrics[name]
    nc = g.numberOfContours
    if nc > 0:
        pts = [tuple(p) for p in g.coordinates]
        on = [f & 1 for f in g.flags]
        ends = list(g.endPtsOfContours)
    elif nc < 0:
        # A composite's points are its components' offsets; a matched
        # component's is nothing, and deltas to it change nothing.
        pts = [(getattr(c, "x", 0), getattr(c, "y", 0)) for c in g.components]
        on, ends = [], []
    else:
        pts, on, ends = [], [], []
    xmin = getattr(g, "xMin", 0)
    phantoms = [(xmin - lsb, 0), (xmin - lsb + adv, 0), (0, 0), (0, 0)]
    coords = GlyphCoordinates(pts + phantoms)
    if loc and "gvar" in font:
        orig = GlyphCoordinates(coords)
        contours = ends if nc > 0 else list(range(len(pts)))
        for var in font["gvar"].variations.get(name, []):
            scalar = supportScalar(loc, var.axes)
            if not scalar:
                continue
            d = var.coordinates
            if None in d:
                d = iup_delta(d, orig, contours)
            coords += GlyphCoordinates(d) * scalar
    coords = [tuple(p) for p in coords]
    phantoms = coords[-4:]
    if nc >= 0:
        return coords[:-4], on, ends, phantoms
    out, out_on, out_ends = [], [], []
    for i, c in enumerate(g.components):
        sp, son, sends, sph = compose(font, c.glyphName, loc)
        if hasattr(c, "transform"):
            (xx, xy), (yx, yy) = c.transform
            scaled = True
        else:
            xx, xy, yx, yy = 1, 0, 0, 1
            scaled = False

        def tr(p):
            return (p[0] * xx + p[1] * yx, p[0] * xy + p[1] * yy)

        if hasattr(c, "firstPt"):
            if scaled:
                sp = [tr(p) for p in sp]
            a, b = c.firstPt, c.secondPt
            dx, dy = out[a][0] - sp[b][0], out[a][1] - sp[b][1]
            sp = [(x + dx, y + dy) for x, y in sp]
        else:
            dx, dy = coords[i]
            if not scaled:
                sp = [(x + dx, y + dy) for x, y in sp]
            elif c.flags & G.SCALED_COMPONENT_OFFSET and not c.flags & G.UNSCALED_COMPONENT_OFFSET:
                sp = [tr((x + dx, y + dy)) for x, y in sp]
            else:
                sp = [(x + dx, y + dy) for x, y in (tr(p) for p in sp)]
        base = len(out)
        out += sp
        out_on += son
        out_ends += [e + base for e in sends]
        if c.flags & G.USE_MY_METRICS:
            phantoms = sph
    return out, out_on, out_ends, phantoms


def draw_glyf(font, name, loc, out):
    # The composed glyph, its first phantom point at the origin, drawn the
    # way fontTools draws a simple glyph. Its phantom points, shifted, back.
    pts, on, ends, ph = compose(font, name, loc)
    shift = -ph[0][0]
    if ends:
        g = Glyph()
        g.numberOfContours = len(ends)
        g.coordinates = GlyphCoordinates([(x + shift, y) for x, y in pts])
        g.flags = bytearray(on)
        g.endPtsOfContours = ends
        g.program = None
        g.draw(Segments(out), font["glyf"])
    return [(x + shift, y) for x, y in ph]


class _Points:
    # A pen keeping every point a drawing passes through or near.
    def __init__(self):
        self.pts = []

    def moveTo(self, p):
        self.pts.append(p)

    def lineTo(self, p):
        self.pts.append(p)

    def qCurveTo(self, *ps):
        self.pts += [p for p in ps if p is not None]

    def curveTo(self, *ps):
        self.pts += list(ps)

    def closePath(self):
        pass

    def endPath(self):
        pass


def _points_of(lines):
    pts = []
    for line in lines:
        w = line.split()
        if w[0] in ("M", "L", "Q", "C"):
            vals = [float(v) for v in w[1:]]
            pts += list(zip(vals[0::2], vals[1::2]))
    return pts


def _near(p, pts):
    tol = 0.005 + 1e-6 * (abs(p[0]) + abs(p[1]))
    return any(abs(p[0] - q[0]) <= tol and abs(p[1] - q[1]) <= tol for q in pts)


def hb_font(data, index=0):
    return hb.Font(hb.Face(data, index))


def check_against_hb(hbfont, gid, lines, what):
    # The drawing in lines (M/L/Q/C/Z) and HarfBuzz's of the same glyph pass
    # through the same points; raises otherwise.
    pen = _Points()
    hbfont.draw_glyph_with_pen(gid, pen)
    mine = _points_of(lines)
    for p in mine:
        if not _near(p, pen.pts):
            raise SystemExit("%s glyph %d: point %r is not HarfBuzz's" % (what, gid, p))
    for p in pen.pts:
        if not _near(p, mine):
            raise SystemExit("%s glyph %d: HarfBuzz's point %r is not here" % (what, gid, p))


def round_hb(v):
    # roundf: halves away from zero.
    import math
    return int(math.floor(v + 0.5)) if v >= 0 else -int(math.floor(-v + 0.5))
