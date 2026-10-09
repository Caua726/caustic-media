#!/bin/sh
# tools/run_wine.sh — a Windows program of this repository run under wine:
# its own console output, wine's own noise off, and a bound on how long it
# may take — a program that hangs fails rather than holding the suite.
#
#   sh tools/run_wine.sh build/program.exe [args]
#
# Outside a session of tools/wine_session.sh it starts one for itself, so it
# never runs in the user's ~/.wine or on the user's display.
set -eu
if [ -z "${CAUSTIC_WINE_SESSION:-}" ] || [ "${CAUSTIC_WINE_SESSION}" != "${WINEPREFIX:-}" ]; then
    exec sh "$(dirname "$0")/wine_session.sh" sh "$0" "$@"
fi
exec timeout 120 wine "$@"
