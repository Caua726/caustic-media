#!/bin/sh
# Cross-checks dbus/ against busctl (sd-bus) on a private bus: run it through
# run_dbus.sh with the built peer, e.g.
#   sh dbus/tools/run_dbus.sh sh dbus/tools/busctl_check.sh ./build/busctl_peer
peer="$1"
fails=0
"$peer" &
pid=$!
# Wait for the name, at most two seconds.
i=0
while [ $i -lt 40 ] && ! busctl --user status org.caustic.Serve >/dev/null 2>&1; do
    sleep 0.05
    i=$((i + 1))
done

expect() {
    name="$1"; want="$2"; shift 2
    got=$("$@" 2>&1)
    if [ "$got" != "$want" ]; then
        echo "FAIL: $name: got [$got], want [$want]"
        fails=$((fails + 1))
    fi
}
contains() {
    name="$1"; want="$2"; shift 2
    got=$("$@" 2>&1)
    case "$got" in
        *"$want"*) ;;
        *) echo "FAIL: $name: [$want] not in [$got]"; fails=$((fails + 1)) ;;
    esac
}

O="org.caustic.Serve"; P="/org/caustic/Serve"
expect "a call" "i 5" busctl --user call $O $P $O Add ii 2 3
# busctl prints bytes past ASCII as octal escapes: these are á's two in UTF-8.
expect "a string, not ASCII" 's "ol\303\241, barramento"' busctl --user call $O $P $O Echo s "olá, barramento"
expect "a property" 's "first"' busctl --user get-property $O $P $O Label
busctl --user set-property $O $P $O Label s second
expect "a property set" 's "second"' busctl --user get-property $O $P $O Label
expect "values of every kind" 'a{sv} 4 "scheme" u 1 "accent" (ddd) 0.25 0.5 1 "names" as 2 "one" "two" "big" x -9000000000' \
    busctl --user call $O $P $O Values
contains "an error" "it says no" busctl --user call $O $P $O Fail
expect "Peer" "" busctl --user call $O / org.freedesktop.DBus.Peer Ping
contains "introspection: a method" ".Add" busctl --user introspect $O $P
contains "introspection: a property" ".Label" busctl --user introspect $O $P
contains "introspection: Properties" "org.freedesktop.DBus.Properties" busctl --user introspect $O $P
contains "the tree" "/org/caustic/Serve" busctl --user tree $O
# busctl prints an error's text, not its name.
contains "no such method" "no such method" busctl --user call $O $P $O Nope

busctl --user call $O $P $O Quit
wait $pid
code=$?
if [ $code -ne 0 ]; then echo "FAIL: the peer exited with $code"; fails=$((fails + 1)); fi
if [ $fails -gt 0 ]; then
    echo "dbus busctl: $fails failures"
    exit 1
fi
echo "dbus busctl: all checks passed"
