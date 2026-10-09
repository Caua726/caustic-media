#!/bin/sh
# Run one client against a private Weston, never the desktop session.
#
#   sh window/wayland/tools/run_wayland.sh <program> [args...]
#
# The default is Weston's headless backend: outputs and frame callbacks, no
# seat. WESTON_BACKEND=x11 nests Weston in an X server instead — run it
# inside window/x11/tools/run_headless.sh — and its keyboard and pointer
# become a real Wayland seat that XTest (xdotool) drives. WESTON_SCALE sets
# the output scale; WESTON_SHELL=kiosk shows every toplevel fullscreen at the
# output's origin, so where a popup has room is known.
set -eu
[ $# -gt 0 ] || { echo "usage: run_wayland.sh <program> [args...]" >&2; exit 2; }
command -v weston >/dev/null 2>&1 || { echo "run_wayland: weston not found" >&2; exit 127; }
backend=${WESTON_BACKEND:-headless}
case "$backend" in
    headless) ;;
    x11) [ -n "${DISPLAY:-}" ] || { echo "run_wayland: WESTON_BACKEND=x11 needs DISPLAY" >&2; exit 2; } ;;
    *) echo "run_wayland: unknown WESTON_BACKEND $backend" >&2; exit 2 ;;
esac
run=$(mktemp -d "${TMPDIR:-/tmp}/caustic-wayland.XXXXXX")
chmod 700 "$run"
mkdir "$run/runtime"
chmod 700 "$run/runtime"
export XDG_RUNTIME_DIR="$run/runtime"
export XDG_SESSION_TYPE=wayland
export WAYLAND_DISPLAY=wayland-0
weston_pid=""
cleanup() {
    if [ -n "$weston_pid" ]; then
        kill "$weston_pid" 2>/dev/null || true
        wait "$weston_pid" 2>/dev/null || true
    fi
    rm -rf "$run"
}
trap cleanup EXIT
. "$(dirname "$0")/../../../tools/child.sh"
shell=${WESTON_SHELL:-desktop}
case "$shell" in
    desktop|kiosk) ;;
    *) echo "run_wayland: unknown WESTON_SHELL $shell" >&2; exit 2 ;;
esac
weston --backend="$backend" --renderer=pixman --socket="$WAYLAND_DISPLAY" --idle-time=0 \
    --width=1280 --height=1024 --scale="${WESTON_SCALE:-1}" --shell="$shell" --no-config \
    --log="$run/weston.log" >/dev/null 2>&1 &
weston_pid=$!
# Ready once a client's registry round trip is answered; the socket file alone
# can exist before the compositor serves it. Bounded by the clock, probes
# included.
started=$(date +%s)
while :; do
    if [ -S "$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY" ]; then
        if ! command -v wayland-info >/dev/null 2>&1; then break; fi
        if timeout 1 wayland-info >/dev/null 2>&1; then break; fi
    fi
    if ! kill -0 "$weston_pid" 2>/dev/null; then
        cat "$run/weston.log" >&2
        echo "run_wayland: weston exited before it answered" >&2
        exit 1
    fi
    if [ $(($(date +%s) - started)) -ge 5 ]; then
        cat "$run/weston.log" >&2
        echo "run_wayland: weston did not answer in 5s" >&2
        exit 1
    fi
    sleep 0.05
done
# Clients must speak to this compositor, not an X server they might inherit.
if [ "$backend" = headless ]; then unset DISPLAY; fi
status=0
run_child "$@" || status=$?
if [ "$status" -ne 0 ]; then cat "$run/weston.log" >&2; fi
exit "$status"
