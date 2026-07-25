# KMS

Direct kernel mode setting: a screen with no compositor and no display server
under it. The console, an embedded target, a kiosk — and CausticOS, which has no
compositor to run under.

```
kms/
  drm.cst       the ioctls — no library involved
  backend.cst   window/device.cst implemented over them
```

**This is the backend that fits the project exactly.** It is ioctls on
`/dev/dri/card0` with flat structs: no FFI, no external library, no libc in the
address space, nothing but syscalls. Every other backend links something written
by someone else; this one does not.

It is also the reason it goes first — it proves the pull presentation model in
its rawest form with no generator and no binding work in the way.

---

## The surface

| | |
|---|---|
| ioctls in `drm.h` | **111** |
| modeset entries in `drm_mode.h` | 55 |
| `libdrm`, if it were used | 212 symbols, 89 KB — **not needed** |

`libdrm` is a thin wrapper over these ioctls. There is nothing in it worth the
dependency.

Present on this machine: `/dev/dri/card1` and `/dev/dri/renderD128`, and the user
is in the `video` group.

---

## The display model

Four objects, discovered in order, and getting the relationship right is most of
the work:

```
Connector   a physical port — HDMI, DisplayPort, eDP. Carries the modes the
            attached monitor reports, and whether anything is attached at all.
Encoder     converts the CRTC's output to the connector's signal.
CRTC        the scanout engine: reads a framebuffer, drives a mode.
Plane       a layer the CRTC composites — primary, cursor, overlay.
```

The path: `GETRESOURCES` lists them → `GETCONNECTOR` finds one that is connected
and reads its modes → `GETENCODER` finds a CRTC that can drive it → `SETCRTC`
binds a framebuffer and a mode to that CRTC.

The relevant ioctls, all present in `drm.h`:

```
MODE_GETRESOURCES  MODE_GETCONNECTOR  MODE_GETENCODER  MODE_GETCRTC
MODE_SETCRTC       MODE_ADDFB         MODE_RMFB        MODE_PAGE_FLIP
MODE_CREATE_DUMB   MODE_MAP_DUMB      MODE_DESTROY_DUMB
MODE_GETPLANE      MODE_SETPLANE      MODE_CURSOR      MODE_ATOMIC
```

---

## Dumb buffers

The framebuffer comes from the kernel without any GPU driver involvement:

1. `MODE_CREATE_DUMB` — width, height, bpp; the kernel allocates
2. `MODE_MAP_DUMB` — get an offset
3. `mmap` the DRM fd at that offset — now it is memory you write pixels into
4. `MODE_ADDFB` — register it as a framebuffer object
5. `MODE_SETCRTC` — scan it out

"Dumb" means exactly that: linear memory, no tiling, no acceleration. Which is
what a software rasterizer wants, and it is the reason the software path reaches
a real screen here with nothing else present.

---

## Presentation is pull, in its own way

`MODE_PAGE_FLIP` schedules a buffer to become the front one at the next vertical
blank, and the kernel writes an event on the DRM fd when it happens. So:

```
next_frame  ->  read the DRM fd, waiting for the flip event
acquire     ->  whichever buffer is not currently being scanned out
submit      ->  MODE_PAGE_FLIP
```

That is the same contract as Wayland's frame callback arriving through a
different door, which is a good sign the portable shape is right rather than
X11-shaped or Wayland-shaped.

**Two buffers minimum**, three to avoid stalling: the scanout hardware is reading
the front buffer continuously, and drawing into it tears.

---

## What it costs to own the screen

There is no compositor to arbitrate, so this backend takes the display:

- **DRM master.** Only one process at a time may modeset. A desktop session's
  compositor already holds it, so a KMS program has to run on a free VT — or take
  a lease (`MODE_CREATE_LEASE`), which is how VR compositors grab a headset out
  from under the desktop.
- **Permissions.** `/dev/dri/card*` is root or the `video` group.
- **The VT.** Switching away has to be handled, or the program keeps scanning out
  over whatever took over.

None of this is hard, but it is why KMS cannot be tested by running it inside a
desktop session, and that shapes how it gets developed: on a spare VT, or under
CausticOS where the question does not arise.

---

## Input does not come with it

X11, Wayland and Win32 all deliver input through the same channel as windowing.
KMS does not: there is no server, so keyboard and mouse come from **evdev** —
reading `/dev/input/event*` directly. That is more syscalls and no library, which
suits, but it means this backend implements input the others get for free.

`libinput` exists and is not worth the dependency for the same reason `libdrm`
is not.

---

## Order of work

First of the four, per [`../window.md`](../window.md).

1. **Resource discovery** — connector, encoder, CRTC, modes.
2. **Dumb buffer, ADDFB, SETCRTC** — pixels on a real screen.
3. **Page flip and vblank events** — the pull loop.
4. **evdev input.**
5. **Atomic modesetting** (`MODE_ATOMIC`), which supersedes SETCRTC/PAGE_FLIP and
   is how planes and multi-display are done properly. Worth doing second, not
   first, because the legacy path is much shorter to get a picture with.
