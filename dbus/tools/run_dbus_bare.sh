#!/bin/sh
# Runs a command on a private session bus that starts none of the desktop's
# services, only the stand-ins in dbus/testdata/services: started, each
# writes its name to the file CAUSTIC_DBUS_STARTED names, and does nothing
# else, so the name stays unowned. The command reads that file to know what
# it caused to be started; the file is removed afterwards.
here=$(cd "$(dirname "$0")/.." && pwd)
CAUSTIC_DBUS_STARTED=$(mktemp "${TMPDIR:-/tmp}/caustic-dbus-started.XXXXXX") || exit 1
export CAUSTIC_DBUS_STARTED
dbus-run-session --config-file="$here/testdata/bare.conf" -- "$@"
status=$?
rm -f "$CAUSTIC_DBUS_STARTED"
exit $status
