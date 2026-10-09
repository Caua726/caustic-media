#!/bin/sh
# tools/wine_session.sh — a command run with a Wine prefix and an X server of
# its own, both removed afterwards. Nothing reaches the user's ~/.wine, desktop
# menus or display:
#
#   sh tools/wine_session.sh sh tools/run_wine.sh build/program.exe
#
# The X server comes from window/x11/tools/run_headless.sh, which waits for it
# to answer. Shutdown is bounded: a prefix whose wineserver does not stop in
# 10 s is kept, named, and the run fails rather than deleting files a live
# process still uses. CAUSTIC_WINE_DPI=144 runs at that system DPI (the
# prefix's LogPixels, read when its wineserver starts).
# CAUSTIC_WINE_GRAPHICS=wayland has Wine present through its Wayland driver
# to a Weston nested in that X server (window/wayland/tools/run_wayland.sh,
# WESTON_CONFIG passed on), so a compositor blends what it shows and the X
# screen can still be read back.
set -eu
[ $# -gt 0 ] || { echo "usage: wine_session.sh command [args...]" >&2; exit 2; }
. "$(dirname "$0")/child.sh"
if [ "${1:-}" = "--inside" ]; then
    shift
    unset WAYLAND_DISPLAY
    export CAUSTIC_WINE_SESSION="$WINEPREFIX"
    if [ -n "${CAUSTIC_WINE_DPI:-}" ]; then
        wine reg add 'HKCU\Control Panel\Desktop' /v LogPixels /t REG_DWORD /d "$CAUSTIC_WINE_DPI" /f >/dev/null 2>&1
        timeout 10 wineserver -w 2>/dev/null || { echo "wine_session: wineserver did not settle" >&2; exit 1; }
    fi
    case "${CAUSTIC_WINE_GRAPHICS:-x11}" in
        x11) ;;
        wayland)
            wine reg add 'HKCU\Software\Wine\Drivers' /v Graphics /d wayland /f >/dev/null 2>&1
            timeout 10 wineserver -w 2>/dev/null || { echo "wine_session: wineserver did not settle" >&2; exit 1; }
            set -- env WESTON_BACKEND=x11 sh "$(dirname "$0")/../window/wayland/tools/run_wayland.sh" "$@"
            ;;
        *) echo "wine_session: unknown CAUSTIC_WINE_GRAPHICS ${CAUSTIC_WINE_GRAPHICS}" >&2; exit 2 ;;
    esac
    status=0
    run_child "$@" || status=$?
    # Wine's processes leave while their X server still answers.
    wineserver -k 2>/dev/null || true
    timeout 10 wineserver -w 2>/dev/null || status=1
    exit "$status"
fi
command -v wine >/dev/null 2>&1 || { echo "wine_session: wine not found" >&2; exit 127; }
export WINEPREFIX="$(mktemp -d "${TMPDIR:-/tmp}/caustic-wine.XXXXXX")"
export WINEARCH=win64 WINEDEBUG=-all
# No debugger dialog for a crash, no menu entries in the user's desktop, no
# Mono/Gecko installer prompts.
export WINEDLLOVERRIDES="winedbg.exe=d;winemenubuilder.exe=d;mscoree=d;mshtml=d"
cleanup() {
    status=$?
    trap - EXIT
    wineserver -k 2>/dev/null || true
    if timeout 10 wineserver -w 2>/dev/null; then
        rm -rf "$WINEPREFIX"
    else
        echo "wine_session: wineserver did not stop; keeping $WINEPREFIX" >&2
        status=1
    fi
    exit "$status"
}
trap cleanup EXIT
run_child sh "$(dirname "$0")/../window/x11/tools/run_headless.sh" sh "$0" --inside "$@"
