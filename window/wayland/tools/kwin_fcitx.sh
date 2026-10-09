#!/bin/sh
# window/wayland/tools/kwin_fcitx.sh — the installed Fcitx5 as the input method
# a KWin of run_kwin.sh starts (KWIN_INPUT_METHOD): KWin runs it with the
# input-method socket in WAYLAND_SOCKET, and Fcitx5's Wayland frontend is
# told what the text field with the keyboard holds. Its configuration is the
# KWin's private one, no X frontend, no tray. FCITX_LOG=path: its log, the
# protocol it was spoken included, kept there.
#
#   env KWIN_BACKEND=x11 KWIN_INPUT_METHOD=window/wayland/tools/kwin_fcitx.sh \
#       FCITX_LOG=build/kwin_fcitx.log sh window/wayland/tools/run_kwin.sh <program>
set -eu
command -v fcitx5 >/dev/null 2>&1 || { echo "kwin_fcitx: fcitx5 not found" >&2; exit 127; }
export LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8
exec fcitx5 -r --verbose='*=5' \
    --disable=xim,notificationitem,kimpanel,ibusfrontend,dbusfrontend,fcitx4frontend,virtualkeyboard,clipboard \
    >"${FCITX_LOG:-/dev/null}" 2>&1
