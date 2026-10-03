#!/usr/bin/env python3
# text/tools/raster_reference.py — paths and glyphs drawn as coverage, written
# as text for raster_test.cst to hold the rasterizer to.
#
# Development tool: needs fontTools and freetype-py (see the README in
# ../testdata); its output, ../testdata/raster.ref, is versioned:
#
#   shape NAME          a path given here, in whole units:
#   M x y / L x y / Q cx cy x y / C c1x c1y c2x c2y x y / Z
#   glyph FONT GID      or a glyph of a test font, as the outline reader
#                       draws it
#   rule nonzero|evenodd
#   transform A B C D E F G   x' = A/G x + B/G y + E/G, y' = C/G x + D/G y + F/G,
#                       in pixels, y down — integers, so that both sides
#                       divide them into the same doubles
#   box LEFT TOP W H    the pixels the path's points and controls reach
#   row HEX             W coverage bytes per row, H rows
#   halves X Y ..       the pixels whose coverage is a half level, k + 1/2
#                       of 255, which arithmetic of any precision may round
#                       either way
#   end
#
# The rasterizer's contract, which this computes independently: the path's
# points transformed; curves flattened into n chords at t = i/n, n found by
# Wang's formula so that no chord strays more than TOLERANCE from its curve;
# each contour closed. A pixel's coverage is then the winding number integrated
# over its square — found here by clipping every contour to the pixel, Sutherland
# and Hodgman, and taking the signed area of what is left — its magnitude
# clamped to 1 (non-zero) or folded into 0..1 (even-odd), and written as a byte,
# halves up. That is exact wherever contours do not overlap within a pixel, and
# is what FreeType and every analytic rasterizer compute where they do.
#
# Every glyph drawn without a slant is drawn by FreeType too, unhinted, and the
# two must agree but for flattening: FreeType places points on a 1/64 grid and
# flattens more coarsely — within 48 levels of 255 anywhere and 12 on average,
# where a quarter-pixel misplacement shows as 64 and 30.
#
#   ~/.cache/caustic-media/venv/bin/python text/tools/raster_reference.py
import ctypes
import math
import os

from fontTools.ttLib import TTFont

from reference_common import Segments, draw_glyf

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")

TOLERANCE = 1.0 / 64
MAX_SEGMENTS = 1024


# --- The path, as the rasterizer reads it ---

def parse(lines):
    # Segment strings (Segments' M/L/Q/C/Z) into commands of float points.
    cmds = []
    for line in lines:
        w = line.split()
        cmds.append((w[0], [float(v) for v in w[1:]]))
    return cmds


def apply(t, x, y):
    return (t[0] * x + t[1] * y + t[4], t[2] * x + t[3] * y + t[5])


def box(cmds, t):
    xs, ys = [], []
    for _, v in cmds:
        for i in range(0, len(v), 2):
            x, y = apply(t, v[i], v[i + 1])
            xs.append(x)
            ys.append(y)
    if not xs:
        return 0, 0, 0, 0
    left, top = math.floor(min(xs)), math.floor(min(ys))
    return left, top, math.ceil(max(xs)) - left, math.ceil(max(ys)) - top


def segments_quad(p0, p1, p2):
    ddx = p0[0] - 2.0 * p1[0] + p2[0]
    ddy = p0[1] - 2.0 * p1[1] + p2[1]
    dd = math.sqrt(ddx * ddx + ddy * ddy)
    n = math.ceil(math.sqrt(dd / (4.0 * TOLERANCE)))
    return min(max(n, 1), MAX_SEGMENTS)


def segments_cubic(p0, p1, p2, p3):
    ax = p0[0] - 2.0 * p1[0] + p2[0]
    ay = p0[1] - 2.0 * p1[1] + p2[1]
    bx = p1[0] - 2.0 * p2[0] + p3[0]
    by = p1[1] - 2.0 * p2[1] + p3[1]
    m = max(math.sqrt(ax * ax + ay * ay), math.sqrt(bx * bx + by * by))
    n = math.ceil(math.sqrt(3.0 * m / (4.0 * TOLERANCE)))
    return min(max(n, 1), MAX_SEGMENTS)


def flatten(cmds, t, left, top):
    # Contours as lists of points relative to the box, each closed.
    contours = []
    cur = None
    pts = None

    def rel(p):
        return (p[0] - left, p[1] - top)

    for k, v in cmds:
        if k == "M":
            if pts is not None:
                contours.append(pts)
            cur = apply(t, v[0], v[1])
            pts = [rel(cur)]
        elif k == "L":
            cur = apply(t, v[0], v[1])
            pts.append(rel(cur))
        elif k == "Q":
            p1 = apply(t, v[0], v[1])
            p2 = apply(t, v[2], v[3])
            n = segments_quad(cur, p1, p2)
            for i in range(1, n):
                s = i / n
                m = 1.0 - s
                pts.append(rel((m * m * cur[0] + 2.0 * m * s * p1[0] + s * s * p2[0],
                                m * m * cur[1] + 2.0 * m * s * p1[1] + s * s * p2[1])))
            cur = p2
            pts.append(rel(cur))
        elif k == "C":
            p1 = apply(t, v[0], v[1])
            p2 = apply(t, v[2], v[3])
            p3 = apply(t, v[4], v[5])
            n = segments_cubic(cur, p1, p2, p3)
            for i in range(1, n):
                s = i / n
                m = 1.0 - s
                pts.append(rel((m * m * m * cur[0] + 3.0 * m * m * s * p1[0] + 3.0 * m * s * s * p2[0] + s * s * s * p3[0],
                                m * m * m * cur[1] + 3.0 * m * m * s * p1[1] + 3.0 * m * s * s * p2[1] + s * s * s * p3[1])))
            cur = p3
            pts.append(rel(cur))
        elif k == "Z":
            contours.append(pts)
            pts = None
    if pts is not None:
        contours.append(pts)
    return contours


# --- Coverage, by clipping ---

def clip(poly, axis, value, keep_above):
    out = []
    n = len(poly)
    for i in range(n):
        a = poly[i]
        b = poly[(i + 1) % n]
        ina = a[axis] >= value if keep_above else a[axis] <= value
        inb = b[axis] >= value if keep_above else b[axis] <= value
        if ina:
            out.append(a)
        if ina != inb:
            s = (value - a[axis]) / (b[axis] - a[axis])
            p = [a[0] + s * (b[0] - a[0]), a[1] + s * (b[1] - a[1])]
            p[axis] = value
            out.append(tuple(p))
    return out


def area(poly):
    s = 0.0
    n = len(poly)
    for i in range(n):
        a = poly[i]
        b = poly[(i + 1) % n]
        s += a[0] * b[1] - b[0] * a[1]
    return 0.5 * s


def coverage(contours, w, h, rule, halves):
    rows = []
    for r in range(h):
        acc = [0.0] * w
        for c in contours:
            band = clip(clip(c, 1, float(r), True), 1, float(r + 1), False)
            if len(band) < 3:
                continue
            lo = max(0, math.floor(min(p[0] for p in band)))
            hi = min(w, math.ceil(max(p[0] for p in band)))
            for x in range(lo, hi):
                cell = clip(clip(band, 0, float(x), True), 0, float(x + 1), False)
                if len(cell) >= 3:
                    acc[x] += area(cell)
        out = []
        for a in acc:
            v = abs(a)
            if rule == "nonzero":
                v = min(v, 1.0)
            else:
                v = math.fmod(v, 2.0)
                if v > 1.0:
                    v = 2.0 - v
            if abs(v * 255.0 - math.floor(v * 255.0) - 0.5) < 1e-9:
                halves.append((len(out), r))
            out.append(int(math.floor(v * 255.0 + 0.5)))
        rows.append(out)
    return rows


def record(out, head, cmds, rule, nums, lines_of_path=None):
    t = [nums[i] / nums[6] for i in range(6)]
    left, top, w, h = box(cmds, t)
    halves = []
    rows = coverage(flatten(cmds, t, left, top), w, h, rule, halves)
    out.append(head)
    if lines_of_path is not None:
        out += lines_of_path
    out.append("rule " + rule)
    out.append("transform " + " ".join("%d" % v for v in nums))
    out.append("box %d %d %d %d" % (left, top, w, h))
    for row in rows:
        out.append("row " + "".join("%02x" % v for v in row))
    if halves:
        out.append("halves " + " ".join("%d %d" % p for p in halves))
    out.append("end")
    return left, top, rows


# --- Shapes ---

def shapes():
    # Whole-unit paths: name, rule, transform, commands.
    one = [1, 0, 0, 1, 0, 0, 1]
    q = [1, 0, 0, 1, 0, 0, 64]   # units of 1/64 pixel
    star = ["M 500 0", "L 794 905", "L 24 345", "L 976 345", "L 206 905", "Z"]
    circle = ["M 0 -100", "Q 100 -100 100 0", "Q 100 100 0 100", "Q -100 100 -100 0", "Q -100 -100 0 -100", "Z"]
    return [
        ("whole pixels", "nonzero", one, ["M 1 1", "L 5 1", "L 5 4", "L 1 4", "Z"]),
        ("at quarters", "nonzero", q, ["M 80 48", "L 300 48", "L 300 270", "L 80 270", "Z"]),
        ("the other way round", "nonzero", q, ["M 80 48", "L 80 270", "L 300 270", "L 300 48", "Z"]),
        ("steep and shallow", "nonzero", q, ["M 10 10", "L 900 70", "L 60 400", "Z"]),
        ("a star", "nonzero", [1, 0, 0, 1, 0, 0, 100], star),
        ("a star, even-odd", "evenodd", [1, 0, 0, 1, 0, 0, 100], star),
        ("a hole", "nonzero", q, ["M 0 0", "L 640 0", "L 640 640", "L 0 640", "Z",
                                  "M 150 150", "L 150 490", "L 490 490", "L 490 150", "Z"]),
        ("overlapping", "nonzero", q, ["M 0 0", "L 400 0", "L 400 400", "L 0 400", "Z",
                                       "M 200 130", "L 600 130", "L 600 530", "L 200 530", "Z"]),
        ("overlapping, even-odd", "evenodd", q, ["M 0 0", "L 400 0", "L 400 400", "L 0 400", "Z",
                                                 "M 200 130", "L 600 130", "L 600 530", "L 200 530", "Z"]),
        ("a sliver", "nonzero", q, ["M 0 0", "L 1000 20", "L 1000 23", "Z"]),
        ("inside one pixel", "nonzero", q, ["M 70 70", "L 120 80", "L 90 120", "Z"]),
        ("across many columns", "nonzero", q, ["M 0 0", "L 19200 40", "L 19200 64", "L 0 64", "Z"]),
        ("left open", "nonzero", q, ["M 32 32", "L 600 32", "L 600 300", "M 32 400", "L 600 400", "L 32 640"]),
        ("back and forth", "nonzero", q, ["M 0 0", "L 500 300", "L 0 0", "Z"]),
        ("a circle of quadratics", "nonzero", [1, 0, 0, 1, 1000, 1000, 100], circle),
        ("a cubic loop", "nonzero", [1, 0, 0, 1, 0, 0, 16], ["M 0 0", "C 400 0 0 300 300 300", "C 600 300 600 0 0 0", "Z"]),
        ("turned", "nonzero", [3, -4, 4, 3, 80, 0, 5], ["M 0 0", "L 60 0", "L 60 20", "L 0 20", "Z"]),
        ("large", "nonzero", [1, 0, 0, 1, 0, 0, 8], ["M 0 40", "Q 0 0 40 0", "L 760 0", "Q 800 0 800 40",
                                                      "L 800 560", "Q 800 600 760 600", "L 40 600", "Q 0 600 0 560", "Z"]),
        ("nothing", "nonzero", one, []),
    ]


# --- Glyphs, and FreeType ---

def glyph_lines(font, gid):
    name = font.getGlyphOrder()[gid]
    lines = []
    if "glyf" in font:
        draw_glyf(font, name, {}, lines)
    else:
        gs = font.getGlyphSet()
        gs[name].draw(Segments(lines, gs))
    return lines


def freetype_bitmap(path, gid, ppem64, dx64, dy64):
    import freetype
    face = freetype.Face(path)
    face.set_char_size(0, ppem64, 72, 72)
    face.load_glyph(gid, freetype.FT_LOAD_NO_HINTING | freetype.FT_LOAD_NO_BITMAP)
    outline = face.glyph._FT_GlyphSlot.contents.outline
    freetype.raw.FT_Outline_Translate(ctypes.byref(outline), dx64, -dy64)
    face.glyph.render(freetype.FT_RENDER_MODE_NORMAL)
    bm = face.glyph.bitmap
    px = {}
    for j in range(bm.rows):
        for i in range(bm.width):
            v = bm.buffer[j * bm.pitch + i]
            if v:
                px[(face.glyph.bitmap_left + i, -face.glyph.bitmap_top + j)] = v
    return px


def check_freetype(what, path, gid, ppem64, dx64, dy64, left, top, rows):
    ft = freetype_bitmap(path, gid, ppem64, dx64, dy64)
    mine = {}
    for j, row in enumerate(rows):
        for i, v in enumerate(row):
            if v:
                mine[(left + i, top + j)] = v
    keys = set(ft) | set(mine)
    diffs = [abs(ft.get(k, 0) - mine.get(k, 0)) for k in keys]
    worst = max(diffs) if diffs else 0
    mean = sum(diffs) / len(diffs) if diffs else 0.0
    # FreeType flattens cubics more coarsely still: CFF glyphs part by more.
    if worst > 48 or mean > 12.0:
        raise SystemExit("%s: FreeType differs by %d at most, %.2f on average" % (what, worst, mean))
    return worst, mean


GLYPHS = [
    # font, characters, whole sizes in 64ths of a pixel
    ("glyf.ttf", "agO&@é", [9 * 64, 13 * 64 + 32, 24 * 64]),
    ("cff.otf", "agO&@é", [9 * 64, 13 * 64 + 32, 24 * 64]),
    ("kern.ttf", "aW", [12 * 64 + 48, 40 * 64]),
    ("cid.otf", "一か", [16 * 64, 32 * 64]),
]
OFFSETS = [(0, 0), (16, 32), (40, 8)]


def main():
    out = []
    for name, rule, nums, lines in shapes():
        record(out, "shape " + name, parse(lines), rule, nums, lines)
    stats = []
    for fname, chars, sizes in GLYPHS:
        path = os.path.join(DATA, fname)
        font = TTFont(path)
        upem = font["head"].unitsPerEm
        cmap = font.getBestCmap()
        order = font.getGlyphOrder()
        for ch in chars:
            gid = order.index(cmap[ord(ch)])
            cmds = parse(glyph_lines(font, gid))
            for ppem64 in sizes:
                for dx64, dy64 in OFFSETS:
                    g = 4096 * upem
                    nums = [ppem64 * 64, 0, 0, -ppem64 * 64, dx64 * 64 * upem, dy64 * 64 * upem, g]
                    left, top, rows = record(out, "glyph %s %d" % (fname, gid), cmds, "nonzero", nums)
                    what = "%s glyph %d at %d/64 px, offset %d/64 %d/64" % (fname, gid, ppem64, dx64, dy64)
                    stats.append(check_freetype(what, path, gid, ppem64, dx64, dy64, left, top, rows))
            # Slanted, as a synthetic oblique: x moves by a fifth of y.
            ppem64 = 20 * 64
            nums = [ppem64 * 64, ppem64 * 64 // 5, 0, -ppem64 * 64, 0, 0, 4096 * upem]
            record(out, "glyph %s %d" % (fname, gid), cmds, "nonzero", nums)
    # One large glyph.
    font = TTFont(os.path.join(DATA, "glyf.ttf"))
    gid = font.getGlyphOrder().index(font.getBestCmap()[ord("g")])
    nums = [96 * 64 * 64, 0, 0, -96 * 64 * 64, 0, 0, 4096 * font["head"].unitsPerEm]
    left, top, rows = record(out, "glyph glyf.ttf %d" % gid, parse(glyph_lines(font, gid)), "nonzero", nums)
    stats.append(check_freetype("glyf.ttf glyph %d at 96 px" % gid, os.path.join(DATA, "glyf.ttf"),
                                gid, 96 * 64, 0, 0, left, top, rows))
    with open(os.path.join(DATA, "raster.ref"), "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("raster.ref", len(out), "lines; FreeType: worst %d, mean %.3f" %
          (max(s[0] for s in stats), sum(s[1] for s in stats) / len(stats)))


if __name__ == "__main__":
    main()
