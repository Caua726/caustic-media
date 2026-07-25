# PulseAudio

The sound server that was standard on Linux desktops for over a decade, and is
still what many systems run — and what remote-audio setups and older distributions
expect.

```
pulse/
  protocol.cst  the native wire protocol
  backend.cst   audio/device.cst implemented over it
```

---

## Where it sits now

PipeWire replaced it on current desktops, and provides a PulseAudio-compatible
server, so a native PulseAudio client works on both. That is the argument for
implementing it: **one backend covers PulseAudio systems and PipeWire systems
alike**, at the cost of PipeWire's lower latency.

So the ordering between this and [`pipewire/`](../pipewire/pipewire.md) is a real
choice rather than obvious. This one reaches more machines; that one behaves
better on the machines that matter most now.

---

## The protocol

A socket at `$XDG_RUNTIME_DIR/pulse/native`, with authentication by a cookie in
`~/.config/pulse/cookie` — the same idea as X11's magic cookie, and the same
work: read the file, present it in the handshake.

The protocol is tag-value: each message is a command, a sequence number and typed
fields. It is well documented, it has been stable for years, and it is simpler to
speak than PipeWire's graph model — there is no session manager to negotiate
with, just a stream you create and write to.

Shared-memory playback passes file descriptors over the socket and therefore
needs the same **`sendmsg`/`SCM_RIGHTS`** work as Wayland and PipeWire. Without
it there is a simpler path: the protocol also accepts samples inline over the
socket, which costs a copy per buffer and works with nothing more than `write`.

That makes PulseAudio the **only sound server reachable with the standard library
as it stands today**, which is worth knowing when deciding what to build first.

`libpulse` is 314 KB — the smallest of the three sound-server libraries, and
still a dependency we do not need.

---

## Order of work

Its position depends on what is wanted: the earliest sound server we can speak
without new syscalls, or second to PipeWire once those syscalls exist.
