#!/usr/bin/env python3
# text/tools/make_use_table.py — text/shape/use_table.cst: each character's
# category under the Universal Shaping Engine, as Microsoft's specification of it
# derives them from Unicode's properties, and the vowel sequences Unicode and
# Microsoft say not to write.
#
# From Unicode 16.0's IndicSyllabicCategory.txt, IndicPositionalCategory.txt,
# DerivedJoiningType.txt, DerivedCoreProperties.txt, UnicodeData.txt and
# Scripts.txt, and Microsoft's own additions to the first two and list of invalid
# clusters (as kept in HarfBuzz's src/ms-use), downloaded once into
# ~/.cache/caustic-media/registry.
#
#   python3 text/tools/make_use_table.py
import os
import re
import subprocess

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "..", "shape", "use_table.cst")
CACHE = os.path.expanduser("~/.cache/caustic-media/registry")
UCD = "https://www.unicode.org/Public/16.0.0/ucd/"
MS = "https://raw.githubusercontent.com/harfbuzz/harfbuzz/main/src/ms-use/"
SOURCES = {
    "IndicSyllabicCategory.txt": UCD + "IndicSyllabicCategory.txt",
    "IndicPositionalCategory.txt": UCD + "IndicPositionalCategory.txt",
    "DerivedJoiningType.txt": UCD + "extracted/DerivedJoiningType.txt",
    "DerivedCoreProperties.txt": UCD + "DerivedCoreProperties.txt",
    "UnicodeData.txt": UCD + "UnicodeData.txt",
    "Scripts.txt": UCD + "Scripts.txt",
    "IndicSyllabicCategory-Additional.txt": MS + "IndicSyllabicCategory-Additional.txt",
    "IndicPositionalCategory-Additional.txt": MS + "IndicPositionalCategory-Additional.txt",
    "IndicShapingInvalidCluster.txt": MS + "IndicShapingInvalidCluster.txt",
}


def source(name):
    path = os.path.join(CACHE, name)
    if not os.path.exists(path):
        os.makedirs(CACHE, exist_ok=True)
        subprocess.run(["curl", "-fsSL", SOURCES[name], "-o", path], check=True)
    with open(path, encoding="utf-8") as fh:
        return fh.read()


def ranges(text, want=None):
    out = {}
    for line in text.splitlines():
        line = line.split("#")[0].strip()
        if not line:
            continue
        cps, value = [p.strip() for p in line.split(";")[:2]]
        if want is not None and value != want:
            continue
        if ".." in cps:
            a, b = cps.split("..")
        else:
            a = b = cps
        for u in range(int(a, 16), int(b, 16) + 1):
            out[u] = value
    return out


def general_categories():
    gc = {}
    prev = None
    for line in source("UnicodeData.txt").splitlines():
        f = line.split(";")
        u = int(f[0], 16)
        if f[1].endswith(", Last>"):
            for k in range(prev, u + 1):
                gc[k] = f[2]
        else:
            gc[u] = f[2]
        prev = u
    return gc


GC = general_categories()
ISC = ranges(source("IndicSyllabicCategory.txt"))
ISC.update(ranges(source("IndicSyllabicCategory-Additional.txt")))
IPC = ranges(source("IndicPositionalCategory.txt"))
IPC.update(ranges(source("IndicPositionalCategory-Additional.txt")))
JT = ranges(source("DerivedJoiningType.txt"))
DI = set(ranges(source("DerivedCoreProperties.txt"), "Default_Ignorable_Code_Point"))
SCRIPT = ranges(source("Scripts.txt"))
# Scripts shaped some other way: nothing in them is ever asked about.
OTHER_SHAPERS = {"Arabic", "Lao", "Samaritan", "Syriac", "Thai"}
OTHER_UDI = {0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3}
PLACEHOLDERS = {0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE}


def category(u):
    """The specification's category, from its table of properties: exactly one."""
    isc = ISC.get(u, "Other")
    gc = GC.get(u, "Cn")
    udi = u in DI
    jt = JT.get(u, "U")
    if 0x1CE2 <= u <= 0x1CE8:
        isc = "Cantillation_Mark"
    if 0x0F18 <= u <= 0x0F19 or 0x0F3E <= u <= 0x0F3F:
        isc = "Vowel_Dependent"
    if u == 0x1CED:
        isc = "Tone_Mark"
    is_base = (isc in ("Number", "Consonant", "Consonant_Head_Letter", "Tone_Letter", "Vowel_Independent") or
               (jt in ("C", "D", "L", "R") and isc != "Joiner") or
               (gc == "Lo" and isc in ("Avagraha", "Bindu", "Consonant_Final", "Consonant_Medial",
                                       "Consonant_Subjoined", "Vowel", "Vowel_Dependent")))
    is_base_other = isc == "Consonant_Placeholder" or u in PLACEHOLDERS
    is_cgj = isc == "Joiner" or (udi and gc in ("Mc", "Me", "Mn"))
    is_sym_mod = isc == "Symbol_Modifier"
    is_wj = (udi and u not in OTHER_UDI and isc == "Other" and not is_cgj) or gc == "Cn"
    found = []
    if is_base: found.append("B")
    if isc == "Brahmi_Joining_Number": found.append("N")
    if is_base_other: found.append("GB")
    if is_cgj: found.append("CGJ")
    if (isc == "Consonant_Final" and gc != "Lo") or isc == "Consonant_Succeeding_Repha": found.append("F")
    if isc in ("Syllable_Modifier", "Consonant_Final_Modifier"): found.append("FM")
    if (isc == "Consonant_Medial" and gc != "Lo") or isc == "Consonant_Initial_Postfixed": found.append("M")
    if isc in ("Nukta", "Gemination_Mark", "Consonant_Killer"): found.append("CM")
    if isc == "Consonant_Subjoined" and gc != "Lo": found.append("SUB")
    if isc == "Consonant_With_Stacker": found.append("CS")
    if isc == "Virama" and u != 0x0DCA: found.append("H")
    if u == 0x0DCA: found.append("HVM")
    if isc == "Number_Joiner": found.append("HN")
    if isc == "Hieroglyph": found.append("G")
    if isc == "Hieroglyph_Joiner": found.append("J")
    if isc == "Hieroglyph_Mirror": found.append("HR")
    if isc == "Hieroglyph_Modifier": found.append("HM")
    if isc in ("Hieroglyph_Mark_Begin", "Hieroglyph_Segment_Begin"): found.append("SB")
    if isc in ("Hieroglyph_Mark_End", "Hieroglyph_Segment_End"): found.append("SE")
    if isc == "Invisible_Stacker" and u != 0x1A60: found.append("IS")
    if isc == "Non_Joiner": found.append("ZWNJ")
    if ((gc == "Po" or isc in ("Consonant_Dead", "Joiner", "Modifying_Letter", "Other")) and
            not is_base and not is_base_other and not is_cgj and not is_sym_mod and not is_wj):
        found.append("O")
    if isc == "Reordering_Killer": found.append("RK")
    if isc in ("Consonant_Preceding_Repha", "Consonant_Prefixed"): found.append("R")
    if u == 0x1A60: found.append("Sk")
    if is_sym_mod: found.append("SM")
    if isc == "Pure_Killer" or (gc != "Lo" and isc in ("Vowel", "Vowel_Dependent")): found.append("V")
    if isc in ("Tone_Mark", "Cantillation_Mark", "Register_Shifter", "Visarga") or (gc != "Lo" and isc == "Bindu"):
        found.append("VM")
    if is_wj: found.append("WJ")
    assert len(found) == 1, (hex(u), isc, gc, jt, udi, found)
    return found[0]


POSITIONS = {
    "F": {"Abv": ["Top"], "Blw": ["Bottom"], "Pst": ["Right"]},
    "M": {"Abv": ["Top"], "Blw": ["Bottom", "Bottom_And_Left", "Bottom_And_Right"], "Pst": ["Right"],
          "Pre": ["Left", "Top_And_Bottom_And_Left"]},
    "CM": {"Abv": ["Top"], "Blw": ["Bottom", "Overstruck"]},
    "V": {"Abv": ["Top", "Top_And_Bottom", "Top_And_Bottom_And_Right", "Top_And_Right"],
          "Blw": ["Bottom", "Overstruck", "Bottom_And_Right"], "Pst": ["Right"],
          "Pre": ["Left", "Top_And_Left", "Top_And_Bottom_And_Left", "Left_And_Right", "Top_And_Left_And_Right"]},
    "VM": {"Abv": ["Top"], "Blw": ["Bottom", "Overstruck"], "Pst": ["Right"], "Pre": ["Left"]},
    "SM": {"Abv": ["Top"], "Blw": ["Bottom"]},
    "FM": {"Abv": ["Top"], "Blw": ["Bottom"], "Pst": ["NA"]},
}

# Letters of the grammars in use.cst.
LETTERS = {"O": "O", "B": "B", "N": "N", "GB": "g", "SUB": "u", "H": "H", "HN": "n", "ZWNJ": "z", "WJ": "w",
           "R": "R", "CS": "T", "IS": "I", "CGJ": "c", "G": "G", "J": "J", "SB": "s", "SE": "e", "HVM": "V",
           "HM": "m", "HR": "x", "RK": "k", "Sk": "K", "FAbv": "a", "FBlw": "b", "FPst": "f", "MAbv": "d",
           "MBlw": "h", "MPst": "i", "MPre": "j", "CMAbv": "l", "CMBlw": "o", "VAbv": "p", "VBlw": "q",
           "VPst": "r", "VPre": "t", "VMAbv": "v", "VMBlw": "y", "VMPst": "A", "VMPre": "C", "SMAbv": "D",
           "SMBlw": "E", "FMAbv": "F", "FMBlw": "L", "FMPst": "M"}


UNPLACED = []


def use_of(u):
    cat = category(u)
    ipc = IPC.get(u, "NA")
    if u in (0x11302, 0x11303, 0x114C1):
        ipc = "Top"
    mapping = POSITIONS.get(cat)
    if mapping:
        for name, values in mapping.items():
            if ipc in values:
                return cat + name
        # What the specification gives no place: what it has no form of its own for.
        UNPLACED.append("%04X %s %s" % (u, cat, ipc))
        return "O"
    return cat


table = []
for u in sorted(set(ISC) | set(IPC) | {u for u, j in JT.items() if j in "CDLR"} | DI | PLACEHOLDERS):
    if u < 0x80 or SCRIPT.get(u) in OTHER_SHAPERS or u not in GC:
        continue
    table.append((u, LETTERS[use_of(u)] if use_of(u) in LETTERS else "O"))

merged = []
for u, c in table:
    if merged and merged[-1][1] == u - 1 and merged[-1][2] == c:
        merged[-1][1] = u
    elif c != "O":
        merged.append([u, u, c])

# The sequences not to write: the dotted circle goes before their last character.
sequences = []
for line in source("IndicShapingInvalidCluster.txt").splitlines():
    line = line.split("#")[0].strip().rstrip(";").strip()
    if line:
        sequences.append([int(x, 16) for x in line.split()])
sequences.sort()
assert max(len(s) for s in sequences) <= 3

out = ["// text/shape/use_table.cst — generated by text/tools/make_use_table.py from Unicode",
       "// 16.0 and Microsoft's additions for the Universal Shaping Engine. Do not edit.",
       "",
       "let is i64 as RANGES_COUNT with imut = %d;" % len(merged),
       "// First and last character, six hexadecimal digits each, and the category's",
       "// letter, sorted; every other character is O or asked for no further.",
       'let is *u8 as RANGES with imut = "%s";' % "".join("%06X%06X%s" % (a, b, c) for a, b, c in merged),
       "",
       "let is i64 as SEQUENCES_COUNT with imut = %d;" % len(sequences),
       "// A sequence not to write: three characters of six digits, 000000 for none, sorted.",
       'let is *u8 as SEQUENCES with imut = "%s";' % "".join("".join("%06X" % c for c in (s + [0, 0, 0])[:3]) for s in sequences),
       ""]
with open(OUT, "w", encoding="utf-8") as fh:
    fh.write("\n".join(out))
print(os.path.basename(OUT), len(merged), "ranges", len(sequences), "sequences")
print("unplaced:", sorted(set(UNPLACED)))
