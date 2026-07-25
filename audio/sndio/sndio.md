# sndio

OpenBSD's audio interface: small, careful, and the only one on a system that
takes deliberate pride in having exactly one.

```
sndio/
  backend.cst   audio/device.cst implemented over it
```

---

## Why it exists here

OpenBSD does not ship ALSA, PulseAudio, PipeWire or JACK. It ships `sndiod`, a
small server, and `libsndio` to talk to it — present on this machine at
`/usr/lib/libsndio.so.7`, since it has been ported to Linux as well.

So this is the OpenBSD backend, and the reason [`oss/`](../oss/oss.md) does not
cover every BSD: FreeBSD and NetBSD use OSS, OpenBSD uses sndio.

---

## Shape

Deliberately minimal, and the API reflects OpenBSD's taste: open a handle,
describe the format you want, get back what you will actually receive, then
write. There is no negotiation protocol and no configuration language. The
protocol between client and `sndiod` is a unix socket, and unlike PulseAudio and
PipeWire it does not pass file descriptors — so it can be spoken directly with
the standard library exactly as it stands.

That makes it, alongside PulseAudio's inline path, one of the two sound servers
reachable today with no new syscalls.

Its authentication and device naming follow the `AUDIODEVICE` environment
variable, in the same spirit as `DISPLAY`.

---

## Order of work

Low priority and low cost. Worth doing when BSD support is being taken seriously
as a whole, alongside OSS — not before, since nothing else in the library targets
OpenBSD yet.
