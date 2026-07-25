# PipeWire

What a modern Linux desktop actually runs. It replaced PulseAudio for consumer
audio and JACK for professional audio at once, and it handles video streams and
screen capture on the same bus.

```
pipewire/
  protocol.cst  the wire protocol — objects, methods, events
  backend.cst   audio/device.cst implemented over it
```

---

## Why bother, when ALSA already works

PipeWire emulates ALSA, so [`alsa/`](../alsa/alsa.md) plays here without knowing.
Speaking PipeWire natively buys what the emulation cannot:

- **Lower latency.** The emulation adds a hop; a native client sits in the graph.
- **Per-application volume**, and appearing by name in the system's mixer.
- **Device hotplug** — a headset plugged in mid-game moves the stream.
- **Routing** — a program that wants to be recorded, or to record another
  program's output, is asking the graph for a connection.

None of that is needed to make a sound, all of it is what users expect from an
application rather than from a test tone.

---

## It is a graph, not a device

The model is different enough from ALSA that the backend is not a translation of
the same code.

A client creates **nodes** with **ports**, and the session manager connects them
to whatever should be playing. So a program does not open "the speakers" — it
declares a stream with a role, a format and a latency requirement, and the graph
decides where it lands. Which is why the same program follows the user's default
device changing under it, and why the ALSA path does not.

Buffers are shared memory passed by file descriptor, and the process is woken by
an eventfd when the graph needs the next quantum. So the deadline described in
[`../audio.md`](../audio.md) applies here in exactly the same way, with the graph
rather than the hardware setting it.

---

## Two transports, and one prerequisite

`libpipewire-0.3` is 827 KB and drags libc.

The protocol is a socket at `$XDG_RUNTIME_DIR/pipewire-0` — object ids, method
opcodes and file descriptors, the same *shape* as Wayland though not the same
protocol. Speaking it directly needs the same thing Wayland needs and
`std/os/linux.cst` does not have: **`sendmsg` with `SCM_RIGHTS`**, since the
shared buffers cross the socket as file descriptors, and **`memfd_create`** to
make them.

That prerequisite is shared with Wayland, PulseAudio and JACK, which is four
reasons to put it in the standard library.

---

## Order of work

After ALSA, and after the syscalls it shares with Wayland exist. Third or fourth
in the layer — a program makes sound long before this, and this is what makes it
behave like an application.
