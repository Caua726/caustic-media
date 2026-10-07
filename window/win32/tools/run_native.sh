#!/bin/sh
# One private Wine prefix and one X server for the complete native GUI group.
set -eu
if [ "${1:-}" = "--inside" ]; then
    shift
    for program in "$@"; do sh tools/run_wine.sh "$program"; done
    exit 0
fi
[ $# -gt 0 ] || { echo "usage: run_native.sh <program.exe>..." >&2; exit 2; }
export WINEPREFIX="$(mktemp -d "${TMPDIR:-/tmp}/caustic-win32.XXXXXX")"
# A crashing consumer must fail, not wait for a graphical debugger dialog.
export WINEDLLOVERRIDES="winedbg.exe=d"
cleanup() {
    wineserver -k 2>/dev/null || true
    timeout 10 wineserver -w 2>/dev/null || true
    rm -rf "$WINEPREFIX"
}
trap cleanup EXIT HUP INT TERM
sh window/x11/tools/run_headless.sh sh "$0" --inside "$@"
