#!/bin/sh
# gpu/vk/tools/check_isolation.sh — bind/ is a binding, and stays one.
#
# The whole point of gpu/vk/bind/ is that a program wanting to drive Vulkan
# itself can take it and leave the rest of the library behind — which is what
# the README promises when it says a program "reaches past it to the raw
# bindings, and leaves the rest".
#
# That promise decays quietly. Someone needs a matrix, imports math/; someone
# wants a nicer error, imports gpu/device.cst for its error codes; and one
# release later the binding drags the abstraction it was supposed to sit under.
# Nothing fails, nothing looks wrong, and the property is gone.
#
# So it is asserted instead of intended. bind/*.cst may import the standard
# library and each other. Nothing else.
#
# Exit code is the number of forbidden imports.
set -u

cd "$(dirname "$0")/.." || exit 1

fails=0

# A use line is `use "<path>" as <alias>;`. Anything reaching outside bind/ that
# is not std/ is a violation — including a relative path climbing out with ../
for f in bind/*.cst; do
    [ -f "$f" ] || continue
    sed -n 's/^[[:space:]]*use[[:space:]]*"\([^"]*\)".*/\1/p' "$f" \
    | while read -r path; do
        case "$path" in
            std/*)      continue ;;
            */*)        echo "$f: $path" ;;
            *.cst)      continue ;;   # a sibling inside bind/
            *)          echo "$f: $path" ;;
        esac
    done
done > "${TMPDIR:-/tmp}/vk_iso_$$"

if [ -s "${TMPDIR:-/tmp}/vk_iso_$$" ]; then
    echo "FAIL  bind/ importando fora da stdlib:"
    sed 's/^/        /' "${TMPDIR:-/tmp}/vk_iso_$$"
    echo
    echo "      bind/ e um binding, nao uma camada. Se o binding precisa disso,"
    echo "      quem precisa e o backend — poe em gpu/vk/backend.cst."
    fails=$(wc -l < "${TMPDIR:-/tmp}/vk_iso_$$")
fi
rm -f "${TMPDIR:-/tmp}/vk_iso_$$"

# --- Why there is no second check for layer types leaking in ---
#
# The obvious companion rule is "no gpu/device.cst type may appear in bind/'s
# surface", grepping for something like `device\.`. It was written and removed,
# because it does not work: the alias is chosen at the import site, so a file
# that says `use "../../device.cst" as dev;` and then `*dev.Device` matches
# nothing the grep looks for. A rule that catches a violation only when the
# author happened to pick the expected alias is worse than no rule, because it
# reads as coverage.
#
# The import check above is complete on its own. Caustic has no way to name a
# type from another module without a `use`, so a layer type cannot get in
# without a forbidden import, and a forbidden import is already a failure here.

if [ "$fails" -eq 0 ]; then
    n=$(ls bind/*.cst 2>/dev/null | wc -l)
    echo "check_isolation: ok  ($n arquivos, nenhum importa fora da stdlib)"
fi
exit "$fails"
