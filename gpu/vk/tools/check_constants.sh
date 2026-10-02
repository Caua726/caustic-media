#!/bin/sh
# gpu/vk/tools/check_constants.sh — the numbers in bind/ are the registry's.
#
# Thousands of constants transcribed by hand, every one of which is silently
# wrong if mistyped. A wrong VkStructureType does not fail to compile and does
# not crash: the driver reads the tag, does not recognise it, and rejects the
# call — or worse, recognises it as a different struct and reads the rest of the
# bytes as that one.
#
# Why a script rather than assertions in a .cst test: an assertion would be
#
#     chk_i("VK_STRUCTURE_TYPE_APPLICATION_INFO", vk.VK_STRUCTURE_TYPE_APPLICATION_INFO, 0)
#
# where both zeros were typed by the same person reading the same registry. It
# proves nothing. The value has to come from the registry to mean anything, so
# the comparison is against tools/enums.txt, which came out of vk.xml.
#
# --- Two differences from the X11 version of this script ---
#
# Vulkan enumerants are conventionally hexadecimal and thirteen groups are 64
# bits wide, so both sides are normalised to decimal before comparing rather
# than matched as text.
#
# And "in the registry but not in bind/" is REPORTED, not failed. The binding
# lands over several milestones; a partial count is the honest state during
# them, and the day it should become a failure is the day the last group is
# transcribed. Both other comparisons are hard failures from the first line.
#
# Exit code is the number of hard failures.
set -u

# join and comm assume their input is sorted the way THEY collate, which is C.
# Under a UTF-8 locale sort orders underscores differently and join stops with
# "input is not sorted" partway through — having already emitted a partial,
# wrong answer. Every set comparison below depends on this line.
LC_ALL=C
export LC_ALL

cd "$(dirname "$0")/.." || exit 1

TABLE=tools/enums.txt
TMP="${TMPDIR:-/tmp}/vk_const_$$"
mkdir -p "$TMP" || exit 1
trap 'rm -rf "$TMP"' EXIT INT TERM

[ -f "$TABLE" ] || {
    echo "check_constants: $TABLE nao existe — rode tools/vk_tables.py"
    exit 1
}

fails=0

# Hex, decimal and negatives all reach this as text. awk has no portable
# strtonum, so the conversion is spelled out; without it 0x10 and 16 compare
# unequal and every bitmask in the binding reads as a mismatch.
#
# The whole program lives in the file, main rule included: `awk -f prog '{...}'`
# does not take a second program, it treats the braces as an input FILENAME, and
# the result is a silent empty set rather than an error.
cat > "$TMP/todec.awk" <<'AWK'
function todec(s,   neg, i, c, v, d) {
    neg = 0
    if (substr(s, 1, 1) == "-") { neg = 1; s = substr(s, 2) }
    if (substr(s, 1, 2) == "0x" || substr(s, 1, 2) == "0X") {
        s = substr(s, 3); v = 0
        for (i = 1; i <= length(s); i++) {
            c = tolower(substr(s, i, 1))
            d = index("0123456789abcdef", c) - 1
            if (d < 0) return "?"
            v = v * 16 + d
        }
    } else {
        v = s + 0
    }
    return neg ? -v : v
}
{ print $(nf), todec($(vf)) }
AWK

# What the registry says. enums.txt is `<bits> <name> <value>`.
grep -v '^#' "$TABLE" | grep -v '^[[:space:]]*$' \
  | awk -v nf=2 -v vf=3 -f "$TMP/todec.awk" | sort > "$TMP/registry"

# What bind/ declares. Only `with imut` globals count, and both i32 and i64
# because the 64-bit flag groups cannot fit the first.
sed -n 's/^let is \(i32\|i64\) as \(VK_[A-Za-z0-9_]*\)[[:space:]]*with imut = \(-\{0,1\}[0-9A-Fa-fxX]*\);.*/\2 \3/p' \
    bind/*.cst 2>/dev/null \
  | awk -v nf=1 -v vf=2 -f "$TMP/todec.awk" | sort > "$TMP/declared"

# --- Values, for the names both sides have ---

join "$TMP/declared" "$TMP/registry" | awk '$2 != $3 {
    printf "FAIL  %s: bind/ diz %s, o registry diz %s\n", $1, $2, $3; n++
} END { exit n+0 }'
fails=$((fails + $?))

# --- Declared but not in the registry ---

join -v1 "$TMP/declared" "$TMP/registry" > "$TMP/invented"
if [ -s "$TMP/invented" ]; then
    echo "FAIL  declarado em bind/ mas ausente do registry:"
    awk '{print "        " $1}' "$TMP/invented"
    fails=$((fails + $(wc -l < "$TMP/invented")))
fi

# --- Coverage, reported ---

d=$(wc -l < "$TMP/declared")
r=$(wc -l < "$TMP/registry")
if [ "$fails" -eq 0 ]; then
    if [ "$d" -eq "$r" ]; then
        echo "check_constants: $d/$r conferidos contra o registry, ok"
    else
        echo "check_constants: $d/$r conferidos contra o registry, ok"
        echo "                 (faltam $((r - d)) — o binding ainda esta sendo escrito)"
    fi
fi
exit "$fails"
