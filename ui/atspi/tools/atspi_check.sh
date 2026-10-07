#!/bin/sh
# Cross-checks the bridge against the desktop's own AT-SPI stack — its
# accessibility bus, its registry, and libatspi reading through them, as Orca
# does — on a private session bus of its own, never the desktop's:
#   sh ui/atspi/tools/atspi_check.sh ./build/atspi_peer
# The private bus starts org.a11y.Bus (at-spi-bus-launcher) when first asked
# for it. It runs without a display, so the launcher has no X root window to
# write the bus's address on, and with settings kept in memory, so turning
# accessibility on is never written to the user's. The buses log to a file,
# shown when something fails.
if [ -z "$ATSPI_CHECK_INSIDE" ]; then
    log=$(mktemp)
    env -u DISPLAY -u WAYLAND_DISPLAY GSETTINGS_BACKEND=memory ATSPI_CHECK_INSIDE=1 \
        dbus-run-session -- sh "$0" "$@" 2>"$log"
    code=$?
    if [ $code -ne 0 ]; then sed '/systemd1/d' "$log"; fi
    rm -f "$log"
    exit $code
fi
peer="$1"
here=$(dirname "$0")
# Accessibility turned on, as a desktop's settings would.
if ! gdbus call --session --dest org.a11y.Bus --object-path /org/a11y/bus \
        --method org.freedesktop.DBus.Properties.Set org.a11y.Status IsEnabled "<true>" >/dev/null; then
    echo "FAIL: no org.a11y.Bus to turn on"
    exit 1
fi
# The registry, started here: a bus that starts services through systemd
# (dbus-broker) has none to start it on a private bus. It finds the
# accessibility bus through org.a11y.Bus, as everything else does.
/usr/lib/at-spi2-registryd >/dev/null &
registry=$!
address=$(gdbus call --session --dest org.a11y.Bus --object-path /org/a11y/bus --method org.a11y.Bus.GetAddress |
    sed "s/^('//; s/',)\$//")
i=0
until gdbus call --address "$address" --dest org.freedesktop.DBus --object-path /org/freedesktop/DBus \
        --method org.freedesktop.DBus.NameHasOwner org.a11y.atspi.Registry 2>/dev/null | sed -n '/true/q0; q1'; do
    i=$((i + 1))
    if [ $i -gt 100 ]; then
        echo "FAIL: the registry never came"
        kill $registry
        exit 1
    fi
    sleep 0.05
done
"$peer" &
pid=$!
python3 -I "$here/atspi_check.py"
code=$?
wait $pid
pcode=$?
kill $registry 2>/dev/null
if [ $pcode -ne 0 ]; then
    echo "FAIL: the peer exited with $pcode"
    code=1
fi
exit $code
