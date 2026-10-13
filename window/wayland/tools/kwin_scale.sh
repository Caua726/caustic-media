#!/bin/sh
# Set the scale of a private KWin's first output (kscreen-doctor, in
# process), WAYLAND_DISPLAY naming it — once the file ready exists when one
# is given, so a program running meanwhile sees the scale change live:
#
#   sh window/wayland/tools/kwin_scale.sh <scale> [ready]
set -eu
[ $# -ge 1 ] || { echo "usage: kwin_scale.sh <scale> [ready]" >&2; exit 2; }
if [ $# -ge 2 ]; then
    waited=0
    while [ ! -e "$2" ] && [ "$waited" -lt 600 ]; do sleep 0.05; waited=$((waited + 1)); done
    [ -e "$2" ] || { echo "kwin_scale: $2 never came" >&2; exit 1; }
fi
# Its listing is coloured even into a pipe: the codes taken out first.
output=$(QT_QPA_PLATFORM=wayland KSCREEN_BACKEND_INPROCESS=1 timeout 10 kscreen-doctor -o 2>/dev/null |
    tr -d '\033' | sed 's/\[[0-9;]*m//g' | sed -n 's/^Output: [0-9]* \([^ ]*\) .*/\1/p' | sed -n 1p)
[ -n "$output" ] || { echo "kwin_scale: no output to scale" >&2; exit 1; }
QT_QPA_PLATFORM=wayland KSCREEN_BACKEND_INPROCESS=1 timeout 10 kscreen-doctor "output.$output.scale.$1" >/dev/null 2>&1 ||
    { echo "kwin_scale: kscreen-doctor did not scale $output" >&2; exit 1; }
