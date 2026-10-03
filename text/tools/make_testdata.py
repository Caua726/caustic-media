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


def main():
    os.makedirs(os.path.join(OUT, "licenses"), exist_ok=True)
    synthetic_big()
    synthetic_var()
    synthetic_var_cff2()
    synthetic_cmaps()
    synthetic_composites()
    synthetic_ops()
    synthetic_bombs()
    for out, src, _, text, family in SOURCES:
        font = TTFont(src, recalcTimestamp=False)
        licence_font = TTFont(src, lazy=True)
        old = families(font)
        subset_font(font, text)
        rename(font, old, family)
        font.save(os.path.join(OUT, out))
        licence(licence_font, out)
        print(out, os.path.getsize(os.path.join(OUT, out)), "bytes")

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
