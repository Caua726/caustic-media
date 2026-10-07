#!/bin/sh
# Validate every XML used by the Caustic metadata generator with wayland-scanner.
set -eu
command -v wayland-scanner >/dev/null 2>&1 || { echo "check_protocols: wayland-scanner not found" >&2; exit 127; }
run=$(mktemp -d "${TMPDIR:-/tmp}/caustic-protocols.XXXXXX")
cleanup() { rm -rf "$run"; }
trap cleanup EXIT HUP INT TERM
core=/usr/share/wayland/wayland.xml
protocols=/usr/share/wayland-protocols
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
"
i=0
for xml in $xmls; do
    [ -r "$xml" ] || { echo "check_protocols: missing protocol XML: $xml" >&2; exit 2; }
    wayland-scanner private-code "$xml" "$run/protocol-$i.h"
    i=$((i + 1))
done
python3 window/wayland/tools/generate.py --out window/wayland/protocol.cst $xmls
caustic -q window/wayland/protocol_test.cst -o build/wayland_protocol_test
./build/wayland_protocol_test
