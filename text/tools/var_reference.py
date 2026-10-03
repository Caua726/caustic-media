#!/usr/bin/env python3
# text/tools/var_reference.py — variable fonts at chosen instances, written as
# text for var_test.cst and outline_test.cst.
#
# Development tool: needs fontTools and uharfbuzz (see reference_common.py);
# its output is versioned. One file per variable font, <font>.var.ref:
#
#   axes N
#   axis TAG MIN DEFAULT MAX FLAGS NAMEID       fvar's, in order
#   instances N
#   instance NAMEID PSNAMEID FLAGS V1 .. VN     PSNAMEID -1 when absent
#   location N          an instance by its axes' values, N of them set:
#   set TAG VALUE
#   normalized C1 .. CN HarfBuzz's coordinates, 2.14 fixed point as integers
#   advance GID HB EXACT    HarfBuzz's advance, rounded as it rounds, and the
#                       unrounded one: hmtx and HVAR, or the phantom points —
#                       or hmtx's alone when every coordinate is 0
#   metric TAG DELTA    MVAR's delta for each of its tags
#   glyph GID           every glyph's outline at the instance, as in
#   M .. L .. Q .. C .. Z   *.outline.ref
#   end
#   named I             a named instance: its coordinates alone, as
#   normalized C1 .. CN the rest follows from them as for any other
#
# Coordinates are HarfBuzz's; the deltas at them fontTools', and each glyph is
# drawn by HarfBuzz too and must pass through the same points, each advance
# must round to HarfBuzz's, each MVAR delta must be HarfBuzz's.
#
#   ~/.cache/caustic-media/venv/bin/python text/tools/var_reference.py
import os

import uharfbuzz as hb
from fontTools.ttLib import TTFont
from fontTools.varLib.varStore import VarStoreInstancer

from reference_common import num, Segments, draw_glyf, hb_font, check_against_hb, round_hb

HERE = os.path.dirname(os.path.abspath(__file__))
DATA = os.path.join(HERE, "..", "testdata")

FONTS = [
    ("vf.ttf", [{"wght": 650, "opsz": 20}, {"wght": 100, "opsz": 32}]),
    ("cff2.otf", [{"wght": 250}, {"wght": 800}]),
    ("var.ttf", [{"wght": 900}, {"wght": 100}, {"wght": 650, "wdth": 110}, {"wdth": 125, "ZHID": 50},
                 {"wght": 300, "wdth": 80, "ZHID": 100}, {"wght": 2000, "wdth": 0}]),
    ("var.otf", [{"wght": 900}, {"wght": 200, "wdth": 125}, {"wght": 650, "wdth": 90}]),
]


def normalized(out, hbf):
    norm = [round_hb(v * 16384) for v in hbf.get_var_coords_normalized()]
    out.append("normalized " + " ".join(str(v) for v in norm))
    return norm


def instance(out, font, data, hbf, what):
    tags = [a.axisTag for a in font["fvar"].axes]
    norm = normalized(out, hbf)
    loc = {t: v / 16384 for t, v in zip(tags, norm) if v != 0}
    order = font.getGlyphOrder()
    hvar = font["HVAR"].table if "HVAR" in font else None
    hvar_inst = VarStoreInstancer(hvar.VarStore, font["fvar"].axes, loc) if hvar else None
    gs = font.getGlyphSet(location=loc, normalized=True) if "glyf" not in font else None
    outlines = []
    for gid, name in enumerate(order):
        lines = []
        if "glyf" in font:
            ph = draw_glyf(font, name, loc, lines)
            exact = ph[1][0] - ph[0][0] if loc else font["hmtx"].metrics[name][0]
        else:
            gs[name].draw(Segments(lines, gs))
            exact = font["hmtx"].metrics[name][0]
        if hvar is not None:
            m = hvar.AdvWidthMap
            varidx = gid if m is None else m.mapping[name]
            exact = font["hmtx"].metrics[name][0] + hvar_inst[varidx]
        check_against_hb(hbf, gid, lines, what)
        adv = hbf.get_glyph_h_advance(gid)
        if round_hb(exact) != adv and not (exact < 0 and adv == 0):
            raise SystemExit("%s glyph %d: advance %r, HarfBuzz %d" % (what, gid, exact, adv))
        out.append("advance %d %d %s" % (gid, adv, num(exact)))
        outlines.append("glyph %d" % gid)
        outlines += lines
    if "MVAR" in font:
        mvar = font["MVAR"].table
        inst = VarStoreInstancer(mvar.VarStore, font["fvar"].axes, loc)
        for r in mvar.ValueRecord:
            d = inst[r.VarIdx]
            theirs = hbf.get_metric_variation(hb.OTMetricsTag(int.from_bytes(r.ValueTag.encode(), "big")))
            if abs(d - theirs) > 0.01:
                raise SystemExit("%s MVAR %s: %r, HarfBuzz %r" % (what, r.ValueTag, d, theirs))
            out.append("metric %s %s" % (r.ValueTag, num(d)))
    out += outlines
    out.append("end")


def main():
    for fname, locations in FONTS:
        p = os.path.join(DATA, fname)
        data = open(p, "rb").read()
        font = TTFont(p)
        fvar = font["fvar"]
        out = ["axes %d" % len(fvar.axes)]
        for a in fvar.axes:
            out.append("axis %s %s %s %s %d %d" % (a.axisTag, num(a.minValue), num(a.defaultValue),
                                                    num(a.maxValue), a.flags, a.axisNameID))
        out.append("instances %d" % len(fvar.instances))
        for ins in fvar.instances:
            ps = getattr(ins, "postscriptNameID", 0xFFFF)
            out.append("instance %d %d %d %s" % (ins.subfamilyNameID, -1 if ps == 0xFFFF else ps, ins.flags,
                                                 " ".join(num(ins.coordinates[a.axisTag]) for a in fvar.axes)))
        for loc in locations:
            out.append("location %d" % len(loc))
            for t, v in loc.items():
                out.append("set %s %s" % (t, num(v)))
            hbf = hb_font(data)
            hbf.set_variations(loc)
            instance(out, font, data, hbf, "%s %r" % (fname, loc))
        for i in range(len(fvar.instances)):
            out.append("named %d" % i)
            hbf = hb_font(data)
            hbf.var_named_instance = i
            normalized(out, hbf)
        with open(p + ".var.ref", "w") as fh:
            fh.write("\n".join(out) + "\n")
        print(fname + ".var.ref", len(out), "lines")


if __name__ == "__main__":
    main()
