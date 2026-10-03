#!/bin/sh
# tools/run_wine.sh — a Windows program of this repository run under wine:
# its own console output, wine's own noise off, and a bound on how long it
# may take — a program that hangs fails rather than holding the suite.
#
#   tools/run_wine.sh build/program.exe [args]
WINEDEBUG=-all exec timeout 120 wine "$@"
