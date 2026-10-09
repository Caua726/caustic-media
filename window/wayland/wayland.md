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
`send_fds`/`recv_fds` (`SCM_RIGHTS`), `memfd_create`, `poll`, `eventfd` and
`timerfd`; Wayland can use that transport without libwayland-client or libffi.

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

The raw client exposes a socket descriptor and protocol object IDs, not
`wl_display*` or `wl_surface*` pointers from `libwayland-client`:

```cst
let is backend.Window as w with mut;
if (backend.open(&w, "Example", 640, 480) == 1) {
    let is i64 as fd = backend.native_fd(&w);
    let is i32 as surface_id = backend.native_surface(&w);
    let is i32 as toplevel_id = backend.native_toplevel(&w);
    // Pump, paint and close w before its storage goes away.
    backend.close(&w);
}
```

`backend` above is `window/wayland/backend.cst`. These IDs cannot be passed to
EGL or `VK_KHR_wayland_surface` as native library pointers. `open_config`
also takes caller-owned `*Window` storage and returns success; the address
must remain stable until `close`. The portable device owns that storage.

---

## Current state

`backend.cst` is a direct Wayland socket client; it does not link
`libwayland-client` or `libffi`. It implements registry binding, xdg-shell
windows, configure/close/state events, double-buffered `wl_shm` presentation,
frame pacing, output and fractional scale, constraints, decorations, dialogs,
cursors, keyboard/pointer input, text selections, and file/text drag-and-drop.
Input is translated as it arrives into `input/event.cst`'s cooked events, a
queue per window (`poll_input`): keys with xkbcommon's keysym, their keycode
as their place and `wl_keyboard` 10's repeated state as a repeat, the text
they type (compose included; never a control character nor a command's),
the pointer in buffer pixels, axis and axis_discrete gathered per frame into
one `SCROLL`, text-input-v3's commit, preedit and delete-surrounding,
keyboard focus leaving as a `CANCEL` that lets buttons go too, and a press
`xdg_toplevel.move`/`resize` takes ending in `BUTTON_TAKEN`.
The portable `window/device.cst` and `ui/host.cst` use it to open and paint
toolkit windows.

All live windows share one connection, registry and client object namespace.
This lets native dialogs refer to their parent's actual object ID. The last
close releases the shared globals and connection; closing the first window
does not invalidate its siblings. Capacity is 64 windows and 8191 nonzero
client IDs, recycled only after `wl_display.delete_id`. Ancillary descriptors
form a connection-wide FIFO: intervening messages do not discard them, and
late events for released keyboards or data sources close their descriptors.
Connection state is single-threaded: open, pump and close from the owning
application loop. A normal buffer grows to the toolkit's published minimum
when the compositor leaves sizing to the client; maximize/fullscreen sizing
remains compositor-controlled.

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
windows on Weston at scale 1 and 2, and runs the toolkit application loop.
Native regressions cover parent/child and independent-window lifetimes,
abandoned acquisitions, presentation permits, maximize/fullscreen frames and
normal-size restoration.
The toolkit regression checks that dialogs fit their native buffer and stay
below their client-side header. The headless Weston has no keyboard/pointer
seat; it cannot prove desktop clipboard, DnD or IME interaction.

KWin is the second compositor (`tools/run_kwin.sh`: its own runtime,
configuration and bare session bus; `KWIN_BACKEND=x11` nests it in Xvfb,
`KWIN_SCALE=1.5` sets its output's scale with kscreen-doctor), the one that
has what Weston does not: server-side decorations, fractional scale, tiling
and a script interface to drive it as a user's shortcut would
(`tools/kwin_script.sh`, `testdata/kwin/*.js`). `kwin_test` asks for the
server's decoration, then its own, then the server's again, each granted as
asked (`zxdg_toplevel_decoration_v1.configure`); is quick-tiled to the left
half — `STATE_TILED_LEFT | TOP | BOTTOM`, exactly half the output — then taken
out of its tile, back to its size; is minimized and shown again
(`xdg_toplevel` 6's `suspended` is `STATE_MINIMIZED`, its size kept); and at a
1.5 output scale opens at a preferred scale of 180, its buffer 450 by 300 for
300 by 200 logical pixels, read back from the X screen as 450 by 300. On
Weston, with no decoration manager, `decorated` is 0 and the toolkit draws
its own frame. KWin found two faults no Weston run could: the `xdg_surface`'s
id was handed out before the viewport's and fractional scale's, which a
server refuses ("not a valid new object id"), so fractional scale failed on
every compositor offering it; and an output's integer scale, arriving after
the surface's preferred fractional one, replaced it. Each new id is now taken
where its request is sent, and a surface with a fractional scale ignores the
output's integer one.

`monitor_of` says the output the window entered as `wl_output` describes it:
its place in the compositor's space (geometry), its current mode and refresh
rate, the window's scale there and its name (version 4); Wayland has no work
area, so it is the whole output.

Popups are `xdg_popup`s of their owner's `xdg_surface`: a positioner made
from the toolkit's numbers (logical pixels, relative to the owner's window
geometry), placed and placed again (`reposition`, xdg_popup 3) by the
compositor, `popup_done` reported. A grab is asked under the owner's last
press or key serial before the first commit; with no seat there is nothing to
grab, and `grabbing` says so. The compositor gives a press on any surface of
this client to that surface; one on a window that is not a popup takes the
innermost grabbing popup down here and is dropped, press and release, as on
X11 and Windows. `popup_wayland_test` runs on a kiosk Weston (the owner fills
the output, so the flipped and slid places are known), headless and seated —
Weston nested in Xvfb, XTest pointer and keys: the grab granted, the keys and
presses of a popup chain, a submenu under its menu's serial, a press outside.

Subsurfaces (`wl_subcompositor`) are windows of the same connection with no
shell role: `open_subsurface` makes one of a given logical size at a place in
its parent, at the parent's scale and taking the parent's scale changes; it
has no window geometry. Its place (`sub_set_position`) and order among
siblings (`sub_place_above`, `sub_place_below`) take effect with the parent's
next commit (`commit`), as do its own commits while it is synchronized
(`sub_set_sync`, the default); desynchronized, a commit is shown at once. A
closed child is gone, its role destroyed with it; a parent closed first
leaves its child inert until it is closed. `subsurface_test` reads the screen
back from the X server Weston is nested in, at scales 1 and 2, then on two
outputs of scales 1 and 2 (`testdata/two-scales.ini`): the parent dragged by
an XTest pointer from one to the other, parent and child both take the new
scale and are shown at it.

A separate Weston/X11 software-rendering smoke uses a real compositor seat,
with XTest keyboard and pointer input. The editor accepts typing and Ctrl+A,
opens Preferences with Ctrl+, and resumes input after its dialog is closed.
The settings form accepts text and Tab; header maximize/restore preserves
its original buffer size. The browser accepts an edited directory path and
scrolls its table. Closing the modified settings form shows its native
confirmation dialog; selecting No exits cleanly. Screenshots establish the
visible native surfaces and client-side frames. External desktop clipboard,
DnD and installed IME interaction remain unverified. Presentation sends
explicit surface damage
before the frame request and commit, including when a buffer is reused.

Regenerate the protocol inventory from XML with
`sh window/wayland/tools/check_protocols.sh`. It links wayland-scanner's
private code with `tools/scanner_dump.c` and requires the generated catalog to
match the scanner's interface versions, opcodes, since versions, signatures and
argument interfaces exactly. The catalog covers core, xdg-shell, viewporter,
xdg-decoration, text-input-v3, primary selection, fractional scale, cursor
shape, activation, dialog, xdg-foreign-v2, toplevel icon, presentation time,
single-pixel buffer, system bell, toplevel drag, tablet-v2 and pointer
gestures. Metadata alone implements none of these protocols; each client
behavior is tracked and exercised separately.
