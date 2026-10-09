#!/bin/sh
# Each listed program in turn, in one Wine prefix and X server of their own;
# the first failure stops the group.
set -eu
[ $# -gt 0 ] || { echo "usage: run_native.sh <program.exe>..." >&2; exit 2; }
exec sh tools/wine_session.sh sh -c 'for program do sh tools/run_wine.sh "$program" || exit; done' run_native "$@"
