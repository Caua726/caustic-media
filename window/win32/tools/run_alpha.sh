#!/bin/sh
# window/win32/tools/run_alpha.sh — transparent_win32_test and alpha_check
# together, inside a session of tools/wine_session.sh with Wine's Wayland
# driver (CAUSTIC_WINE_GRAPHICS=wayland): the Windows window stays up until
# alpha_check has read the screen back; alpha_check's verdict is the result.
#
#   env CAUSTIC_WINE_GRAPHICS=wayland WESTON_CONFIG=window/win32/testdata/blue-desktop.ini \
#       sh tools/wine_session.sh sh window/win32/tools/run_alpha.sh
set -eu
[ -n "${CAUSTIC_WINE_SESSION:-}" ] || { echo "run_alpha: run it inside tools/wine_session.sh" >&2; exit 2; }
done_file="$WINEPREFIX/alpha-done"
CAUSTIC_ALPHA_UNIX_DONE="$done_file"
CAUSTIC_ALPHA_DONE=$(winepath -w "$done_file" 2>/dev/null)
export CAUSTIC_ALPHA_UNIX_DONE CAUSTIC_ALPHA_DONE
./build/win32_alpha_check &
check=$!
status=0
sh tools/run_wine.sh build/transparent_win32_test.exe || status=1
wait "$check" || status=1
exit "$status"
