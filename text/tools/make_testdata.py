#!/usr/bin/env python3
# text/tools/make_testdata.py — the fonts text/'s tests read, cut down from
# fonts installed on the machine that made them.
#
# Development tool, not part of the build: it needs fontTools (pyftsubset's
# library), and the source fonts at the paths below. What it writes is
# versioned, so the tests run anywhere without either.
#
# Every source is under the SIL Open Font License 1.1. A subset is a Modified
# Version in the licence's terms, which may not carry a Reserved Font Name
# ('Source', 'Liberation'); every subset here is renamed "Caustic Test ...",
# reserved name or not, and each one's licence — its own copyright line and the
# OFL — is written beside it.
#
#   python3 text/tools/make_testdata.py
#   python3 text/tools/sfnt_reference.py
import io
import os
import sys

from fontTools import subset
from fontTools.ttLib import TTFont, TTCollection

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "..", "testdata")

# Latin with its accents (composites in most fonts), Greek, Cyrillic, the
# punctuation layout meets, ligatures, and combining marks.
LATIN = "".join(chr(c) for c in range(0x20, 0x7F))
LATIN += " ÀÁÂÃÄÅÇÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜÝàáâãäåçèéêëìíîïñòóôõöùúûüýÿ"
LATIN += "ĀāĞğİıŁłŒœŠšŸŽž"
LATIN += "ΑΒΓΔΩαβγδω" + "АБВГДабвгд"
LATIN += " –—‘’“”•…€™ﬁﬂ"
LATIN += "̧̀́̂̃̈"
# Kanji with registered variation sequences (葛 and 辻 have Adobe-Japan1
# forms) and the selectors themselves, kana, and two ideographs beyond the
# Basic Multilingual Plane — which is what keeps a format 12 cmap.
CJK = "漢字かなカナ一二三葛辻\U00020B9F\U00029E3D"
CJK += "\uFE00\uFE01" + "".join(chr(c) for c in range(0xE0100, 0xE0104))
CJK += LATIN[:95]

SOURCES = [
    # (output, source, face index in a collection or None, text, new family)
    ("glyf.ttf", "/usr/share/fonts/noto/NotoSans-Regular.ttf", None, LATIN, "Caustic Test Glyf"),
    ("cff.otf", "/usr/share/fonts/adobe-source-sans/SourceSans3-Regular.otf", None, LATIN, "Caustic Test CFF"),
    ("cff2.otf", "/usr/share/fonts/cantarell/Cantarell-VF.otf", None, LATIN, "Caustic Test CFF2"),
    ("vf.ttf", "/usr/share/fonts/inter/InterVariable.ttf", None, LATIN, "Caustic Test Variable"),
    ("kern.ttf", "/usr/share/fonts/liberation/LiberationSans-Regular.ttf", None, LATIN, "Caustic Test Kern"),
    ("cid.otf", "/usr/share/fonts/adobe-source-han-sans/SourceHanSansJP-Regular.otf", None, CJK, "Caustic Test CID"),
]
# Two faces of one collection, cut down and put back into one.
COLLECTION = ("pair.ttc", "/usr/share/fonts/inter/Inter.ttc", ["Inter Regular", "Inter Bold"], LATIN,
              ["Caustic Test Pair", "Caustic Test Pair"])

OFL_TEXT_FROM = "/usr/share/licenses/inter-font/LICENSE.txt"


def ofl_body():
    # The licence text proper, after its copyright lines.
    text = open(OFL_TEXT_FROM, encoding="utf-8").read()
    return text[text.index("This Font Software is licensed"):]


def subset_font(font, text):
    opts = subset.Options()
    # The default features, not every alternate a font carries; the legacy
    # cmap and kern tables kept, since they are what is being tested.
    opts.legacy_cmap = True
    opts.legacy_kern = True
    opts.name_IDs = ["*"]
    opts.name_languages = ["*"]
    opts.notdef_outline = True
    opts.glyph_names = True
    opts.recalc_timestamp = False
    opts.drop_tables = ["DSIG"]
    s = subset.Subsetter(options=opts)
    s.populate(text=text)
    s.subset(font)


def rename(font, old_names, family):
    # Every name record naming the original family, and the CFF's own names.
    name = font["name"]
    ps = family.replace(" ", "")
    for rec in name.names:
        s = rec.toUnicode()
        for old in old_names:
            if old and old in s:
                s = s.replace(old, family)
        old_ps = [o.replace(" ", "") for o in old_names if o]
        for o in old_ps:
            if o in s:
                s = s.replace(o, ps)
        rec.string = s
    for tag in ("CFF ",):
        if tag in font:
            cff = font[tag].cff
            cff.fontNames = [ps + "-" + n.split("-", 1)[-1] if "-" in n else ps for n in cff.fontNames]
            top = cff.topDictIndex[0]
            for key in ("FamilyName", "FullName", "Notice", "CIDFontName"):
                if hasattr(top, key):
                    v = getattr(top, key)
                    for old in old_names:
                        if old and isinstance(v, str):
                            v = v.replace(old, family).replace(old.replace(" ", ""), ps)
                    setattr(top, key, v)
            if hasattr(top, "FDArray"):
                for fd in top.FDArray:
                    if hasattr(fd, "FontName"):
                        fd.FontName = fd.FontName.replace(old_names[0].replace(" ", ""), ps)


def make_subset(out, src, text, family):
    font = TTFont(src, recalcTimestamp=False)
    licence_font = TTFont(src, lazy=True)
    old = families(font)
    subset_font(font, text)
    rename(font, old, family)
    font.save(os.path.join(OUT, out))
    licence(licence_font, out)
    print(out, os.path.getsize(os.path.join(OUT, out)), "bytes")


def licence(font, out_name):
    copyright = font["name"].getDebugName(0) or ""
    with open(os.path.join(OUT, "licenses", out_name + ".txt"), "w", encoding="utf-8") as f:
        f.write(copyright.strip() + "\n")
        f.write("Subset and renamed \"" + font["name"].getDebugName(1) + "\" for caustic-media's tests;\n")
        f.write("see text/tools/make_testdata.py.\n\n")
        f.write(ofl_body())


def families(font):
    n = font["name"]
    out = []
    for i in (16, 1):
        v = n.getDebugName(i)
        if v and v not in out:
            out.append(v)
    return out


def synthetic_cmaps():
    # One font, a cmap subtable of every format a reader meets: 0 (bytes,
    # Mac), 4 (BMP ranges), 6 (a dense run), 12 (32-bit ranges), 13 (many
    # characters to one glyph, as last-resort fonts do) and 14 (variation
    # sequences, default and not). Glyphs are empty squares; only the
    # mappings matter.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    from fontTools.ttLib.tables._c_m_a_p import CmapSubtable
    names = [".notdef"] + ["g%d" % i for i in range(1, 40)]
    fb = FontBuilder(1000, isTTF=True)
    fb.setupGlyphOrder(names)
    pen = TTGlyphPen(None)
    pen.moveTo((100, 0)); pen.lineTo((100, 700)); pen.lineTo((500, 700)); pen.lineTo((500, 0)); pen.closePath()
    sq = pen.glyph()
    pen = TTGlyphPen(None)
    pen.moveTo((100, 0)); pen.lineTo((300, 700)); pen.lineTo((500, 0)); pen.closePath()
    tri = pen.glyph()
    fb.setupGlyf({n: (sq if i % 2 == 0 else tri) for i, n in enumerate(names)})
    fb.setupHorizontalMetrics({n: (600 + 10 * i, 100) for i, n in enumerate(names)})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupNameTable({"familyName": "Caustic Test Cmaps", "styleName": "Regular"})
    bmp = {0x41: "g1", 0x42: "g2", 0x43: "g3", 0x61: "g4", 0x3B1: "g5", 0x4E00: "g6", 0x4E01: "g7", 0xFFFD: "g8"}
    fb.setupCharacterMap(bmp)
    # Italic, monospaced and asking for its typographic metrics: what no
    # subset above is, so the reader's flags are checked somewhere.
    fb.setupOS2(fsSelection=0x81, sTypoAscender=900, sTypoDescender=-300, sTypoLineGap=50,
                usWinAscent=1100, usWinDescent=400, sxHeight=480, sCapHeight=690, version=4,
                yStrikeoutSize=55, yStrikeoutPosition=260)
    fb.setupPost(isFixedPitch=1, italicAngle=-12.5, underlinePosition=-120, underlineThickness=60)
    # Fixed dates, so the file comes out the same every time.
    fb.updateHead(macStyle=2, created=3786825600, modified=3786825600)
    cmap = fb.font["cmap"]
    cmap.tables = []
    def add(plat, enc, fmt, mapping):
        t = CmapSubtable.newSubtable(fmt)
        t.platformID, t.platEncID, t.language = plat, enc, 0
        t.cmap = dict(mapping)
        cmap.tables.append(t)
        return t
    add(0, 3, 4, bmp)
    full = dict(bmp)
    full.update({0x1F600: "g9", 0x1F601: "g10", 0x1D400: "g11", 0x10FFFD: "g12"})
    add(3, 10, 12, full)
    add(1, 0, 0, {0x20: "g13", 0x41: "g14", 0x42: "g15", 0xFF: "g16"})
    add(1, 1, 6, {0x30 + i: "g%d" % (17 + i) for i in range(10)})
    t13 = add(0, 6, 13, {})
    t13.cmap = {c: "g27" for c in range(0x2000, 0x2100)}
    t13.cmap.update({c: "g28" for c in range(0x30000, 0x30010)})
    t13.cmap[0x10FFFF] = "g29"
    t14 = CmapSubtable.newSubtable(14)
    t14.platformID, t14.platEncID, t14.language = 0, 5, 0
    t14.cmap = {}
    t14.uvsDict = {0xFE00: [(0x41, None), (0x4E00, "g30")], 0xE0100: [(0x4E01, "g31"), (0x42, None)]}
    cmap.tables.append(t14)
    # Unpadded, so glyph offsets are odd and loca has to be the long form,
    # which no subset above uses.
    fb.font["glyf"].padding = 0
    fb.save(os.path.join(OUT, "cmaps.ttf"))
    print("cmaps.ttf", os.path.getsize(os.path.join(OUT, "cmaps.ttf")), "bytes")


def synthetic_composites():
    # TrueType composites in every form a reader meets: an offset, a uniform
    # scale (offset not scaled, the Microsoft way), the same with the offset
    # scaled (the Apple way), x and y scales, a 2x2 matrix, a component placed
    # by matching points, composites of composites — and a base glyph with a
    # contour of off-curve points only, and one with consecutive off-curves.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    from fontTools.ttLib.tables._g_l_y_f import Glyph, GlyphComponent, GlyphCoordinates
    from fontTools.ttLib.tables import _g_l_y_f as G
    names = [".notdef", "base", "round", "offset", "scaled", "scaled_apple", "xy", "matrix",
             "matched", "nested", "empty", "matched_scaled", "rotated"]
    fb = FontBuilder(2048, isTTF=True)
    fb.setupGlyphOrder(names)

    def simple(points, flags, ends):
        g = Glyph()
        g.numberOfContours = len(ends)
        g.coordinates = GlyphCoordinates(points)
        g.flags = bytearray(flags)
        g.endPtsOfContours = ends
        from fontTools.ttLib.tables import ttProgram
        g.program = ttProgram.Program()
        g.program.fromBytecode(b"")
        return g

    # Two contours: on, off, off, on, off (wrapping); and four off-curves.
    base = simple([(0, 0), (100, 300), (300, 300), (400, 0), (200, -100),
                   (500, 500), (700, 500), (700, 700), (550, 700)],
                  [1, 0, 0, 1, 0, 0, 0, 0, 0], [4, 8])
    # Starting off the curve, with on-curve points after.
    rotated = simple([(0, 100), (100, 0), (200, 100), (100, 200)], [0, 1, 0, 1], [3])
    pen = TTGlyphPen(None)
    pen.moveTo((10, 10)); pen.lineTo((10, 90)); pen.qCurveTo((50, 130), (90, 90)); pen.closePath()
    rnd = pen.glyph()

    def comp(name, x=0, y=0, transform=None, flags=0, first=None, second=None):
        c = GlyphComponent()
        c.glyphName = name
        c.flags = flags
        if first is not None:
            c.firstPt, c.secondPt = first, second
        else:
            c.x, c.y = x, y
            c.flags |= G.ARGS_ARE_XY_VALUES
        if transform is not None:
            c.transform = transform
        return c

    def composite(comps):
        g = Glyph()
        g.numberOfContours = -1
        g.components = comps
        return g

    glyphs = {
        ".notdef": rnd, "base": base, "round": rnd,
        "offset": composite([comp("base", 100, 50)]),
        "scaled": composite([comp("base", 100, 50, [[0.5, 0], [0, 0.5]])]),
        "scaled_apple": composite([comp("base", 100, 50, [[0.5, 0], [0, 0.5]], G.SCALED_COMPONENT_OFFSET)]),
        "xy": composite([comp("base", -30, 20, [[0.75, 0], [0, 1.25]])]),
        "matrix": composite([comp("round", 5, 7, [[0.5, 0.25], [-0.25, 0.75]])]),
        "matched": composite([comp("base", 0, 0), comp("round", first=3, second=0)]),
        "nested": composite([comp("offset", 10, 0), comp("matrix", 0, 300)]),
        "empty": Glyph(),
        "matched_scaled": composite([comp("base", 0, 0), comp("round", first=3, second=0,
                                                              transform=[[0.5, 0], [0, 0.5]])]),
        "rotated": rotated,
    }
    fb.setupGlyf(glyphs)
    fb.setupHorizontalMetrics({n: (800, 0) for n in names})
    fb.setupHorizontalHeader(ascent=1900, descent=-500)
    fb.setupNameTable({"familyName": "Caustic Test Composites", "styleName": "Regular"})
    fb.setupCharacterMap({0x41 + i: n for i, n in enumerate(names[1:])})
    fb.setupOS2()
    fb.setupPost()
    fb.updateHead(created=3786825600, modified=3786825600)
    fb.save(os.path.join(OUT, "composite.ttf"))
    print("composite.ttf", os.path.getsize(os.path.join(OUT, "composite.ttf")), "bytes")


def synthetic_ops():
    # A CFF font whose glyphs between them use every Type 2 path operator:
    # each line and curve form with its odd and even argument counts, the four
    # flexes, stems and masks, local and global subroutines, and endchar's
    # deprecated accent building (seac), which needs glyphs by their standard
    # names.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.misc.psCharStrings import T2CharString
    names = [".notdef", "A", "acute", "Aacute", "lines", "hv", "curves", "flexes", "hints", "manystems", "subrs",
             "nomove"]
    gsubrs = [[10, 20, "rlineto", "return"]]
    # Subroutine numbers are biased: with fewer than 1240, by 107.
    lsubrs = [[30, -40, "rlineto", "return"], [5, 5, 5, -5, 5, 0, "rrcurveto", -107, "callsubr", "return"]]
    progs = {
        ".notdef": [100, 0, "hmoveto", 400, 700, -400, "hlineto", "endchar"],
        "A": [500, 0, 0, "rmoveto", 250, 700, 250, -700, "rlineto", "endchar"],
        "acute": [200, 100, 600, "rmoveto", 100, 150, -30, 10, "rlineto", "endchar"],
        # Accent building: the base A, the acute moved by 120, 80.
        "Aacute": [500, 120, 80, 65, 194, "endchar"],
        "lines": [600, 50, "vmoveto", 100, "hlineto", 50, 60, 70, "vlineto", 10, 20, 30, 40, "hlineto",
                  -5, -6, "rlineto", "endchar"],
        "hv": [600, 10, 10, "rmoveto",
               # hhcurveto with a leading dy, vvcurveto with a leading dx
               5, 20, 10, 30, 40, "hhcurveto", 7, 20, 10, 30, 40, "vvcurveto",
               20, 10, 30, 40, 20, 10, 30, 40, "hhcurveto", 20, 10, 30, 40, "vvcurveto",
               # hvcurveto and vhcurveto: 4, 8 and 9 arguments
               10, 20, 30, 40, "hvcurveto", 10, 20, 30, 40, 50, 60, 70, 80, "hvcurveto",
               10, 20, 30, 40, 50, 60, 70, 80, 90, "hvcurveto",
               10, 20, 30, 40, "vhcurveto", 10, 20, 30, 40, 50, 60, 70, 80, 15, "vhcurveto",
               10, 20, 30, 40, 50, "vhcurveto", "endchar"],
        "curves": [600, 0, 0, "rmoveto", 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, "rrcurveto",
                   10, 20, 30, 40, 50, 60, 70, 80, "rcurveline", 10, 20, 30, 40, 50, 60, 70, 80, "rlinecurve",
                   "endchar"],
        "flexes": [600, 0, 0, "rmoveto",
                   10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 50, "flex",
                   10, 20, 30, 40, 50, 60, 70, "hflex",
                   10, 20, 30, 40, 50, 60, 70, 80, 90, "hflex1",
                   10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, "flex1",
                   -10, -20, -30, -40, -50, -60, -70, -80, -90, -100, 110, "flex1", "endchar"],
        # Four stems, then a fifth declared by the vstem before cntrmask: the
        # masks grow from one byte's worth of stems to... still one byte, but
        # the stems must be counted to know.
        "hints": [600, 0, 50, 100, 50, "hstemhm", 10, 20, 30, 40, "vstemhm", "hintmask", bytes([0b11110000]),
                  0, 0, "rmoveto", 100, "hlineto", 5, 5, "vstem", "cntrmask", bytes([0b11111000]),
                  100, "vlineto", "hintmask", bytes([0b10101000]), -100, "hlineto", "endchar"],
        # Nine stems: the masks take two bytes, and the first hintmask's
        # pending arguments are vertical stems.
        "manystems": [600, 0, 10, 20, 10, 40, 10, 60, 10, 80, 10, "hstemhm",
                      0, 10, 20, 10, 40, 10, 60, 10, "hintmask", bytes([255, 128]),
                      0, 0, "rmoveto", 50, "hlineto", "hintmask", bytes([1, 0]), 50, "vlineto", "endchar"],
        # Drawing before any moveto: the contour starts where the pen is.
        "nomove": [10, 20, "rlineto", 30, 40, "rlineto", "endchar"],
        "subrs": [600, 0, 0, "rmoveto", -107, "callgsubr", -107, "callsubr", -106, "callsubr",
                  -50, "hlineto", "endchar"],
    }
    fb = FontBuilder(1000, isTTF=False)
    fb.setupGlyphOrder(names)
    fb.setupCharacterMap({0x41: "A", 0xB4: "acute", 0xC1: "Aacute"})
    charstrings = {}
    for n in names:
        charstrings[n] = T2CharString(program=progs[n])
    fb.setupCFF("CausticTestOps-Regular", {"FullName": "Caustic Test Ops"}, charstrings, {})
    cff = fb.font["CFF "].cff
    top = cff.topDictIndex[0]
    from fontTools.cffLib import GlobalSubrsIndex, SubrsIndex
    for p in gsubrs:
        cff.GlobalSubrs.append(T2CharString(program=p))
    top.Private.Subrs = SubrsIndex()
    for p in lsubrs:
        top.Private.Subrs.append(T2CharString(program=p))
    for n in names:
        cs = top.CharStrings[n]
        cs.private = top.Private
        cs.globalSubrs = cff.GlobalSubrs
    fb.setupHorizontalMetrics({n: (600, 0) for n in names})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupNameTable({"familyName": "Caustic Test Ops", "styleName": "Regular"})
    fb.setupOS2()
    fb.setupPost()
    fb.updateHead(created=3786825600, modified=3786825600)
    fb.save(os.path.join(OUT, "ops.otf"))
    print("ops.otf", os.path.getsize(os.path.join(OUT, "ops.otf")), "bytes")


def synthetic_bombs():
    # Fonts a reader must refuse rather than follow. bomb.ttf: composites
    # fourteen deep, each holding the next twice — 16384 components with no
    # points, past the reader's limit though no deeper than it allows — and a
    # composite holding itself; composites of 4096 components all told, the
    # limit, and of 4097. bomb.otf: ten subroutines, each calling the
    # next four times, nested no deeper than allowed, a quarter of a million
    # calls. No outline reference is written for either.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.ttLib.tables._g_l_y_f import Glyph, GlyphComponent
    from fontTools.ttLib.tables import _g_l_y_f as G
    from fontTools.misc.psCharStrings import T2CharString
    from fontTools.cffLib import SubrsIndex
    depth = 14
    chain = 18
    names = ([".notdef", "empty", "loop"] + ["dag%d" % i for i in range(depth)]
             + ["chain%d" % i for i in range(chain)] + ["big", "twice", "dot", "edge", "over"])
    fb = FontBuilder(1000, isTTF=True)
    fb.setupGlyphOrder(names)

    def composite(children):
        g = Glyph()
        g.numberOfContours = -1
        g.components = []
        for ch in children:
            c = GlyphComponent()
            c.glyphName = ch
            c.x, c.y = 0, 0
            c.flags = G.ARGS_ARE_XY_VALUES
            g.components.append(c)
        return g

    # "loop" is saved holding "empty", which fontTools allows, and patched
    # below to hold itself, which it does not.
    glyphs = {".notdef": Glyph(), "empty": Glyph(), "loop": composite(["empty"])}
    for i in range(depth):
        child = "dag%d" % (i + 1) if i + 1 < depth else "empty"
        glyphs["dag%d" % i] = composite([child, child])
    # One component each, 18 deep: chain1 reaches the empty glyph 17 below,
    # past the limit of 16; chain2 does not.
    for i in range(chain):
        child = "chain%d" % (i + 1) if i + 1 < chain else "empty"
        glyphs["chain%d" % i] = composite([child])
    # 40000 points: drawn alone, but twice is more than a glyph may expand to.
    from fontTools.ttLib.tables._g_l_y_f import GlyphCoordinates
    from fontTools.ttLib.tables import ttProgram
    big = Glyph()
    big.numberOfContours = 1
    big.coordinates = GlyphCoordinates([(i % 200, i // 200) for i in range(40000)])
    big.flags = bytearray([1] * 40000)
    big.endPtsOfContours = [39999]
    big.program = ttProgram.Program()
    big.program.fromBytecode(b"")
    glyphs["big"] = big
    glyphs["twice"] = composite(["big", "big"])
    # One on-curve point at the origin: one flag and no coordinates.
    dot = Glyph()
    dot.numberOfContours = 1
    dot.coordinates = GlyphCoordinates([(0, 0)])
    dot.flags = bytearray([1])
    dot.endPtsOfContours = [0]
    dot.program = ttProgram.Program()
    dot.program.fromBytecode(b"")
    glyphs["dot"] = dot
    # dag3 is 4094 components below it: with itself and one more, 4096; with
    # two more, 4097.
    glyphs["edge"] = composite(["dag3", "empty"])
    glyphs["over"] = composite(["dag3", "empty", "empty"])
    fb.setupGlyf(glyphs, validateGlyphFormat=False)
    fb.font.recalcBBoxes = False
    fb.setupHorizontalMetrics({n: (500, 0) for n in names})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupNameTable({"familyName": "Caustic Test Bomb", "styleName": "Regular"})
    fb.setupCharacterMap({0x41: "dag0"})
    fb.setupOS2()
    fb.setupPost()
    fb.updateHead(created=3786825600, modified=3786825600)
    path = os.path.join(OUT, "bomb.ttf")
    fb.save(path)
    import struct
    font = TTFont(path)
    glyf_off = font.reader.tables["glyf"].offset
    loop = names.index("loop")
    start = font["loca"][loop]
    data = bytearray(open(path, "rb").read())
    # The first component's glyph index, after the 10-byte header and its flags.
    struct.pack_into(">H", data, glyf_off + start + 12, loop)
    open(path, "wb").write(bytes(data))
    print("bomb.ttf", os.path.getsize(path), "bytes")

    names = [".notdef", "fan"]
    fb = FontBuilder(1000, isTTF=False)
    fb.setupGlyphOrder(names)
    fb.setupCharacterMap({0x41: "fan"})
    progs = {".notdef": [500, "endchar"], "fan": [500, 0, 0, "rmoveto", -107, "callsubr", "endchar"]}
    fb.setupCFF("CausticTestBomb-Regular", {"FullName": "Caustic Test Bomb"},
                {n: T2CharString(program=progs[n]) for n in names}, {})
    top = fb.font["CFF "].cff.topDictIndex[0]
    top.Private.Subrs = SubrsIndex()
    for k in range(10):
        if k < 9:
            prog = [-107 + k + 1, "callsubr"] * 4 + ["return"]
        else:
            prog = ["return"]
        top.Private.Subrs.append(T2CharString(program=prog))
    for n in names:
        top.CharStrings[n].private = top.Private
    fb.font.recalcBBoxes = False
    fb.setupHorizontalMetrics({n: (500, 0) for n in names})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupNameTable({"familyName": "Caustic Test Bomb CFF", "styleName": "Regular"})
    fb.setupOS2()
    fb.setupPost()
    fb.updateHead(created=3786825600, modified=3786825600)
    fb.save(os.path.join(OUT, "bomb.otf"))
    print("bomb.otf", os.path.getsize(os.path.join(OUT, "bomb.otf")), "bytes")
    # The same, saying its charstrings are Type 1: not read at all.
    fb.font["CFF "].cff.topDictIndex[0].CharstringType = 1
    fb.save(os.path.join(OUT, "bomb-type1.otf"))
    print("bomb-type1.otf", os.path.getsize(os.path.join(OUT, "bomb-type1.otf")), "bytes")


def synthetic_big():
    # A CFF font big enough for what small fonts never need: charstrings past
    # 64 KB, so the INDEX offsets take three bytes; 1300 local subroutines, so
    # their numbers are biased by 1131; fixed-point numbers; glyph names out of
    # the standard strings' order, so the charset is format 0 — with an accent
    # built by seac through it; and every name a standard string, with no
    # FontInfo, so the Strings INDEX is empty and the global subroutine one of
    # the glyphs calls is found only by stepping over it.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.misc.psCharStrings import T2CharString
    from fontTools.cffLib import SubrsIndex
    names = [".notdef", "A", "B", "acute", "C", "Aacute", "D"]
    stems = []
    for k in range(1750):
        stems += [0.5, 0.25, 0.5, 0.25, 0.5, 0.25, 0.5, 0.25, "hstemhm"]
    progs = {
        ".notdef": [500, "endchar"],
        "A": [500, 0, 0, "rmoveto", 250, 700, 250, -700, "rlineto", "endchar"],
        "B": [500, 0.5, 0, "rmoveto", 100.25, "hlineto", 1250 - 1131, "callsubr", "endchar"],
        "acute": [200, 100, 600, "rmoveto", 100, 150, -30, 10, "rlineto", "endchar"],
        "C": [500, 0, 0, "rmoveto", 10, "hlineto", -107, "callgsubr", "endchar"],
        "Aacute": [500, 120, 80, 65, 194, "endchar"],
        "D": stems + [0, 0, "rmoveto", 10.5, "hlineto", "endchar"],
    }
    fb = FontBuilder(1000, isTTF=False)
    fb.setupGlyphOrder(names)
    fb.setupCharacterMap({0x41: "A", 0xB4: "acute", 0xC1: "Aacute"})
    fb.setupCFF("CausticTestBig-Regular", {}, {n: T2CharString(program=progs[n]) for n in names}, {})
    cff = fb.font["CFF "].cff
    top = cff.topDictIndex[0]
    top.Private.Subrs = SubrsIndex()
    for k in range(1300):
        prog = [0, 50.5, "rlineto", "return"] if k == 1250 else ["return"]
        top.Private.Subrs.append(T2CharString(program=prog))
    cff.GlobalSubrs.append(T2CharString(program=[0, 30, "rlineto", "return"]))
    for n in names:
        top.CharStrings[n].private = top.Private
        top.CharStrings[n].globalSubrs = cff.GlobalSubrs
    fb.setupHorizontalMetrics({n: (500, 0) for n in names})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupNameTable({"familyName": "Caustic Test Big", "styleName": "Regular"})
    fb.setupOS2()
    fb.setupPost()
    fb.updateHead(created=3786825600, modified=3786825600)
    fb.save(os.path.join(OUT, "big.otf"))
    print("big.otf", os.path.getsize(os.path.join(OUT, "big.otf")), "bytes")


def synthetic_var():
    # A variable TrueType font for what real ones seldom show: deltas on
    # every point and on a few, left to interpolation (IUP) in each of its
    # cases; regions with an intermediate start and end, over two axes, on the
    # negative side; point numbers past 255, more than 255 of them, and runs
    # past 128; deltas of every width, 32 bits included; components moved by
    # deltas, scaled every way with more after them, matched ones that are
    # not — by points past 255 too —, one lending the composite its metrics,
    # an empty one lending them too; phantom points moving the
    # side bearings and advance, with no HVAR to say otherwise; an axis whose
    # minimum is its default, hidden; avar version 2; and MVAR.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.ttLib import newTable
    from fontTools.ttLib.tables import otTables as ot
    from fontTools.ttLib.tables.TupleVariation import TupleVariation
    from fontTools.ttLib.tables._g_l_y_f import Glyph, GlyphComponent, GlyphCoordinates
    from fontTools.ttLib.tables import _g_l_y_f as G
    from fontTools.ttLib.tables import ttProgram
    from fontTools.varLib.builder import buildVarRegionList, buildVarData, buildVarStore
    names = [".notdef", "square", "iup", "many", "comp", "matched", "mymetrics", "phantom", "space", "nested",
             "spaces", "scales", "manymatched"]
    fb = FontBuilder(1000, isTTF=True)
    fb.setupGlyphOrder(names)

    def simple(points, flags, ends):
        g = Glyph()
        g.numberOfContours = len(ends)
        g.coordinates = GlyphCoordinates(points)
        g.flags = bytearray(flags)
        g.endPtsOfContours = ends
        g.program = ttProgram.Program()
        g.program.fromBytecode(b"")
        return g

    def comp(name, x=0, y=0, transform=None, flags=0, first=None, second=None):
        c = GlyphComponent()
        c.glyphName = name
        c.flags = flags
        if first is not None:
            c.firstPt, c.secondPt = first, second
        else:
            c.x, c.y = x, y
            c.flags |= G.ARGS_ARE_XY_VALUES
        if transform is not None:
            c.transform = transform
        return c

    def composite(comps):
        g = Glyph()
        g.numberOfContours = -1
        g.components = comps
        return g

    square = simple([(100, 0), (100, 700), (500, 700), (500, 0)], [1, 1, 1, 1], [3])
    # Four contours for interpolation: one moved point moves its contour
    # whole; a contour with none stays; two moved points at the same x with
    # different deltas, untouched points outside and between them; the first
    # moved point not the first of its contour, so a run wraps around.
    iup_pts = [(0, 0), (0, 100), (100, 100), (100, 0), (50, -50), (20, 10),
               (200, 0), (200, 100), (300, 100), (300, 0), (250, 50),
               (400, 0), (400, 300), (450, 400), (500, 300), (500, 0), (450, -100), (380, 150), (520, 150),
               (600, 0), (600, 200), (700, 250), (800, 200), (800, 0), (700, -50)]
    iup_ends = [5, 10, 18, 24]
    iup = simple(iup_pts, [1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0], iup_ends)
    # A zigzag of 300 points.
    many_pts = [(i * 3, (i % 2) * 40) for i in range(300)]
    many = simple(many_pts, [1] * 300, [299])
    space = Glyph()
    glyphs = {
        ".notdef": simple([(50, 0), (50, 500), (350, 500), (350, 0)], [1, 1, 1, 1], [3]),
        "square": square, "iup": iup, "many": many,
        "comp": composite([comp("square", 50, 10), comp("square", 600, 0, [[0.5, 0], [0, 0.5]])]),
        "matched": composite([comp("square", 0, 0), comp("square", first=2, second=0)]),
        "mymetrics": composite([comp("iup", 600, 0), comp("square", 0, 0, flags=G.USE_MY_METRICS)]),
        "phantom": simple([(150, 0), (150, 300), (350, 300)], [1, 1, 1], [2]),
        "space": space,
        "nested": composite([comp("comp", 0, 100)]),
        # More components than points, the empty glyph's metrics the whole's.
        "spaces": composite([comp("phantom", 0, 0), comp("space", 30, 0, flags=G.USE_MY_METRICS),
                             comp("space", 60, 0), comp("space", 90, 0)]),
        "scales": composite([comp("square", 10, 0, [[0.5, 0], [0, 0.75]]),
                             comp("square", 300, 0, [[0.5, 0.25], [0.25, 0.5]]),
                             comp("square", 600, 0, [[0.5, 0], [0, 0.5]]),
                             comp("square", 900, 0)]),
        "manymatched": composite([comp("many", 0, 0), comp("square", first=299, second=2)]),
    }
    fb.setupGlyf(glyphs)
    fb.setupHorizontalMetrics({".notdef": (400, 50), "square": (600, 100), "iup": (900, 0), "many": (900, 0),
                               "comp": (900, 50), "matched": (1000, 0), "mymetrics": (1500, 0),
                               "phantom": (500, 150), "space": (250, 0), "nested": (900, 50),
                               "spaces": (700, 150), "scales": (1400, 60), "manymatched": (1300, 0)})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupNameTable({"familyName": "Caustic Test Var", "styleName": "Regular"})
    fb.setupCharacterMap({0x41 + i: n for i, n in enumerate(names[1:])})
    fb.setupOS2(sxHeight=500, sCapHeight=700, sTypoAscender=800, sTypoDescender=-200, sTypoLineGap=100,
                usWinAscent=900, usWinDescent=250, yStrikeoutSize=50, yStrikeoutPosition=300)
    fb.setupPost(underlinePosition=-100, underlineThickness=50)
    fb.setupFvar([("wght", 100, 400, 900, "Weight"), ("wdth", 75, 100, 125, "Width"),
                  ("ZHID", 0, 0, 100, "Hidden")],
                 [{"location": {"wght": 400, "wdth": 100, "ZHID": 0}, "stylename": "Regular",
                   "postscriptfontname": "CausticTestVar-Regular"},
                  {"location": {"wght": 700, "wdth": 100, "ZHID": 0}, "stylename": "Bold",
                   "postscriptfontname": "CausticTestVar-Bold"},
                  {"location": {"wght": 250, "wdth": 75, "ZHID": 40}, "stylename": "Light Condensed",
                   "postscriptfontname": "CausticTestVar-LightCondensed"}])
    fb.font["fvar"].axes[2].flags = 1   # HIDDEN_AXIS

    def deltas(n, pts):
        # Deltas for points pts (a dict), None for the rest: n points and four
        # phantom points.
        return [pts.get(i) for i in range(n + 4)]

    W = {"wght": (0, 1, 1)}
    Wn = {"wght": (-1, -1, 0)}
    D = {"wdth": (0, 1, 1)}
    variations = {
        "square": [
            TupleVariation(W, [(10, 0), (10, 20), (30, 20), (30, 0), (-20, 0), (60, 0), (0, 0), (0, 0)]),
            TupleVariation(Wn, deltas(4, {0: (-5, 0), 2: (5, -10)})),
            TupleVariation({"wdth": (0, 0.5, 1)}, deltas(4, {1: (0, 40), 5: (25, 0)})),
            TupleVariation({"wght": (0, 1, 1), "wdth": (0, 1, 1)}, deltas(4, {3: (7, 7)})),
            TupleVariation({"ZHID": (0, 1, 1)}, deltas(4, {0: (0, -30), 1: (0, 30)})),
        ],
        "iup": [
            TupleVariation(W, deltas(25, {2: (10, 20),
                                          12: (5, 8), 15: (-5, 8),
                                          21: (30, -10), 23: (0, 4)})),
            TupleVariation(D, deltas(25, {0: (-4, 0), 3: (6, 2), 8: (1, 1), 9: (3, 3),
                                          11: (0, 0), 12: (0, 0), 14: (10, 10), 16: (2, -2)})),
        ],
        "many": [
            TupleVariation(W, deltas(300, {0: (1, 1), 5: (200, -300), 270: (40000, 0), 299: (0, 129)})),
            TupleVariation(D, deltas(300, {i: (i % 7 - 3, 0) for i in range(0, 260)})),
        ],
        "comp": [TupleVariation(W, [(30, 5), (0, 40), (0, 0), (80, 0), (0, 0), (0, 0)])],
        "matched": [TupleVariation(W, [(30, 5), (500, 500), (0, 0), (0, 0), (0, 0), (0, 0)])],
        "mymetrics": [TupleVariation(W, [(0, 0), (0, 0), (-300, 0), (300, 0), (0, 0), (0, 0)])],
        "phantom": [TupleVariation(W, deltas(3, {3: (40, 0), 4: (100, 0)}))],
        "space": [TupleVariation(W, deltas(0, {1: (150, 0)}))],
        "nested": [TupleVariation(D, [(0, -60), (0, 0), (0, 0), (0, 0), (0, 0)])],
        "spaces": [TupleVariation(W, [(20, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0)])],
        "scales": [TupleVariation(W, [(20, 5), (-30, 10), (40, 0), (0, 7), (0, 0), (60, 0), (0, 0), (0, 0)])],
        "manymatched": [TupleVariation(D, [(0, 0), (500, 500), (0, 0), (-80, 0), (0, 0), (0, 0)])],
    }
    fb.setupGvar(variations)
    axisTags = ["wght", "wdth", "ZHID"]
    avar = newTable("avar")
    avar.majorVersion = 2
    avar.minorVersion = 0
    avar.segments = {"wght": {-1.0: -1.0, -0.5: -0.75, 0.0: 0.0, 0.5: 0.25, 1.0: 1.0},
                     "wdth": {-1.0: -1.0, 0.0: 0.0, 1.0: 1.0},
                     "ZHID": {}}
    avar.table = ot.avar()
    avar.table.VarIdxMap = ot.DeltaSetIndexMap()
    avar.table.VarIdxMap.Format = 0
    avar.table.VarIdxMap.mapping = [0, 1, 1]
    avar.table.VarStore = buildVarStore(buildVarRegionList([{"wdth": (0, 1, 1)}], axisTags),
                                        [buildVarData([0], [[2000], [0]], optimize=False)])
    fb.font["avar"] = avar
    # MVAR: line metrics and the rest over weight, one past 16 bits.
    tags = ["hasc", "hcla", "hcld", "hdsc", "hlgp", "cpht", "stro", "strs", "undo", "unds", "xhgt"]
    rows = [[40, -10], [60, 0], [-30, 5], [-20, 0], [10, 10], [25, 0], [-15, 3], [40000, 0], [7, 0], [-3, 0], [33, -8]]
    store = buildVarStore(buildVarRegionList([W, D], axisTags), [buildVarData([0, 1], rows, optimize=False)])
    mvar = newTable("MVAR")
    mvar.table = ot.MVAR()
    mvar.table.Version = 0x00010000
    mvar.table.Reserved = 0
    mvar.table.ValueRecordSize = 8
    mvar.table.VarStore = store
    mvar.table.ValueRecord = []
    # Sorted by tag, which readers search by halves.
    for t, i in sorted((t, i) for i, t in enumerate(tags)):
        r = ot.MetricsValueRecord()
        r.ValueTag = t
        r.VarIdx = i
        mvar.table.ValueRecord.append(r)
    mvar.table.ValueRecordCount = len(tags)
    fb.font["MVAR"] = mvar
    fb.updateHead(created=3786825600, modified=3786825600)
    fb.save(os.path.join(OUT, "var.ttf"))
    print("var.ttf", os.path.getsize(os.path.join(OUT, "var.ttf")), "bytes")


def synthetic_var_cff2():
    # A variable CFF2 font: two sets of variation data, the second chosen by
    # the Private DICT (three regions a value) and the first by vsindex in a
    # charstring (two); regions
    # over two axes, one intermediate; blends of one value and of several;
    # and HVAR without a mapping, glyph by glyph.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.misc.psCharStrings import T2CharString
    from fontTools.ttLib import newTable
    from fontTools.ttLib.tables import otTables as ot
    from fontTools.varLib.builder import buildVarRegionList, buildVarData, buildVarStore
    from fontTools.cffLib import VarStoreData
    names = [".notdef", "box", "vs0", "multi", "curve"]
    axisTags = ["wght", "wdth"]
    fb = FontBuilder(1000, isTTF=False)
    fb.setupGlyphOrder(names)
    fb.setupNameTable({"familyName": "Caustic Test Var CFF2", "styleName": "Regular"})
    fb.setupFvar([("wght", 100, 400, 900, "Weight"), ("wdth", 75, 100, 125, "Width")], [])
    # Data 0 weighs regions 0 and 1; data 1, the Private DICT's, regions 2, 3, 0.
    regions = [{"wght": (0, 1, 1)}, {"wdth": (0, 1, 1)}, {"wght": (-1, -1, 0)},
               {"wght": (0, 0.5, 1), "wdth": (0, 1, 1)}]
    progs = {
        ".notdef": [50, 0, "rmoveto", 300, 500, -300, "hlineto"],
        "box": [100, 10, -20, 5, 1, "blend", 0, "rmoveto", 400, 30, 0, 60, 1, "blend", "hlineto",
                700, 0, 40, -15, 1, "blend", "vlineto", -400, -30, 0, -60, 1, "blend", "hlineto"],
        "vs0": [0, "vsindex", 50, 0, 10, 20, 0, 0, 2, "blend", "rmoveto", 200, 15, 25, 1, "blend", 300, "rlineto"],
        "multi": [10, 20, 1, 2, 3, 4, 5, 6, 2, "blend", "rmoveto", 100, 0, 0, 0, 1, "blend", "hlineto"],
        "curve": [0, 0, "rmoveto", 100, 50, 100, 100, 0, 100, 4, 4, 4, 0, 0, 0, 8, 8, 8, -8, -8, -8,
                  0, 0, 0, 3, 3, 3, 6, "blend", "rrcurveto"],
    }
    fb.setupCFF2({n: T2CharString(program=progs[n]) for n in names}, fdArrayList=[{"vsindex": 1}])
    top = fb.font["CFF2"].cff.topDictIndex[0]
    store = buildVarStore(buildVarRegionList(regions, axisTags),
                          [buildVarData([0, 1], None, optimize=False), buildVarData([2, 3, 0], None, optimize=False)])
    vstore = VarStoreData(otVarStore=store)
    top.VarStore = vstore
    for fd in top.FDArray:
        fd.Private.vstore = vstore
    for n in names:
        top.CharStrings[n].private = top.FDArray[0].Private
    fb.setupHorizontalMetrics({n: (500, 0) for n in names})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupCharacterMap({0x41 + i: n for i, n in enumerate(names[1:])})
    fb.setupOS2()
    fb.setupPost()
    # HVAR, glyph by glyph: advance deltas over weight and width.
    hstore = buildVarStore(buildVarRegionList([regions[0], regions[1]], axisTags),
                           [buildVarData([0, 1], [[0, 0], [100, -50], [20, 0], [0, 33], [-7, 7]], optimize=False)])
    hvar = newTable("HVAR")
    hvar.table = ot.HVAR()
    hvar.table.Version = 0x00010000
    hvar.table.VarStore = hstore
    hvar.table.AdvWidthMap = None
    hvar.table.LsbMap = None
    hvar.table.RsbMap = None
    fb.font["HVAR"] = hvar
    fb.updateHead(created=3786825600, modified=3786825600)
    fb.save(os.path.join(OUT, "var.otf"))
    print("var.otf", os.path.getsize(os.path.join(OUT, "var.otf")), "bytes")


FONTS = os.path.join(OUT, "fonts")

BASIC = "".join(chr(c) for c in range(0x20, 0x7F))
GREEK = " " + "".join(chr(c) for c in range(0x3B1, 0x3CA))
CYRILLIC = " абвгд"
CJK_CHARS = "Aa一か가ㄅＡ\u02EA\uFF66\u1160"


def make_face(family, style, chars, weight=400, width=5, selection=0x40, typo=None, cff=False,
              mono=False, colr=False, axes=None, os2=True, mac_style=0, short_os2=False, nameless=False):
    # A face whose glyphs are squares, one per character, and whose names,
    # OS/2 and fvar say what font matching reads: everything else is the
    # least a valid font has. typo is (typographic family, typographic
    # style) when the face has them.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    from fontTools.pens.t2CharStringPen import T2CharStringPen
    cps = sorted(set(ord(c) for c in chars))
    names = [".notdef"] + ["u%04X" % c for c in cps]
    fb = FontBuilder(1000, isTTF=not cff)
    fb.setupGlyphOrder(names)
    fb.setupCharacterMap({c: "u%04X" % c for c in cps})
    if cff:
        glyphs = {}
        for n in names:
            pen = T2CharStringPen(500, None)
            pen.moveTo((100, 0)); pen.lineTo((100, 700)); pen.lineTo((400, 700)); pen.lineTo((400, 0)); pen.closePath()
            glyphs[n] = pen.getCharString()
        fb.setupCFF(family.replace(" ", "") + "-" + style.replace(" ", ""), {"FullName": family + " " + style},
                    glyphs, {})
    else:
        pen = TTGlyphPen(None)
        pen.moveTo((100, 0)); pen.lineTo((100, 700)); pen.lineTo((400, 700)); pen.lineTo((400, 0)); pen.closePath()
        sq = pen.glyph()
        fb.setupGlyf({n: sq for n in names})
    fb.setupHorizontalMetrics({n: (500, 100) for n in names})
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    strings = {"familyName": family, "styleName": style,
               "uniqueFontIdentifier": family + " " + style,
               "fullName": family + " " + style,
               "psName": (family + "-" + style).replace(" ", "")}
    if typo:
        strings["typographicFamily"] = typo[0]
        strings["typographicSubfamily"] = typo[1]
    fb.setupNameTable({} if nameless else strings, mac=False)
    if os2:
        fb.setupOS2(usWeightClass=weight, usWidthClass=width, fsSelection=selection, version=4,
                    sTypoAscender=800, sTypoDescender=-200, usWinAscent=800, usWinDescent=200)
    fb.setupPost(isFixedPitch=1 if mono else 0)
    if os2 and not short_os2:
        mac_style = (1 if selection & 0x20 else 0) | (2 if selection & 0x01 else 0)
    fb.updateHead(macStyle=mac_style, created=3786825600, modified=3786825600)
    if axes:
        fb.setupFvar(axes, [])
    if colr:
        fb.setupCPAL([[(1.0, 0.0, 0.0, 1.0), (0.0, 0.0, 1.0, 1.0)]])
        fb.setupCOLR({n: [(n, 0), (n, 1)] for n in names[1:]})
    if short_os2:
        # An OS/2 table cut to its first 20 bytes: too short to be read.
        from fontTools.ttLib.tables.DefaultTable import DefaultTable
        t = DefaultTable("OS/2")
        t.data = bytes(range(20))
        fb.font["OS/2"] = t
    return fb.font


def save_face(font, rel):
    path = os.path.join(FONTS, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    font.save(path)
    print("fonts/" + rel, os.path.getsize(path), "bytes")


def synthetic_fonts():
    # The fonts of a pretend system, for text/fonts: a static family in
    # weights from 100 to 900, two widths, italic and oblique, one face of it
    # a CFF one and some with their legacy family split from their
    # typographic one; a variable family over weight, width, slant and
    # optical size; one with an italic axis; Greek for fallback, once on its
    # own and once as the family's own companion; Cyrillic in two
    # companions, one condensed; a collection of two CJK faces, with the CJK
    # letters that are not wide; a colour face standing for emoji, with a
    # flag; a monospaced one; weights written the old way, no OS/2, an OS/2
    # too short, axes past CSS's ranges, no names; weights between 400 and
    # 500, off the first page. Then what is not a font. Made from nothing, so
    # under no licence.
    import shutil
    from fontTools.ttLib import TTCollection
    shutil.rmtree(FONTS, ignore_errors=True)
    sans = "Caustic Test Sans"
    with_eng = BASIC + "é"
    save_face(make_face(sans, "Thin", with_eng, weight=100), "sans/TestSans-Thin.ttf")
    save_face(make_face(sans + " Light", "Regular", with_eng, weight=300, cff=True, typo=(sans, "Light")),
              "sans/TestSans-Light.otf")
    save_face(make_face(sans, "Regular", with_eng), "sans/TestSans-Regular.ttf")
    save_face(make_face(sans, "Italic", with_eng, selection=0x01), "sans/TestSans-Italic.ttf")
    save_face(make_face(sans, "Bold", with_eng + "ŋд", weight=700, selection=0x20), "sans/TestSans-Bold.TTF")
    save_face(make_face(sans, "Bold Italic", with_eng, weight=700, selection=0x21), "sans/TestSans-BoldItalic.ttf")
    save_face(make_face(sans + " Black", "Regular", with_eng, weight=900, typo=(sans, "Black")),
              "sans/TestSans-Black.ttf")
    save_face(make_face(sans, "Oblique", with_eng, selection=0x200), "sans/TestSans-Oblique.ttf")
    save_face(make_face(sans + " Condensed", "Regular", with_eng, width=3, typo=(sans, "Condensed")),
              "sans/TestSans-Condensed.ttf")
    save_face(make_face(sans + " Expanded", "Regular", with_eng, width=7, typo=(sans, "Expanded")),
              "sans/TestSans-Expanded.ttf")
    # Variable: weight 200 to 800, width 75% to 100%, slant 0 to -12 degrees.
    save_face(make_face("Caustic Test Var", "Regular", BASIC + "ß",
                        axes=[("wght", 200, 400, 800, "Weight"), ("wdth", 75, 100, 100, "Width"),
                              ("slnt", -12, 0, 0, "Slant"), ("opsz", 8, 14, 144, "Optical size")]), "var/TestVar.ttf")
    save_face(make_face("Caustic Test Ital", "Regular", BASIC, axes=[("ital", 0, 0, 1, "Italic")]),
              "var/TestItal.ttf")
    save_face(make_face("Caustic Test Greek", "Regular", GREEK), "more/deeper/TestGreek.ttf")
    save_face(make_face("Caustic Test Sans Greek", "Regular", GREEK), "more/TestSansGreek-Regular.ttf")
    save_face(make_face("Caustic Test Sans Greek", "Bold", GREEK, weight=700, selection=0x20),
              "more/TestSansGreek-Bold.ttf")
    # Two companions with Cyrillic: one only condensed, first by name, and
    # one of normal width.
    save_face(make_face("Caustic Test Sans Cyr", "Condensed", CYRILLIC, width=3), "more/TestSansCyr.ttf")
    save_face(make_face("Caustic Test Sans Cyrillic", "Regular", CYRILLIC), "more/TestSansCyrillic.ttf")
    tc = TTCollection()
    tc.fonts = [make_face("Caustic Test CJK JP", "Regular", CJK_CHARS),
                make_face("Caustic Test CJK SC", "Regular", CJK_CHARS)]
    os.makedirs(os.path.join(FONTS, "cjk"), exist_ok=True)
    tc.save(os.path.join(FONTS, "cjk", "TestCJK.ttc"))
    print("fonts/cjk/TestCJK.ttc", os.path.getsize(os.path.join(FONTS, "cjk", "TestCJK.ttc")), "bytes")
    save_face(make_face("Caustic Test Emoji", "Regular", "#α☺\U0001F600\U0001F1E6", colr=True), "emoji/TestEmoji.ttf")
    save_face(make_face("Caustic Test Mono", "Regular", BASIC, mono=True), "mono/TestMono.ttf")
    save_face(make_face("Caustic Test Odd", "Bold", BASIC + "Ω", weight=7, width=0, selection=0x20), "odd/TestOdd.ttf")
    save_face(make_face("Caustic Test Plain", "Bold Italic", BASIC + "Ω", os2=False, mac_style=3), "odd/TestPlain.ttf")
    # Axes past CSS's ranges, an italic axis on a face italic already, and
    # 512 pages in a row beyond the first plane.
    save_face(make_face("Caustic Test Wild", "Italic", BASIC + "".join(chr(0x10041 + p * 256) for p in range(512)),
                        selection=0x01,
                        axes=[("wght", 0, 400, 1000.5, "Weight"), ("wdth", 25, 100, 1500, "Width"),
                              ("ital", 0, 1, 1, "Italic"), ("slnt", -20, 0, 0, "Slant")]), "odd/TestWild.ttf")
    # OS/2 too short to read, head saying bold and italic, and a slant axis.
    save_face(make_face("Caustic Test Short", "Bold Italic", BASIC, short_os2=True, mac_style=3,
                        axes=[("slnt", -15, 0, 0, "Slant")]), "odd/TestShort.ttf")
    # Weights between 400 and 500, where CSS looks heavier first, and no
    # character on the first page.
    for w in (300, 460, 480, 500):
        save_face(make_face("Caustic Test Weights", "W%d" % w, "ежз", weight=w), "weights/TestWeights-%d.ttf" % w)
    # No names at all.
    save_face(make_face("", "", BASIC, nameless=True), "odd/TestNameless.ttf")
    open(os.path.join(FONTS, "odd", "broken.ttf"), "wb").write(b"\x00\x01\x00\x00" + b"\xff" * 60)
    open(os.path.join(FONTS, "odd", "empty.otf"), "wb").write(b"")
    open(os.path.join(FONTS, "odd", "readme.txt"), "w").write("Not a font; ignored for its name.\n")


# What shape.ttf maps, and the marks among them by where they sit: above,
# below, through the base, or spacing (an advance of their own, which
# shaping takes away).
SHAPE_BASES = ("".join(chr(c) for c in range(0x20, 0x7F)) + "«»çéÄäñḉαᾳ‐⁄◌∈‹›❤" +
               "ހށނ" + "\U00010900\U00010901\U00010902" +
               "\U0001F600\U0001F468\U0001F469\U0001F3FB\U0001F1E7\U0001F1F7")
SHAPE_ABOVE = "ַָּّ̀́̂̃̈̊ަާ᩠༹"
SHAPE_BELOW = "ِ̧̨̣࿆ͅ"
SHAPE_THROUGH = "̴"
SHAPE_SPACING = "༹"
# Added after the rest, so that no other glyph moves: a composite whose mark,
# an overlay, is of combining class 1 — what composes with it alone; and
# marks of the classes that put them on an edge or touching (214, 216, 218,
# 222, 228, 232).
SHAPE_LATE_ABOVE = "\u1DCE\u031B\u302B\u0315"
SHAPE_LATE_BELOW = "\u1DFA\u302D"
SHAPE_LATE = "∉" + SHAPE_LATE_ABOVE + SHAPE_LATE_BELOW


def _left(g):
    """A glyph's left side bearing: its ink's left edge, so that the font
    draws it where its points are."""
    g.recalcBounds(None)
    return getattr(g, "xMin", 0)


def make_shape_font(gpos):
    # Glyphs are rectangles, each base its own advance so that positions
    # tell glyphs apart; marks sit to the left of their origin, as combining
    # marks are drawn, over, under or through where the base before them is.
    # The emoji heart has a second glyph, chosen by VS16 through cmap format
    # 14, and the plain one VS15 asks for by default. ∉ is there, its overlay
    # solidus not. With gpos, an empty GPOS table: positioning by the font,
    # which does nothing — no fallback; without, a legacy kern table, and
    # marks placed by fallback.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    from fontTools.ttLib import newTable
    from fontTools.ttLib.tables import otTables as ot

    def rect(x0, y0, x1, y1):
        pen = TTGlyphPen(None)
        pen.moveTo((x0, y0)); pen.lineTo((x0, y1)); pen.lineTo((x1, y1)); pen.lineTo((x1, y0)); pen.closePath()
        return pen.glyph()

    above = SHAPE_ABOVE + SHAPE_LATE_ABOVE
    below = SHAPE_BELOW + SHAPE_LATE_BELOW
    marks = above + below + SHAPE_THROUGH
    cps = sorted(set(ord(c) for c in SHAPE_BASES + SHAPE_ABOVE + SHAPE_BELOW + SHAPE_THROUGH))
    late = [ord(c) for c in SHAPE_LATE]
    names = [".notdef"] + ["u%04X" % c for c in cps] + ["u2764.emoji"] + ["u%04X" % c for c in late]
    glyphs, metrics = {}, {}
    glyphs[".notdef"] = rect(50, 0, 450, 700)
    metrics[".notdef"] = (500, 50)
    for i, c in enumerate(cps + late):
        n = "u%04X" % c
        ch = chr(c)
        if ch in marks and ch not in SHAPE_SPACING:
            adv = 0
        elif c in late:
            adv = 600
        else:
            adv = 200 + (i * 73) % 600
        if c == 0x20:
            adv, g = 260, None
        elif ch in above:
            g = rect(-300 + adv, 550, -100 + adv, 700)
        elif ch in below:
            g = rect(-300, -200, -100, -50)
        elif ch in SHAPE_THROUGH:
            g = rect(-350, 200, -50, 300)
        else:
            g = rect(40, 0, adv - 40, 500 if ch.islower() else 700)
        if g is None:
            pen = TTGlyphPen(None)
            g = pen.glyph()
        glyphs[n] = g
        metrics[n] = (adv, _left(g))
    glyphs["u2764.emoji"] = rect(30, -100, 970, 800)
    metrics["u2764.emoji"] = (1000, 30)
    fb = FontBuilder(1000, isTTF=True)
    fb.setupGlyphOrder(names)
    fb.setupCharacterMap({c: "u%04X" % c for c in cps + late},
                         uvs=[(0x2764, 0xFE0F, "u2764.emoji"), (0x2764, 0xFE0E, None)])
    fb.setupGlyf(glyphs)
    fb.setupHorizontalMetrics(metrics)
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    family = "Caustic Test Shape" + ("" if gpos else " Plain")
    fb.setupNameTable({"familyName": family, "styleName": "Regular", "uniqueFontIdentifier": family,
                       "fullName": family, "psName": family.replace(" ", "")}, mac=False)
    fb.setupOS2(usWeightClass=400, version=4, sTypoAscender=800, sTypoDescender=-200,
                usWinAscent=800, usWinDescent=200)
    fb.setupPost()
    fb.updateHead(created=3786825600, modified=3786825600)
    if not gpos:
        # Without GPOS, the legacy kern table kerns: version 0, one format 0
        # subtable.
        from fontTools.ttLib.tables._k_e_r_n import KernTable_format_0
        kern = newTable("kern")
        kern.version = 0
        sub = KernTable_format_0()
        sub.version, sub.coverage, sub.format = 0, 1, 0
        sub.kernTable = {("u0041", "u0056"): -80, ("u0056", "u0041"): -70, ("u0054", "u006F"): -55,
                         ("u0057", "u006F"): -40, ("u0061", "u0076"): -25, ("u0066", "u0069"): 15}
        kern.kernTables = [sub]
        fb.font["kern"] = kern
    if gpos:
        t = newTable("GPOS")
        t.table = ot.GPOS()
        t.table.Version = 0x00010000
        t.table.ScriptList = ot.ScriptList()
        t.table.ScriptList.ScriptRecord = []
        t.table.ScriptList.ScriptCount = 0
        t.table.FeatureList = ot.FeatureList()
        t.table.FeatureList.FeatureRecord = []
        t.table.FeatureList.FeatureCount = 0
        t.table.LookupList = ot.LookupList()
        t.table.LookupList.Lookup = []
        t.table.LookupList.LookupCount = 0
        fb.font["GPOS"] = t
    return fb.font


def synthetic_shape():
    # Made from nothing, so under no licence.
    for name, gpos in (("shape.ttf", True), ("shape-plain.ttf", False)):
        path = os.path.join(OUT, name)
        make_shape_font(gpos).save(path)
        print(name, os.path.getsize(path), "bytes")


# --- lookups.ttf: every lookup type, format and flag ---

LOOKUPS_FEA = """
languagesystem DFLT dflt;
languagesystem latn dflt;
languagesystem latn TRK;
languagesystem latn KOH;
languagesystem deva dflt;

markClass [acutecomb gravecomb tildecomb ringcomb] <anchor 0 600> @TOP;
markClass [dotbelowcomb cedillacomb] <anchor 0 0> @BOTTOM;
@ABOVE = [acutecomb gravecomb tildecomb ringcomb];

lookup SC1 { sub [a b c] by [a.sc b.sc c.sc]; } SC1;
lookup SC2 { sub [d e] by [e.sc d.sc]; } SC2;
feature smcp { lookup SC1; lookup SC2; } smcp;

feature salt { sub a from [a.alt1 a.alt2 a.alt3]; } salt;
feature rand { sub b from [b.alt1 b.alt2]; } rand;

feature ccmp { lookup MULT { sub w by v v; sub q by NULL; } MULT; } ccmp;

feature liga {
    lookup LIGA { lookupflag IgnoreMarks; sub f f i by f_f_i; sub f i by f_i; sub f l by f_l; } LIGA;
    lookup LIGA_TH useExtension { sub T h by T_h; } LIGA_TH;
} liga;

feature dlig { lookup SPLIT { sub f_i by f i; } SPLIT; } dlig;

feature calt {
    lookup CALT1 { sub x y' z by y.alt; } CALT1;
    lookup CALT2 { sub [m n] o' [p r] by o.alt; } CALT2;
} calt;

feature rclt { lookup REV { rsub [m n] s' [t] by s.alt; } REV; } rclt;

feature locl { script latn; language TRK required; sub i by i.trk; } locl;

# A language's second tag ("ko": KOR, then KOH) and a script's ("Deva": dev2,
# then deva), each the only one the font has, with a feature of its own.
feature ss06 {
    script latn; language KOH exclude_dflt; sub a by a.alt2;
    script deva; language dflt; sub a by a.alt3;
} ss06;

lookup PH1 { sub u by u.alt; } PH1;
feature ss01 { lookup PH1; } ss01;

feature kern {
    lookup KPAIR { pos a v -40; pos v a -35; pos A <0 0 -80 0> V <10 0 0 0>; pos V A -30; } KPAIR;
    lookup KCLASS { pos [T Y] [o e] -60; pos [L] [T Y] -70; } KCLASS;
    lookup KSINGLE { pos x <10 20 30 0>; pos [g j] <5 -10 0 0>; } KSINGLE;
    lookup KEXT useExtension { pos k k -25; } KEXT;
} kern;

feature mark {
    lookup MBASE { pos base [a b c e o x u g h v v.alt] <anchor 250 520> mark @TOP <anchor 250 -20> mark @BOTTOM; } MBASE;
    lookup MLIG {
        pos ligature f_i <anchor 150 720> mark @TOP <anchor 150 -20> mark @BOTTOM
            ligComponent <anchor 450 720> mark @TOP <anchor 450 -20> mark @BOTTOM;
        pos ligature f_f_i <anchor 120 720> mark @TOP ligComponent <anchor 300 720> mark @TOP
            ligComponent <anchor 480 720> mark @TOP;
        pos ligature F_G_H <anchor 100 720> mark @TOP ligComponent <anchor 300 740> mark @TOP
            ligComponent <anchor 500 760> mark @TOP;
        pos ligature f_i_l <anchor 110 720> mark @TOP ligComponent <anchor 310 740> mark @TOP
            ligComponent <anchor 510 760> mark @TOP;
    } MLIG;
} mark;

feature mkmk {
    lookup MKMK { lookupflag MarkAttachmentType @ABOVE;
        pos mark [acutecomb gravecomb tildecomb ringcomb] <anchor 0 700> mark @TOP; } MKMK;
    lookup MKMK2 { lookupflag UseMarkFilteringSet [dotbelowcomb];
        pos mark [dotbelowcomb] <anchor 0 -150> mark @BOTTOM; } MKMK2;
} mkmk;

feature curs {
    lookup CURS { lookupflag RightToLeft IgnoreMarks;
        pos cursive k <anchor 0 100> <anchor 300 150>;
        pos cursive l <anchor 0 150> <anchor 350 100>; } CURS;
} curs;

feature dist { lookup DIST1 { pos x y' 25 z; } DIST1; } dist;

lookup PP1 { pos u <0 0 15 0>; } PP1;
feature ss02 { lookup PP1; } ss02;

# What the other features leave untried, each on letters of its own: a glyph
# made a mark; a ligature made a mark; a ligature of a ligature the font
# calls a base, and of one it calls a ligature, marks between their parts;
# reverse chaining at the text's end; ligatures of marks on a ligature's
# components, the lookup passing over the ligature or not; a mark joined to
# a piece of a glyph a multiple substitution split; an advance of -32768;
# cursive attachment from both ends, the second turning the first round.
feature ss03 {
    lookup TOMARK { sub B by acutecomb; } TOMARK;
    lookup TORING { sub M N by ringcomb; } TORING;
    lookup LIG2 { sub F G by F_G; } LIG2;
    lookup LIG3 { lookupflag IgnoreMarks; sub F_G H by F_G_H; } LIG3;
    lookup LIG4 { lookupflag IgnoreMarks; sub f_i l by f_i_l; } LIG4;
    lookup REV2 { rsub [J] K' by K.alt; } REV2;
    lookup MLIGA { lookupflag IgnoreLigatures; sub acutecomb gravecomb by tildecomb; } MLIGA;
    lookup MLIGC { lookupflag IgnoreLigatures; sub n acutecomb by n.alt; } MLIGC;
    lookup PIECEMARK { sub v acutecomb by v.alt; } PIECEMARK;
    lookup FAR { pos O <0 0 -32768 0>; } FAR;
    lookup CURSA { pos cursive C <anchor NULL> <anchor 300 200>; pos cursive D <anchor 0 120> <anchor 350 250>; } CURSA;
    lookup CURSB { lookupflag RightToLeft;
        pos cursive D <anchor 0 50> <anchor 300 250>; pos cursive E <anchor 0 220> <anchor NULL>; } CURSB;
} ss03;

feature ss04 {
    lookup MLIGB { sub acutecomb gravecomb by tildecomb; } MLIGB;
    lookup MKMK3 { lookupflag IgnoreLigatures; pos mark [acutecomb] <anchor 0 650> mark @TOP; } MKMK3;
} ss04;

# Reached from a context alone, where reverse chaining is not applied.
lookup REVN { rsub o' by o.alt; } REVN;

# Lookups of several subtables, an earlier one passing over what a later one
# takes, each way a subtable can pass a glyph by; rule sets of several rules
# are added by hand. Pairs several to a first glyph, one of zero, pairs of
# classes with both glyphs valued. Cursive attachment chained 70 long, then
# turned round from its far end; and turned back over the same pair. A
# glyph the font calls a base put on another as a mark. A glyph split into
# two and three pieces, marks after them. Reverse chaining in subtables of
# a rule each.
markClass [acutecomb] <anchor 0 610> @A5;
markClass [gravecomb] <anchor 0 620> @G5;
markClass [tildecomb] <anchor 0 630> @T5;
markClass [y] <anchor 20 0> @Y5;
feature ss05 {
    lookup MULT5 { sub H by h i; sub G by h i i; } MULT5;
    lookup VVLIG { sub v v by f_i; } VVLIG;
    lookup REV5 {
        rsub [c] b' [d] by b.alt1;
        rsub b' [e] by b.alt2;
        rsub [a] b' by b.alt1;
        rsub x' [e] by x.alt;
    } REV5;
    lookup PSET {
        pos a <0 0 -11 0> c <0 1 0 0>; pos a <0 0 -12 0> d <0 2 0 0>; pos a <0 0 -13 0> e <0 3 0 0>;
        pos a <0 0 0 0> f <0 0 0 0>; pos a <0 0 -14 0> i <0 4 0 0>;
    } PSET;
    lookup PPLACE { pos o <0 0 0 0> c <7 0 0 0>; pos o <0 0 0 0> d <0 9 0 0>; } PPLACE;
    lookup PCLASS { pos [T Y] <0 0 -60 0> [o e] <0 7 0 0>; pos [L] <0 0 -70 0> [T Y] <3 0 0 0>; } PCLASS;
    lookup CURS4 { lookupflag 0; pos cursive D <anchor 0 120> <anchor 350 250>; } CURS4;
    lookup CURS5 { lookupflag RightToLeft;
        pos cursive D <anchor NULL> <anchor 300 260>; pos cursive E <anchor 0 210> <anchor NULL>; } CURS5;
    lookup MBASE2 { lookupflag 0;
        pos base [d] <anchor 100 500> mark @A5;
        pos base [k] <anchor 150 520> mark @G5;
        subtable;
        pos base [d z] <anchor 200 510> mark @A5 <anchor 210 520> mark @G5 <anchor 220 530> mark @T5;
    } MBASE2;
    lookup MLIG2 { lookupflag 0;
        pos ligature T_h <anchor 100 700> mark @A5 ligComponent <anchor 300 700> mark @A5;
        subtable;
        pos ligature T_h <anchor 110 710> mark @A5 <anchor 120 720> mark @G5
            ligComponent <anchor 310 710> mark @A5 <anchor 320 720> mark @G5;
        pos ligature f_l <anchor 130 730> mark @A5 <anchor 140 740> mark @G5
            ligComponent <anchor 330 730> mark @A5 <anchor 340 740> mark @G5;
    } MLIG2;
    lookup MKMK5 { lookupflag 0;
        pos mark [acutecomb] <anchor 0 800> mark @G5;
        subtable;
        pos mark [acutecomb ringcomb] <anchor 0 810> mark @G5 <anchor 0 820> mark @T5;
    } MKMK5;
    lookup YMARK { lookupflag 0; pos base [x] <anchor 100 300> mark @Y5; } YMARK;
} ss05;

# Fractions, each part of them its own glyph.
feature frac { sub [one two] by [one.fr two.fr]; } frac;
feature numr { sub [one two fraction] by [one.nu two.nu fraction.nu]; } numr;
feature dnom { sub [one two fraction] by [one.dn two.dn fraction.dn]; } dnom;

table GDEF {
    GlyphClassDef [a b c d e f g h i j k l m n o p q r s t u v w x y z A L T V Y a.sc b.sc c.sc d.sc e.sc
                   a.alt1 a.alt2 a.alt3 b.alt1 b.alt2 y.alt o.alt s.alt i.trk h.alt k.alt r.alt g.alt j.alt
                   v.alt x.alt u.alt t.alt z.alt B C D E F G H I J K M N O K.alt n.alt F_G
                   one two fraction one.fr two.fr one.nu two.nu fraction.nu one.dn two.dn fraction.dn],
                  [f_f_i f_i f_l T_h F_G_H f_i_l],
                  [acutecomb gravecomb tildecomb ringcomb dotbelowcomb cedillacomb], ;
} GDEF;
"""

LOOKUP_MARKS = {"acutecomb": 0x301, "gravecomb": 0x300, "tildecomb": 0x303, "ringcomb": 0x30A,
                "dotbelowcomb": 0x323, "cedillacomb": 0x327}
LOOKUP_EXTRA = ["a.sc", "b.sc", "c.sc", "d.sc", "e.sc", "a.alt1", "a.alt2", "a.alt3", "b.alt1", "b.alt2",
                "y.alt", "o.alt", "s.alt", "i.trk", "h.alt", "k.alt", "r.alt", "g.alt", "j.alt", "v.alt",
                "x.alt", "u.alt", "t.alt", "z.alt", "f_f_i", "f_i", "f_l", "T_h"]
# Added later, after the rest so that their glyph ids are new: letters for
# ss03's lookups alone, their alternates and ligatures, and fractions.
LOOKUP_LETTERS = "BCDEFGHIJKMNO"
LOOKUP_MORE = (list(LOOKUP_LETTERS) +
               ["K.alt", "n.alt", "F_G", "F_G_H", "f_i_l", "one", "two", "fraction", "one.fr", "two.fr",
                "one.nu", "two.nu", "fraction.nu", "one.dn", "two.dn", "fraction.dn"])
LOOKUP_MORE_CMAP = dict([(ord(c), c) for c in LOOKUP_LETTERS] + [(0x31, "one"), (0x32, "two"), (0x2044, "fraction")])


def _extra_lookups(font):
    """What the feature file cannot say: context substitution in each format,
    chained by classes, nested lookups that add and take away glyphs, lookup
    flags over ligatures and bases; context positioning in each format,
    chained by classes and by coverages. Substitutions join ss01's lookups,
    positionings ss02's."""
    from fontTools.ttLib.tables import otTables as ot
    from fontTools.otlLib import builder as ob
    gmap = font.getReverseGlyphMap()

    def cov(glyphs):
        return ob.buildCoverage(glyphs, gmap)

    def classes(m):
        cd = ot.ClassDef()
        cd.classDefs = dict(m)
        return cd

    def rec(kind, seq, index):
        r = getattr(ot, kind)()
        r.SequenceIndex, r.LookupListIndex = seq, index
        return r

    def add(table, kind, subtables, flag=0):
        lk = ot.Lookup()
        lk.LookupType, lk.LookupFlag = kind, flag
        lk.SubTable = subtables
        lk.SubTableCount = len(subtables)
        table.LookupList.Lookup.append(lk)
        table.LookupList.LookupCount = len(table.LookupList.Lookup)
        return table.LookupList.LookupCount - 1

    gsub, gpos = font["GSUB"].table, font["GPOS"].table
    single = lambda m: add(gsub, 1, [ob.buildSingleSubstSubtable(m)])
    l_h = single({"h": "h.alt", "k": "k.alt"})
    l_r = single({"r": "r.alt"})
    l_gj = single({"g": "g.alt", "j": "j.alt"})
    l_v = single({"v": "v.alt"})
    l_x = single({"x": "x.alt"})
    l_b = single({"b": "b.alt1"})
    l_acute = single({"acutecomb": "tildecomb"})
    # The lookups the feature file made, by what they do.
    names = {}
    for i, lk in enumerate(gsub.LookupList.Lookup):
        st = lk.SubTable[0]
        if lk.LookupType == 2 and "w" in getattr(st, "mapping", {}):
            names["mult"] = i
        if lk.LookupType == 4 and "f" in getattr(st, "ligatures", {}) and lk.LookupFlag == 8:
            names["liga"] = i

    # Context, format 1: g h, and j k r.
    f1 = ot.ContextSubst()
    f1.Format = 1
    f1.Coverage = cov(["g", "j"])
    sets = []
    for first, rest, records in (("g", ["h"], [(1, l_h)]), ("j", ["k", "r"], [(1, l_h), (2, l_r)])):
        rs = ot.SubRuleSet()
        rule = ot.SubRule()
        rule.Input = rest
        rule.GlyphCount = len(rest) + 1
        rule.SubstLookupRecord = [rec("SubstLookupRecord", q, k) for q, k in records]
        rule.SubstCount = len(records)
        rs.SubRule = [rule]
        rs.SubRuleCount = 1
        sets.append(rs)
    f1.SubRuleSet = sets
    f1.SubRuleSetCount = len(sets)
    # Format 2: a class of h and k followed by one of m and n.
    f2 = ot.ContextSubst()
    f2.Format = 2
    f2.Coverage = cov(["h", "k"])
    f2.ClassDef = classes({"h": 1, "k": 1, "m": 2, "n": 2})
    cr = ot.SubClassRule()
    cr.Class = [2]
    cr.GlyphCount = 2
    cr.SubstLookupRecord = [rec("SubstLookupRecord", 0, l_h)]
    cr.SubstCount = 1
    cs = ot.SubClassSet()
    cs.SubClassRule = [cr]
    cs.SubClassRuleCount = 1
    f2.SubClassSet = [None, cs, None]
    f2.SubClassSetCount = 3
    # Format 3: r, then t.
    f3 = ot.ContextSubst()
    f3.Format = 3
    f3.Coverage = [cov(["r"]), cov(["t"])]
    f3.GlyphCount = 2
    f3.SubstLookupRecord = [rec("SubstLookupRecord", 0, l_r)]
    f3.SubstCount = 1
    c5 = add(gsub, 5, [f1, f2, f3])
    # Nested lookups that change the length: p w x — w made v v, the second v
    # varied after it; and f i x — f i made one, the x after it varied.
    grow = ot.ContextSubst()
    grow.Format = 1
    grow.Coverage = cov(["f", "p"])
    sets = []
    for first, rest, records in (("f", ["i", "x"], [(0, names["liga"]), (1, l_x)]),
                                 ("p", ["w", "x"], [(1, names["mult"]), (2, l_v), (3, l_x)]),
                                 ):
        rs = ot.SubRuleSet()
        rule = ot.SubRule()
        rule.Input = rest
        rule.GlyphCount = len(rest) + 1
        rule.SubstLookupRecord = [rec("SubstLookupRecord", q, k) for q, k in records]
        rule.SubstCount = len(records)
        rs.SubRule = [rule]
        rs.SubRuleCount = 1
        sets.append(rs)
    grow.SubRuleSet = sets
    grow.SubRuleSetCount = len(sets)
    c5b = add(gsub, 5, [grow])
    # Chained, format 2: g or j after a, before o.
    ch = ot.ChainContextSubst()
    ch.Format = 2
    ch.Coverage = cov(["g", "j"])
    ch.BacktrackClassDef = classes({"a": 1})
    ch.InputClassDef = classes({"g": 1, "j": 1})
    ch.LookAheadClassDef = classes({"o": 1})
    rule = ot.ChainSubClassRule()
    rule.Backtrack, rule.BacktrackGlyphCount = [1], 1
    rule.Input, rule.InputGlyphCount = [], 1
    rule.LookAhead, rule.LookAheadGlyphCount = [1], 1
    rule.SubstLookupRecord = [rec("SubstLookupRecord", 0, l_gj)]
    rule.SubstCount = 1
    cset = ot.ChainSubClassSet()
    cset.ChainSubClassRule = [rule]
    cset.ChainSubClassRuleCount = 1
    ch.ChainSubClassSet = [None, cset]
    ch.ChainSubClassSetCount = 2
    c6 = add(gsub, 6, [ch])
    # Flags: a and b with ligatures between passed over; an acute and a grave
    # with a base between.
    over = ot.ContextSubst()
    over.Format = 3
    over.Coverage = [cov(["a"]), cov(["b"])]
    over.GlyphCount = 2
    over.SubstLookupRecord = [rec("SubstLookupRecord", 1, l_b)]
    over.SubstCount = 1
    c_lig = add(gsub, 5, [over], flag=4)
    marks = ot.ContextSubst()
    marks.Format = 3
    marks.Coverage = [cov(["acutecomb"]), cov(["gravecomb"])]
    marks.GlyphCount = 2
    marks.SubstLookupRecord = [rec("SubstLookupRecord", 0, l_acute)]
    marks.SubstCount = 1
    c_base = add(gsub, 5, [marks], flag=2)
    for fr in gsub.FeatureList.FeatureRecord:
        if fr.FeatureTag == "ss01":
            fr.Feature.LookupListIndex += [c5, c5b, c6, c_lig, c_base]
            fr.Feature.LookupCount = len(fr.Feature.LookupListIndex)

    # ss03's, by hand: a multiple substitution into one glyph; a context
    # whose records come out of order, the first growing the text before the
    # second goes back (b w: w made v v, then b varied); lookups nested as
    # deep as they may go, and deeper; chained contexts with two glyphs
    # behind and two ahead — by glyph, by class, by coverage.
    for i, lk in enumerate(gsub.LookupList.Lookup):
        st = lk.SubTable[0]
        if lk.LookupType == 1 and getattr(st, "mapping", {}).get("a") == "a.sc":
            names["sc1"] = i
    one_seq = add(gsub, 2, [ob.buildMultipleSubstSubtable({"I": ["J"]})])
    ooo = ot.ContextSubst()
    ooo.Format = 3
    ooo.Coverage = [cov(["b"]), cov(["w"])]
    ooo.GlyphCount = 2
    ooo.SubstLookupRecord = [rec("SubstLookupRecord", 1, names["mult"]), rec("SubstLookupRecord", 0, names["sc1"])]
    ooo.SubstCount = 2
    c_ooo = add(gsub, 5, [ooo])
    # Nested 64 deep, each level adding a y after n: as deep as nesting goes;
    # and 65 deep after u, one level too many.
    def nested(first, depth):
        grow = add(gsub, 2, [ob.buildMultipleSubstSubtable({first: [first, "y"]})])
        top = len(gsub.LookupList.Lookup)
        for k in range(depth):
            ctx = ot.ContextSubst()
            ctx.Format = 3
            ctx.Coverage = [cov([first])]
            ctx.GlyphCount = 1
            records = [rec("SubstLookupRecord", 0, grow)]
            if k + 1 < depth:
                records.append(rec("SubstLookupRecord", 0, top + k + 1))
            ctx.SubstLookupRecord, ctx.SubstCount = records, len(records)
            add(gsub, 5, [ctx])
        return top
    deep = nested("n", 64)
    too_deep = nested("u", 65)
    l_c = single({"c": "c.sc"})
    l_d = single({"d": "d.sc"})
    l_e = single({"e": "e.sc"})
    # The backtrack is stored nearest first: b, then a.
    g1 = ot.ChainContextSubst()
    g1.Format = 1
    g1.Coverage = cov(["c"])
    rule = ot.ChainSubRule()
    rule.Backtrack, rule.BacktrackGlyphCount = ["b", "a"], 2
    rule.Input, rule.InputGlyphCount = [], 1
    rule.LookAhead, rule.LookAheadGlyphCount = ["d", "e"], 2
    rule.SubstLookupRecord, rule.SubstCount = [rec("SubstLookupRecord", 0, l_c)], 1
    rs = ot.ChainSubRuleSet()
    rs.ChainSubRule, rs.ChainSubRuleCount = [rule], 1
    g1.ChainSubRuleSet, g1.ChainSubRuleSetCount = [rs], 1
    g2 = ot.ChainContextSubst()
    g2.Format = 2
    g2.Coverage = cov(["d"])
    g2.BacktrackClassDef = classes({"a": 1, "b": 2})
    g2.InputClassDef = classes({"d": 1})
    g2.LookAheadClassDef = classes({"e": 1, "f": 2})
    rule = ot.ChainSubClassRule()
    rule.Backtrack, rule.BacktrackGlyphCount = [2, 1], 2
    rule.Input, rule.InputGlyphCount = [], 1
    rule.LookAhead, rule.LookAheadGlyphCount = [1, 2], 2
    rule.SubstLookupRecord, rule.SubstCount = [rec("SubstLookupRecord", 0, l_d)], 1
    cset = ot.ChainSubClassSet()
    cset.ChainSubClassRule, cset.ChainSubClassRuleCount = [rule], 1
    g2.ChainSubClassSet, g2.ChainSubClassSetCount = [None, cset], 2
    g3 = ot.ChainContextSubst()
    g3.Format = 3
    g3.BacktrackCoverage, g3.BacktrackGlyphCount = [cov(["g"]), cov(["h"])], 2
    g3.InputCoverage, g3.InputGlyphCount = [cov(["e"])], 1
    g3.LookAheadCoverage, g3.LookAheadGlyphCount = [cov(["g"]), cov(["m"])], 2
    g3.SubstLookupRecord, g3.SubstCount = [rec("SubstLookupRecord", 0, l_e)], 1
    c_chains = add(gsub, 6, [g1, g2, g3])
    for fr in gsub.FeatureList.FeatureRecord:
        if fr.FeatureTag == "ss03":
            fr.Feature.LookupListIndex += [one_seq, c_ooo, deep, too_deep, c_chains]
            fr.Feature.LookupCount = len(fr.Feature.LookupListIndex)

    # ss05's, by hand: rule sets of two rules, the first applied when both
    # match and the second when it alone does — in a context by glyph (M N:
    # N varied; M alone: M varied) and by class (B F; B), and in a chain by
    # glyph (I J before K; I) and by class (p u; p) — the classes with a set
    # of no rules for C and m, which a later subtable takes; and a context
    # nesting reverse chaining. What each becomes is a glyph not its own: the
    # letters are ones no other lookup of ss05 looks at. The chain by glyph
    # covers K too, its lookahead, so that what comes right after an input
    # is always something the lookup goes on to vary.
    l_N, l_M = single({"N": "n.alt"}), single({"M": "K.alt"})
    l_F, l_B, l_C = single({"F": "h.alt"}), single({"B": "k.alt"}), single({"C": "r.alt"})
    l_J, l_I, l_K = single({"J": "j.alt"}), single({"I": "g.alt"}), single({"K": "k.alt"})
    l_u, l_p, l_m = single({"u": "u.alt"}), single({"p": "t.alt"}), single({"m": "z.alt"})

    def sub_rules(rules):
        rs = ot.SubRuleSet()
        rs.SubRule = []
        for rest, records in rules:
            rule = ot.SubRule()
            rule.Input, rule.GlyphCount = rest, len(rest) + 1
            rule.SubstLookupRecord = [rec("SubstLookupRecord", q, k) for q, k in records]
            rule.SubstCount = len(records)
            rs.SubRule.append(rule)
        rs.SubRuleCount = len(rs.SubRule)
        return rs

    two1 = ot.ContextSubst()
    two1.Format = 1
    two1.Coverage = cov(["M"])
    two1.SubRuleSet, two1.SubRuleSetCount = [sub_rules([(["N"], [(1, l_N)]), ([], [(0, l_M)])])], 1
    c_two1 = add(gsub, 5, [two1])
    two2 = ot.ContextSubst()
    two2.Format = 2
    two2.Coverage = cov(["B", "C"])
    two2.ClassDef = classes({"B": 1, "F": 2, "C": 3})
    cs = ot.SubClassSet()
    cs.SubClassRule = []
    for rest, records in (([2], [(1, l_F)]), ([], [(0, l_B)])):
        cr = ot.SubClassRule()
        cr.Class, cr.GlyphCount = rest, len(rest) + 1
        cr.SubstLookupRecord = [rec("SubstLookupRecord", q, k) for q, k in records]
        cr.SubstCount = len(records)
        cs.SubClassRule.append(cr)
    cs.SubClassRuleCount = len(cs.SubClassRule)
    two2.SubClassSet, two2.SubClassSetCount = [None, cs, None, None], 4
    c_alone = ot.ContextSubst()
    c_alone.Format = 3
    c_alone.Coverage, c_alone.GlyphCount = [cov(["C"])], 1
    c_alone.SubstLookupRecord, c_alone.SubstCount = [rec("SubstLookupRecord", 0, l_C)], 1
    c_two2 = add(gsub, 5, [two2, c_alone])
    two3 = ot.ChainContextSubst()
    two3.Format = 1
    two3.Coverage = cov(["I", "K"])

    def chain_rules(rules):
        rs = ot.ChainSubRuleSet()
        rs.ChainSubRule = []
        for rest, ahead, records in rules:
            rule = ot.ChainSubRule()
            rule.Backtrack, rule.BacktrackGlyphCount = [], 0
            rule.Input, rule.InputGlyphCount = rest, len(rest) + 1
            rule.LookAhead, rule.LookAheadGlyphCount = ahead, len(ahead)
            rule.SubstLookupRecord = [rec("SubstLookupRecord", q, k) for q, k in records]
            rule.SubstCount = len(records)
            rs.ChainSubRule.append(rule)
        rs.ChainSubRuleCount = len(rs.ChainSubRule)
        return rs
    two3.ChainSubRuleSet = [chain_rules([(["J"], ["K"], [(1, l_J)]), ([], [], [(0, l_I)])]),
                            chain_rules([([], [], [(0, l_K)])])]
    two3.ChainSubRuleSetCount = 2
    c_two3 = add(gsub, 6, [two3])
    two4 = ot.ChainContextSubst()
    two4.Format = 2
    two4.Coverage = cov(["p", "m"])
    two4.BacktrackClassDef = classes({})
    two4.InputClassDef = classes({"p": 1, "m": 2, "u": 3})
    two4.LookAheadClassDef = classes({})
    cset = ot.ChainSubClassSet()
    cset.ChainSubClassRule = []
    for rest, records in (([3], [(1, l_u)]), ([], [(0, l_p)])):
        rule = ot.ChainSubClassRule()
        rule.Backtrack, rule.BacktrackGlyphCount = [], 0
        rule.Input, rule.InputGlyphCount = rest, len(rest) + 1
        rule.LookAhead, rule.LookAheadGlyphCount = [], 0
        rule.SubstLookupRecord = [rec("SubstLookupRecord", q, k) for q, k in records]
        rule.SubstCount = len(records)
        cset.ChainSubClassRule.append(rule)
    cset.ChainSubClassRuleCount = len(cset.ChainSubClassRule)
    two4.ChainSubClassSet, two4.ChainSubClassSetCount = [None, cset, None], 3
    m_alone = ot.ChainContextSubst()
    m_alone.Format = 3
    m_alone.BacktrackCoverage, m_alone.BacktrackGlyphCount = [], 0
    m_alone.InputCoverage, m_alone.InputGlyphCount = [cov(["m"])], 1
    m_alone.LookAheadCoverage, m_alone.LookAheadGlyphCount = [], 0
    m_alone.SubstLookupRecord, m_alone.SubstCount = [rec("SubstLookupRecord", 0, l_m)], 1
    c_two4 = add(gsub, 6, [two4, m_alone])
    # Contexts that pass over marks, so that their input is not side by side,
    # nesting a lookup that changes the length before the records after it:
    # 1 2 1 with 1 made 1 2 2, then the second item (now the first 2 made)
    # and the fourth (the 2 that was there); 2 1 2 with the first 2 taken
    # out, then the second item (now the last 2).
    l_grow = add(gsub, 2, [ob.buildMultipleSubstSubtable({"one": ["one", "two", "two"]})])
    l_gone = add(gsub, 2, [ob.buildMultipleSubstSubtable({"two": []})])
    l_nu = single({"two": "two.nu"})
    l_dn = single({"one": "one.dn", "two": "two.dn"})
    grows = ot.ContextSubst()
    grows.Format = 3
    grows.Coverage, grows.GlyphCount = [cov(["one"]), cov(["two"]), cov(["one"])], 3
    grows.SubstLookupRecord = [rec("SubstLookupRecord", 0, l_grow), rec("SubstLookupRecord", 1, l_nu),
                               rec("SubstLookupRecord", 3, l_dn)]
    grows.SubstCount = 3
    c_grows = add(gsub, 5, [grows], flag=8)
    shrinks = ot.ContextSubst()
    shrinks.Format = 3
    shrinks.Coverage, shrinks.GlyphCount = [cov(["two"]), cov(["one"]), cov(["two"])], 3
    shrinks.SubstLookupRecord = [rec("SubstLookupRecord", 0, l_gone), rec("SubstLookupRecord", 1, l_dn)]
    shrinks.SubstCount = 2
    c_shrinks = add(gsub, 5, [shrinks], flag=8)
    for i, lk in enumerate(gsub.LookupList.Lookup):
        st = lk.SubTable[0]
        if lk.LookupType == 8 and getattr(st, "Substitute", None) == ["o.alt"]:
            names["revn"] = i
    nest_rev = ot.ContextSubst()
    nest_rev.Format = 3
    nest_rev.Coverage, nest_rev.GlyphCount = [cov(["o"])], 1
    nest_rev.SubstLookupRecord, nest_rev.SubstCount = [rec("SubstLookupRecord", 0, names["revn"])], 1
    c_nest_rev = add(gsub, 5, [nest_rev])
    for fr in gsub.FeatureList.FeatureRecord:
        if fr.FeatureTag == "ss05":
            fr.Feature.LookupListIndex += [c_two1, c_two2, c_two3, c_two4, c_nest_rev, c_grows, c_shrinks]
            fr.Feature.LookupCount = len(fr.Feature.LookupListIndex)

    # Positioning.
    def single_pos(glyphs, value):
        return add(gpos, 1, [ob.buildSinglePosSubtable({g: value for g in glyphs}, gmap)])
    vr = lambda **kw: ob.buildValue(kw)
    p_x = single_pos(["x"], vr(XAdvance=50))
    p_y = single_pos(["y"], vr(YPlacement=-30))
    p_z = single_pos(["z"], vr(XPlacement=12))
    p_t = single_pos(["t"], vr(XAdvance=-20, YPlacement=8))
    # A value for each glyph: format 2.
    p_2 = add(gpos, 1, [ob.buildSinglePosSubtable({"m": vr(XAdvance=11), "n": vr(YPlacement=5)}, gmap)])
    q1 = ot.ContextPos()
    q1.Format = 1
    q1.Coverage = cov(["x"])
    rs = ot.PosRuleSet()
    rule = ot.PosRule()
    rule.Input, rule.GlyphCount = ["y"], 2
    rule.PosLookupRecord = [rec("PosLookupRecord", 0, p_x), rec("PosLookupRecord", 1, p_y)]
    rule.PosCount = 2
    rs.PosRule, rs.PosRuleCount = [rule], 1
    q1.PosRuleSet, q1.PosRuleSetCount = [rs], 1
    q2 = ot.ContextPos()
    q2.Format = 2
    q2.Coverage = cov(["x"])
    q2.ClassDef = classes({"x": 1, "z": 2})
    cr = ot.PosClassRule()
    cr.Class, cr.GlyphCount = [2], 2
    cr.PosLookupRecord = [rec("PosLookupRecord", 1, p_z)]
    cr.PosCount = 1
    cs = ot.PosClassSet()
    cs.PosClassRule, cs.PosClassRuleCount = [cr], 1
    q2.PosClassSet, q2.PosClassSetCount = [None, cs, None], 3
    q3 = ot.ContextPos()
    q3.Format = 3
    q3.Coverage = [cov(["t"]), cov(["u"])]
    q3.GlyphCount = 2
    q3.PosLookupRecord = [rec("PosLookupRecord", 0, p_t)]
    q3.PosCount = 1
    c7 = add(gpos, 7, [q1, q2, q3])
    k2 = ot.ChainContextPos()
    k2.Format = 2
    k2.Coverage = cov(["z"])
    k2.BacktrackClassDef = classes({"y": 1})
    k2.InputClassDef = classes({"z": 1})
    k2.LookAheadClassDef = classes({"t": 1})
    rule = ot.ChainPosClassRule()
    rule.Backtrack, rule.BacktrackGlyphCount = [1], 1
    rule.Input, rule.InputGlyphCount = [], 1
    rule.LookAhead, rule.LookAheadGlyphCount = [1], 1
    rule.PosLookupRecord = [rec("PosLookupRecord", 0, p_z)]
    rule.PosCount = 1
    cset = ot.ChainPosClassSet()
    cset.ChainPosClassRule, cset.ChainPosClassRuleCount = [rule], 1
    k2.ChainPosClassSet, k2.ChainPosClassSetCount = [None, cset], 2
    k3 = ot.ChainContextPos()
    k3.Format = 3
    k3.BacktrackCoverage, k3.BacktrackGlyphCount = [cov(["s"])], 1
    k3.InputCoverage, k3.InputGlyphCount = [cov(["t"]), cov(["t"])], 2
    k3.LookAheadCoverage, k3.LookAheadGlyphCount = [cov(["s"])], 1
    k3.PosLookupRecord = [rec("PosLookupRecord", 1, p_t)]
    k3.PosCount = 1
    c8 = add(gpos, 8, [k2, k3])
    for fr in gpos.FeatureList.FeatureRecord:
        if fr.FeatureTag == "ss02":
            fr.Feature.LookupListIndex += [p_2, c7, c8]
            fr.Feature.LookupCount = len(fr.Feature.LookupListIndex)

    # ss05's cursive attachment in two subtables, which the feature file
    # cannot keep apart: the first has g with no exit, j with no entry and r
    # where the second has them otherwise, and lacks h, s and u; then the
    # pair h u turned back by a lookup right to left.
    def anchors(entry, exit):
        return (entry and ob.buildAnchor(*entry), exit and ob.buildAnchor(*exit))
    curs_a = ob.buildCursivePosSubtable({"g": anchors((5, 5), None), "j": anchors(None, (999, 999)),
                                         "r": anchors((0, 50), None)}, gmap)
    curs_b = ob.buildCursivePosSubtable({"g": anchors(None, (300, 100)), "j": anchors((0, 100), (320, 80)),
                                         "r": anchors((0, 60), None), "h": anchors(None, (280, 90)),
                                         "s": anchors((0, 40), None), "u": anchors((0, 70), None)}, gmap)
    curs2 = add(gpos, 3, [curs_a, curs_b])
    curs3 = add(gpos, 3, [ob.buildCursivePosSubtable({"h": anchors(None, (310, 70)), "u": anchors((0, 90), None)},
                                                     gmap)], flag=1)
    for fr in gpos.FeatureList.FeatureRecord:
        if fr.FeatureTag == "ss05":
            fr.Feature.LookupListIndex += [curs2, curs3]
            fr.Feature.LookupCount = len(fr.Feature.LookupListIndex)


def make_lookups_font():
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    from fontTools.feaLib.builder import addOpenTypeFeaturesFromString

    def rect(x0, y0, x1, y1):
        pen = TTGlyphPen(None)
        pen.moveTo((x0, y0)); pen.lineTo((x0, y1)); pen.lineTo((x1, y1)); pen.lineTo((x1, y0)); pen.closePath()
        return pen.glyph()

    letters = [chr(c) for c in range(ord("a"), ord("z") + 1)] + list("ALTVY")
    names = [".notdef", "space"] + letters + list(LOOKUP_MARKS) + LOOKUP_EXTRA + LOOKUP_MORE
    glyphs, metrics = {}, {}
    for i, n in enumerate(names):
        if n == ".notdef":
            g, adv = rect(50, 0, 450, 700), 500
        elif n == "space":
            g, adv = TTGlyphPen(None).glyph(), 260
        elif n in LOOKUP_MARKS:
            below = LOOKUP_MARKS[n] in (0x323, 0x327)
            g, adv = (rect(-250, -200, -50, -50) if below else rect(-250, 600, -50, 750)), 0
        else:
            adv = 300 + (i * 37) % 400
            g = rect(30, 0, adv - 30, 500 if n[0].islower() else 700)
        glyphs[n] = g
        metrics[n] = (adv, _left(g))
    fb = FontBuilder(1000, isTTF=True)
    fb.setupGlyphOrder(names)
    cmap = {ord(c): c for c in letters}
    cmap.update(LOOKUP_MORE_CMAP)
    cmap[32] = "space"
    for n, c in LOOKUP_MARKS.items():
        cmap[c] = n
    fb.setupCharacterMap(cmap)
    fb.setupGlyf(glyphs)
    fb.setupHorizontalMetrics(metrics)
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    family = "Caustic Test Lookups"
    fb.setupNameTable({"familyName": family, "styleName": "Regular", "uniqueFontIdentifier": family,
                       "fullName": family, "psName": family.replace(" ", "")}, mac=False)
    fb.setupOS2(usWeightClass=400, version=4, sTypoAscender=800, sTypoDescender=-200,
                usWinAscent=800, usWinDescent=200)
    fb.setupPost()
    fb.updateHead(created=3786825600, modified=3786825600)
    addOpenTypeFeaturesFromString(fb.font, LOOKUPS_FEA)
    _extra_lookups(fb.font)
    return fb.font


def synthetic_lookups():
    # Made from nothing, so under no licence.
    path = os.path.join(OUT, "lookups.ttf")
    make_lookups_font().save(path)
    print("lookups.ttf", os.path.getsize(path), "bytes")


# --- varlookups.ttf: lookups that depend on the instance, and on no GDEF ---

VARLOOKUPS_FEA = """
languagesystem DFLT dflt;
languagesystem latn dflt;
languagesystem grek dflt;

# Ligatures the font has no class for: one of a base and a mark, which
# stays a base; one that starts with a mark, which is a mark no longer. First
# of all, before the mark is varied and the contexts pass over ligatures.
lookup NLIG { sub n acutecomb by f_i; } NLIG;
lookup MLIG { sub acutecomb x by y.alt; } MLIG;
lookup RV_A { sub a by a.alt; } RV_A;
lookup RV_C { sub c by c.alt; } RV_C;
lookup CALT_B { sub b by b.alt; } CALT_B;
lookup MAIN_E { sub e.alt by x; } MAIN_E;
lookup RV_D { sub d by d.alt; } RV_D;
lookup DUP { sub d by d d; } DUP;
lookup REQ { sub e by e.alt; } REQ;
lookup ACUTE { sub acutecomb by gravecomb; } ACUTE;
lookup LIGA { sub f i by f_i; } LIGA;
lookup SPLIT { sub f_i by f i; } SPLIT;
lookup CTX_X { lookupflag IgnoreLigatures; sub n x' by x.alt; } CTX_X;
lookup CTX_Y { lookupflag IgnoreLigatures; sub n y' by y.alt; } CTX_Y;

feature rvrn { script DFLT; language dflt; lookup RV_D; script latn; language dflt; lookup DUP; } rvrn;
feature calt { lookup MAIN_E; } calt;
feature liga { lookup LIGA; } liga;
feature dlig { lookup SPLIT; } dlig;
feature ss01 { lookup ACUTE; lookup CTX_X; lookup CTX_Y; } ss01;
feature ss02 { lookup NLIG; lookup MLIG; } ss02;

markClass [acutecomb gravecomb] <anchor 0 600> @TOP;
feature mark {
    pos base [a b c d e n x y] <anchor (wght=100:200 wght=400:250 wght=900:300) 500> mark @TOP;
} mark;
feature kern {
    pos x <(wght=100:-20 wght=400:0 wght=900:40) (wght=100:10 wght=400:0 wght=900:-30)
           (wght=100:-15 wght=400:0 wght=900:25) 3>;
    pos y <0 0 (wght=100:-30 wght=400:0 wght=900:60) 0>;
    pos a b <0 0 (wght=100:-30 wght=400:0 wght=900:40) 0>;
    pos a c <0 (wght=100:5 wght=400:0 wght=900:-8) 0 0>;
    pos a d <(wght=100:6 wght=400:0 wght=900:-9) 0 0 0>;
} kern;
"""


def _sort_features(table):
    """The feature list in order of tag, every index into it moved with it."""
    recs = table.FeatureList.FeatureRecord
    order = sorted(range(len(recs)), key=lambda i: recs[i].FeatureTag)
    new = {old: k for k, old in enumerate(order)}
    table.FeatureList.FeatureRecord = [recs[i] for i in order]
    for sr in table.ScriptList.ScriptRecord:
        for ls in [sr.Script.DefaultLangSys] + [r.LangSys for r in sr.Script.LangSysRecord]:
            if ls is None:
                continue
            ls.FeatureIndex = sorted(new[i] for i in ls.FeatureIndex)
            if ls.ReqFeatureIndex != 0xFFFF:
                ls.ReqFeatureIndex = new[ls.ReqFeatureIndex]
    fv = getattr(table, "FeatureVariations", None)
    if fv is not None:
        for r in fv.FeatureVariationRecord:
            subs = r.FeatureTableSubstitution.SubstitutionRecord
            for sub in subs:
                sub.FeatureIndex = new[sub.FeatureIndex]
            subs.sort(key=lambda sub: sub.FeatureIndex)


def make_varlookups_font():
    """A variable font, weight and width, whose lookups change with the
    instance: FeatureVariations that swap rvrn's lookups — at a weight from
    exactly half way up, or at a light weight and narrow width together — and
    with them calt's, the second substitution of the set; placements and an
    advance moved by device tables, and an anchor. Its DFLT language system
    has a required feature tagged rvrn, which goes in rvrn's stage, before a
    calt lookup listed earlier; Greek's has it too, without rvrn itself;
    Latin's rvrn lists one lookup twice. GDEF has no glyph classes: marks are
    known by their characters, and what substitutions make by what they do."""
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    from fontTools.feaLib.builder import addOpenTypeFeaturesFromString
    from fontTools.ttLib.tables import otTables as ot

    def rect(x0, y0, x1, y1):
        pen = TTGlyphPen(None)
        pen.moveTo((x0, y0)); pen.lineTo((x0, y1)); pen.lineTo((x1, y1)); pen.lineTo((x1, y0)); pen.closePath()
        return pen.glyph()

    letters = list("abcdefinxy")
    marks = {"acutecomb": 0x301, "gravecomb": 0x300}
    names = ([".notdef", "space"] + letters + list(marks) +
             ["a.alt", "b.alt", "c.alt", "d.alt", "e.alt", "x.alt", "y.alt", "f_i"])
    glyphs, metrics = {}, {}
    for i, n in enumerate(names):
        if n == ".notdef":
            g, adv = rect(50, 0, 450, 700), 500
        elif n == "space":
            g, adv = TTGlyphPen(None).glyph(), 260
        elif n in marks:
            # An advance of their own, for shaping to take away.
            g, adv = rect(-250, 600, -50, 750), 200
        else:
            adv = 300 + (i * 41) % 400
            g = rect(30, 0, adv - 30, 500)
        glyphs[n] = g
        metrics[n] = (adv, _left(g))
    fb = FontBuilder(1000, isTTF=True)
    fb.setupGlyphOrder(names)
    cmap = {ord(c): c for c in letters}
    cmap[32] = "space"
    for n, c in marks.items():
        cmap[c] = n
    fb.setupCharacterMap(cmap)
    fb.setupGlyf(glyphs)
    fb.setupHorizontalMetrics(metrics)
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    family = "Caustic Test Var Lookups"
    fb.setupNameTable({"familyName": family, "styleName": "Regular", "uniqueFontIdentifier": family,
                       "fullName": family, "psName": family.replace(" ", "")}, mac=False)
    fb.setupOS2(usWeightClass=400, version=4, sTypoAscender=800, sTypoDescender=-200,
                usWinAscent=800, usWinDescent=200)
    fb.setupPost()
    fb.setupFvar([("wght", 100, 400, 900, "Weight"), ("wdth", 75, 100, 125, "Width")], [])
    fb.updateHead(created=3786825600, modified=3786825600)
    addOpenTypeFeaturesFromString(fb.font, VARLOOKUPS_FEA)

    gsub = fb.font["GSUB"].table
    index = {}
    for i, lk in enumerate(gsub.LookupList.Lookup):
        st = lk.SubTable[0]
        m = getattr(st, "mapping", {})
        for name, src, dst in (("RV_A", "a", "a.alt"), ("RV_C", "c", "c.alt"), ("CALT_B", "b", "b.alt"),
                               ("MAIN_E", "e.alt", "x"), ("RV_D", "d", "d.alt"), ("REQ", "e", "e.alt")):
            if m.get(src) == dst:
                index[name] = i
            if lk.LookupType == 2 and tuple(m.get("d", ())) == ("d", "d"):
                index["DUP"] = i

    def feature_of(script, tag):
        for sr in gsub.ScriptList.ScriptRecord:
            if sr.ScriptTag == script:
                for k in sr.Script.DefaultLangSys.FeatureIndex:
                    if gsub.FeatureList.FeatureRecord[k].FeatureTag == tag:
                        return k
        raise KeyError((script, tag))

    # Latin's rvrn: one lookup, twice.
    latn = gsub.FeatureList.FeatureRecord[feature_of("latn", "rvrn")].Feature
    latn.LookupListIndex = [index["DUP"], index["DUP"]]
    latn.LookupCount = 2
    # The required feature, a second rvrn: DFLT's and Greek's.
    req = ot.FeatureRecord()
    req.FeatureTag = "rvrn"
    req.Feature = ot.Feature()
    req.Feature.FeatureParams = None
    req.Feature.LookupListIndex = [index["REQ"]]
    req.Feature.LookupCount = 1
    gsub.FeatureList.FeatureRecord.append(req)
    gsub.FeatureList.FeatureCount = len(gsub.FeatureList.FeatureRecord)
    for sr in gsub.ScriptList.ScriptRecord:
        if sr.ScriptTag in ("DFLT", "grek"):
            sr.Script.DefaultLangSys.ReqFeatureIndex = len(gsub.FeatureList.FeatureRecord) - 1

    # The variations, made here to say exactly what is swapped.
    def condition(axis, lo, hi):
        c = ot.ConditionTable()
        c.Format = 1
        c.AxisIndex, c.FilterRangeMinValue, c.FilterRangeMaxValue = axis, lo, hi
        return c

    def swap(feature, lookups):
        s = ot.FeatureTableSubstitutionRecord()
        s.FeatureIndex = feature
        s.Feature = ot.Feature()
        s.Feature.FeatureParams = None
        s.Feature.LookupListIndex = lookups
        s.Feature.LookupCount = len(lookups)
        return s

    def record(conditions, swaps):
        r = ot.FeatureVariationRecord()
        r.ConditionSet = ot.ConditionSet()
        r.ConditionSet.ConditionTable = conditions
        r.ConditionSet.ConditionCount = len(conditions)
        r.FeatureTableSubstitution = ot.FeatureTableSubstitution()
        r.FeatureTableSubstitution.Version = 0x00010000
        swaps.sort(key=lambda s: s.FeatureIndex)
        r.FeatureTableSubstitution.SubstitutionRecord = swaps
        r.FeatureTableSubstitution.SubstitutionCount = len(swaps)
        return r

    rvrn, calt = feature_of("DFLT", "rvrn"), feature_of("DFLT", "calt")
    fv = ot.FeatureVariations()
    fv.Version = 0x00010000
    fv.FeatureVariationRecord = [
        record([condition(0, 0.5, 1.0)],
               [swap(rvrn, [index["RV_A"]]), swap(calt, [index["MAIN_E"], index["CALT_B"]])]),
        record([condition(0, -1.0, -0.5), condition(1, -1.0, -0.4)], [swap(rvrn, [index["RV_C"]])]),
    ]
    fv.FeatureVariationCount = len(fv.FeatureVariationRecord)
    gsub.FeatureVariations = fv
    gsub.Version = 0x00010001
    _sort_features(gsub)

    gdef = fb.font["GDEF"].table
    gdef.GlyphClassDef = None
    gdef.MarkAttachClassDef = None
    return fb.font


def synthetic_varlookups():
    # Made from nothing, so under no licence.
    path = os.path.join(OUT, "varlookups.ttf")
    make_varlookups_font().save(path)
    print("varlookups.ttf", os.path.getsize(path), "bytes")


def main():
    os.makedirs(os.path.join(OUT, "licenses"), exist_ok=True)
    synthetic_shape()
    synthetic_lookups()
    synthetic_varlookups()
    synthetic_fonts()
    synthetic_big()
    synthetic_var()
    synthetic_var_cff2()
    synthetic_cmaps()
    synthetic_composites()
    synthetic_ops()
    synthetic_bombs()
    for out, src, _, text, family in SOURCES:
        make_subset(out, src, text, family)

    out, src, faces, text, fams = COLLECTION
    coll = TTCollection(src)
    picked = []
    for want, family in zip(faces, fams):
        for f in coll.fonts:
            if f["name"].getDebugName(4) == want:
                f.recalcTimestamp = False
                buf = io.BytesIO()
                f.save(buf)
                buf.seek(0)
                one = TTFont(buf, recalcTimestamp=False)
                old = families(one)
                subset_font(one, text)
                rename(one, old, family)
                b2 = io.BytesIO()
                one.save(b2)
                b2.seek(0)
                picked.append(TTFont(b2, recalcTimestamp=False))
                break
    if len(picked) != len(faces):
        sys.exit("collection: faces not found")
    tc = TTCollection()
    for p in picked:
        p.recalcTimestamp = False
    tc.fonts = picked
    tc.save(os.path.join(OUT, out), shareTables=True)
    licence(coll.fonts[0], out)
    print(out, os.path.getsize(os.path.join(OUT, out)), "bytes")


if __name__ == "__main__":
    main()
