# WASAPI

Windows' modern audio API, since Vista. The one a program should use, and the one
every other current Windows audio path is built on top of.

```
wasapi/
  mmdevice.cst  device enumeration — 8 interfaces, 64 methods
  audioclient.cst  the stream — 12 interfaces, 155 methods
  backend.cst   audio/device.cst implemented over them
```

---

## Two modes, and the difference is the whole point

**Shared mode** mixes with everything else on the system. The format is whatever
the system mixer is running at, so a stream is resampled to match, and latency is
around 10 ms. This is what an application uses.

**Exclusive mode** takes the device. The hardware's own format, no system mixer,
latency in low single-digit milliseconds — and nothing else on the machine can
make a sound while it is held. This is what a program that cares about latency
uses, and what it must not do by default.

Offering both, and defaulting to shared, is the whole design decision here.

---

## COM, and 219 methods

`mmdeviceapi.h` and `audioclient.h` between them declare 20 interfaces and 219
methods — `IMMDeviceEnumerator`, `IMMDevice`, `IAudioClient`,
`IAudioRenderClient`, `IAudioCaptureClient`, `ISimpleAudioVolume` and the rest.

Every call is a vtable slot fixed by declaration order including the `IUnknown`
ancestry, with the silent-failure risk described in
[`gpu/d3d`](../../gpu/d3d/d3d.md). So this backend waits on the COM extractor,
and shares it with Direct3D — which is the argument for building that tool once
and well.

Reference counting comes with it, and the same rule applies: the backend owns
`AddRef`/`Release` and never lets a refcount escape into the abstraction.

---

## The event-driven path

`IAudioClient` can be initialised to signal an event when it wants the next
buffer, rather than being polled. That is the low-latency arrangement and the one
that matches the layer's callback contract exactly: the event fires, the callback
fills the buffer, nothing waits.

The polled alternative exists and is worse in every respect except that it is
easier to get running first.

---

## Order of work

After the COM extractor, and it is the Windows backend that matters — waveOut is
a bootstrap, this is the destination. Device enumeration and shared mode first,
then event-driven, then exclusive mode as an opt-in a program has to ask for.
