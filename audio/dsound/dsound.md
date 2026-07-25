# DirectSound

Windows audio from 1995 to Vista. Deprecated, emulated on top of WASAPI since,
and present on every Windows machine that has ever existed.

```
dsound/
  dsound.cst    21 interfaces, 202 methods
  backend.cst   audio/device.cst implemented over them
```

---

## The compatibility floor

There is no technical reason to choose DirectSound on a current system: it is
slower than WASAPI, it is emulated rather than native, and Microsoft has
recommended against it for fifteen years.

The reason to have it is reach. Windows XP and Vista-era machines, virtualised
guests with cut-down audio stacks, and wine configurations where WASAPI is
imperfect all speak DirectSound. It is the backend that answers "it does not work
on my machine" for the machines nobody tests on.

That is a weak argument for building it early and a fine one for building it
eventually, which is where it sits.

---

## Shape

A **circular buffer the program writes into while the hardware reads**, with a
play cursor and a write cursor that must not cross. That is a genuinely different
model from both waveOut's queued headers and WASAPI's buffer requests, and it maps
onto the layer's ring buffer more directly than either.

21 interfaces and 202 methods, COM, so it waits on the same extractor as
[WASAPI](../wasapi/wasapi.md) and Direct3D.

---

## Order of work

Last of the Windows backends. Built when compatibility with old machines becomes
a goal rather than a hypothetical.
