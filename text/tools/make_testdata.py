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


def main():
    os.makedirs(os.path.join(OUT, "licenses"), exist_ok=True)
    synthetic_cmaps()
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
