#!/bin/sh
# Run one client against a private headless Weston, never the desktop session.
set -eu
[ $# -gt 0 ] || { echo "usage: run_wayland.sh <program> [args...]" >&2; exit 2; }
command -v weston >/dev/null 2>&1 || { echo "run_wayland: weston not found" >&2; exit 127; }
run=$(mktemp -d "${TMPDIR:-/tmp}/caustic-wayland.XXXXXX")
chmod 700 "$run"
mkdir "$run/runtime"
chmod 700 "$run/runtime"
export XDG_RUNTIME_DIR="$run/runtime"
export XDG_SESSION_TYPE=wayland
export WAYLAND_DISPLAY=wayland-0
weston --backend=headless --renderer=pixman --socket="$WAYLAND_DISPLAY" --idle-time=0 --width=1280 --height=1024 --scale="${WESTON_SCALE:-1}" --no-config --log="$run/weston.log" >/dev/null 2>&1 &
weston_pid=$!
cleanup() {
    kill "$weston_pid" 2>/dev/null || true
    wait "$weston_pid" 2>/dev/null || true
    rm -rf "$run"
}
trap cleanup EXIT HUP INT TERM
i=0
while [ ! -S "$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY" ]; do
    if ! kill -0 "$weston_pid" 2>/dev/null; then
        cat "$run/weston.log" >&2
        echo "run_wayland: weston exited before its socket appeared" >&2
        exit 1
    fi
    if [ "$i" -ge 100 ]; then
        cat "$run/weston.log" >&2
        echo "run_wayland: weston socket did not appear in 5s" >&2
        exit 1
    fi
    sleep 0.05
    i=$((i + 1))
done
if "$@"; then
    exit 0
else
    status=$?
    cat "$run/weston.log" >&2
    exit "$status"
fi
