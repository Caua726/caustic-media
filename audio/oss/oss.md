# OSS

The original Unix audio interface: `open` a device, `write` samples to it. Legacy
on Linux, and **the native interface on FreeBSD and NetBSD**.

```
oss/
  backend.cst   audio/device.cst implemented over the ioctls
```

---

## Why it is worth having

Two reasons, and neither is Linux.

**It is how a Caustic program reaches BSD.** FreeBSD and NetBSD use OSS as their
own sound interface, not as a compatibility layer. Without this backend, those
systems have no audio at all; with it, they need nothing else.

**It is the simplest audio interface that exists.** Open `/dev/dsp`, three ioctls
to set format, channels and rate, then `write` and the samples play. No parameter
negotiation, no state machine, no underrun recovery protocol. That makes it a
good place to prove the layer's own abstraction: if `audio/device.cst` fits over
OSS as naturally as over ALSA, the abstraction is at the right level.

On Linux it is emulated when present and often absent entirely — `/dev/dsp` does
not exist on the development machine. So this is a BSD backend that happens to
also work on some Linux systems, rather than the other way round.

---

## The interface

`SNDCTL_DSP_SETFMT`, `SNDCTL_DSP_CHANNELS`, `SNDCTL_DSP_SPEED` to configure;
`SNDCTL_DSP_SETFRAGMENT` to choose the buffer size, which is the latency knob;
then ordinary `write`. `SNDCTL_DSP_GETOSPACE` says how much room is left, which
is what a callback loop waits on.

Modern OSS — OSS4 on the BSDs — adds per-application volume and better mixing,
and it is a superset, so a backend written to the classic ioctls works on both.

---

## Order of work

Directly after ALSA. It is a short step from that code, it brings a whole
operating-system family with it, and it tests whether the abstraction generalises
before four more backends are built on it.
