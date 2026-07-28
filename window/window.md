# The window layer

How `caustic-media` reaches a screen, on every platform it targets, and why the
shape is what it is.

Two things live here, and keeping them apart is the whole design:

1. **Bindings.** Every function each platform exposes, declared in Caustic. Not
   the subset this library happens to need — all of it, so a program that wants
   to drive X11 or Win32 itself never has to write an `extern` by hand.
2. **The abstraction.** One portable window API, built *on top of those
   bindings* rather than on FFI directly. It calls `window.x11.XCreateWindow`,
   not a private `extern`.

A user picks a level and stays there, or mixes: open a window with the portable
API and still reach the raw display handle underneath.

---

## The tree

```
window/
  window.cst      hub
  device.cst      our portable window: open, next_frame, acquire, submit, close
  display.cst     monitors, modes, DPI, work area
  event.cst       raw platform events and the queue they arrive in
  cursor.cst      shape, visibility, capture
  clipboard.cst   text and data
  native.cst      native handle accessors — the public contract

  x11/      done — 8 modules over bind/, see x11/x11.md
  wayland/  protocol.cst  + backend.cst      see wayland/wayland.md
  kms/      drm.cst       + backend.cst      see kms/kms.md
  win32/    user32.cst gdi32.cst + backend.cst   see win32/win32.md
```

`device.cst` is to this layer what `gpu.open()` is to `gpu/`: **one portable
window, across all four backends.** The backends are not the API — they are what
it is implemented on, and what a program reaches for when it wants the platform
directly.

Three placements worth their reasoning:

**Bindings and backend share a directory.** `x11/xlib.cst` is the raw binding,
`x11/backend.cst` is our abstraction implemented over it. Whoever touches one
touches the other, and `x11/` as a whole is what you delete if you drop X11.

**`window/` owns the raw event queue; `input/` interprets it.** `event.cst`
delivers "key 38 pressed, pointer at (x, y)". Turning that into "the JUMP action
is active", mapping a gamepad, or tracking held state is `input/`'s work. Without
this line either `input/` has to speak to four platforms, or `window/` grows game
logic.

**The swapchain is not here.** It belongs to `gpu/`, because when there is a GPU
it is the swapchain that presents, not the window. This layer only hands over the
native handles it is built from.

---

## The four backends

| Backend | Platform | Transport |
|---------|----------|-----------|
| `x11` | Linux, BSD — and **Wayland sessions through XWayland** | libX11 today, X11 wire protocol later |
| `wayland` | Linux, native | libwayland-client today, wire protocol later |
| `kms` | Linux with no compositor: console, embedded, CausticOS | DRM ioctls — no library at all |
| `win32` | Windows | user32 + gdi32 |

**XWayland is not a fifth backend.** From a client's side it *is* X11: same
protocol, same library, same code path. A Wayland session running XWayland is
served by `x11` with nothing extra.

`AUTO` selects on Linux in this order: `WAYLAND_DISPLAY` set → `wayland`;
otherwise `DISPLAY` set → `x11`; otherwise → `kms`.

**The program can always name one instead.** `window.open_with(WAYLAND, ...)`
opens a Wayland window in a session where `AUTO` would have chosen X11, and
fails honestly if it cannot. A build that wants only one backend excludes the
rest in the Causticfile, and then there is nothing to select. Both levels of that
choice are described in the [README](../README.md#choosing-a-backend), and this
layer follows the same rule as every other.

---

## Why the abstraction is pull-shaped

This is the decision everything else follows from, and it is not obvious from
any single platform.

- **X11 is push.** Call `XPutImage` whenever you like. No pacing. The client
  decides when a frame happens.
- **Win32 is push** as well, though `Present` blocks on vsync.
- **Wayland is pull.** You do *not* draw when you feel like it. You request a
  `frame` callback and the compositor tells you when drawing is worth doing.
  Presenting is `attach` + `damage` + `commit`.
- **KMS is pull too**, in its own way: you page-flip and wait for the vblank
  event on the DRM fd.

Writing the obvious loop — `while (poll()) { draw(); present(); }` — bakes the
X11 model into the interface. Wayland then has to be faked into it, and the
only ways to do that are spinning (burning a core while the compositor throttles
you anyway) or blocking inside `present`, which hides the frame callback and
makes it impossible to do anything else while waiting.

The asymmetry decides it: **faking pull on a push platform is trivial** — return
immediately, or fire a timer. **Faking push on a pull platform is not.**

So the interface is pull:

```cst
while (window.next_frame(&w) == 1) {     // blocks until drawing is useful
    // draw
    window.submit(&w);
}
```

On Wayland `next_frame` waits for the frame callback and on KMS it waits for
vblank. On X11 it returns immediately **on the socket path** — and blocks when
both MIT-SHM buffers are still being read, because `XShmPutImage` returns before
the server has finished with the segment. That was a surprise: MIT-SHM makes X11
behave like Wayland, and the same two-buffer-plus-release handling applies. See
[`x11/x11.md`](x11/x11.md).

---

## Buffer ownership differs, and that follows too

- **X11**: on the socket path `XPutImage` copies synchronously, so one buffer is
  enough. Under MIT-SHM it does not copy at all, and X11 needs two buffers and
  release tracking exactly as Wayland does.
- **Wayland**: the compositor may still be reading the buffer you just
  committed. Touching it before `wl_buffer.release` arrives corrupts the frame
  on screen. Two buffers minimum, with release tracking.
- **KMS**: the scanout hardware is reading the front buffer. You draw to the
  back one and flip. Two minimum, three to avoid stalling on vblank.

So "the window owns *the* surface" is wrong. The window hands out **the back
buffer of the moment**, and that pointer may differ between frames:

```cst
let is soft.target.Target as t = window.acquire(&w);   // this frame's buffer
// draw into t
window.submit(&w);                                      // commit + damage
```

A caller that caches the Target across frames is writing into a buffer the
compositor is scanning out. The API returns it per frame precisely so that
mistake is hard to make.

---

## Where the GPU enters

When Vulkan or OpenGL renders, **the swapchain owns presentation entirely**.
`vkQueuePresentKHR` and `eglSwapBuffers` put the pixels on screen; the window's
own shm/XImage path is bypassed completely.

The window's remaining job is to hand over the **native handles**:

| API | Needs |
|-----|-------|
| `VK_KHR_xlib_surface` | `Display*` + `Window` |
| `VK_KHR_wayland_surface` | `wl_display*` + `wl_surface*` |
| `VK_KHR_win32_surface` | `HINSTANCE` + `HWND` |
| EGL / WGL | the same pairs |

One `present()` cannot serve both paths. This is why SDL makes you choose
between `SDL_GetWindowSurface` and `SDL_Vulkan_CreateSurface` and treats using
both on one window as an error.

Therefore the native handles are **public contract**, not an implementation
detail:

```cst
window.x11.display(&w)      // Display*     — implemented
window.x11.xid(&w)          // Window       — implemented
window.wayland.display(&w)  // wl_display*
window.wayland.surface(&w)  // wl_surface*
window.win32.hwnd(&w)       // HWND
```

That seam is what lets `media.gpu.vk.surface_from(&w)` exist, and it is the same
escape-hatch rule the rest of the library follows: a layer never hides the layer
beneath it.

---

## The surface to be bound

Measured, not estimated:

| Platform | Functions / requests | Types | Constants |
|----------|---------------------|-------|-----------|
| X11 (Xlib) | **774** public — the other 407 are `_X*` internals | **168 structs** in `Xlib.h` | 349 in `X.h` alone |
| Wayland core | 71 requests, 61 events, 23 interfaces | from XML | from XML |
| xdg-shell | 36 requests, 9 events, 5 interfaces | from XML | from XML |
| DRM / KMS | 111 ioctls + 55 modeset entries | ~55 structs | many |
| Win32 | user32 + gdi32 ≈ 1600 exports; windowing core ~150 | hundreds | thousands |

Roughly **1000 functions, 400 structs, 1500+ constants** for full coverage.

The `_X*` symbols are deliberately excluded: they are libX11's private
implementation, not API. "All functions" means **all functions plus the types
they need** — binding `XkbGetState` without `XkbStateRec` produces a
declaration nobody can call.

---

## Three obstacles specific to Caustic

**Caustic has no unions, and X11's event model is one.** `XEvent` is a union of
33 event structs sharing a common header. The options are offset accessors —
the first thing `x11.cst` did — or spelling out 33 structs plus a discriminator.
`x11/bind/xevent.cst` took the second: the union is 192 bytes of storage and one
pointer cast per member, so the cast *is* the union and nothing copies. Either
way a wrong offset is silent memory corruption rather than a compile error,
which is why the offsets come from the C compiler and 565 assertions check them.

**Wayland is not a function API.** `libwayland-client` exports only 63 symbols
because the protocol is *data*: everything goes through
`wl_proxy_marshal(proxy, opcode, ...)`, which is variadic and driven by a
`wl_interface` descriptor table. There is no function-by-function binding to
write. Either the stubs are generated from the XML — what `wayland-scanner`
does for C — or the marshalling is written by hand. Callback dispatch is also
why libwayland pulls in **libffi**.

**KMS is the one that fits the manifesto exactly.** It is ioctls on
`/dev/dri/card0` with flat structs: no library, no FFI, no external dependency,
nothing but syscalls. It is both the easiest to cover completely and the only
backend that keeps a binary fully self-contained.

---

## What this costs a binary

Linking a display library is not free, and the cost is not the library:

| | Size | Symbols | Pulls in |
|---|---|---|---|
| `libX11.so.6` | 1282 KB | 1181 | libxcb + **libc (2135 KB)** + libXau + libXdmcp ≈ **3.6 MB** |
| `libxcb.so.1` | 166 KB | 674 | libc, libXau, libXdmcp |
| `libwayland-client.so.0` | 58 KB | 63 | **libffi** + libc |

The real cost is **libc landing in the address space** of a project whose whole
identity is not having it. `libX11` is also implemented *on top of* `libxcb`
and has been since around 2007 — it is a legacy façade over the real base, not
the base itself.

The universal base for X11 is **the wire protocol**: stable since 1987, fully
documented, reachable over the socket at `/tmp/.X11-unix/X0` after parsing the
MIT-MAGIC-COOKIE out of `~/.Xauthority`. Same for Wayland, over the socket at
`$XDG_RUNTIME_DIR/wayland-0`.

One more reason the library adds less than it appears to: `XPutImage` sends the
whole framebuffer through the socket every frame — 1.2 MB at 640×480, 8 MB at
1080p. Any serious path uses MIT-SHM instead, so the fast route needs work the
library does not do for you either way.

---

## Generators, not typing

A thousand declarations and four hundred structs written by hand is weeks of
mechanical work with a high error rate and silent failures, and it goes stale
the moment a system header changes.

Two tools remove that:

- **C header → Caustic.** Parses function declarations, structs and `#define`
  constants. Covers X11, DRM and Win32 in one stroke. Roughly 1500–2500 lines —
  and parsing declarations is what this project already does for a living.
- **Wayland XML → Caustic.** Not optional: the XML *is* the protocol
  definition. Generates the interface tables, request stubs and listener
  structs.

With both, all four backends become generated and regenerable. Without them,
every system update is a manual audit.

Note for whoever starts: `caustic-maker/parser/cfile_lexer.cst` is the
**Causticfile** lexer, not a C one. There is no existing C parser to build on.

---

## Prerequisites in the standard library

Talking to Wayland over the raw socket needs two things `std/` does not have:

- **`sendmsg`/`recvmsg` with control messages.** The shm pool is handed to the
  compositor as a file descriptor over `SCM_RIGHTS`. `std/os/linux.cst` has the
  `AF_UNIX` constant and nothing else.
- **`memfd_create`.** How that shm pool is made in the first place.

Both are plain syscalls and belong in `std/os/linux.cst` regardless of this
library.

---

## Order of work

1. **KMS.** 111 ioctls, no FFI, no generator, no dependency. Proves the pull
   presentation model in its rawest form and is the backend CausticOS needs.
2. **The C header generator.** Unlocks the full X11 and DRM bindings at once.
3. **The Wayland XML generator**, plus the `sendmsg`/`memfd` syscalls.
4. **Win32**, through the same header generator, verified under wine.

Vulkan should land against **one** backend before a second exists: it is the
GPU path that proves whether the native-handle contract is right, and that is
better discovered with one platform than with three.

---

## Current state

`x11/` is finished and tested: **1030 of the 1035 functions** across libX11,
libXext, libXrandr, libXcursor, libXfixes and libXi, plus a window layer built on
top of them that is pull-shaped, hands out a per-frame buffer, and exposes its
native handles. It presents through MIT-SHM with a socket fallback, and it has
cursors, clipboard and monitor enumeration. [`x11/x11.md`](x11/x11.md) has the
detail.

The five unbound exports are named there rather than left as a silent gap: each
is exported without a leading `_` but declared in no shipped public header, and
binding one would mean inventing a prototype.

**A window is whatever size the window manager says it is**, and that turned out
to be the backend's sharpest edge rather than a detail. A tiling compositor
grants 1366×768 for a 640×480 request, immediately, before the program has drawn
anything. The backend used to record the new width and height on
`ConfigureNotify` and stop there, which broke two things at once and neither of
them loudly:

- The scratch buffer stayed its original size while `present` walked
  `height` rows at a stride of `width`, writing megabytes past the end of the
  mapping on every frame.
- The `XImage` kept its original `bytes_per_line` while `XPutImage` was told
  the image had grown, so the server read every row at the wrong offset. On
  screen that is interleaved banding; in the code it is nothing at all.

Both are fixed, and the fix is now a test rather than a memory: `x11_test.cst`
fills a frame with a known pattern, submits it, reads the window back with
`XGetImage` and compares byte for byte — at 640×480, 641×481 and 1023×769,
because non-multiples of four are where the stride assumption breaks. A
screenshot would have shown the banding without ever saying whose fault it was,
and the first two guesses were both wrong.

Three things this cost that are worth writing down for whoever does Wayland:

**Packed structs are the hardest part of a binding, not the FFI.** Caustic
structs have no alignment padding and C structs do, so all 82 bound structs carry
explicit `_padN` fields. A missing one is silent memory corruption with no
diagnostic. The defence is `tools/x11_layout.c`, which asks the C compiler for
every offset, and 565 assertions that check the compiler's layout against it.

**Declaring a binding is free; importing a module that calls one is not.** The
compiler emits a library import at the call site, so `bind/` can declare a
thousand symbols and a program that opens a window still links two libraries. But
importing a *module* that calls into an extension makes that extension reachable
from `main` even when nothing calls the module, and lazy initialisation does not
help. That is why `x11/cursor.cst` and `x11/monitors.cst` are not wired into the
window and a program takes them directly.

**The toolchain has a ceiling worth knowing about.** A single binary importing
more than roughly 256 distinct extern symbols gets PLT relocations for every
soname but a `DT_NEEDED` entry for only the first, and dies at `exec` naming a
function whose library was never recorded. The link tests are one binary per
shared object because of it.

---

## Order of work, from here

1. **KMS.** 111 ioctls, no FFI, no generator, no dependency. Proves the pull
   presentation model in its rawest form and is the backend CausticOS needs.
2. **Vulkan against X11**, which now has the native-handle contract to build on.
   It is the GPU path that proves whether that contract is right, and that is
   better discovered with one platform than with three.
3. **The C header generator.** `bind/` was written by hand; the layout table and
   the symbol manifest are already the shape a generator would consume, and DRM
   and Win32 would come out of the same tool.
4. **The Wayland XML generator**, plus the `sendmsg`/`memfd` syscalls.
5. **Win32**, through the same header generator, verified under wine.
