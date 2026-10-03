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
#   flags WORD..            "-", or any of: start (the text begins the
#                           paragraph), keep and drop (default ignorables
#                           kept, or removed), nocircle (no dotted circle)
#   features N              then N lines: TAG VALUE START END, START and END
#                           clusters (byte offsets) — 0 4294967295 for all
#   variations N            then N lines: TAG VALUE, the instance's axes
#   glyphs N                then N lines: GID CLUSTER XA YA XO YO — glyph,
#                           cluster, advances and offsets in font units
#
# HarfBuzz is a black box here: it is asked as a program would ask it, with
# the font at its own units (scale = units per em), so every position is in
# font units. Its glyph flags are not written: whether text may be broken is
# the shaper's own to say, and shape_test checks that what it says holds.
#
#   ~/.cache/caustic-media/venv/bin/python text/tools/shape_reference.py
import os

import uharfbuzz as hb

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")

# The features a shaper turns on by itself for horizontal text (shape.md):
# off, it is the shaping steps alone — Unicode, clusters, normalization, the
# cmap, glyph classes, advances, ignorables, spaces.
DEFAULTS = ["rvrn", "ltra", "ltrm", "rtla", "rtlm", "frac", "numr", "dnom", "rand", "abvm", "blwm",
            "ccmp", "locl", "mark", "mkmk", "rlig", "calt", "clig", "curs", "dist", "kern", "liga", "rclt"]
ALL = 4294967295
OFF = [(t, 0, 0, ALL) for t in DEFAULTS]

BOT, EOT = 1, 2
PRESERVE, REMOVE, NO_CIRCLE = 4, 8, 16
FLAG_WORDS = [(BOT, "start"), (PRESERVE, "keep"), (REMOVE, "drop"), (NO_CIRCLE, "nocircle")]


def flag_words(flags):
    words = [w for bit, w in FLAG_WORDS if flags & bit]
    return " ".join(words) if words else "-"


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
    case("greek", "ᾳά"),
    case("marks", "é̂ẍ̴"),
    case("many marks", "a" + "́" * 20 + "̧" * 20),
    case("cgj", "á͏̀à͏̧"),
    case("a mark moved from third place", "a\u0301\u0300\u0323"),
    case("composition blocked by a mark of its class", "e\u0304\u0301"),
    case("composed with an overlay", "\u2208\u0338"),
    case("marks of every edge", "a\u0315\u302D\u031B\u1DFA\u302B\u1DCE"),
    case("two marks of one class, kept in order", "e\u0300\u0301"),
    case("a mark after a control", "a\u200b\u0301"),
    case("a grapheme joiner first", "\u034fa"),
    case("a grapheme joiner last, after a mark", "a\u0301\u034f"),
    case("a grapheme joiner between marks, the class rising", "a\u0323\u034f\u0301"),
    case("a grapheme joiner between marks of one class", "a\u0301\u034f\u0300"),
    # Default ignorables, hidden; and kept or removed as the flags say.
    case("ignorables", "a‍b­c‌d﻿e⁠f️g\U000E0041h͏i"),
    case("ignorables kept", "a‍b­", flags=PRESERVE),
    case("ignorables removed", "a‍b­c", flags=REMOVE),
    case("ignorable first, removed", "\u200ba\u0301b", flags=REMOVE),
    case("ignorable before a mark, removed", "a\u200b\u0301b", flags=REMOVE),
    case("ignorable removed right to left", "ހ\u200bށ", script="Thaa", direction="rtl", flags=REMOVE),
    case("ignorable inside a cluster, removed", "a\u034f\u0301b", flags=REMOVE),
    case("ignorable removed right to left, after a cluster of two", "ހ\u200bށ\u07a6", script="Thaa",
         direction="rtl", flags=REMOVE),
    case("an ignorable alone, removed", "\u200b", flags=REMOVE),
    case("two ignorables first, removed", "\u200b\u200ba\u0301b", flags=REMOVE),
    case("ignorable last, removed", "ab\u0301\u200b", flags=REMOVE),
    case("ignorable before a cluster of three, removed", "a\u200be\u0301\u0302b", flags=REMOVE),
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
    case("thaana letters and digits", "ހ12", script="Thaa"),
    case("thaana punctuation", "«(»", script="Thaa"),
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
    subst("fraction", "1⁄2 3⁄4 10⁄20 ⁄5 6⁄"),
    subst("fraction rtl", "1⁄2", direction="rtl"),
    subst("fraction off", "1⁄2", features=[("frac", 0, 0, ALL)]),
    subst("frac by hand", "1/2 3/4", features=[("frac", 1, 0, ALL)]),
    subst("numr and dnom by hand", "12/34", features=[("numr", 1, 0, 2), ("dnom", 1, 3, 5)]),
    subst("greek", "ἄλφα ά ΐ", script="Grek"),
    subst("calt", "->=> <= :: www"),
    subst("calt off", "->=> <= :: www", features=[("calt", 0, 0, ALL)]),
    subst("calt, Thaana left to right, no letters or digits", "->=>", script="Thaa"),
    subst("calt, Thaana left to right, a letter among digits", " \u07801->", script="Thaa"),
    subst("calt, Thaana left to right, a capital among digits", "A1->", script="Thaa"),
    subst("unknown script", "office fi", script="Zzzz"),
    subst("ignorables in ligatures", "o‍f‍f‍i‍c‍e"),
    subst("dotted circle", "́fi", flags=BOT),
]

# Everything on: positioning too — kerning, marks on bases, on ligatures,
# on marks.
def full(name, text, features=(), **kw):
    return case(name, text, features=list(features), **kw)


POS = [
    full("kerning", "AVATAR Wolf Type ToT yy. LT P, V. W. Y."),
    full("kerning rtl", "AVATAR Wolf", direction="rtl"),
    full("kerning off", "AVATAR Wolf", features=[("kern", 0, 0, ALL)]),
    full("kerning in part", "AVATAR Wolf", features=[("kern", 0, 2, 5)]),
    full("marks", "é̂ ẍ̴ ḉ q̣̇ ǭ a̧̧ ɑ̃ ŋ̊"),
    full("marks off", "é̂ ẍ̴ ḉ", features=[("mark", 0, 0, ALL), ("mkmk", 0, 0, ALL)]),
    full("marks on marks", "ậ̃ ŏ̧̈́ ŭ̇̂"),
    full("marks on ligatures", "f́i fí ffí ff̂ĩ fl̃ ﬁ́"),
    full("marks after joiners", "a‍́ a‌́ a͏́"),
    full("greek", "ἄλφα ΐ ὦ ᾷ", script="Grek"),
    full("cyrillic", "Ёлка й ў Ѷ", script="Cyrl"),
    full("fraction", "1⁄2 AV 3⁄4"),
    full("thaana", "ހަށް ނިޔަ", script="Thaa", direction="rtl"),
    full("palt", "日本語、テスト。", script="Jpan", features=[("palt", 1, 0, ALL)]),
    full("dotted circle", "́AV", flags=BOT),
]

# lookups.ttf, made by the tool: every lookup type, format and flag.
def look(name, text, features=(), **kw):
    return case(name, text, features=list(features), **kw)


ON = lambda tag, value=1: (tag, value, 0, ALL)
LOOKUPS = [
    look("single", "abcde", [ON("smcp")]),
    look("alternate 1", "a", [ON("salt")]),
    look("alternate 2", "a", [ON("salt", 2)]),
    look("alternate 3", "a", [ON("salt", 3)]),
    look("alternate past the last", "a", [ON("salt", 4)]),
    look("an alternate lowered for a stretch", "aa", [ON("salt", 3), ("salt", 2, 0, 1)]),
    look("alternate at random", "bbbbbbbb"),
    look("multiple and none", "awqa"),
    look("ligatures over marks", "fi f́i ffi f́fi fl Th"),
    look("ligatures split", "fi fí f́i", [ON("dlig")]),
    look("chained contexts", "xyz xyy moop nor"),
    look("reverse chaining", "msst nst mst"),
    look("required feature", "i fi", lang="tr"),
    look("required feature elsewhere", "i fi"),
    look("a language's second tag", "a", [ON("ss06")], lang="ko"),
    look("contexts by glyph, class, coverage", "gh jkr hm km rt", [ON("ss01")]),
    look("nested lookups that add and remove", "pwx fix", [ON("ss01"), ON("liga", 0)]),
    look("chained by classes", "ajo ajp gjo", [ON("ss01")]),
    look("passing over ligatures", "afib", [ON("ss01")]),
    look("passing over bases", "áè áe", [ON("ss01")]),
    look("pairs", "av va AV TYo Le LT LY kk"),
    look("pairs rtl", "av va AV kk", direction="rtl"),
    look("single adjustments", "xgj mn", [ON("ss02")]),
    look("marks on bases", "á à̃ ạ̣ ç ọ̣ x̊ ǵ"),
    look("marks on ligatures", "fí f́i ffí f́fi ff́i fị"),
    look("marks on pieces", "fí f́i", [ON("dlig")]),
    look("marks rtl", "á à̃ fí", direction="rtl"),
    look("cursive", "kllk klkl kĺk"),
    look("cursive rtl", "kllk kĺk", direction="rtl"),
    look("contextual positioning", "xyz xy xz tu yzt stts", [ON("ss02")]),
    look("everything off", "fi av á", OFF),
    # What the cases above leave untried.
    look("passing over ligatures, nothing at random", "afib", [ON("ss01"), ON("rand", 0)]),
    look("nested lookups that add, ccmp off", "pwx", [ON("ss01"), ON("ccmp", 0)]),
    look("records out of order", "bw", [ON("ss03"), ON("ccmp", 0), ON("rand", 0)]),
    look("records out of order, later in the text", "abw", [ON("ss03"), ON("ccmp", 0), ON("rand", 0)]),
    look("lookups nested 64 deep", "n", [ON("ss03")]),
    look("chained by glyph, two behind and two ahead", "abcde", [ON("ss03"), ON("rand", 0)]),
    look("chained by class, two behind and two ahead", "abdef", [ON("ss03"), ON("rand", 0)]),
    look("chained by coverage, two behind and two ahead", "hgegm", [ON("ss03")]),
    look("a glyph made a mark", "aB", [ON("ss03")]),
    look("a ligature made a mark", "a\u0301MN", [ON("ss03")]),
    look("a ligature of a ligature called a base", "FG\u0301H", [ON("ss03")]),
    look("a ligature of a ligature", "fi\u0301l", [ON("ss03")]),
    look("one glyph for one, by a multiple substitution", "I", [ON("ss03")]),
    look("reverse chaining at the end", "JK", [ON("ss03")]),
    look("marks of two components joined, passing over the ligature", "f\u0301i\u0300", [ON("ss03")]),
    look("marks of two components joined, not passing over it", "f\u0301i\u0300", [ON("ss04")]),
    look("a mark not joined past another ligature", "nfi\u0301", [ON("ss03")]),
    look("a mark joined to a piece", "w\u0301\u0303", [ON("ss03")]),
    look("a mark on the first piece", "w\u0301"),
    look("the largest negative advance", "O", [ON("ss03")]),
    look("cursive from both ends", "CDE", [ON("ss03")]),
    look("pairs, the second passed", "AVA"),
    look("marks on two components", "f\u0301i\u0301"),
    look("marks on marks past a ligature", "a\u0301fi\u0301", [ON("ss04")]),
    look("mark filtering past a joiner", "a\u0323\u0301\u034f\u0323"),
    look("grapheme joiner between bases", "f\u034fi"),
    look("grapheme joiner between marks", "a\u0301\u034f\u0323"),
    look("variation selector in a ligature", "f\ufe00i"),
    look("variation selector in a pair", "a\ufe00v"),
    look("32 marks, ordered", "a" + "\u0301\u0323" * 16),
    look("33 marks, not", "a" + "\u0301\u0323" * 16 + "\u0323"),
    look("four marks ordered", "a\u0301\u0323\u0301\u0323"),
    look("a fraction", "1\u20442"),
    look("a fraction, numr off", "1\u20442", [ON("numr", 0)]),
    look("a fraction right to left", "1\u20442", script="Thaa", direction="rtl"),
    look("ranged to the end", "abcde", [("smcp", 1, 2, ALL)]),
    look("ranged over a ligature", "fiabc", [("smcp", 1, 0, 3)]),
    look("deleted first", "qa"),
    look("deleted, right to left", "awqa qa", direction="rtl"),
    look("deleted twice", "aqqa"),
    look("deleted with its mark, last", "aq\u0301"),
    look("one for one, twice", "II", [ON("ss03")]),
    look("alternate 3, its bits moved by a feature before", "ba", [("smcp", 1, 0, 1), ON("salt", 3)]),
    look("many multiplied", "w" * 100),
    look("cursive, Thaana", "kllk kĺk", script="Thaa", direction="rtl"),
    look("marks, Thaana", "á à̃ fí", script="Thaa", direction="rtl"),
    look("pairs, Thaana", "av va AV kk", script="Thaa", direction="rtl"),
    # ss05: lookups of several subtables and rules, each way an earlier one
    # passes over what a later one takes.
    look("pairs, several for one glyph", "acadaeai", [ON("ss05")]),
    look("a pair of zero", "af", [ON("ss05")]),
    look("pairs, the second glyph placed alone", "oc od", [ON("ss05")]),
    look("pairs of classes, both valued", "ToYeLTLY", [ON("ss05")]),
    look("pairs, both valued, twice", "AVAV"),
    look("cursive, a subtable with no entry", "gj", [ON("ss05")]),
    look("cursive, a subtable with no exit", "gr", [ON("ss05")]),
    look("cursive, a subtable without the glyph before", "hr", [ON("ss05")]),
    look("cursive, a subtable without the glyph", "hs", [ON("ss05")]),
    look("cursive by subtables, right to left", "gj gr hr hs", [ON("ss05")], direction="rtl"),
    look("cursive turned back over the same pair", "hu", [ON("ss05")]),
    look("cursive by subtables, Thaana", "gj gr hr hs", [ON("ss05")], script="Thaa", direction="rtl"),
    look("cursive chained 70 long", "k" * 70),
    look("cursive chained 70 long, right to left", "k" * 70, direction="rtl"),
    look("cursive turned round from 70 away", "D" * 70 + "E", [ON("ss05")]),
    look("marks, a subtable without the mark", "d\u0303", [ON("ss05")]),
    look("marks, a subtable with no anchor for the mark's class", "d\u0300", [ON("ss05")]),
    look("marks, the first subtable applying", "d\u0301", [ON("ss05")]),
    look("a mark on a mark at the start", "\u0301\u0303"),
    look("marks, a subtable without the base", "ź", [ON("ss05")]),
    look("marks on ligatures, a subtable without the mark", "Th̀", [ON("ss05")]),
    look("marks on ligatures, a subtable without the ligature", "fĺ", [ON("ss05")]),
    look("marks on marks, a subtable without the mark", "á̃", [ON("ss05")]),
    look("marks on marks, a subtable without the one below", "å̀", [ON("ss05")]),
    look("a base put on another as a mark", "xy", [ON("ss05")]),
    look("a base put on another as a mark, right to left", "xy", [ON("ss05")], direction="rtl"),
    look("a base put on another as a mark, Thaana", "xy", [ON("ss05")], script="Thaa", direction="rtl"),
    look("a base put on another past a mark, Thaana", "x\u0301y", [ON("ss05")], script="Thaa", direction="rtl"),
    look("rules, the first applied", "MNM BFB IJK pup", [ON("ss05")]),
    look("rules, the second applied", "MO BI IO pv", [ON("ss05")]),
    look("a class with no rules", "CC mm", [ON("ss05")]),
    look("a nested lookup growing the input, marks passed over", "1\u03012\u03011", [ON("ss05")]),
    look("a nested lookup shrinking the input, marks passed over", "2\u03011\u03012", [ON("ss05")]),
    look("a ligature of a glyph's pieces, a mark after", "w\u0301", [ON("ss05")]),
    look("marks of one component and the next joined", "f\u0300\u0301i\u0300", [ON("ss03")]),
    look("reverse, a subtable without the lookahead", "cbe", [ON("ss05"), ON("rand", 0)]),
    look("reverse, subtables without the backtrack", "abd", [ON("ss05"), ON("rand", 0)]),
    look("reverse, subtables without the glyph", "xe", [ON("ss05"), ON("rand", 0)]),
    look("reverse chaining nested", "o", [ON("ss05")]),
    look("a mark after two pieces", "H́", [ON("ss05")]),
    look("a mark after three pieces", "Ǵ", [ON("ss05")]),
    look("grapheme joiner after a mark, before a base", "f́͏i"),
    look("grapheme joiner after a base, before a mark", "n͏́", [ON("ss03")]),
    look("tags at both ends of their block", "a\U000E0020b\U000E007Fc\U000E001Fd\U000E0080e"),
    look("a ligature within the feature's stretch", "fifl", [("liga", 0, 0, ALL), ("liga", 1, 2, 4)]),
    look("a ligature past the feature's end", "fi", [("liga", 0, 0, ALL), ("liga", 1, 0, 1)]),
    look("a context past the feature's end", "xyz", [("calt", 0, 0, ALL), ("calt", 1, 0, 2)]),
    look("a backtrack before the feature's start", "xyz", [("calt", 0, 0, ALL), ("calt", 1, 1, 3)]),
    look("cursive, the glyph before outside the feature", "kl", [("curs", 0, 0, ALL), ("curs", 1, 1, 2)]),
    look("cursive across a joiner", "k\u200dl"),
    look("a tag inside a ligature", "f\U000E0041i"),
    look("a grapheme joiner inside a ligature of marks", "a\u0301\u034f\u0300", [ON("ss04")]),
    look("a grapheme joiner first, before a ligature of marks", "\u034f\u0301\u0300", [ON("ss04")]),
    look("a grapheme joiner last, after a ligature of marks", "a\u0301\u0300\u034f", [ON("ss04")]),
    look("a grapheme joiner between marks, the class rising", "a\u0323\u034f\u0301\u0300", [ON("ss04")]),
    look("a grapheme joiner between marks, the class falling", "a\u0301\u034f\u0323\u0300", [ON("ss04")]),
]

# varlookups.ttf, made by the tool: lookups the instance changes, and a GDEF
# with no glyph classes. Its DFLT language system is reached by common text.
def vlook(name, text, features=(), **kw):
    return case(name, text, features=list(features), **kw)


VARLOOKUPS = [
    vlook("default", "abcde", script="Zyyy"),
    vlook("half way up", "abcde", script="Zyyy", variations={"wght": 650}),
    vlook("heaviest", "abcde", script="Zyyy", variations={"wght": 900}),
    vlook("just below half way", "abcde", script="Zyyy", variations={"wght": 649}),
    vlook("light and narrow", "abcde", script="Zyyy", variations={"wght": 100, "wdth": 75}),
    vlook("light, not narrow", "abcde", script="Zyyy", variations={"wght": 100, "wdth": 100}),
    vlook("narrow, not light", "abcde", script="Zyyy", variations={"wght": 400, "wdth": 75}),
    vlook("one lookup listed twice", "d"),
    vlook("a required feature whose tag is not listed", "ex", script="Grek"),
    vlook("no glyph classes: a mark replaced", "a\u0301", [ON("ss01")]),
    vlook("no glyph classes: a ligature passed over", "nfix", [ON("ss01")]),
    vlook("no glyph classes: a ligature's pieces", "nfiy", [ON("ss01"), ON("dlig")]),
    vlook("no glyph classes: a base and a mark joined, still a base", "nn\u0301x", [ON("ss01"), ON("ss02")]),
    vlook("no glyph classes: a ligature that starts with a mark", "a\u0301x", [ON("ss02")]),
    vlook("devices at the default", "xy a\u0301"),
    vlook("pairs varied, heavy", "ab ac ad", variations={"wght": 900}),
    vlook("pairs varied, light", "ab ac ad", variations={"wght": 100}),
    vlook("devices, light", "xy a\u0301", variations={"wght": 100}),
    vlook("devices, heavy", "xy a\u0301", variations={"wght": 900}),
]

FONTS = [
    ("glyf.ttf", [CORE, GSUB, POS]),
    ("cff.otf", [CORE, GSUB, POS]),
    ("kern.ttf", [CORE, GSUB, POS]),
    ("cid.otf", [CORE, GSUB, POS]),
    ("cmaps.ttf", [CORE, GSUB, POS]),
    ("shape.ttf", [CORE, GSUB, POS]),
    ("shape-plain.ttf", [CORE, GSUB, POS]),
    ("lookups.ttf", [LOOKUPS]),
    ("varlookups.ttf", [VARLOOKUPS]),
    ("vf.ttf", [CORE, GSUB, POS] + [[dict(c, name=c["name"] + " at 650", variations={"wght": 650, "opsz": 20})
                                     for c in g] for g in (CORE, GSUB, POS)]),
    ("cff2.otf", [CORE, GSUB, POS] + [[dict(c, name=c["name"] + " at 250", variations={"wght": 250})
                                       for c in g] for g in (CORE, GSUB, POS)]),
    ("pair.ttc", [[dict(c, face=1) for c in g] for g in (CORE, GSUB, POS)]),
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
        out.append("%d %d %d %d %d %d" % (info.codepoint, info.cluster,
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
                lines.append("flags " + flag_words(c["flags"]))
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
