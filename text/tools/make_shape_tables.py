#!/usr/bin/env python3
# text/tools/make_shape_tables.py — text/shape/tables.cst, the data the shaper
# needs from outside: which scripts are written right to left, and which
# OpenType language system tags a language has.
#
# From the primary sources, downloaded once into ~/.cache/caustic-media/registry:
#   - Unicode 16.0's Scripts.txt, DerivedBidiClass.txt, DerivedGeneralCategory.txt
#     and PropertyValueAliases.txt: a script is right to left when most of its
#     letters are of bidi class R or AL; DerivedCoreProperties.txt's
#     Default_Ignorable_Code_Point, the characters that draw nothing.
#   - Microsoft's OpenType Language System Tags registry: each tag with the
#     ISO 639 codes it stands for.
#   - SIL's ISO 639-3 code table (for the two-letter ISO 639-1 codes) and its
#     macrolanguage mappings.
#   - caustic-unicode's Script values (its src/ucd/tables/script.cst, in
#     .caustic/deps), each made its ISO 15924 code through
#     PropertyValueAliases.txt, for layout to name the scripts it finds.
#
# The output is the same byte for byte each time the sources are the same.
#
#   python3 text/tools/make_shape_tables.py
import collections
import html
import os
import re
import subprocess

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "..", "shape", "tables.cst")
CACHE = os.path.expanduser("~/.cache/caustic-media/registry")

UCD = "https://www.unicode.org/Public/16.0.0/ucd/"
SOURCES = {
    "Scripts.txt": UCD + "Scripts.txt",
    "PropertyValueAliases.txt": UCD + "PropertyValueAliases.txt",
    "DerivedBidiClass.txt": UCD + "extracted/DerivedBidiClass.txt",
    "DerivedGeneralCategory.txt": UCD + "extracted/DerivedGeneralCategory.txt",
    "DerivedCoreProperties.txt": UCD + "DerivedCoreProperties.txt",
    "languagetags.html": "https://learn.microsoft.com/en-us/typography/opentype/spec/languagetags",
    "iso-639-3.tab": "https://iso639-3.sil.org/sites/iso639-3/files/downloads/iso-639-3.tab",
    "iso-639-3-macrolanguages.tab":
        "https://iso639-3.sil.org/sites/iso639-3/files/downloads/iso-639-3-macrolanguages.tab",
}


def source(name):
    path = os.path.join(CACHE, name)
    if not os.path.exists(path):
        os.makedirs(CACHE, exist_ok=True)
        subprocess.run(["curl", "-fsSL", SOURCES[name], "-o", path], check=True)
    with open(path, encoding="utf-8") as fh:
        return fh.read()


def ranges(text):
    """(first, last, value) for each line of a UCD property file."""
    for line in text.splitlines():
        line = line.split("#")[0].strip()
        if not line:
            continue
        cps, value = [p.strip() for p in line.split(";")[:2]]
        if ".." in cps:
            a, b = cps.split("..")
        else:
            a = b = cps
        yield int(a, 16), int(b, 16), value


DEPS = os.path.join(HERE, "..", "..", ".caustic", "deps", "caustic-unicode", "src", "ucd", "tables", "script.cst")


def script_codes():
    """caustic-unicode's script values, in order, as ISO 15924 codes."""
    codes = {}
    for line in source("PropertyValueAliases.txt").splitlines():
        parts = [p.strip() for p in line.split("#")[0].split(";")]
        if len(parts) >= 3 and parts[0] == "sc":
            codes[parts[2]] = parts[1]
    values = {}
    for m in re.finditer(r"let is i64 as SC_(\w+) with imut = (\d+);", open(DEPS).read()):
        values[int(m.group(2))] = m.group(1)
    out = []
    for v in range(max(values) + 1):
        out.append(codes.get(values.get(v, "Unknown"), "Zzzz"))
    return out


def rtl_scripts():
    codes = {}
    for line in source("PropertyValueAliases.txt").splitlines():
        parts = [p.strip() for p in line.split("#")[0].split(";")]
        if len(parts) >= 3 and parts[0] == "sc":
            codes[parts[2]] = parts[1]
    script = {}
    for a, b, v in ranges(source("Scripts.txt")):
        for cp in range(a, b + 1):
            script[cp] = v
    letters = set()
    for a, b, v in ranges(source("DerivedGeneralCategory.txt")):
        if v in ("Lu", "Ll", "Lt", "Lm", "Lo"):
            letters.update(range(a, b + 1))
    votes = collections.defaultdict(lambda: [0, 0])
    for a, b, v in ranges(source("DerivedBidiClass.txt")):
        for cp in range(a, b + 1):
            if cp in letters and cp in script:
                votes[script[cp]][1 if v in ("R", "AL") else 0] += 1
    return sorted(codes[s] for s, (ltr, rtl) in votes.items() if rtl > ltr)


def ignorables():
    """Default_Ignorable_Code_Point as sorted, merged ranges."""
    cps = set()
    for a, b, v in ranges(source("DerivedCoreProperties.txt")):
        if v == "Default_Ignorable_Code_Point":
            cps.update(range(a, b + 1))
    out = []
    for cp in sorted(cps):
        if out and out[-1][1] == cp - 1:
            out[-1][1] = cp
        else:
            out.append([cp, cp])
    return out


def language_tags():
    """ISO 639-3 code -> OpenType tags: the most particular first — a tag
    that stands for fewer languages before one that stands for more (Korean's
    own before Old Hangul's, which is Korean's and Middle Korean's) — then in
    the registry's order; those it marks deprecated after the rest."""
    page = source("languagetags.html")
    current, old = collections.OrderedDict(), collections.OrderedDict()
    names = {}
    for row in re.findall(r"<tr>(.*?)</tr>", page, re.S):
        cells = [html.unescape(re.sub(r"<.*?>", "", c)).strip()
                 for c in re.findall(r"<t[dh][^>]*>(.*?)</t[dh]>", row, re.S)]
        if len(cells) < 3:
            continue
        t = re.match(r"'(.{4})'(.*)", cells[1].replace("\xa0", " "))
        if not t:
            continue
        tag, note = t.group(1), t.group(2)
        names[tag] = cells[0]
        into = old if "deprecated" in note else current
        # The codes come first; a note may follow them, glued on.
        m = re.match(r"\s*([a-z]{3}(?:\s*,\s*[a-z]{3})*)", cells[2])
        if not m:
            continue
        for code in re.findall(r"[a-z]{3}", m.group(1)):
            into.setdefault(code, [])
            if tag not in into[code]:
                into[code].append(tag)
    width = collections.Counter(t for table in (current, old) for ts in table.values() for t in ts)
    tags = collections.OrderedDict()
    for code in sorted(set(current) | set(old)):
        now = sorted(current.get(code, []), key=lambda t: width[t])
        tags[code] = now + [t for t in sorted(old.get(code, []), key=lambda t: width[t]) if t not in now]
    return tags, names


def iso639():
    two, macro, names = {}, [], {}
    rows = source("iso-639-3.tab").splitlines()[1:]
    for line in rows:
        f = line.split("\t")
        names[f[0]] = f[6]
        if f[3]:
            two[f[3]] = f[0]
    for line in source("iso-639-3-macrolanguages.tab").splitlines()[1:]:
        f = line.split("\t")
        if len(f) >= 3 and f[2].strip() == "A":
            macro.append((f[1], f[0]))
    return two, sorted(macro), names


def chunks(s, width=96):
    return [s[i:i + width] for i in range(0, len(s), width)]


def string_constant(name, s, comment):
    lines = ["// " + comment, "let is *u8 as %s with imut =" % name]
    parts = chunks(s)
    if not parts:
        parts = [""]
    # One literal, written over several lines by joining: the compiler takes
    # a long literal, and the pieces keep the file readable.
    body = "".join(parts)
    lines.append('    "%s";' % body)
    return "\n".join(lines)


def main():
    rtl = rtl_scripts()
    sc = script_codes()
    ign = ignorables()
    tags, tag_names = language_tags()
    two, macro, iso_names = iso639()
    # A macrolanguage the registry does not name takes its members' tags —
    # first the one the registry calls by the macrolanguage's own name
    # (Norwegian: Bokmål's), then the rest in the members' order.
    members = collections.OrderedDict()
    for i, m in macro:
        members.setdefault(m, []).append(i)
    for m, ms in sorted(members.items()):
        if m in tags:
            continue
        found = [t for i in ms for t in tags.get(i, [])]
        found = list(collections.OrderedDict.fromkeys(found))
        if found:
            own = [t for t in found if tag_names.get(t) == iso_names.get(m)]
            tags[m] = own + [t for t in found if t not in own]
    tags = collections.OrderedDict(sorted(tags.items()))
    most = max(len(v) for v in tags.values())
    assert most <= 5, most

    for code, t in tags.items():
        assert all(len(x) == 4 for x in t), (code, t)
    langs = "".join(code + str(len(t)) + "".join(t).ljust(20) for code, t in sorted(tags.items()))
    iso1 = "".join(k + v for k, v in sorted(two.items()))
    mac = "".join(i + m for i, m in macro)

    out = [
        "// text/shape/tables.cst — generated by text/tools/make_shape_tables.py from",
        "// Unicode 16.0, Microsoft's OpenType Language System Tags registry and SIL's",
        "// ISO 639-3 tables. Do not edit.",
        "",
        "let is i64 as RTL_COUNT with imut = %d;" % len(rtl),
        string_constant("RTL", "".join(rtl),
                        "Scripts written right to left, by ISO 15924 code, sorted: four letters each."),
        "",
        "let is i64 as SCRIPT_COUNT with imut = %d;" % len(sc),
        string_constant("SCRIPTS", "".join(sc),
                        "caustic-unicode's Script values, from 0, as ISO 15924 codes: four letters each."),
        "",
        "let is i64 as IGNORABLE_COUNT with imut = %d;" % len(ign),
        string_constant("IGNORABLE", "".join("%06X%06X" % (a, b) for a, b in ign),
                        "Default ignorable code points (Default_Ignorable_Code_Point), as ranges:\n"
                        "// first and last, six hexadecimal digits each."),
        "",
        "let is i64 as LANG_COUNT with imut = %d;" % len(tags),
        string_constant("LANGS", langs,
                        "ISO 639-3 codes with OpenType language system tags, sorted: the code, how many\n"
                        "// tags, then five tags of four characters, blank past the count."),
        "",
        "let is i64 as ISO1_COUNT with imut = %d;" % len(two),
        string_constant("ISO1", iso1, "ISO 639-1 codes and their ISO 639-3 code, sorted: five letters each."),
        "",
        "let is i64 as MACRO_COUNT with imut = %d;" % len(macro),
        string_constant("MACRO", mac,
                        "Individual languages and the macrolanguage each belongs to, sorted by the\n"
                        "// individual: six letters each."),
        "",
    ]
    with open(OUT, "w", encoding="utf-8") as fh:
        fh.write("\n".join(out))
    print(os.path.basename(OUT), len(rtl), "scripts,", len(tags), "languages,", len(two), "two-letter codes,",
          len(macro), "macrolanguage members")


if __name__ == "__main__":
    main()
