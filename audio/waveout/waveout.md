# waveOut

Windows' original audio API, from 1991. High latency, no mixer control, and
**the only Windows audio backend that is not COM** — which is exactly why it goes
first.

```
waveout/
  mmeapi.cst    the flat functions — 102 of them in mmeapi.h
  backend.cst   audio/device.cst implemented over them
```

---

## Why the worst one is the first one

Four of Windows' five audio APIs are COM, which means the vtable extractor that
[`gpu/d3d`](../../gpu/d3d/d3d.md) needs must exist before any of them can be
bound. waveOut is flat C: `waveOutOpen`, `waveOutPrepareHeader`, `waveOutWrite`,
`waveOutReset`, `waveOutClose`.

So a Windows build makes a sound with nothing more than the extern mechanism
Caustic already has, months before the COM machinery is ready. The same reasoning
as taking KMS's legacy `SETCRTC` before atomic modesetting: get a picture, then
get a good one.

---

## What it costs

**Latency of tens of milliseconds.** The API predates any notion of low-latency
audio, and the mixing happens in the system with buffers sized for 1991. Fine for
a menu click, audible for a rhythm game.

**No device enumeration worth the name**, no per-application volume, no format
negotiation beyond trying and being refused.

**A callback that runs on a system thread** with severe restrictions on what may
be called from it — the documented advice is to signal an event and do the work
elsewhere, which is what the layer's ring buffer arrangement does anyway.

---

## Shape

Buffers are `WAVEHDR` structures prepared, queued and returned as they finish, so
the model is a small pool of buffers cycled forever rather than a callback asking
for the next quantum. That maps onto the layer's design without difficulty: the
ring buffer already exists because the real callback cannot allocate.

---

## Order of work

First of the Windows backends and early in the layer overall, because it is the
cheapest proof that the abstraction survives leaving Linux.
