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
# So the server is started explicitly and polled until it actually answers.
# xdpyinfo connecting is the readiness signal, because it is the same thing the
# program under test is about to do.
set -eu

[ $# -ge 1 ] || { echo "uso: run_headless.sh <programa> [args...]" >&2; exit 2; }

command -v Xvfb >/dev/null 2>&1 || {
    echo "run_headless: Xvfb nao encontrado (apt: xvfb)" >&2; exit 127
}
command -v xdpyinfo >/dev/null 2>&1 || {
    echo "run_headless: xdpyinfo nao encontrado (apt: x11-utils)" >&2; exit 127
}

# A display number nobody is using. The lock file is what X itself checks, so
# checking it too is the same question the server would ask.
num=99
while [ "$num" -lt 200 ]; do
    [ -e "/tmp/.X${num}-lock" ] || [ -e "/tmp/.X11-unix/X${num}" ] || break
    num=$((num + 1))
done
[ "$num" -lt 200 ] || { echo "run_headless: nenhum display livre" >&2; exit 1; }

Xvfb ":$num" -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
xvfb_pid=$!

cleanup() {
    kill "$xvfb_pid" 2>/dev/null || true
    wait "$xvfb_pid" 2>/dev/null || true
    rm -f "/tmp/.X${num}-lock" 2>/dev/null || true
}
trap cleanup EXIT INT TERM

# Poll until it answers, rather than sleeping a guess. 100 tries at 50 ms is
# five seconds, which is far more than Xvfb has ever needed and still bounded.
i=0
while [ "$i" -lt 100 ]; do
    if DISPLAY=":$num" xdpyinfo >/dev/null 2>&1; then break; fi
    if ! kill -0 "$xvfb_pid" 2>/dev/null; then
        echo "run_headless: Xvfb morreu antes de aceitar conexoes" >&2
        exit 1
    fi
    sleep 0.05
    i=$((i + 1))
done
[ "$i" -lt 100 ] || { echo "run_headless: Xvfb nao respondeu em 5s" >&2; exit 1; }

DISPLAY=":$num" "$@"
