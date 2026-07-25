# ALSA

The kernel's own audio interface, reached by ioctl. **No library, no
dependency** — the same shape as [KMS](../../window/kms/kms.md) in the window
layer, and the reason audio is the second subsystem where the manifesto holds
without compromise.

```
alsa/
  pcm.cst       playback and capture — the part that makes sound
  ctl.cst       the mixer: volume, mute, routing
  midi.cst      rawmidi and UMP, if MIDI is ever in scope
  backend.cst   audio/device.cst implemented over them
```

---

## The surface

`sound/asound.h` defines six ioctl families:

| | What it is |
|---|---|
| `SNDRV_PCM_IOCTL` | **33 ioctls** — playback and capture. The whole of making sound |
| `SNDRV_CTL_IOCTL` | the mixer: elements, volume, mute, enumerated routing |
| `SNDRV_RAWMIDI_IOCTL` | MIDI 1.0 byte streams |
| `SNDRV_UMP_IOCTL` | MIDI 2.0 universal packets |
| `SNDRV_TIMER_IOCTL` | the sound timer, for synchronisation |
| `SNDRV_HWDEP_IOCTL` | device-specific escape hatch |

Only the first is needed to play audio. The second is needed to change the
volume, which programs expect to be able to do.

`libasound` is 994 KB and is not on the path here: it is a configuration engine
that resolves names like `default` and `plughw:0,0` through a text configuration
language, plus a format converter for what the hardware refuses. We know which
device we opened, and format conversion is `audio/convert.cst`, which exists
anyway for mixing.

---

## Opening a device

Devices are `/dev/snd/pcmC<card>D<device><p|c>` — `p` for playback, `c` for
capture. On the development machine: `pcmC0D0p`, `pcmC1D3p` and several more,
one per HDMI output.

The sequence:

1. `open` the device node
2. `HW_REFINE` / `HW_PARAMS` — negotiate format, rate, channels, period and
   buffer size. The kernel narrows a set of constraints rather than accepting a
   request outright, which is why refine exists separately
3. `SW_PARAMS` — when to wake the process, when to start
4. `PREPARE`
5. `WRITEI` or `WRITEN` to hand over frames — interleaved or not
6. `START`

Period and buffer size are the latency knob and the only one that matters:
period is how much audio the kernel takes at a time, buffer is how much it holds.
Small periods mean low latency and a callback that must be quick; large ones mean
the opposite. This is where the deadline described in
[`../audio.md`](../audio.md) comes from.

---

## Underruns are normal and must be handled

If the process does not deliver in time, the device stops in the `XRUN` state and
stays there. Every subsequent write fails until `PREPARE` is issued again.

A backend that treats this as an error kills the sound permanently on the first
scheduling hiccup. A backend that handles it recovers with a click. It is not an
edge case — it happens on a loaded machine, and it is the first thing to get
right after sound comes out at all.

---

## Two ways to wait

`poll` on the device fd tells the process when space is available, which fits a
callback driven by a thread that sleeps.

`mmap` of the ring buffer plus `SYNC_PTR` avoids the copy entirely and is how
low-latency audio is actually done — the process writes into memory the hardware
reads. Worth having eventually, not worth having first.

---

## And it covers more machines than it looks

PipeWire emulates ALSA. PulseAudio emulates ALSA. So a program written to this
interface plays on a modern desktop, on an older one, and on a machine running
neither — which makes the zero-dependency path the *default* rather than the
fallback, and is unusual enough to be worth saying twice.

---

## MIDI is here but out of scope

`RAWMIDI` and `UMP` are in the same header and would be the natural home for MIDI
input and output. Nothing in the library needs it, SDL does not have it, and it
is a different domain from playing samples — noted so that if it is ever wanted,
it is known where it lives rather than being discovered as missing.

---

## Order of work

First backend of the layer, and the one the others are measured against.

1. **`pcm.cst`** — open, negotiate, write, start. A sine wave out of a real
   speaker with no dependency.
2. **Underrun recovery**, before anything else is built on top.
3. **`ctl.cst`**, so volume works.
4. **mmap and `SYNC_PTR`**, when latency starts to matter.
