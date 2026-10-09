#!/bin/sh
# window/wayland/tools/run_kwin.sh — a program run against a KWin of its own,
# the other compositor Wayland tests prove behaviour on: one that decorates
# windows itself (xdg-decoration), scales by fractions
# (wp_fractional_scale), tiles them, and can be scripted to (org.kde.KWin
# /Scripting on its session bus).
#
#   sh window/wayland/tools/run_kwin.sh <program> [args...]
#
# Everything is private: a runtime directory, a configuration and cache of
# its own (KWin writes kwinoutputconfig.json and more there), and a bare
# session bus (dbus/tools/run_dbus_bare.sh) that starts none of the
# desktop's services. KWIN_BACKEND=virtual (the default) draws nowhere;
# KWIN_BACKEND=x11 nests it in the X server DISPLAY names, so the screen can
# be read back. KWIN_SCALE=1.5 sets its output's scale (kscreen-doctor, in
# process) before the program starts.
set -eu
[ $# -gt 0 ] || { echo "usage: run_kwin.sh <program> [args...]" >&2; exit 2; }
if [ "${1:-}" != "--inside" ]; then
    command -v kwin_wayland >/dev/null 2>&1 || { echo "run_kwin: kwin_wayland not found" >&2; exit 127; }
    exec sh "$(dirname "$0")/../../../dbus/tools/run_dbus_bare.sh" sh "$0" --inside "$@"
fi
shift
backend=${KWIN_BACKEND:-virtual}
case "$backend" in
    virtual) where="--virtual" ;;
    x11)
        [ -n "${DISPLAY:-}" ] || { echo "run_kwin: KWIN_BACKEND=x11 needs DISPLAY" >&2; exit 2; }
        where="--x11-display=$DISPLAY"
        ;;
    *) echo "run_kwin: unknown KWIN_BACKEND $backend" >&2; exit 2 ;;
esac
run=$(mktemp -d "${TMPDIR:-/tmp}/caustic-kwin.XXXXXX")
chmod 700 "$run"
mkdir -m 700 "$run/runtime" "$run/config" "$run/cache" "$run/data"
export XDG_RUNTIME_DIR="$run/runtime" XDG_CONFIG_HOME="$run/config" XDG_CACHE_HOME="$run/cache" XDG_DATA_HOME="$run/data"
export XDG_SESSION_TYPE=wayland
unset WAYLAND_DISPLAY
kwin_pid=""
cleanup() {
    if [ -n "$kwin_pid" ]; then
        kill "$kwin_pid" 2>/dev/null || true
        wait "$kwin_pid" 2>/dev/null || true
    fi
    rm -rf "$run"
}
trap cleanup EXIT
. "$(dirname "$0")/../../../tools/child.sh"
kwin_wayland "$where" --no-lockscreen --no-global-shortcuts --socket=wayland-k --width=1280 --height=1024 \
    >"$run/kwin.log" 2>&1 &
kwin_pid=$!
export WAYLAND_DISPLAY=wayland-k
started=$(date +%s)
while :; do
    if [ -S "$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY" ] && timeout 1 wayland-info >/dev/null 2>&1; then break; fi
    if ! kill -0 "$kwin_pid" 2>/dev/null; then
        cat "$run/kwin.log" >&2
        echo "run_kwin: kwin exited before it answered" >&2
        exit 1
    fi
    if [ $(($(date +%s) - started)) -ge 10 ]; then
        cat "$run/kwin.log" >&2
        echo "run_kwin: kwin did not answer in 10s" >&2
        exit 1
    fi
    sleep 0.05
done
if [ -n "${KWIN_SCALE:-}" ]; then
    # Its listing is coloured even into a pipe: the codes taken out first.
    output=$(QT_QPA_PLATFORM=wayland KSCREEN_BACKEND_INPROCESS=1 timeout 10 kscreen-doctor -o 2>/dev/null |
        tr -d '\033' | sed 's/\[[0-9;]*m//g' | sed -n 's/^Output: [0-9]* \([^ ]*\) .*/\1/p' | sed -n 1p)
    [ -n "$output" ] || { echo "run_kwin: no output to scale" >&2; exit 1; }
    QT_QPA_PLATFORM=wayland KSCREEN_BACKEND_INPROCESS=1 timeout 10 kscreen-doctor "output.$output.scale.$KWIN_SCALE" >/dev/null 2>&1 ||
        { echo "run_kwin: kscreen-doctor did not scale $output" >&2; exit 1; }
fi
# Clients must speak to this compositor, not an X server they might inherit,
# unless it is nested there and they read the screen back.
if [ "$backend" = virtual ]; then unset DISPLAY; fi
# The log, for a program that wants to read what a KWin script printed.
export CAUSTIC_KWIN_LOG="$run/kwin.log"
status=0
run_child "$@" || status=$?
if [ "$status" -ne 0 ]; then tail -40 "$run/kwin.log" >&2; fi
exit "$status"
