# XAudio2

Microsoft's game audio API, and part of DirectX. Sits on WASAPI and adds what a
game would otherwise write itself.

```
xaudio2/
  xaudio2.cst   8 interfaces, 167 methods
  backend.cst   audio/device.cst implemented over them
```

---

## What it adds, and why we mostly do not want it

XAudio2 carries a **voice graph**: source voices, submix voices, a mastering
voice, with per-voice volume, pitch, filters, reverb and 3D positioning through
X3DAudio.

Which is very close to what [`audio/mixer.cst`](../audio.md) is. So using it
fully would mean two mixers stacked, and the program's own would be doing nothing
on Windows and everything everywhere else — the exact situation that makes a
cross-platform library behave differently per platform for no reason a user can
see.

So the backend uses it as an **output**, feeding a single source voice from our
own mixer, the same as every other backend. The graph, the effects and X3DAudio
stay unused.

---

## Then why have it at all

Three reasons, none decisive alone:

- It is **the path Xbox uses**. A program that might one day run there is closer
  if this backend exists.
- It handles **format conversion and device changes** more gracefully than raw
  WASAPI, at no cost to us.
- It is what game developers on Windows expect to see supported, which matters
  for a library asking to be taken seriously.

It is COM — 8 interfaces, 167 methods — so it shares the extractor with
[WASAPI](../wasapi/wasapi.md) and Direct3D.

---

## Order of work

After WASAPI, and only because it is cheap once the COM machinery exists. Nothing
depends on it.
