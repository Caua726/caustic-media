#!/bin/sh
# window/x11/tools/check_symbols.sh — keeps "all 774 of libX11" true over time.
#
# Three sets, compared against each other:
#
#   DECLARED   the extern names in bind/*.cst
#   MANIFEST   symbols.txt, the contract
#   EXPORTED   what the shared objects on this machine actually export
#
# Two of the comparisons are hard failures and one is a warning, and the split
# is deliberate:
#
#   DECLARED not in MANIFEST   fail. A binding drifted from the contract, or a
#                              name is misspelled in a way the link test would
#                              only catch after a much slower build.
#   MANIFEST not in EXPORTED   fail. The contract promises something this
#                              machine's libX11 cannot provide, so the link
#                              test cannot pass.
#   EXPORTED not in MANIFEST   warn. CI runs Ubuntu's libX11 and a developer may
#                              run Arch's; the two legitimately differ. The
#                              manifest is the contract, not the machine.
#
# Coverage is reported rather than enforced, because the bindings land over
# several milestones and a partial count is the honest state during them.
#
# Exit code is the number of hard failures.
set -u

cd "$(dirname "$0")/.." || exit 1

MANIFEST=tools/symbols.txt
TMP="${TMPDIR:-/tmp}/x11_check_$$"
mkdir -p "$TMP" || exit 1
trap 'rm -rf "$TMP"' EXIT INT TERM

[ -f "$MANIFEST" ] || { echo "check_symbols: $MANIFEST nao existe"; exit 1; }

fails=0

# --- The three sets ---

# An extern line is `extern "<soname>" fn <Name>(`. Nothing else in these files
# introduces a foreign symbol, so this is the whole declared set.
grep -h '^extern "' bind/*.cst 2>/dev/null \
  | sed -n 's/^extern "\([^"]*\)" fn \([A-Za-z_][A-Za-z0-9_]*\).*/\1 \2/p' \
  | sort -u > "$TMP/declared"

grep -v '^#' "$MANIFEST" | grep -v '^[[:space:]]*$' | sort -u > "$TMP/manifest"

: > "$TMP/exported"
for so in $(awk '{print $1}' "$TMP/manifest" | sort -u); do
    path=""
    for dir in /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu /lib/x86_64-linux-gnu; do
        [ -f "$dir/$so" ] && { path="$dir/$so"; break; }
    done
    if [ -z "$path" ]; then
        echo "FAIL  $so nao encontrado — o link test nao pode rodar aqui"
        fails=$((fails + 1))
        continue
    fi
    nm -D --defined-only "$path" 2>/dev/null \
      | awk -v s="$so" '$2=="T"{print s, $3}' >> "$TMP/exported"
done
sort -u "$TMP/exported" -o "$TMP/exported"

# --- The two hard failures ---

comm -23 "$TMP/declared" "$TMP/manifest" > "$TMP/undeclared"
if [ -s "$TMP/undeclared" ]; then
    echo "FAIL  declarado em bind/ mas fora do manifesto:"
    sed 's/^/        /' "$TMP/undeclared"
    fails=$((fails + $(wc -l < "$TMP/undeclared")))
fi

comm -23 "$TMP/manifest" "$TMP/exported" > "$TMP/missing"
if [ -s "$TMP/missing" ]; then
    echo "FAIL  no manifesto mas ausente das .so desta maquina:"
    sed 's/^/        /' "$TMP/missing"
    fails=$((fails + $(wc -l < "$TMP/missing")))
fi

# --- The link test has to cover what is declared ---
#
# A link test only proves a name resolves if it CALLS it: the compiler
# emits the symbol import at the call site, so a declaration the test forgets is
# a declaration nothing checks. That gap opened once already — XDestroyImage was
# added to the binding after the test was written, and check_symbols reported
# 408 declared while the test resolved 407 — so it is asserted here rather than
# left to whoever remembers.

if ls ./*_link_test.cst >/dev/null 2>&1; then
    # Every link test, and any module alias: the bindings are split one file per
    # C header and the tests one binary per shared object, so a symbol may be
    # exercised from any of them.
    grep -hoE '\b[a-z][a-z0-9_]*\.[A-Za-z_][A-Za-z0-9_]*\(' ./*_link_test.cst \
      | sed 's/^[a-z][a-z0-9_]*\.//; s/(//' | sort -u > "$TMP/touched"
    awk '{print $2}' "$TMP/declared" | sort -u > "$TMP/decl_all"
    comm -23 "$TMP/decl_all" "$TMP/touched" > "$TMP/untouched"
    if [ -s "$TMP/untouched" ]; then
        echo "FAIL  declarado mas nao exercitado pelo link test:"
        sed 's/^/        /' "$TMP/untouched"
        fails=$((fails + $(wc -l < "$TMP/untouched")))
    fi
fi

# --- The warning ---

comm -13 "$TMP/manifest" "$TMP/exported" > "$TMP/extra"
if [ -s "$TMP/extra" ]; then
    echo "aviso as .so locais exportam $(wc -l < "$TMP/extra") simbolo(s) fora do"
    echo "      manifesto — outra distro ou outra versao. O contrato e o manifesto."
fi

# --- Coverage, reported not enforced ---

echo
printf '%-18s %8s %8s\n' soname declarado manifesto
awk '{print $1}' "$TMP/manifest" | sort -u | while read -r so; do
    m=$(awk -v s="$so" '$1==s' "$TMP/manifest" | wc -l)
    d=$(awk -v s="$so" '$1==s' "$TMP/declared" | wc -l)
    printf '%-18s %8s %8s\n' "$so" "$d" "$m"
done
printf '%-18s %8s %8s\n' TOTAL "$(wc -l < "$TMP/declared")" "$(wc -l < "$TMP/manifest")"

if [ "$fails" -eq 0 ]; then echo; echo "check_symbols: ok"; fi
exit "$fails"
