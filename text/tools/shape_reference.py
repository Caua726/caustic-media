#!/usr/bin/env python3
# text/tools/shape_reference.py — text shaped by HarfBuzz, written as text for
# shape_test.cst.
#
# Development tool: needs uharfbuzz (see reference_common.py); its output is
# versioned. One file per font, <font>.shape.ref, a list of cases:
#
#   case NAME
#   face I                  the face of a collection; 0 for a single font
#   text HEX..              the UTF-8 bytes shaped, "-" for none
#   item OFFSET LENGTH      the run within them, in bytes; LENGTH -1 to the end
#   props DIR SCRIPT LANG   ltr or rtl; ISO 15924; BCP 47, "-" for none
#   flags N                 HarfBuzz's buffer flags
#   features N              then N lines: TAG VALUE START END, START and END
#                           clusters (byte offsets) — 0 4294967295 for all
#   variations N            then N lines: TAG VALUE, the instance's axes
#   glyphs N                then N lines: GID CLUSTER FLAGS XA YA XO YO —
#                           glyph, cluster, glyph flags (unsafe to break),
#                           advances and offsets in font units
#
# HarfBuzz is asked as a program would ask it, with the font at its own
# units (scale = units per em), so every position is in font units, rounded
# where HarfBuzz rounds.
#
#   ~/.cache/caustic-media/venv/bin/python text/tools/shape_reference.py
import os

import uharfbuzz as hb

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")

# Every feature HarfBuzz turns on by itself, for the default shaper in
# horizontal text: off, it is the shaping pipeline alone — Unicode, clusters,
# normalization, the cmap, glyph classes, advances, ignorables, spaces.
DEFAULTS = ["rvrn", "ltra", "ltrm", "rtla", "rtlm", "frac", "numr", "dnom", "rand", "Harf", "HARF",
            "Buzz", "BUZZ", "abvm", "blwm", "ccmp", "locl", "mark", "mkmk", "rlig", "calt", "clig",
            "curs", "dist", "kern", "liga", "rclt"]
ALL = 4294967295
OFF = [(t, 0, 0, ALL) for t in DEFAULTS]

BOT, EOT = 1, 2
PRESERVE, REMOVE, NO_CIRCLE = 4, 8, 16


def grouped(features):
    """The features in the order uharfbuzz hands them to HarfBuzz: by tag, in
    the order each tag first appears."""
    order = []
    for f in features:
        if f[0] not in order:
            order.append(f[0])
    return [f for t in order for f in features if f[0] == t]


def case(name, text, script="Latn", direction="ltr", lang=None, features=OFF, flags=0, item=(0, -1),
         face=0, variations=None):
    if isinstance(text, str):
        text = text.encode("utf-8")
    return dict(name=name, text=text, script=script, dir=direction, lang=lang, features=grouped(features),
                flags=flags, item=item, face=face, variations=variations or {})


# The pipeline alone, every feature off.
CORE = [
    case("plain", "Hello, world!"),
    case("empty", ""),
    case("item", "abcdef", item=(2, 3)),
    case("item to the end", "abcdef", item=(4, -1)),
    # Normalization: decomposed when the font lacks the character, composed
    # when it has the composite; marks in HarfBuzz's order of classes.
    case("decompose", "ẽ"),
    case("compose", "éÄ"),
    case("compose twice", "ḉ"),
    case("singleton", "Å"),
    case("reorder", "á̧̂"),
    case("modified classes", "xِّxַּx᩠́x༹́x࿆́"),
    case("greek", "ᾳά"),
    case("marks", "é̂ẍ̴"),
    case("many marks", "a" + "́" * 20 + "̧" * 20),
    case("cgj", "á͏̀à͏̧"),
    # Default ignorables, hidden; and kept or removed as the flags say.
    case("ignorables", "a‍b­c‌d﻿e⁠f️g\U000E0041h͏i"),
    case("ignorables kept", "a‍b­", flags=PRESERVE),
    case("ignorables removed", "a‍b­c", flags=REMOVE),
    # Spaces the font lacks, made from its space; and U+2011 from U+2010.
    case("spaces", "a              　 b"),
    case("nb hyphen", "a‑b"),
    case("missing", "一a"),
    # Graphemes and their clusters.
    case("emoji", "\U0001F468‍\U0001F469\U0001F600\U0001F3FB\U0001F1E7\U0001F1F7\U0001F1E6x"),
    case("katakana marks", "ｶﾞﾟ"),
    # Variation selectors, through cmap format 14 or not at all.
    case("variation selectors", "❤️❤︎❤️️❤A︀B\U000E0100"),
    case("cjk variants", "一︀丁\U000E0100漢︀葛\U000E0100葛\U000E0101"
         "漢\U000E0101\U00029E3D\U000E0101一\U000E0100"),
    case("variation selector after marks", "❤́️"),
    # Bytes that are not UTF-8: one replacement for each.
    case("ill-formed", b"a\xffb\xe2\x82c\xc0\xafd\xed\xa0\x80e\xf4\x90\x80\x80f\xf0\x9f\x98g\x80"),
    # Right to left: Latin reversed into its own direction and back, brackets
    # mirrored; Thaana and Phoenician, right to left by nature.
    case("rtl latin", "(ab́c) ∈", direction="rtl"),
    case("thaana", "ހަށ (ނ)∈«‹", script="Thaa", direction="rtl"),
    case("thaana ltr", "ހަށ", script="Thaa"),
    case("thaana digits", "12 3", script="Thaa"),
    case("thaana flags", "\U0001F1E7\U0001F1F7", script="Thaa"),
    case("phoenician", "\U00010900\U00010901 \U00010902", script="Phnx", direction="rtl"),
    # A mark first: a dotted circle for it at the start of the text alone.
    case("dotted circle", "́a", flags=BOT),
    case("no dotted circle", "́a"),
    case("dotted circle after context", "x́a", flags=BOT, item=(1, -1)),
    case("dotted circle refused", "́a", flags=BOT | NO_CIRCLE),
    case("fraction", "1⁄2"),
]

# Substitution: HarfBuzz's features as it sets them, but those of GPOS off.
GPOS_OFF = [(t, 0, 0, ALL) for t in ("abvm", "blwm", "mark", "mkmk", "curs", "dist", "kern")]


def subst(name, text, features=(), **kw):
    return case(name, text, features=GPOS_OFF + list(features), **kw)


GSUB = [
    subst("liga", "office fluffy fifth"),
    subst("liga rtl", "office fluffy", direction="rtl"),
    subst("liga off in part", "office fluffy fifth", features=[("liga", 0, 7, 13)]),
    subst("liga on in part", "office fluffy fifth", features=[("liga", 0, 0, ALL), ("liga", 1, 3, 9)]),
    subst("liga by value", "office", features=[("liga", 2, 0, ALL)]),
    subst("liga off", "office fluffy fifth", features=[("liga", 0, 0, ALL)]),
    subst("liga item", "office fluffy fifth", item=(2, 10)),
    subst("ccmp marks", "i̇ j́ í ı̇ į́"),
    subst("ccmp off", "i̇ j́ í", features=[("ccmp", 0, 0, ALL)]),
    subst("joiners", "f‍i f‌i f­i f́i f͏i"),
    subst("locl tr", "fi i İ", lang="tr"),
    subst("locl az", "fi i", lang="az"),
    subst("locl ro", "şţ Şţ", lang="ro"),
    subst("locl nl", "ĳ IJ ij", lang="nl"),
    subst("locl ca", "l·l", lang="ca"),
    subst("locl sr", "бгдпт", script="Cyrl", lang="sr"),
    subst("locl bg", "бгдпт", script="Cyrl", lang="bg"),
    subst("locl mk", "бгдпт", script="Cyrl", lang="mk"),
    subst("locl unknown language", "fi i", lang="xx"),
    subst("locl private tag", "fi i", lang="x-hbottrk"),
    subst("fraction", "1⁄2 3⁄4 10⁄20 ⁄5 6⁄"),
    subst("fraction rtl", "1⁄2", direction="rtl"),
    subst("fraction off", "1⁄2", features=[("frac", 0, 0, ALL)]),
    subst("frac by hand", "1/2 3/4", features=[("frac", 1, 0, ALL)]),
    subst("numr and dnom by hand", "12/34", features=[("numr", 1, 0, 2), ("dnom", 1, 3, 5)]),
    subst("greek", "ἄλφα ά ΐ", script="Grek"),
    subst("calt", "->=> <= :: www"),
    subst("calt off", "->=> <= :: www", features=[("calt", 0, 0, ALL)]),
    subst("unknown script", "office fi", script="Zzzz"),
    subst("ignorables in ligatures", "o‍f‍f‍i‍c‍e"),
    subst("dotted circle", "́fi", flags=BOT),
]

FONTS = [
    ("glyf.ttf", [CORE, GSUB]),
    ("cff.otf", [CORE, GSUB]),
    ("kern.ttf", [CORE, GSUB]),
    ("cid.otf", [CORE, GSUB]),
    ("cmaps.ttf", [CORE, GSUB]),
    ("shape.ttf", [CORE, GSUB]),
    ("vf.ttf", [CORE, GSUB] + [[dict(c, name=c["name"] + " at 650", variations={"wght": 650, "opsz": 20})
                                for c in g] for g in (CORE, GSUB)]),
    ("cff2.otf", [CORE, GSUB] + [[dict(c, name=c["name"] + " at 250", variations={"wght": 250})
                                  for c in g] for g in (CORE, GSUB)]),
    ("pair.ttc", [[dict(c, face=1) for c in g] for g in (CORE, GSUB)]),
]


def shape(blob, c):
    face = hb.Face(blob, c["face"])
    font = hb.Font(face)
    upem = face.upem
    font.scale = (upem, upem)
    if c["variations"]:
        font.set_variations(c["variations"])
    buf = hb.Buffer()
    off, length = c["item"]
    buf.add_utf8(c["text"], off, length)
    buf.direction = c["dir"]
    buf.script = c["script"]
    if c["lang"]:
        buf.language = c["lang"]
    buf.flags = hb.BufferFlags(c["flags"])
    feats = {}
    for tag, value, start, end in c["features"]:
        feats.setdefault(tag, []).append((start, end, value))
    hb.shape(font, buf, feats)
    out = []
    for info, pos in zip(buf.glyph_infos or [], buf.glyph_positions or []):
        out.append("%d %d %d %d %d %d %d" % (info.codepoint, info.cluster, info.flags,
                                              pos.x_advance, pos.y_advance, pos.x_offset, pos.y_offset))
    return out


def main():
    for name, groups in FONTS:
        blob = hb.Blob.from_file_path(os.path.join(DATA, name))
        lines = ["# %s shaped by HarfBuzz %s; see text/tools/shape_reference.py" % (name, hb.version_string())]
        n = 0
        for group in groups:
            for c in group:
                lines.append("case " + c["name"])
                lines.append("face %d" % c["face"])
                lines.append("text " + (" ".join("%02x" % b for b in c["text"]) if c["text"] else "-"))
                lines.append("item %d %d" % c["item"])
                lines.append("props %s %s %s" % (c["dir"], c["script"], c["lang"] or "-"))
                lines.append("flags %d" % c["flags"])
                lines.append("features %d" % len(c["features"]))
                for f in c["features"]:
                    lines.append("%s %d %d %d" % f)
                lines.append("variations %d" % len(c["variations"]))
                for tag, value in sorted(c["variations"].items()):
                    lines.append("%s %s" % (tag, value))
                glyphs = shape(blob, c)
                lines.append("glyphs %d" % len(glyphs))
                lines += glyphs
                n += 1
        out = os.path.join(DATA, os.path.splitext(name)[0] + ".shape.ref")
        with open(out, "w") as fh:
            fh.write("\n".join(lines) + "\n")
        print(os.path.basename(out), n, "cases")


if __name__ == "__main__":
    main()
