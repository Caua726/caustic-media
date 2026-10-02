#!/bin/sh
# Runs a command on a private session bus of its own, never the desktop's:
# dbus-run-session starts a dbus-daemon, sets DBUS_SESSION_BUS_ADDRESS for the
# command, and stops the daemon when it exits.
exec dbus-run-session -- "$@"
