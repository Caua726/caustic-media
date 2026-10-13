#!/bin/sh
# window/linux/tools/run_portal_kde.sh — a program run against the installed
# settings portal (xdg-desktop-portal with its KDE backend) on a session bus
# of its own, the desktop's configuration in a directory of its own:
#
#   sh window/linux/tools/run_portal_kde.sh <kdeglobals> <program> [args...]
#
# HOME and the XDG directories are a fresh temporary directory, so the
# user's configuration is neither read nor written. The file kdeglobals is
# the desktop's configuration as the portal starts; the program changes it
# as KDE's System Settings does, with `kwriteconfig6 --notify`
# (CAUSTIC_KDE_CONFIG names the directory it is in). The portal is started
# by the bus as a program asks for it, and answering before the program
# starts; every client of the bus is stopped after it. Nothing of the user's
# session is reached: the bus is dbus-run-session's, Qt draws offscreen.
set -eu
[ $# -gt 1 ] || { echo "usage: run_portal_kde.sh <kdeglobals> <program> [args...]" >&2; exit 2; }
if [ "${1:-}" != "--inside" ]; then
    for tool in dbus-run-session kwriteconfig6 gdbus; do
        command -v "$tool" >/dev/null 2>&1 || { echo "run_portal_kde: $tool not found" >&2; exit 127; }
    done
    [ -x /usr/lib/xdg-desktop-portal ] || { echo "run_portal_kde: xdg-desktop-portal not found" >&2; exit 127; }
    home=$(mktemp -d "${TMPDIR:-/tmp}/caustic-portal.XXXXXX")
    trap 'rm -rf "$home"' EXIT
    mkdir -p "$home/config" "$home/data" "$home/cache" "$home/runtime"
    chmod 700 "$home/runtime"
    cp "$1" "$home/config/kdeglobals"
    shift
    status=0
    env -u DISPLAY -u WAYLAND_DISPLAY HOME="$home" XDG_CONFIG_HOME="$home/config" XDG_DATA_HOME="$home/data" \
        XDG_CACHE_HOME="$home/cache" XDG_RUNTIME_DIR="$home/runtime" XDG_CURRENT_DESKTOP=KDE \
        QT_QPA_PLATFORM=offscreen CAUSTIC_KDE_CONFIG="$home/config" \
        dbus-run-session -- sh "$0" --inside "$@" 2>>"$home/bus.log" || status=$?
    [ "$status" = 0 ] || tail -20 "$home/bus.log" >&2
    exit "$status"
fi
shift
bus_call() {
    gdbus call --session --dest org.freedesktop.DBus --object-path /org/freedesktop/DBus \
        --method "org.freedesktop.DBus.$1" ${2:+"$2"}
}
# Every other client of the bus — the portal, its backends, the document
# store (whose FUSE mount goes with it) — stopped, so the bus ends with the
# program.
stop() {
    for name in $(bus_call ListNames | tr -d "()[],'"); do
        case "$name" in
            :*)
                pid=$(bus_call GetConnectionUnixProcessID "$name" 2>/dev/null | sed -n 's/.*uint32 \([0-9]*\).*/\1/p')
                if [ -n "$pid" ] && [ "$pid" != "$$" ]; then kill "$pid" 2>/dev/null || true; fi
                ;;
        esac
    done
    sleep 0.3
}
trap stop EXIT
up=0
i=0
while [ "$i" -lt 100 ]; do
    if gdbus call --session --dest org.freedesktop.portal.Desktop --object-path /org/freedesktop/portal/desktop \
        --method org.freedesktop.portal.Settings.ReadOne org.freedesktop.appearance color-scheme >/dev/null 2>&1; then
        up=1
        break
    fi
    sleep 0.1
    i=$((i + 1))
done
[ "$up" = 1 ] || { echo "run_portal_kde: the portal did not answer in 10s" >&2; exit 1; }
status=0
"$@" || status=$?
exit "$status"
