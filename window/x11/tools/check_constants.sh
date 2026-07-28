#!/bin/sh
# window/x11/tools/check_constants.sh — the numbers in bind/x.cst are X.h's.
#
# 348 constants transcribed by hand, every one of which is silently wrong if
# mistyped: a wrong event mask selects the wrong events, a wrong error code
# misreports a failure, a wrong ZPixmap makes the server read a framebuffer as
# bitplanes. None of that produces a diagnostic.
#
# Why a script rather than assertions in x11_layout_test.cst, which is where the
# struct offsets are checked: an offset assertion is not tautological, because
# the compiler *computes* the layout from the _pad fields and the test compares
# that computation against C's. A constant assertion would be
#
#     chk_i("KeyPress", x.KeyPress, 2)
#
# where both 2s were typed by the same person reading the same header. It proves
# nothing. The value has to come from the header to mean anything, so the
# comparison is against tools/layout.txt, which came out of the C compiler.
#
# Coverage is checked in both directions, and both are hard failures: a constant
# in X.h that bind/x.cst never declared is a gap, and one bind/x.cst declares
# that X.h does not have is an invention.
#
# Exit code is the number of failures.
set -u

cd "$(dirname "$0")/.." || exit 1

LAYOUT=tools/layout.txt
# Two files, because the constants come from two headers. X.h is the protocol
# and bind/x.cst carries it; Xutil.h is the conventions layered on top, and the
# XSizeHints flags live in bind/xutil.cst beside the struct they are the flags
# word of. Both are checked as one set, so a constant declared in neither is
# still a gap and one declared in either is still checked against the header.
SRCS="bind/x.cst bind/xutil.cst"
TMP="${TMPDIR:-/tmp}/x11_const_$$"
mkdir -p "$TMP" || exit 1
trap 'rm -rf "$TMP"' EXIT INT TERM

[ -f "$LAYOUT" ] || { echo "check_constants: $LAYOUT nao existe"; exit 1; }
for f in $SRCS; do
    [ -f "$f" ] || { echo "check_constants: $f nao existe"; exit 1; }
done

fails=0

# What the header says, via the C compiler.
awk '$1=="const"{print $2, $3}' "$LAYOUT" | sort > "$TMP/header"

# What bind/x.cst declares. Only `with imut` globals count; anything else in
# that file would not be a constant.
sed -n 's/^let is \(i32\|i64\) as \([A-Za-z_][A-Za-z0-9_]*\)[[:space:]]*with imut = \([0-9][0-9]*\);.*/\2 \3/p' \
    $SRCS | sort > "$TMP/declared"

# Values, for the names both sides have.
join "$TMP/declared" "$TMP/header" | awk '$2 != $3 {
    printf "FAIL  %s: bind/ diz %s, o header diz %s\n", $1, $2, $3; n++
} END { exit n+0 }'
fails=$((fails + $?))

# Coverage, both ways.
join -v1 "$TMP/declared" "$TMP/header" > "$TMP/invented"
if [ -s "$TMP/invented" ]; then
    echo "FAIL  declarado em bind/ mas ausente dos headers:"
    awk '{print "        " $1}' "$TMP/invented"
    fails=$((fails + $(wc -l < "$TMP/invented")))
fi

join -v2 "$TMP/declared" "$TMP/header" > "$TMP/missing"
if [ -s "$TMP/missing" ]; then
    echo "FAIL  no header mas ausente de bind/:"
    awk '{print "        " $1}' "$TMP/missing"
    fails=$((fails + $(wc -l < "$TMP/missing")))
fi

n=$(wc -l < "$TMP/declared")
if [ "$fails" -eq 0 ]; then
    echo "check_constants: $n/$n conferidos contra X.h e Xutil.h, ok"
fi
exit "$fails"
