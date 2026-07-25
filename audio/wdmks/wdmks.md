# WDM Kernel Streaming

Talking to the audio driver directly, underneath WASAPI and everything else.
The lowest latency available on Windows without a third-party driver.

```
wdmks/
  ks.cst        the kernel streaming interface
  backend.cst   audio/device.cst implemented over it
```

---

## What it is

Every Windows audio API eventually becomes a kernel streaming pin on a WDM
driver. This backend skips the layers and opens the pin itself: no system mixer,
no shared-mode resampling, and latency limited by the hardware rather than by
software.

It is the Windows equivalent of what [ALSA](../alsa/alsa.md) already is on Linux
— the kernel interface with nothing on top — except that where ALSA is the
ordinary way to reach Linux audio, this is emphatically not the ordinary way to
reach Windows audio.

---

## What it costs

**Complexity.** `ks.h` is 118 KB of property sets, pin descriptors and
`DeviceIoControl` calls against GUIDs. There is no friendly path: the interface
is designed for drivers to talk to each other, and an application using it is
using something not meant for it.

**Exclusivity.** Like WASAPI's exclusive mode but more so — the device is taken
and nothing else plays.

**Fragility.** Behaviour varies with the driver, and drivers vary with the
manufacturer. This is the backend most likely to work perfectly on the developer's
machine and not on a user's.

The comparison worth making is ASIO, which achieves similar latency with a much
better interface — and which is a Steinberg SDK with licensing terms, so it is
not something this library can carry.

---

## Order of work

Last, and only if a program appears that genuinely needs it. WASAPI in exclusive
mode is within a few milliseconds for a fraction of the effort, and that is where
the effort belongs first.
