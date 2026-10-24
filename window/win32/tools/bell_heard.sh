#!/bin/sh
# window/win32/tools/bell_heard.sh — a Windows program's bell heard where
# Wine shows it: on the X server of the Wine session, whose bells a watcher
# of its own is told of (XkbBellNotify; build/x11_desktop_test heard, built
# from window/x11/desktop_test.cst, or CAUSTIC_BELL_WATCHER). Inside
# tools/wine_session.sh:
#
#   sh tools/wine_session.sh sh window/win32/tools/bell_heard.sh build/win32_bell_test.exe
#
# Succeeds when the program succeeded and the server rang a bell meanwhile.
set -eu
[ $# -gt 0 ] || { echo "usage: bell_heard.sh <program.exe> [args...]" >&2; exit 2; }
[ -n "${CAUSTIC_WINE_SESSION:-}" ] || { echo "bell_heard: inside tools/wine_session.sh only" >&2; exit 2; }
said=$(mktemp "${TMPDIR:-/tmp}/caustic-bell.XXXXXX")
"${CAUSTIC_BELL_WATCHER:-./build/x11_desktop_test}" heard "$said" &
watcher=$!
trap 'kill "$watcher" 2>/dev/null || true; rm -f "$said"' EXIT
waited=0
until grep -q watching "$said" 2>/dev/null; do
    kill -0 "$watcher" 2>/dev/null || { echo "bell_heard: the watcher did not start" >&2; exit 1; }
    [ "$waited" -lt 200 ] || { echo "bell_heard: the watcher did not start in 10s" >&2; exit 1; }
    sleep 0.05
    waited=$((waited + 1))
done
status=0
sh tools/run_wine.sh "$@" || status=$?
if wait "$watcher"; then :; else echo "bell_heard: no bell rung on the X server" >&2; status=1; fi
exit "$status"
