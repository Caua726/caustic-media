# CausticOS audio

Our own kernel, and therefore the one backend where both sides of the interface
are ours to design.

```
causticos/
  backend.cst   audio/device.cst implemented over the kernel's own calls
```

---

## The one place the interface is a choice

Every other backend adapts to something already decided by somebody else. Here
the kernel's audio syscalls and this library's expectations can be designed
against each other, so the layer's requirements become the specification.

What the layer needs is small and known: a device to open, a format to negotiate,
a buffer of frames to hand over, and a signal when the next one is wanted. That
is what ALSA's 33 PCM ioctls express, and most of them exist to describe hardware
we would not be describing the same way.

So the sensible shape is the one CausticOS already uses elsewhere —
`dev_open(DEV_AUDIO)`, a surface-style mmap for the ring, and an event when space
frees — matching how `std/causticos/gfx.cst` reaches the framebuffer today.

---

## Why it matters more than its user count

CausticOS is where the whole library runs with **no dependency anywhere in the
stack**: KMS-style scanout that is ours, a software rasterizer that is ours, and
audio that is ours. It is the configuration that proves the design was not
quietly leaning on Linux the entire time.

---

## Order of work

Follows the kernel. Nothing here can be built before the kernel has an audio
device, and when it does, this backend is the smallest of the twelve.
