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

## File-descriptor transport

`std/os/linux.cst` still has no `sendmsg`/`recvmsg` or `memfd_create` surface.
The project-level `sys/linux.cst` already supplies `connect_unix`,
`send_fds`/`recv_fds` (`SCM_RIGHTS`), `memfd_create`, `poll` and `eventfd`;
Wayland can use that transport without libwayland-client or libffi.

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

## Current state

`backend.cst` is a direct Wayland socket client; it does not link
`libwayland-client` or `libffi`. It implements registry binding, xdg-shell
windows, configure/close/state events, double-buffered `wl_shm` presentation,
frame pacing, output and fractional scale, constraints, decorations, dialogs,
cursors, keyboard/pointer input, text selections, and file/text drag-and-drop.
The portable `window/device.cst` and `ui/host.cst` use it to open and paint
toolkit windows.

Optional globals degrade independently. Clipboard and DnD require
`wl_data_device_manager`; primary selection requires
`zwp_primary_selection_device_manager_v1`; fractional scaling requires both
`wp_viewporter` and `wp_fractional_scale_manager_v1`; server decorations,
dialog roles and cursor shapes use their corresponding optional protocols.
When `zwp_text_input_manager_v3` and a seat are present, focused text widgets
enable text input, publish their caret rectangle, and receive inline preedit,
committed text and delete-surrounding edits. The latter are expanded to UTF-8
character boundaries by `entry` and `textview`. The backend does not publish
surrounding text, so IME suggestions that need surrounding context are limited.

`caustic-mk run test-wayland` checks the generated protocol catalog, exercises
clipboard/DnD and text-input-v3 through internal wire tests, opens and presents
windows on Weston at scale 1 and 2, and runs the toolkit application loop on
Weston until a timer closes its window. The headless Weston used here does not
provide a keyboard/pointer seat; real key, clipboard, DnD and IME interaction
therefore remains unverified by this test. The current smoke proves surface,
frame, timer and toolkit-host integration, not real desktop input.

A separate seated Weston/X11 smoke showed the file browser on the compositor.
The settings form and editor did not become visible in that run; their normal
startup and keyboard interaction are not accepted as verified. This is not
three-example platform parity. Presentation sends explicit surface damage
before the frame request and commit, including when a buffer is reused.

Regenerate the protocol inventory from XML with
`sh window/wayland/tools/check_protocols.sh`; it also validates the inputs
against the independent `wayland-scanner`.
