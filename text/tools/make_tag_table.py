#!/usr/bin/env python3
# text/tools/make_tag_table.py — HarfBuzz's table of BCP 47 languages to
# OpenType language systems, made Caustic: text/shape/languages.cst.
#
# Development tool, not part of the build. HarfBuzz generates hb-ot-tag-table.hh
# from the IANA language subtag registry and Microsoft's list of language
# system tags; this reads that file, as HarfBuzz 14.5.0 ships it, and writes
# the same tables and the same function, so a language finds here the
# language systems it finds in HarfBuzz. Its output is versioned.
#
#   python3 text/tools/make_tag_table.py [path/to/hb-ot-tag-table.hh]
#
# The tables are text, a tag being four ASCII characters: two-letter and
# three-letter languages with their tags, eight characters an entry; the
# three-letter codes that are not the tag they spell; and the languages with
# several tags, eight characters each — the language, the first of its tags
# among the values as three digits, how many as one.
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "..", "shape", "languages.cst")
DEFAULT = os.path.expanduser("~/.cache/caustic-media/hb-14.5.0/src/hb-ot-tag-table.hh")

TAG = r"HB_TAG\('(.)','(.)','(.)','(.)'\)"


def tag(m, i=0):
    return "".join(m.group(i + k) for k in range(1, 5))


def block(src, name):
    start = src.index(name)
    return src[start:src.index("};", start)]


def pairs(text):
    return [(tag(m), tag(m, 4)) for m in re.finditer(r"\{" + TAG + r",\s*" + TAG + r"\}", text)]


def tags(text):
    return [tag(m) for m in re.finditer(TAG, text)]


def cstr(s):
    assert all(32 <= ord(c) < 127 and c not in '"\\' for c in s), s
    return '"' + s + '"'


def tag_int(t):
    return "%d" % ((ord(t[0]) << 24) | (ord(t[1]) << 16) | (ord(t[2]) << 8) | ord(t[3]))


# --- The function for languages of several subtags ---

def body_tags(body):
    found = tags(body)
    assert found, body
    return found


def parse_rules(text):
    # Each rule: a condition and the tags it gives. The prelude's rules look
    # at subtags after the first; the rest are under the first letter.
    rules = []   # (letter or None, condition, tags)
    pre_start = text.index("if (limit - lang_str >= 7)")
    out = text.index("out:")
    pre = text[pre_start:out]
    for m in re.finditer(r'if \(subtag_matches \(p, limit, "([^"]+)", (\d+)\)\)\s*\{(.*?)return true;', pre, re.S):
        assert len(m.group(1)) == int(m.group(2))
        rules.append((None, ("subtag", m.group(1)), body_tags(m.group(3))))
    rest = text[out:]
    for case in re.finditer(r"case '(.)':(.*?)break;", rest, re.S):
        letter = case.group(1)
        for m in re.finditer(r"if \((.*?)\)\s*\{(.*?)return true;", case.group(2), re.S):
            cond = " ".join(m.group(1).split())
            c = re.fullmatch(r'0 == strcmp \(&lang_str\[1\], "([^"]+)"\)', cond)
            if c:
                rules.append((letter, ("strcmp", c.group(1)), body_tags(m.group(2))))
                continue
            c = re.fullmatch(r'lang_matches \(&lang_str\[1\], limit, "([^"]+)", (\d+)\)', cond)
            if c:
                assert len(c.group(1)) == int(c.group(2))
                rules.append((letter, ("lang", c.group(1)), body_tags(m.group(2))))
                continue
            c = re.fullmatch(r'0 == strncmp \(&lang_str\[1\], "([^"]+)", (\d+)\) && subtag_matches \(lang_str, limit, "([^"]+)", (\d+)\)', cond)
            if c:
                assert len(c.group(1)) == int(c.group(2)) and len(c.group(3)) == int(c.group(4))
                rules.append((letter, ("prefix", c.group(1), c.group(3)), body_tags(m.group(2))))
                continue
            sys.exit("make_tag_table: a condition not understood: " + cond)
    # Every condition read: count them against the source.
    assert len(rules) == text.count("return true;"), (len(rules), text.count("return true;"))
    return rules


def emit_rule(cond, result, indent):
    if cond[0] == "subtag":
        test = "_subtag(s, p, limit, %s) == 1" % cstr(cond[1])
    elif cond[0] == "strcmp":
        test = "_rest_is(s, %s) == 1" % cstr(cond[1])
    elif cond[0] == "lang":
        test = "_lang_matches(s, 1, limit, %s) == 1" % cstr(cond[1])
    else:
        test = "_starts(s, 1, %s) == 1 && _subtag(s, 0, limit, %s) == 1" % (cstr(cond[1]), cstr(cond[2]))
    out = [indent + "if (%s) {" % test]
    if len(result) == 1:
        out.append(indent + "    return _one(count, tags, %s);  // %s" % (tag_int(result[0]), result[0].rstrip()))
    else:
        assert len(result) <= 3, result
        args = list(result) + ["    "] * (3 - len(result))
        out.append(indent + "    return _some(count, tags, %d, %s, %s, %s);  // %s" %
                   (len(result), tag_int(args[0]), tag_int(args[1]) if len(result) > 1 else "0",
                    tag_int(args[2]) if len(result) > 2 else "0", ", ".join(r.rstrip() for r in result)))
    out.append(indent + "}")
    return out


HELPERS = '''
// --- What the generated rules compare ---

fn _len(s as *u8) as i64 {
    let is i64 as n with mut = 0;
    while (s[n] != 0) { n = n + 1; }
    return n;
}

fn _alnum(c as i64) as i32 {
    if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)) { return 1; }
    return 0;
}

// Where sub first occurs in s at or after from; -1 for nowhere.
fn _find(s as *u8, from as i64, sub as *u8) as i64 {
    let is i64 as n = _len(sub);
    let is i64 as i with mut = from;
    while (s[i] != 0) {
        let is i64 as k with mut = 0;
        while (k < n && s[i + k] == sub[k]) { k = k + 1; }
        if (k == n) { return i; }
        i = i + 1;
    }
    return 0 - 1;
}

// HarfBuzz's subtag_matches: sub occurs, as a whole subtag, from at, before
// limit.
fn _subtag(s as *u8, at as i64, limit as i64, sub as *u8) as i32 {
    let is i64 as n = _len(sub);
    if (limit - at < n) { return 0; }
    let is i64 as from with mut = at;
    while (1 == 1) {
        let is i64 as f = _find(s, from, sub);
        if (f < 0 || f >= limit) { return 0; }
        if (_alnum(cast(i64, s[f + n])) == 0) { return 1; }
        from = f + n;
    }
    return 0;
}

// HarfBuzz's lang_matches: s from at begins with spec, which ends there or
// at a hyphen.
fn _lang_matches(s as *u8, at as i64, limit as i64, spec as *u8) as i32 {
    let is i64 as n = _len(spec);
    if (limit - at < n) { return 0; }
    if (_starts(s, at, spec) == 0) { return 0; }
    if (s[at + n] == 0 || s[at + n] == 45) { return 1; }
    return 0;
}

fn _starts(s as *u8, at as i64, prefix as *u8) as i32 {
    let is i64 as k with mut = 0;
    while (prefix[k] != 0) {
        if (s[at + k] != prefix[k]) { return 0; }
        k = k + 1;
    }
    return 1;
}

// All of s after its first letter is rest.
fn _rest_is(s as *u8, rest as *u8) as i32 {
    if (_starts(s, 1, rest) == 0) { return 0; }
    if (s[1 + _len(rest)] == 0) { return 1; }
    return 0;
}

fn _one(count as *i64, tags as *i64, t as i64) as i32 {
    tags[0] = t;
    *count = 1;
    return 1;
}

fn _some(count as *i64, tags as *i64, n as i64, a as i64, b as i64, c as i64) as i32 {
    let is i64 as i with mut = 0;
    while (i < n && i < *count) {
        if (i == 0) { tags[i] = a; } else if (i == 1) { tags[i] = b; } else { tags[i] = c; }
        i = i + 1;
    }
    *count = i;
    return 1;
}
'''


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else DEFAULT
    src = open(path, encoding="utf-8").read()
    header = re.search(r"on files with these headers:\n \*\n((?: \* .*\n)+)", src).group(1)
    l2 = pairs(block(src, "ot_languages2[]"))
    blocked = tags(block(src, "ot_languages3_blocked[]"))
    l3 = pairs(block(src, "ot_languages3[]"))
    values = tags(block(src, "ot_languages3_multi_values[]"))
    multi = [(tag(m), int(m.group(5)), int(m.group(6))) for m in
             re.finditer(r"\{" + TAG + r",\s*(\d+),\s*(\d+)\}", block(src, "ot_languages3_multi[]"))]
    for table in (l2, l3, multi):
        keys = [e[0] for e in table]
        assert keys == sorted(keys), "a table out of order"
    assert blocked == sorted(blocked)
    assert all(off < 1000 and 0 < n < 10 and off + n <= len(values) for _, off, n in multi)
    fn_start = src.index("hb_ot_tags_from_complex_language (const char")
    fn = src[fn_start:src.index("\n}\n", fn_start)]
    rules = parse_rules(fn)

    out = []
    out.append("// text/shape/languages.cst — BCP 47 languages to OpenType language systems,")
    out.append("// HarfBuzz's table. Generated by text/tools/make_tag_table.py from HarfBuzz")
    out.append("// 14.5.0's hb-ot-tag-table.hh, which was made from files with these headers:")
    out.append("//")
    for line in header.splitlines():
        out.append("// " + line[3:].replace("<meta", "meta").replace(" />", ""))
    out.append("//")
    out.append("// Do not edit; see tag.cst for how the tables are read.")
    out.append("")
    out.append("// Two-letter languages and their tags, a language once per tag, in order.")
    out.append("let is i64 as LANG2_N with imut = %d;" % len(l2))
    out.append("let is *u8 as LANG2 with imut = %s;" % cstr("".join(a + b for a, b in l2)))
    out.append("")
    out.append("// Three-letter languages with one tag.")
    out.append("let is i64 as LANG3_N with imut = %d;" % len(l3))
    out.append("let is *u8 as LANG3 with imut = %s;" % cstr("".join(a + b for a, b in l3)))
    out.append("")
    out.append("// Three-letter languages with several tags: the language, where its tags")
    out.append("// start among MULTI_VALUES, how many.")
    out.append("let is i64 as MULTI_N with imut = %d;" % len(multi))
    out.append("let is *u8 as MULTI with imut = %s;" % cstr("".join("%s%03d%d" % e for e in multi)))
    out.append("let is *u8 as MULTI_VALUES with imut = %s;" % cstr("".join(values)))
    out.append("")
    out.append("// Three-letter codes that are not the language system their letters spell.")
    out.append("let is i64 as BLOCKED_N with imut = %d;" % len(blocked))
    out.append("let is *u8 as BLOCKED with imut = %s;" % cstr("".join(blocked)))
    out.append(HELPERS)
    out.append("// HarfBuzz's hb_ot_tags_from_complex_language: the languages a tag of")
    out.append("// several subtags names — a script, a region, a variant — in s, the")
    out.append("// language lowercased, up to limit. Into tags, at most *count of them;")
    out.append("// 0 when none of its rules applies.")
    out.append("fn complex(s as *u8, limit as i64, count as *i64, tags as *i64) as i32 {")
    out.append("    if (limit >= 7) {")
    out.append("        let is i64 as p = _find(s, 0, \"-\");")
    out.append("        if (p >= 0 && p < limit && limit - p >= 5) {")
    for letter, cond, result in rules:
        if letter is None:
            out += emit_rule(cond, result, "            ")
    out.append("        }")
    out.append("    }")
    out.append("    let is i64 as c = cast(i64, s[0]);")
    letters = []
    for letter, _, _ in rules:
        if letter is not None and letter not in letters:
            letters.append(letter)
    for letter in letters:
        out.append("    if (c == %d) {  // '%s'" % (ord(letter), letter))
        for l, cond, result in rules:
            if l == letter:
                out += emit_rule(cond, result, "        ")
        out.append("        return 0;")
        out.append("    }")
    out.append("    return 0;")
    out.append("}")
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("languages.cst: %d two-letter, %d three-letter, %d with several tags, %d blocked, %d rules" %
          (len(l2), len(l3), len(multi), len(blocked), len(rules)))


if __name__ == "__main__":
    main()
