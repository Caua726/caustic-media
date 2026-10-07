# D-Bus

The message bus Linux desktops talk over: the settings portal (dark mode, the
accent, fonts), the file chooser portal, the accessibility bus (AT-SPI), the
tray (StatusNotifierItem), notifications. `window/` and `ui/` reach all of
them through this directory, which speaks the protocol itself — no libdbus, no
sd-bus, nothing linked.

It lives in caustic-media rather than in a package of its own: the toolkit is
what needs it, and one codebase is one thing to build and test.

---

## What is here

```
dbus/
  dbus.cst      hub
  address.cst   DBUS_SESSION_BUS_ADDRESS: unix:path=, unix:abstract=, the escapes
  wire.cst      the wire format: a Writer and a Reader, alignment, signatures
  message.cst   the header and its fields; building and parsing a message
  conn.cst      a connection: authenticate, Hello, call, signals, exported objects

sys/linux.cst   the syscalls under it that the standard library lacks:
                AF_UNIX sockets, sendmsg/recvmsg with SCM_RIGHTS, getuid
```

**The wire format is the whole protocol.** Every value is aligned to its own
size inside the message, arrays carry their length in bytes, a variant carries
its own signature. `wire.cst` writes little-endian and reads both orders, as
the specification asks of every peer. Reading is bounds-checked everywhere: a
message is something another process sent, and a length that runs past the
end makes the Reader fail, never read past it.

**One connection, no thread.** `conn.cst` owns a socket and two buffers sized
when it opens. `call` sends and waits for the reply with that serial;
whatever arrives first — signals, calls to objects this program exports — is
dispatched on the way, so a blocking call never loses a message. A program
with a main loop polls `fd` and calls `dispatch` when it is readable instead.

**Authentication is EXTERNAL** with the process's uid, which every bus on
Linux accepts, and asks for Unix fd passing, which the portals use to hand
over files and the accessibility bus does not need.

**Exporting objects** is a table: a path, its interfaces, and for each the
methods (name, signatures, handler), properties (name, signature, getter,
optional setter) and signals it emits. `Introspect`, `Properties` and `Peer`
are answered from the table, so what `busctl introspect` shows is what the
object does. AT-SPI is the reason this is not optional.

**Sizes are declared.** A connection is opened with `Limits`: buffer bytes, how
many objects, match rules and handlers it can hold. A message that does not
fit is an error reported to the caller, never a buffer grown behind its back.

## Testing

Against a private bus, never the session's: `tools/run_dbus.sh` starts one with
`dbus-run-session` and runs a test inside it. The tests call the bus itself,
export objects and call them from a second connection, and cross-check with
`busctl`, a second implementation, in both directions.

That private bus still starts the desktop's services when asked for one
nobody owns — a real notification daemon, the portal — as the session's
does. A test that must not, or must know whether it asked, runs on
`tools/run_dbus_bare.sh` instead (`testdata/bare.conf`): only the stand-ins
in `testdata/services` can be started there, and each, started, only writes
its name to the file `CAUSTIC_DBUS_STARTED` names, its start given up on
half a second later.

## Current state

All of the above, tested three ways.

- `wire_test`, `message_test`, `address_test`: the format byte for byte —
  alignment, array lengths without their padding, a Hello encoded to the exact
  128 bytes, a big-endian signal written by hand — and everything a Reader or
  parse refuses: truncation, lengths past the end, non-zero padding, booleans
  that are not 0 or 1, strings without their zero, signatures nested past 32,
  variants past 32 deep, fields of the wrong type, bodies that are not their
  signature.
- `conn_test`, on a private bus: two connections, one exporting an object and
  the other calling it — methods, errors, wrong arguments, unknown objects,
  interfaces and methods, Get/GetAll/Set, Introspect at the object, on the way
  to it and at the root, Peer, signals and PropertiesChanged, descriptors
  carried both ways and closed when not taken, several calls in flight,
  timeouts, the serial wrapping, handlers' blocking calls refused. A forked
  fake bus misbehaves on purpose: refusing the AUTH and carrying on, ending
  lines with a bare "\n", answering Hello with a number, refusing descriptors,
  sending two messages with both their descriptors in one write.
- `tools/busctl_check.sh`: busctl (sd-bus) calls an exported object, reads and
  sets its properties, decodes a{sv} with structs, arrays and 64-bit values,
  and parses its introspection and its tree.

The bus in the tests is dbus-daemon (libdbus); this desktop's session bus is
dbus-broker, which window/linux/portal.cst's check reads from. Every module was mutation-tested: the mutants that
survived were either given a test or, where they could not change anything,
the code they changed was removed — except one, sys/'s CMSG_SPACE rounding,
which POSIX asks for and Linux does not check.
