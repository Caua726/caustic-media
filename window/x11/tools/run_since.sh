#!/bin/sh
# window/x11/tools/run_since.sh — run a link test only where its library is new enough.
#
#   run_since.sh <soname> <release> <command...>
#
# A link test binds every name at exec under BIND_NOW, so one that calls a
# symbol the local library does not export dies before main. For the symbols
# symbols.txt marks with the release that introduced them, that is not a bug
# when the library is older: XcursorGetResizable does not exist in Ubuntu
# 24.04's libXcursor 1.2.1, and no binding can change that. Those calls live in
# their own binary, and this decides whether it runs.
#
# The question asked is whether the library exports ANY of the symbols the
# manifest dates to <release>, not all of them. A release lands whole, so none
# means an older library and the test is skipped; any means the library has the
# release, and the test runs — so a misspelled name still dies loudly at exec
# instead of being mistaken for an old library. check_symbols.sh draws the same
# line.
#
# Exit code is the command's, or 0 when skipped.
set -u

[ $# -ge 3 ] || { echo "uso: run_since.sh <soname> <release> <comando...>" >&2; exit 2; }

so=$1
ver=$2
shift 2

MANIFEST="$(dirname "$0")/symbols.txt"
[ -f "$MANIFEST" ] || { echo "run_since: $MANIFEST nao existe" >&2; exit 1; }
command -v nm >/dev/null 2>&1 || {
    echo "run_since: nm nao encontrado (apt: binutils)" >&2; exit 127
}

names=$(awk -v s="$so" -v v="$ver" '!/^#/ && $1==s && $3==v {print $2}' "$MANIFEST")
[ -n "$names" ] || {
    echo "run_since: nenhum simbolo de $so marcado $ver em $MANIFEST" >&2; exit 1
}

path=""
for dir in /usr/lib /usr/lib64 /usr/lib/x86_64-linux-gnu /lib/x86_64-linux-gnu; do
    [ -f "$dir/$so" ] && { path="$dir/$so"; break; }
done
[ -n "$path" ] || { echo "FAIL  $so nao encontrado"; exit 1; }

exported=$(nm -D --defined-only "$path" 2>/dev/null | awk '$2=="T"{print $3}')
for n in $names; do
    if printf '%s\n' "$exported" | grep -qx "$n"; then
        exec "$@"
    fi
done

echo "run_since: $so desta maquina e anterior a $ver — pulado: $*"
exit 0
