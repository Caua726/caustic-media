# Wayland

The backend for a native Wayland session. Not for XWayland — that is
[`x11/`](../x11/x11.md), unchanged.

```
wayland/
  protocol.cst  the interfaces, generated from the XML
  backend.cst   window/device.cst implemented over them
```

This is the backend the portable API was shaped around: everything in
[`../window.md`](../window.md) about the pull model, per-frame buffers and
release tracking exists because of what Wayland requires.

---

## It is not a function API

`libwayland-client` exports **63 symbols** for a protocol with **23 interfaces,
71 requests and 61 events** in the core alone, plus 5 interfaces and 36 requests
in xdg-shell. The reason is that the protocol is *data*, not code:

```
wl_proxy_marshal(proxy, opcode, ...)
```

Variadic, driven by a `wl_interface` descriptor table. There is no
function-by-function binding to write — either the stubs are generated from the
XML, which is what `wayland-scanner` does for C, or the marshalling is written by
hand.

Callback dispatch is also why libwayland pulls in **libffi**: listeners are
structs of function pointers invoked through a generic dispatcher.

**`xdg-shell` is not optional.** The core protocol has surfaces but no concept of
a window a user can move, resize or minimise. Every real window is
`xdg_surface` + `xdg_toplevel`, which is an extension, which means its stubs are
generated from a separate XML the same way.

Present on this machine: `/usr/share/wayland/wayland.xml`,
`/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml`, and the compositor
socket at `$XDG_RUNTIME_DIR/wayland-1`.

---

## Pull is native here, and that is why the API is pull

A Wayland client does not draw when it wants to. It asks:

```
wl_surface.frame(callback)      -> the compositor calls back when drawing is useful
wl_surface.attach(buffer)
wl_surface.damage(x, y, w, h)
wl_surface.commit()
```

Spinning instead of waiting for the callback burns a core and gets throttled
anyway. Blocking inside a `present()` hides the callback and makes it impossible
to do anything else while waiting.

So `next_frame` here waits on the frame callback, which is the honest
implementation of the portable contract, and the reason the contract is shaped
that way at all.

`damage` is worth noting: the compositor only re-reads the region declared
dirty. A backend that always damages the whole surface leaves real performance
on the table for UI, which changes very little between frames.

---

## Buffers, and why the window hands out one per frame

The compositor may still be reading the buffer just committed. Touching it before
`wl_buffer.release` arrives corrupts what is on screen.

So: **two buffers minimum**, with release tracking, and `acquire` returns
whichever is currently free. This is the whole reason the portable API returns
the back buffer per frame rather than owning one surface — X11 alone would never
have forced it.

The buffer itself is shared memory:

1. `memfd_create` a file
2. `mmap` it in the client
3. Hand the **file descriptor** to the compositor over the socket as
   `wl_shm.create_pool`
4. Carve `wl_buffer`s out of the pool

---

## Two syscalls the standard library does not have

Step 3 above is the blocker for speaking the protocol directly, and it is
concrete:

- **`sendmsg` with control messages.** The pool's file descriptor travels as
  `SCM_RIGHTS` ancillary data over the unix socket. `std/os/linux.cst` has the
  `AF_UNIX` constant and nothing else — no `sendmsg`, no `recvmsg`, no `cmsg`.
- **`memfd_create`.** How the pool is made in the first place.

Both are plain syscalls and belong in `std/os/linux.cst` regardless of this
library. They are the prerequisite for the zero-dependency path, and they are not
needed at all for the libwayland path.

---

## Two paths

| | Cost | Dependency |
|---|---|---|
| **libwayland-client** | XML generation for xdg-shell stubs is still required | 58 KB + **libffi** + libc |
| **Raw socket** | the above plus connection, object registry, fd passing | none |

Wayland's protocol is simpler to speak directly than X11's — object IDs and
opcodes, no cookie authentication — and the library is thin enough that it buys
less than libX11 does. The generator is needed either way.

The deciding factor is the same as everywhere: libwayland means libffi and libc
in the address space.

---

## Native handles

```cst
window.wayland.display(&w)   // wl_display*  -> VK_KHR_wayland_surface, EGL
window.wayland.surface(&w)   // wl_surface*
```

---

## Order of work

Wayland comes after KMS and after the C header generator, per
[`../window.md`](../window.md). By then the XML generator is the remaining piece,
and it is the one with no alternative.

1. **XML → Caustic generator**, core protocol plus xdg-shell.
2. **`sendmsg`/`SCM_RIGHTS` and `memfd_create`** in `std/os/linux.cst`.
3. **The backend**, on whichever transport is chosen by then.
