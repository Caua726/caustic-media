#!/bin/sh
# window/wayland/tools/run_cursor_kwin.sh — the pointer's image on a KWin of
# its own (run_kwin.sh) whose output's scale changes while the test runs:
# the test, cursor-shape-v1 off so the image is ours, says it is ready in a
# file; kwin_scale.sh then sets the scale to 2, and the test checks the
# image drawn again at it. The exit status is the test's.
#
#   sh window/wayland/tools/run_cursor_kwin.sh build/wayland_cursor_test
set -u
[ $# -eq 1 ] || { echo "usage: run_cursor_kwin.sh wayland_cursor_test" >&2; exit 2; }
root="$(cd "$(dirname "$0")/../../.." && pwd)"
if [ -z "${CAUSTIC_CURSOR_KWIN_INSIDE:-}" ]; then
    CAUSTIC_CURSOR_KWIN_INSIDE=1 exec sh "$root/window/wayland/tools/run_kwin.sh" sh "$0" "$@"
fi
ready="$root/build/cursor-ready"
rm -f "$ready"
env CAUSTIC_WAYLAND_CURSOR_SHAPE=0 XCURSOR_PATH="$root/build/cursor-themes" "$1" kwin "$ready" &
p=$!
sh "$root/window/wayland/tools/kwin_scale.sh" 2 "$ready"
s=0
wait $p || s=$?
exit $s
