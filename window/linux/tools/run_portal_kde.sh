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
# session is reached: the bus is dbus-run-session's, Qt draws offscreen —
# or, with CAUSTIC_PORTAL_SHOWN=1, on the display the program has, so the
# portal's dialogs can be seen beside its windows: its Wayland one (inside
# window/wayland/tools/run_kwin.sh, whose KWin scripts still reach KWin's
# own bus), else its X one (window/x11/tools/run_headless.sh).
set -eu
[ $# -gt 1 ] || { echo "usage: run_portal_kde.sh <kdeglobals> <program> [args...]" >&2; exit 2; }
if [ "${1:-}" != "--inside" ]; then
    for tool in dbus-run-session kwriteconfig6 gdbus; do
        command -v "$tool" >/dev/null 2>&1 || { echo "run_portal_kde: $tool not found" >&2; exit 127; }
    done
    [ -x /usr/lib/xdg-desktop-portal ] || { echo "run_portal_kde: xdg-desktop-portal not found" >&2; exit 127; }
    shown=""
    xshown=""
    platform=offscreen
    if [ "${CAUSTIC_PORTAL_SHOWN:-0}" = 1 ]; then
        if [ -n "${WAYLAND_DISPLAY:-}" ] && [ -n "${XDG_RUNTIME_DIR:-}" ]; then
            # The runtime directory changes below: the display by its whole path.
            case "$WAYLAND_DISPLAY" in /*) shown=$WAYLAND_DISPLAY ;; *) shown=$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY ;; esac
            platform=wayland
        elif [ -n "${DISPLAY:-}" ]; then
            xshown=$DISPLAY
            platform=xcb
        else
            echo "run_portal_kde: CAUSTIC_PORTAL_SHOWN=1 needs a display" >&2; exit 2
        fi
    fi
    home=$(mktemp -d "${TMPDIR:-/tmp}/caustic-portal.XXXXXX")
    # What the bus started may outlive it — ksecretd, a backend activated
    # late, gvfs with its mount in the runtime directory: stopped too, known
    # by the home made for this run and nothing else.
    reap() {
        tries=0
        while :; do
            left=""
            for environ in /proc/[0-9]*/environ; do
                [ -r "$environ" ] || continue
                if tr '\0' '\n' 2>/dev/null <"$environ" | grep -qx "HOME=$home"; then
                    pid=${environ#/proc/}
                    left="$left ${pid%/environ}"
                fi
            done
            [ -n "$left" ] || return 0
            tries=$((tries + 1))
            if [ "$tries" -gt 20 ]; then kill -9 $left 2>/dev/null || true; else kill $left 2>/dev/null || true; fi
            [ "$tries" -le 21 ] || return 0
            sleep 0.1
        done
    }
    trap 'reap; rm -rf "$home"' EXIT
    mkdir -p "$home/config" "$home/data" "$home/cache" "$home/runtime"
    chmod 700 "$home/runtime"
    cp "$1" "$home/config/kdeglobals"
    shift
    status=0
    env -u DISPLAY -u WAYLAND_DISPLAY ${shown:+WAYLAND_DISPLAY="$shown"} ${xshown:+DISPLAY="$xshown"} HOME="$home" XDG_CONFIG_HOME="$home/config" \
        XDG_DATA_HOME="$home/data" XDG_CACHE_HOME="$home/cache" XDG_RUNTIME_DIR="$home/runtime" XDG_CURRENT_DESKTOP=KDE \
        QT_QPA_PLATFORM="$platform" CAUSTIC_KDE_CONFIG="$home/config" \
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
