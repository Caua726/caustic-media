#!/bin/sh
# window/x11/tools/check_needed.sh — declaring a binding must stay free.
#
#   check_needed.sh <binary> <expected-count> [soname...]
#
# The whole reason binding all 1035 symbols is reasonable is that the compiler
# emits a library import at the CALL SITE rather than at the declaration
# (src/codegen/emit_inst.cst, inside the IR_FFI_CALL lowering). A module holding
# 407 unused externs therefore costs a binary nothing, and the hub in bind/ can
# import every backend's bindings without dragging six shared objects into every
# program.
#
# That is an assumption about the compiler, not a law, and it is exactly the kind
# of thing that changes quietly under a toolchain upgrade. If it ever stops being
# true, a minimal window program starts requiring libXrandr, libXcursor,
# libXfixes and libXi at load time — on a machine that may not have them, for
# features it never uses. The failure would appear on someone else's computer.
#
# So it is asserted rather than assumed, on every CI run.
#
# Exit code is 0 when the count matches.
set -u

[ $# -ge 2 ] || { echo "uso: check_needed.sh <binario> <n> [soname...]" >&2; exit 2; }

bin=$1
want=$2
shift 2

[ -f "$bin" ] || { echo "check_needed: $bin nao existe" >&2; exit 1; }
command -v readelf >/dev/null 2>&1 || {
    echo "check_needed: readelf nao encontrado (apt: binutils)" >&2; exit 127
}

got=$(readelf -d "$bin" | grep -c 'NEEDED')

if [ "$got" -ne "$want" ]; then
    echo "FAIL  $bin tem $got NEEDED, esperado $want:"
    readelf -d "$bin" | grep 'NEEDED' | sed 's/^/        /'
    echo "      Uma contagem maior significa que o import esta sendo emitido na"
    echo "      declaracao em vez do call site — e ai declarar uma binding deixou"
    echo "      de ser gratis."
    exit 1
fi

# Optional: the sonames themselves, when the caller names them.
for so in "$@"; do
    if ! readelf -d "$bin" | grep -q "NEEDED.*$so"; then
        echo "FAIL  $bin nao lista $so"
        exit 1
    fi
done

echo "check_needed: $bin -> $got NEEDED, ok"
exit 0
