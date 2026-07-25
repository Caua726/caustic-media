# The audio layer

Getting samples to a speaker, mixing what a program wants to hear, and placing
sounds in space.

```
audio/
  audio.cst     hub
  device.cst    our abstraction: open an output, the callback, the format
  mixer.cst     voices, gain, panning, the summing bus
  convert.cst   sample formats and resampling
  source.cst    a playing sound: looping, seeking, streaming
  spatial.cst   3D position, distance attenuation, doppler
  decode.cst    file formats — opt-in
  music.cst     streamed playback, crossfade, the long-form path

  alsa/      /dev/snd — ioctls, no library
  pipewire/  the PipeWire socket protocol
  pulse/     the PulseAudio socket protocol
  jack/      the JACK socket protocol — pro audio
  oss/       /dev/dsp — legacy on Linux, native on BSD

  wasapi/    Windows, COM — the modern one
  xaudio2/   Windows, COM — what games use
  dsound/    Windows, COM — legacy, present everywhere
  waveout/   Windows, flat C — ancient, and the only one without COM
  wdmks/     Windows kernel streaming — lowest latency short of ASIO

  causticos/ our own kernel
```

---

## ALSA is ioctls, and that changes what this layer costs

`/dev/snd/pcmC0D0p` is a playback device, and `sound/asound.h` defines **33 PCM
ioctls**. That is the whole interface: open the device, set hardware parameters,
write frames, handle underruns.

`libasound` is 994 KB and is a configuration engine wrapped around those ioctls —
it exists to resolve `default` and `plughw:0,0` through a text configuration
language and to convert formats the hardware does not accept. Neither is
something we need from a library: we know which device we opened, and format
conversion is `convert.cst`, which has to exist anyway for mixing.

So the Linux path is like KMS in [`window/`](../window/window.md): **no
dependency at all, just syscalls.** That makes audio the second subsystem where
the manifesto holds without compromise, and unlike graphics there is no
equivalent of "the driver lives in userspace" to work around.

**And the raw path covers almost every machine.** PipeWire is what runs on a
modern desktop, and it emulates ALSA — a program writing to the ALSA interface
plays through PipeWire without knowing. So `alsa/` is not the fallback backend,
it is the one that works nearly everywhere, with the daemons buying lower
latency, per-application volume and device hotplug rather than basic function.

---

## Every backend, and why more than one

Five on Linux, five on Windows. They are not redundant: they differ in latency,
in whether other applications can play at the same time, and in whether they
exist at all on a given machine.

### Linux

| | Transport | Cost | Why pick it |
|---|---|---|---|
| **alsa** | 33 ioctls on `/dev/snd` | **none** | Works nearly everywhere, including through PipeWire's emulation. The default. |
| **pipewire** | socket protocol | libpipewire is 827 KB, or speak it directly | What a modern desktop actually runs. Lowest latency of the daemons, proper routing, device hotplug. |
| **pulse** | socket protocol | libpulse is 314 KB, or speak it directly | Still what many systems run, and what remote-audio setups expect. |
| **jack** | socket protocol | libjack is 238 KB | Pro audio: sample-accurate, callback-driven, and how a program joins a studio graph. |
| **oss** | ioctls on `/dev/dsp` | none | Legacy on Linux and **native on FreeBSD and NetBSD**. Simpler than ALSA, and the cheapest way to reach BSD. |

The three daemons are socket protocols, so they can be spoken without linking
anything — the same shape as Wayland, and needing the same `sendmsg`/`SCM_RIGHTS`
work in `std/os/linux.cst`.

### Windows

| | Shape | Why pick it |
|---|---|---|
| **wasapi** | COM | The modern API. Shared mode coexists with other programs; exclusive mode gives low latency by taking the device. |
| **xaudio2** | COM | What games use. Carries its own mixer and effects, which we would bypass, so it buys the submix graph rather than the basics. |
| **dsound** | COM | Legacy, present on every Windows since 1995. The compatibility floor. |
| **waveout** | **flat C** | Ancient and high-latency — and the only one that needs no COM at all, which makes it the cheapest way to get a first sound out. |
| **wdmks** | kernel streaming | Lowest latency short of a third-party ASIO driver, and correspondingly awkward. |

Headers for all five are present in mingw, so all five are generated and tested
from Linux.

Four of them are COM, which needs the vtable machinery
[`gpu/d3d`](../gpu/d3d/d3d.md) already requires — so the order there matters:
**`waveout` first**, because flat C means a sound comes out of a Windows build
before the COM extractor exists, and the good backends afterwards. The same
reasoning as taking KMS's legacy `SETCRTC` before atomic modesetting.

---

## The callback runs on a deadline

This is the constraint that shapes everything else, and it has no counterpart
elsewhere in the library.

Audio hardware asks for the next buffer on a hard schedule. Miss it and the
output glitches audibly — not a dropped frame nobody notices, but a click every
listener hears. A 5 ms buffer at 48 kHz is 240 frames, and the callback has that
long, every time, forever.

So inside the callback: **no allocation, no locks, no file I/O, no syscalls that
can block.** Everything it touches is preallocated, and communication with the
rest of the program is through lock-free structures — Caustic has atomics, which
is what this needs.

That inverts the usual ownership: the mixer's voice table, the ring buffers a
streamed source fills, and the parameter changes a program requests all exist
because the callback cannot ask for them when it needs them. `music.cst` is a
whole module rather than a flag for this reason — a streamed file is decoded on
an ordinary thread into a ring buffer that the callback only reads.

---

## Mixing is ours

There is no equivalent of a GPU here: every platform hands over a buffer of
samples and everything before that is the program's own arithmetic. So
`mixer.cst` is not a backend abstraction, it is real work we do on every
platform.

**Voices** — each playing sound with its own gain, pitch, pan and position, summed
into a bus. The count is bounded and stated rather than growing, since the
callback cannot allocate.

**Resampling** — a 44.1 kHz file playing on a 48 kHz device needs conversion, and
pitch shifting is the same operation. Quality is a real choice: linear
interpolation is cheap and audibly poor on sustained tones; a windowed sinc is
what a mixer should default to.

**Format conversion** — sources are 16-bit integer, 24-bit packed, or float;
devices want one specific thing. `convert.cst` handles both directions, and
mixing internally in f32 avoids the clipping and precision loss of accumulating
in integers.

---

## Space

`spatial.cst` places a sound relative to a listener: distance attenuation,
stereo or surround panning, and doppler from relative velocity. It is
straightforward geometry — and `math/` already has the vectors, the distance
functions and the interpolation curves, so this is composition rather than new
mathematics.

Occlusion, reverb zones and HRTF are further out and worth naming as not now:
each is a real subsystem, and none is needed to place a footstep behind the
listener.

---

## File formats

**WAV first** — it is a header and samples, an afternoon's work, and it is what
every tool exports for testing.

**Then the compressed ones**, in the order their complexity earns their place:
QOA is trivially simple and lossy-but-fine for effects; Vorbis and Opus are what
music actually ships as; FLAC is lossless and simpler than either; MP3 is
patent-free now and still not worth writing early.

Decoding is **opt-in**, like image loading: a program that generates its audio,
or ships WAV, links no decoder.

Nothing in the ecosystem covers audio codecs yet — `caustic-compact` is general
compression and `caustic-image` has VP8, which is video. So these are ours to
write, and the order above is by what a program needs rather than by what is
interesting.

---

## Order of work

1. **`alsa/` and `device.cst`** — a sine wave out of a real speaker with no
   dependency, which proves the callback contract.
2. **`convert` and `mixer`** — several sounds at once, at the right rate.
3. **WAV**, so there is something to play that a program did not synthesise.
4. **`source` and `music`** — looping, seeking, and streaming from disk.
5. **`spatial`.**
6. **`oss`**, which is a short step from `alsa` and brings BSD with it.
7. **`waveout`**, so a Windows build makes noise without waiting for COM.
8. **`pipewire`, `pulse`, `jack`**, then **`wasapi`, `xaudio2`, `dsound`,
   `wdmks`** — each earning its place by latency or by reach, none of them
   blocking a program from making sound.
