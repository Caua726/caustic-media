#!/bin/sh
# Regenerate input/xkb/testdata's keymaps from xkeyboard-config with the
# installed libxkbcommon (xkbcli), each stamped with what made it.
#
#   sh input/xkb/tools/make_testdata.sh
#
# weston-*.xkb and kwin-*.xkb are what those compositors sent a client
# (xkbcli dump-keymap-wayland --raw); actions.xkb is written by hand. Both
# stay as they are.
set -eu
here=$(cd "$(dirname "$0")/.." && pwd)
out="$here/testdata"
mkdir -p "$out"
version=$(xkbcli --version)

# name, then xkbcli compile-keymap's options
keymap() {
    name=$1; shift
    {
        echo "// xkbcli $version compile-keymap --output-format 1 $*"
        xkbcli compile-keymap --output-format 1 "$@"
    } > "$out/$name.xkb"
}

# Hex keysyms, unused types and interpretations dropped: what compositors send.
keymap us-hex --layout us --no-pretty --drop-unused
# Names; a second layout, Alt+Shift switching to it, AltGr, Caps Lock as
# Shift Lock.
keymap us-intl-ru --layout us,ru --variant intl, --options grp:alt_shift_toggle,lv3:ralt_switch,caps:shiftlock
# Three layouts, Scroll Lock cycling them, a third-level latch on Caps Lock,
# a fifth level and its lock on AltGr.
keymap de-il-gr --layout de,il,gr --options grp:sclk_toggle,lv3:caps_switch_latch,lv5:ralt_switch_lock
# Arabic with US, Left Alt switching while held, Compose on Right Ctrl.
keymap ara-us --layout ara,us --options grp:lswitch,compose:rctrl --no-pretty

# The syntax libxkbcommon before 1.8 wrote: groups, levels and map levels by
# name (symbols[Group1], level_name[Level1], map[Shift]= Level2).
{
    echo "// us-intl-ru.xkb in libxkbcommon's syntax before 1.8 (GroupN, LevelN)"
    sed -e 's/level_name\[\([0-9]\)\]/level_name[Level\1]/' \
        -e 's/symbols\[\([0-9]\)\]/symbols[Group\1]/' \
        -e 's/actions\[\([0-9]\)\]/actions[Group\1]/' \
        -e 's/type\[\([0-9]\)\]/type[Group\1]/' \
        -e 's/^\tname\[\([0-9]\)\]/\tname[Group\1]/' \
        -e 's/\(map\[[^]]*\]\)= \([0-9]\);/\1= Level\2;/' \
        "$out/us-intl-ru.xkb" | tail -n +2
} > "$out/us-intl-ru-old.xkb"
