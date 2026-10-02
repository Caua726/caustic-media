#!/bin/sh
# gpu/vk/tools/check_symbols.sh — keeps "the binding is the registry" true over time.
#
# The X11 version of this script compares three sets and the middle one is what
# the shared object exports. Vulkan cannot be checked that way: libvulkan.so.1
# exports 269 of the 775 commands, and everything else — including the swapchain,
# which is an extension — exists only through vkGetInstanceProcAddr. Asking the
# .so what it has would report five hundred false failures.
#
# So the sets are:
#
#   DECLARED   the command wrappers in bind/*.cst, which is what a program can
#              actually call
#   MANIFEST   symbols.txt, generated from vk.xml — the contract
#   EXTERN     the handful declared as real externs rather than as pointers
#
# and the comparisons:
#
#   DECLARED not in MANIFEST   fail. A name is misspelled, or the binding
#                              invented something the registry does not have.
#                              A misspelling here is not caught anywhere else:
#                              a wrong name resolves to null at load time and
#                              the wrapper calls through a null pointer.
#   EXTERN not exported        fail. A real extern that libvulkan does not
#                              export cannot link, and the binary dies at exec.
#   DECLARED not exercised     fail. Same reason as X11: the link test only
#                              proves a name resolves if it names it.
#
# Coverage is reported rather than enforced, because the binding lands over
# several milestones and a partial count is the honest state during them.
#
# Exit code is the number of hard failures.
set -u

# comm and join assume their input is sorted the way THEY collate, which is C.
# Under a UTF-8 locale sort orders underscores and case differently, and comm
# then reports differences that are only disagreements about order — a set of
# false failures that look exactly like real ones.
LC_ALL=C
export LC_ALL

cd "$(dirname "$0")/.." || exit 1

MANIFEST=tools/symbols.txt
SONAME=libvulkan.so.1
TMP="${TMPDIR:-/tmp}/vk_check_$$"
mkdir -p "$TMP" || exit 1
trap 'rm -rf "$TMP"' EXIT INT TERM

[ -f "$MANIFEST" ] || {
    echo "check_symbols: $MANIFEST nao existe — rode tools/vk_tables.py"
    exit 1
}

fails=0

# --- The three sets ---
#
# A command reaches a program as `fn vkFoo(`, whether it forwards through a
# pointer or is a real extern. Both forms are declared set members; the
# distinction only matters for the export check below.
grep -h '^fn vk' bind/*.cst 2>/dev/null \
  | sed -n 's/^fn \(vk[A-Za-z0-9_]*\).*/\1/p' | sort -u > "$TMP/declared"

grep -h "^extern \"$SONAME\" fn vk" bind/*.cst 2>/dev/null \
  | sed -n 's/^extern "[^"]*" fn \(vk[A-Za-z0-9_]*\).*/\1/p' | sort -u > "$TMP/extern"

grep -v '^#' "$MANIFEST" | grep -v '^[[:space:]]*$' \
  | awk '{print $2}' | sort -u > "$TMP/manifest"

# --- Declared but not in the registry ---

comm -23 "$TMP/declared" "$TMP/manifest" > "$TMP/invented"
if [ -s "$TMP/invented" ]; then
    echo "FAIL  declarado em bind/ mas ausente do registry:"
    sed 's/^/        /' "$TMP/invented"
    fails=$((fails + $(wc -l < "$TMP/invented")))
fi

# --- Every real extern has to be exported ---
#
# The pointer-backed commands are proven at run time by vk_link_test. These are
# the only ones the loader has to resolve at exec, so they are the only ones
# whose absence kills the process before main.

if [ -s "$TMP/extern" ]; then
    path=""
    for dir in /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu /lib/x86_64-linux-gnu; do
        [ -f "$dir/$SONAME" ] && { path="$dir/$SONAME"; break; }
    done
    if [ -z "$path" ]; then
        echo "FAIL  $SONAME nao encontrado — o link test nao pode rodar aqui"
        fails=$((fails + 1))
    else
        nm -D --defined-only "$path" 2>/dev/null | awk '$2=="T"{print $3}' \
          | sort -u > "$TMP/exported"
        comm -23 "$TMP/extern" "$TMP/exported" > "$TMP/unexported"
        if [ -s "$TMP/unexported" ]; then
            echo "FAIL  declarado como extern mas $SONAME nao exporta:"
            sed 's/^/        /' "$TMP/unexported"
            echo "      (esses tem que virar ponteiro preenchido por load_*)"
            fails=$((fails + $(wc -l < "$TMP/unexported")))
        fi
    fi
fi

# --- The link test has to cover what is declared ---
#
# A declaration nothing calls is a declaration nothing checks. In X11 that gap
# is about the linker; here it is worse, because a misspelled name is not a link
# error at all — vkGetDeviceProcAddr simply returns null for it and the wrapper
# calls through a null pointer at some later, unrelated moment.

if [ -f vk_link_test.cst ]; then
    grep -hoE '\bvk[A-Za-z0-9_]*\(' vk_link_test.cst \
      | sed 's/(//' | sort -u > "$TMP/touched"
    comm -23 "$TMP/declared" "$TMP/touched" > "$TMP/untouched"
    if [ -s "$TMP/untouched" ]; then
        echo "FAIL  declarado mas nao exercitado por vk_link_test.cst:"
        sed 's/^/        /' "$TMP/untouched" | head -20
        n=$(wc -l < "$TMP/untouched")
        [ "$n" -gt 20 ] && echo "        ... e mais $((n - 20))"
        fails=$((fails + n))
    fi
fi

# --- Coverage, reported not enforced ---

echo
printf '%-12s %10s %10s\n' nivel declarado manifesto
for lvl in loader instance device; do
    m=$(awk -v l="$lvl" '$1==l' "$MANIFEST" | grep -vc '^#')
    d=$(awk -v l="$lvl" '$1==l{print $2}' "$MANIFEST" | sort -u \
        | comm -12 - "$TMP/declared" | wc -l)
    printf '%-12s %10s %10s\n' "$lvl" "$d" "$m"
done
printf '%-12s %10s %10s\n' TOTAL "$(wc -l < "$TMP/declared")" "$(wc -l < "$TMP/manifest")"
printf '%-12s %10s\n' 'via extern' "$(wc -l < "$TMP/extern")"

if [ "$fails" -eq 0 ]; then echo; echo "check_symbols: ok"; fi
exit "$fails"
