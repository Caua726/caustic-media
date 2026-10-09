#!/bin/sh
# window/x11/tools/run_fcitx.sh — the installed Fcitx5, private to one program,
# on the X server in DISPLAY (run inside run_headless.sh):
#
#   sh window/x11/tools/run_headless.sh sh window/x11/tools/run_fcitx.sh ./program
#
# Its configuration, data and cache in a directory of its own, so the user's
# are neither read nor written; the only input method the default keyboard
# one (Quick Phrase and the others it comes with). FCITX_ON_THE_SPOT=1 (the
# default) has its XIM frontend offer on-the-spot preedit (UseOnTheSpot), 0
# over-the-spot. The program runs with XMODIFIERS=@im=fcitx in a UTF-8 locale
# once Fcitx5 says its XIM frontend is up; Fcitx5 is stopped after it.
set -eu

[ $# -ge 1 ] || { echo "uso: run_fcitx.sh <programa> [args...]" >&2; exit 2; }
command -v fcitx5 >/dev/null 2>&1 || { echo "run_fcitx: fcitx5 nao encontrado" >&2; exit 127; }
[ -n "${DISPLAY:-}" ] || { echo "run_fcitx: precisa de DISPLAY (run_headless.sh)" >&2; exit 2; }

dir=$(mktemp -d "${TMPDIR:-/tmp}/run_fcitx.XXXXXX")
pid=""
cleanup() {
    if [ -n "$pid" ]; then
        kill "$pid" 2>/dev/null || true
        wait "$pid" 2>/dev/null || true
    fi
    # FCITX_LOG=path: Fcitx5's log kept there (FCITX_VERBOSE its verbosity,
    # xim=5 by default), for when a program and it disagree.
    if [ -n "${FCITX_LOG:-}" ]; then cp "$dir/fcitx.log" "$FCITX_LOG" 2>/dev/null || true; fi
    rm -rf "$dir"
}
trap cleanup EXIT
mkdir -p "$dir/config/fcitx5/conf" "$dir/data" "$dir/cache"
if [ "${FCITX_ON_THE_SPOT:-1}" = 1 ]; then
    printf 'UseOnTheSpot=True\n' > "$dir/config/fcitx5/conf/xim.conf"
else
    printf 'UseOnTheSpot=False\n' > "$dir/config/fcitx5/conf/xim.conf"
fi
export XDG_CONFIG_HOME="$dir/config" XDG_DATA_HOME="$dir/data" XDG_CACHE_HOME="$dir/cache"
export LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8
# No session bus here: its frontends and the tray off, XIM and the panel on.
fcitx5 -r --verbose="${FCITX_VERBOSE:-xim=5}" \
    --disable=dbus,notificationitem,kimpanel,ibusfrontend,dbusfrontend,fcitx4frontend,wayland,waylandim,virtualkeyboard,clipboard \
    > "$dir/fcitx.log" 2>&1 &
pid=$!
i=0
while [ "$i" -lt 100 ]; do
    if grep -q "XIM basic init" "$dir/fcitx.log" 2>/dev/null; then break; fi
    if ! kill -0 "$pid" 2>/dev/null; then
        echo "run_fcitx: fcitx5 saiu antes do XIM" >&2; cat "$dir/fcitx.log" >&2; exit 1
    fi
    sleep 0.1
    i=$((i + 1))
done
[ "$i" -lt 100 ] || { echo "run_fcitx: XIM do fcitx5 nao subiu em 10s" >&2; exit 1; }
# The keyboard input method loaded and the default group set, after XIM.
sleep 1
export XMODIFIERS=@im=fcitx
set +e
"$@"
rc=$?
set -e
exit "$rc"
