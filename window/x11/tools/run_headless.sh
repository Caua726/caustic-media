#!/bin/sh
# window/x11/tools/run_headless.sh — an X server for a machine that has none.
#
# The functional suite needs a real server: it opens a window, blits a frame and
# reads it back with XGetImage to compare byte for byte. Xvfb provides one with
# no screen attached, which is what makes that runnable in CI and over ssh.
#
#   sh tools/run_headless.sh ./build/x11_test
#
# 24-bit depth is not incidental. The backend requires a TrueColor visual at 24
# or 32 bits and refuses to open otherwise; at 16 the test would fail for a
# reason that has nothing to do with the code under test.
#
# --- Why this does not just call xvfb-run ---
#
# `xvfb-run -a` picks a free display number and starts the server, but it does
# not wait for the server to be ready before running the command. Measured here:
# 3 failures in 8 consecutive runs, all XOpenDisplay returning null, all
# clustered right after a previous Xvfb was shutting down. That is a race
# between the lock file appearing and the socket accepting connections, and in
# CI it would read as a flaky window layer rather than as a flaky harness.
#
# So the server is started explicitly and the program run only once the
# server says it accepts connections: with -displayfd it writes its display
# number back when it does.
#
# --- Why the server picks the display ---
#
# This used to pick a number whose lock file was missing and start Xvfb on it.
# Two runs at once — the mutation runner starts several — saw the same number
# free: the second server died on the first one's lock, or both programs ended
# up on one server, where a test playing the window manager is refused the
# root's SubstructureRedirect and exits on the X error. Either read as a
# failing test. -displayfd has the server take the first free display itself,
# atomically, as it creates the lock.
#
# --- Why -noreset ---
#
# An X server resets when its last client leaves, and a connection made while
# it does is dropped (XOpenDisplay fails, errno ECONNRESET). A desktop always
# has other clients; here the program is the only one, and a test that closes
# its last window and opens another reopens the display right then: plain
# Xlib, opening and closing in a loop, failed 27 times in 200.
set -eu

[ $# -ge 1 ] || { echo "uso: run_headless.sh <programa> [args...]" >&2; exit 2; }

command -v Xvfb >/dev/null 2>&1 || {
    echo "run_headless: Xvfb nao encontrado (apt: xvfb)" >&2; exit 127
}
num=""
xvfb_pid=""
numfile=$(mktemp "${TMPDIR:-/tmp}/run_headless.XXXXXX")
cleanup() {
    if [ -n "$xvfb_pid" ]; then
        kill "$xvfb_pid" 2>/dev/null || true
        wait "$xvfb_pid" 2>/dev/null || true
    fi
    [ -z "$num" ] || rm -f "/tmp/.X${num}-lock" 2>/dev/null || true
    rm -f "$numfile"
}
# A signal stops the program and then the server, never a server without
# its program or a program waited for indefinitely.
trap cleanup EXIT
. "$(dirname "$0")/../../../tools/child.sh"
Xvfb -displayfd 3 -screen 0 1280x1024x24 -nolisten tcp -noreset 3>"$numfile" >/dev/null 2>&1 &
xvfb_pid=$!

# Wait for the number, a whole line of it, rather than sleeping a guess. 100
# tries at 50 ms is five seconds, far more than Xvfb has ever needed and still
# bounded.
i=0
while [ "$i" -lt 100 ]; do
    if [ "$(wc -l < "$numfile")" -ge 1 ]; then break; fi
    if ! kill -0 "$xvfb_pid" 2>/dev/null; then
        echo "run_headless: Xvfb morreu antes de aceitar conexoes" >&2
        exit 1
    fi
    sleep 0.05
    i=$((i + 1))
done
[ "$i" -lt 100 ] || { echo "run_headless: Xvfb nao respondeu em 5s" >&2; exit 1; }
num=$(head -n 1 "$numfile")
# A client's round trip, when the probe is installed, confirms what the
# server wrote.
if command -v xdpyinfo >/dev/null 2>&1; then
    timeout 5 xdpyinfo -display ":$num" >/dev/null 2>&1 || {
        echo "run_headless: Xvfb :$num nao respondeu a um cliente" >&2; exit 1
    }
fi

# AUTO must use this private X server, not the caller's Wayland session.
unset WAYLAND_DISPLAY
export XDG_SESSION_TYPE=x11
export DISPLAY=":$num"
run_child "$@"
