#!/bin/sh
# window/win32/tools/run_touch_kwin.sh — fingers on Windows: Wine on KWin's
# Xwayland (tools/wine_session.sh with CAUSTIC_WINE_X=kwin), the touches
# KWin's fake input sets down (touch_kwin_test inject) read by the Windows
# program as WM_POINTER. The injector waits for the program's ready file;
# the exit status is the Windows program's, once the injector has ended too.
#
#   sh window/win32/tools/run_touch_kwin.sh build/touch_kwin_test build/touch_win32_test.exe
set -u
[ $# -eq 2 ] || { echo "usage: run_touch_kwin.sh touch_kwin_test touch_win32_test.exe" >&2; exit 2; }
root="$(cd "$(dirname "$0")/../../.." && pwd)"
if [ -z "${CAUSTIC_WINE_SESSION:-}" ] || [ "${CAUSTIC_WINE_SESSION}" != "${WINEPREFIX:-}" ]; then
    CAUSTIC_WINE_X=kwin exec sh "$root/tools/wine_session.sh" sh "$0" "$@"
fi
ready="$root/build/touch-ready"
rm -f "$ready"
env WAYLAND_DISPLAY="$CAUSTIC_WAYLAND_DISPLAY" "$1" inject "$ready" &
env CAUSTIC_TOUCH_READY="$ready" sh "$root/tools/run_wine.sh" "$2"
s=$?
wait
exit $s
