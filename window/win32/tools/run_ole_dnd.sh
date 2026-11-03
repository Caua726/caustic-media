#!/bin/sh
# window/win32/tools/run_ole_dnd.sh — OLE drag and drop with another
# program: the peer (ole_dnd_peer.c, its log beside it) started in the same
# Wine session (tools/wine_session.sh) as the test, which drives both with
# SendInput. The exit status is the test's.
#
#   sh window/win32/tools/run_ole_dnd.sh build/ole_dnd_peer.exe build/win32_dnd_test.exe [peer's argument]
set -u
[ $# -ge 2 ] || { echo "usage: run_ole_dnd.sh ole_dnd_peer.exe dnd_test.exe [peer's argument]" >&2; exit 2; }
root="$(cd "$(dirname "$0")/../../.." && pwd)"
if [ -z "${CAUSTIC_WINE_SESSION:-}" ] || [ "${CAUSTIC_WINE_SESSION}" != "${WINEPREFIX:-}" ]; then
    exec sh "$root/tools/wine_session.sh" sh "$0" "$@"
fi
# The peer and the test name the log relative to the checkout, as Wine
# hands a Windows program its arguments.
cd "$root"
rm -f build/ole-peer.log*
wine "$1" build/ole-peer.log ${3:-} &
exec sh "$root/tools/run_wine.sh" "$2"
