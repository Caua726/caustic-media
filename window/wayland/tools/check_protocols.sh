#!/bin/sh
# Generate the Caustic Wayland metadata from the protocol XML, then compare it
# with wayland-scanner's own tables for the same files: interface names and
# versions, message opcodes, since versions, signatures and argument
# interfaces must match exactly.
set -eu
command -v wayland-scanner >/dev/null 2>&1 || { echo "check_protocols: wayland-scanner not found" >&2; exit 127; }
run=$(mktemp -d "${TMPDIR:-/tmp}/caustic-protocols.XXXXXX")
cleanup() { rm -rf "$run"; }
trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM
core=/usr/share/wayland/wayland.xml
protocols=/usr/share/wayland-protocols
# Append new files: the generated interface ids follow this order.
xmls="
$core
$protocols/stable/xdg-shell/xdg-shell.xml
$protocols/stable/viewporter/viewporter.xml
$protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml
$protocols/unstable/text-input/text-input-unstable-v3.xml
$protocols/unstable/primary-selection/primary-selection-unstable-v1.xml
$protocols/staging/fractional-scale/fractional-scale-v1.xml
$protocols/staging/cursor-shape/cursor-shape-v1.xml
$protocols/staging/xdg-activation/xdg-activation-v1.xml
$protocols/staging/xdg-dialog/xdg-dialog-v1.xml
$protocols/unstable/xdg-foreign/xdg-foreign-unstable-v2.xml
$protocols/staging/xdg-toplevel-icon/xdg-toplevel-icon-v1.xml
$protocols/stable/presentation-time/presentation-time.xml
$protocols/staging/single-pixel-buffer/single-pixel-buffer-v1.xml
$protocols/staging/xdg-system-bell/xdg-system-bell-v1.xml
$protocols/staging/xdg-toplevel-drag/xdg-toplevel-drag-v1.xml
$protocols/stable/tablet/tablet-v2.xml
$protocols/unstable/pointer-gestures/pointer-gestures-unstable-v1.xml
"
i=0
list=""
: > "$run/interfaces.h"
for xml in $xmls; do
    [ -r "$xml" ] || { echo "check_protocols: missing protocol XML: $xml" >&2; exit 2; }
    wayland-scanner private-code "$xml" "$run/protocol-$i.c"
    for name in $(sed -n 's/^WL_PRIVATE const struct wl_interface \([A-Za-z0-9_]*\) = {$/\1/p' "$run/protocol-$i.c"); do
        echo "extern const struct wl_interface $name;" >> "$run/interfaces.h"
        list="$list &$name,"
    done
    i=$((i + 1))
done
echo "static const struct wl_interface *const all[] = {$list };" >> "$run/interfaces.h"
cc -std=c11 -Wall -Wextra -Werror -I"$run" window/wayland/tools/scanner_dump.c "$run"/protocol-*.c -o "$run/scanner_dump"
python3 window/wayland/tools/generate.py --out window/wayland/protocol.cst $xmls
"$run/scanner_dump" | LC_ALL=C sort > "$run/scanner.txt"
awk -F'[ ]' '
    $1 == "I" { print "I", $3, $4, $5, $6 }
    $1 == "M" { sig = $7; sub(/^[0-9]+:/, "", sig); print "M", $2, $3, $4, $5, $6, sig, $8 }
' window/wayland/protocol.txt | LC_ALL=C sort > "$run/caustic.txt"
diff -u "$run/scanner.txt" "$run/caustic.txt"
echo "wayland protocols: $(grep -c '^I ' "$run/caustic.txt") interfaces and $(grep -c '^M ' "$run/caustic.txt") messages match wayland-scanner"
caustic -q window/wayland/protocol_test.cst -o build/wayland_protocol_test
./build/wayland_protocol_test
