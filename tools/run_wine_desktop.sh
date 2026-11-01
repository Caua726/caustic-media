#!/bin/sh
# tools/run_wine_desktop.sh — a Windows program run as on a user's desktop:
# in a Wine session of its own (tools/wine_session.sh) whose prefix has what
# a Windows installation comes with and a fresh Wine prefix lacks — fonts in
# %WINDIR%\Fonts (the test fonts, text/testdata/fonts, stand in for Segoe UI
# and Consolas) and a text file opened by Notepad.
#
#   sh tools/run_wine_desktop.sh build/text_editor.exe --check
set -eu
[ $# -gt 0 ] || { echo "usage: run_wine_desktop.sh program.exe [args...]" >&2; exit 2; }
if [ -z "${CAUSTIC_WINE_SESSION:-}" ] || [ "${CAUSTIC_WINE_SESSION}" != "${WINEPREFIX:-}" ]; then
    exec sh "$(dirname "$0")/wine_session.sh" sh "$0" "$@"
fi
root="$(cd "$(dirname "$0")/.." && pwd)"
# The prefix is made by Wine's first start; what follows goes into it.
wineboot -i >/dev/null 2>&1
timeout 10 wineserver -w 2>/dev/null || { echo "run_wine_desktop: wineserver did not settle" >&2; exit 1; }
cp "$root"/text/testdata/fonts/sans/* "$root"/text/testdata/fonts/mono/* "$root"/text/testdata/fonts/weights/* \
    "$WINEPREFIX/drive_c/windows/Fonts/"
wine reg add 'HKCR\.txt' /ve /d txtfile /f >/dev/null 2>&1
wine reg add 'HKCR\txtfile\shell\open\command' /ve /d 'notepad.exe "%1"' /f >/dev/null 2>&1
timeout 10 wineserver -w 2>/dev/null || { echo "run_wine_desktop: wineserver did not settle" >&2; exit 1; }
exec sh "$root/tools/run_wine.sh" "$@"
