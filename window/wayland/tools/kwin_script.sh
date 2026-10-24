#!/bin/sh
# window/wayland/tools/kwin_script.sh — a KWin script run once, as a user's
# shortcut would act: loaded over KWin's session bus (org.kde.KWin
# /Scripting), run, unloaded. Inside window/wayland/tools/run_kwin.sh only,
# which names that bus CAUSTIC_KWIN_BUS — still reached from a program with
# a session bus of its own inside it (run_portal_kde.sh).
#
#   sh window/wayland/tools/kwin_script.sh window/wayland/testdata/kwin/quick-tile-left.js
set -eu
[ $# -eq 1 ] || { echo "usage: kwin_script.sh <script.js>" >&2; exit 2; }
if [ -n "${CAUSTIC_KWIN_BUS:-}" ]; then DBUS_SESSION_BUS_ADDRESS=$CAUSTIC_KWIN_BUS; export DBUS_SESSION_BUS_ADDRESS; fi
file=$(cd "$(dirname "$1")" && pwd)/$(basename "$1")
name="caustic-$(basename "$1" .js)-$$"
id=$(dbus-send --session --print-reply --dest=org.kde.KWin /Scripting org.kde.kwin.Scripting.loadScript \
    "string:$file" "string:$name" | sed -n 's/^ *int32 \([0-9-]*\)$/\1/p')
[ -n "$id" ] && [ "$id" -ge 0 ] || { echo "kwin_script: $file not loaded" >&2; exit 1; }
dbus-send --session --print-reply --dest=org.kde.KWin "/Scripting/Script$id" org.kde.kwin.Script.run >/dev/null
dbus-send --session --print-reply --dest=org.kde.KWin /Scripting org.kde.kwin.Scripting.unloadScript "string:$name" >/dev/null
