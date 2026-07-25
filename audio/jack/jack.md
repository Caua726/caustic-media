# JACK

Professional audio: sample-accurate, callback-driven, and how a program joins a
studio's signal graph alongside a DAW, a synthesiser and a set of effects.

```
jack/
  protocol.cst  the wire protocol
  backend.cst   audio/device.cst implemented over it
```

---

## What it is for

Not for a game. JACK exists because musicians need every application in the
machine running on **one clock, in one graph, at a latency measured in single
milliseconds**, with connections a user rewires while everything is playing.

A program that supports it is a program a musician can put inside their setup
instead of alongside it. That is a small audience and a real one, and it costs
one backend.

PipeWire provides a JACK-compatible interface, so this reaches both — the same
compatibility argument PulseAudio has.

---

## The callback is the whole model

JACK does not let a client decide when to run. The server calls every client in
dependency order, once per period, and each has the entire period to fill its
output ports — with **no allocation, no locks, no syscalls**, because a client
that misses its slot is removed from the graph.

That is the deadline described in [`../audio.md`](../audio.md) in its strictest
form. A backend that satisfies JACK satisfies everything else, which makes it a
good discipline to design the callback contract against even before it is
implemented.

Ports are typed and named, and a client publishes them rather than opening a
device. Connection is somebody else's decision — the user's, or the session
manager's.

---

## Transport

`libjack` is 238 KB. The protocol is a unix socket plus shared memory for the
audio buffers, so speaking it directly needs the same **`sendmsg`/`SCM_RIGHTS`**
that Wayland, PipeWire and PulseAudio need.

---

## Order of work

Last of the Linux backends. Nothing depends on it and it serves a specific
audience — but the callback contract it demands is worth designing for from the
start, because loosening a contract later is easy and tightening one is not.
