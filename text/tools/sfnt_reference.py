#!/usr/bin/env python3
# text/tools/sfnt_reference.py — what fontTools reads from each test font,
# written as text for sfnt_test.cst to hold the parser to.
#
# Development tool: needs fontTools; its output is versioned. One file per
# font, <font>.sfnt.ref, a record per line:
#
#   faces N                 how many faces the file holds
#   face I                  the records after it are face I's
#   tables N                how many tables its directory lists
#   outline glyf|cff|cff2
#   upem U
#   glyphs N
#   head xmin ymin xmax ymax mac_style loc_format
#   hhea ascender descender line_gap number_of_hmetrics
#   os2 version weight width fs_selection typo_asc typo_desc typo_gap
#       win_asc win_desc x_height cap_height strike_size strike_pos
#   post underline_pos underline_thickness italic_angle_16_16 fixed_pitch
#   metrics ascender descender line_gap x_height cap_height underline_pos
#       underline_thickness strike_pos strike_size weight width italic bold
#       fixed_pitch                     the convention text/ uses
#   name ID HEX             the UTF-8 of the record chosen for name ID
#   subtable PLAT ENC FORMAT
#   map CP GID              that subtable's mappings
#   uvs CP VS GID           a format 14 mapping; GID -1 for the default glyph
#   cmap CP GID             the face's best Unicode mapping, every entry
#   hmtx GID ADVANCE LSB    every glyph
#   kern LEFT RIGHT VALUE   the legacy kern table's pairs
#
#   python3 text/tools/sfnt_reference.py
import glob
import os

from fontTools.ttLib import TTFont, TTCollection

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")

NAME_IDS = (1, 2, 4, 6, 16, 17)


def pick_name(font, name_id):
    # Windows Unicode in US English, then any English, then any Windows
    # record, then Unicode platform, then Mac Roman: the order text/'s name
    # reader follows.
    recs = [r for r in font["name"].names if r.nameID == name_id]

    def first(pred):
        for r in recs:
            if pred(r):
                return r
        return None

    r = (first(lambda r: r.platformID == 3 and r.platEncID in (1, 10) and r.langID == 0x409)
         or first(lambda r: r.platformID == 3 and r.platEncID in (1, 10) and (r.langID & 0x3FF) == 0x09)
         or first(lambda r: r.platformID == 3 and r.platEncID in (1, 10))
         or first(lambda r: r.platformID == 0)
         or first(lambda r: r.platformID == 1 and r.platEncID == 0 and r.langID == 0))
    if r is None:
        return None
    return r.toUnicode()


def metrics(font):
    hhea = font["hhea"]
    os2 = font["OS/2"] if "OS/2" in font else None
    if os2 is not None and (os2.fsSelection & (1 << 7)):
        a, d, g = os2.sTypoAscender, os2.sTypoDescender, os2.sTypoLineGap
    else:
        a, d, g = hhea.ascent, hhea.descent, hhea.lineGap
    if a == 0 and d == 0 and os2 is not None:
        a, d, g = os2.sTypoAscender, os2.sTypoDescender, os2.sTypoLineGap
        if a == 0 and d == 0:
            a, d, g = os2.usWinAscent, -os2.usWinDescent, 0
    return a, d, g


def face(out, font):
    out.append("tables %d" % len(font.reader.tables))
    order = font.getGlyphOrder()
    gid = {n: i for i, n in enumerate(order)}
    if "glyf" in font:
        kind = "glyf"
    elif "CFF2" in font:
        kind = "cff2"
    else:
        kind = "cff"
    out.append("outline %s" % kind)
    head = font["head"]
    out.append("upem %d" % head.unitsPerEm)
    out.append("glyphs %d" % font["maxp"].numGlyphs)
    out.append("head %d %d %d %d %d %d" % (head.xMin, head.yMin, head.xMax, head.yMax, head.macStyle,
                                           head.indexToLocFormat))
    hhea = font["hhea"]
    out.append("hhea %d %d %d %d" % (hhea.ascent, hhea.descent, hhea.lineGap, hhea.numberOfHMetrics))
    os2 = font["OS/2"]
    out.append("os2 %d %d %d %d %d %d %d %d %d %d %d %d %d" % (
        os2.version, os2.usWeightClass, os2.usWidthClass, os2.fsSelection,
        os2.sTypoAscender, os2.sTypoDescender, os2.sTypoLineGap, os2.usWinAscent, os2.usWinDescent,
        getattr(os2, "sxHeight", 0), getattr(os2, "sCapHeight", 0),
        os2.yStrikeoutSize, os2.yStrikeoutPosition))
    post = font["post"]
    out.append("post %d %d %d %d" % (post.underlinePosition, post.underlineThickness,
                                     round(post.italicAngle * 65536), post.isFixedPitch))
    a, d, g = metrics(font)
    mac = head.macStyle
    italic = 1 if (os2.fsSelection & (1 | 512)) or (mac & 2) else 0
    bold = 1 if (os2.fsSelection & 32) or (mac & 1) else 0
    xh = getattr(os2, "sxHeight", 0) if os2.version >= 2 else 0
    ch = getattr(os2, "sCapHeight", 0) if os2.version >= 2 else 0
    out.append("metrics %d %d %d %d %d %d %d %d %d %d %d %d %d %d" % (
        a, d, g, xh, ch, post.underlinePosition, post.underlineThickness,
        os2.yStrikeoutPosition, os2.yStrikeoutSize, os2.usWeightClass, os2.usWidthClass,
        italic, bold, 1 if post.isFixedPitch else 0))
    for i in NAME_IDS:
        s = pick_name(font, i)
        if s is not None:
            out.append("name %d %s" % (i, s.encode("utf-8").hex() or "-"))
    for t in font["cmap"].tables:
        out.append("subtable %d %d %d" % (t.platformID, t.platEncID, t.format))
        if t.format == 14:
            for vs in sorted(t.uvsDict):
                for cp, name in sorted(t.uvsDict[vs]):
                    out.append("uvs %d %d %d" % (cp, vs, -1 if name is None else gid[name]))
        else:
            for cp in sorted(t.cmap):
                out.append("map %d %d" % (cp, gid[t.cmap[cp]]))
    best = font.getBestCmap() or {}
    for cp in sorted(best):
        out.append("cmap %d %d" % (cp, gid[best[cp]]))
    hmtx = font["hmtx"]
    for i, n in enumerate(order):
        adv, lsb = hmtx[n]
        out.append("hmtx %d %d %d" % (i, adv, lsb))
    if "kern" in font:
        for st in font["kern"].kernTables:
            if getattr(st, "format", None) == 0 and st.coverage & 1:
                for (l, r), v in sorted(st.kernTable.items(), key=lambda kv: (gid[kv[0][0]], gid[kv[0][1]])):
                    out.append("kern %d %d %d" % (gid[l], gid[r], v))


def main():
    paths = sorted(glob.glob(os.path.join(DATA, "*.ttf")) + glob.glob(os.path.join(DATA, "*.otf"))
                   + glob.glob(os.path.join(DATA, "*.ttc")))
    for p in paths:
        fonts = TTCollection(p).fonts if p.endswith(".ttc") else [TTFont(p)]
        out = ["faces %d" % len(fonts)]
        for i, f in enumerate(fonts):
            out.append("face %d" % i)
            face(out, f)
        with open(p + ".sfnt.ref", "w") as fh:
            fh.write("\n".join(out) + "\n")
        print(os.path.basename(p) + ".sfnt.ref", len(out), "lines")


if __name__ == "__main__":
    main()
