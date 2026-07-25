# Wayland input

Keyboard, mouse and touch from a native Wayland session. Not XWayland — that is
[`../x11/x11.md`](../x11/x11.md).

```
wayland/
  seat.cst      wl_seat: pointer, keyboard, touch capabilities
  keymap.cst    the keymap the compositor sends, via xkbcommon or our own
  relative.cst  relative-pointer and pointer-constraints — mouselook
  tablet.cst    tablet-v2: pen pressure, tilt, tools
  gestures.cst  pointer-gestures: pinch and swipe from a touchpad
  backend.cst   input/source.cst implemented over them
```

---

## One seat, three capabilities

Wayland groups input under a `wl_seat` — a user's set of devices. The seat
announces capabilities, and the client binds the ones it wants: `wl_pointer`,
`wl_keyboard`, `wl_touch`. Devices appearing and disappearing is a capability
event, so **hotplug is in the protocol** rather than something to watch for.

That is cleaner than X11 and it comes with a consequence: there is no way to
address an individual device. Two mice are one pointer, always, and a program
that wants per-device input has to read evdev — the protocol does not offer it.

---

## The keymap arrives as a file descriptor

This is the design decision that shapes the backend. Wayland does not translate
keycodes; it sends the client **an XKB keymap** as a shared-memory file
descriptor, once, and then delivers raw keycodes forever after.

So the client must interpret an XKB keymap to know what any key means. Two ways:

- **`libxkbcommon`** — 374 KB and 108 symbols, the reference implementation, and
  a dependency.
- **Our own** — the keymap is a text format, and interpreting it is table lookup
  plus modifier state. Not small, and not enormous either.

Every Wayland client faces this, which is why libxkbcommon exists at all. Taking
the dependency first and replacing it later is reasonable; taking it forever is
not, given the project's position on libraries.

Receiving the keymap also needs `mmap` of a descriptor received over the socket —
the same `SCM_RIGHTS` machinery Wayland needs everywhere and
`std/os/linux.cst` still lacks.

---

## Mouselook needs two extension protocols

Wayland deliberately does not let a client warp the pointer or read absolute
motion outside its surface — both are security decisions. So the X11 trick of
warping to the centre does not exist.

Instead:

- **`relative-pointer-unstable-v1`** delivers unaccelerated deltas.
- **`pointer-constraints-unstable-v1`** locks or confines the pointer to the
  surface.

Both are present on this machine. A program doing mouselook binds both, and one
without the other is useless — locked without relative motion gives nothing;
relative without locking lets the pointer wander onto another window.

This is the case where Wayland is *better* than X11 rather than merely different:
the semantics are explicit instead of being a grab-and-warp convention.

---

## Tablets and gestures are their own protocols

`tablet-v2` carries pens properly: pressure, tilt, distance, rotation, tool
identity, and eraser versus tip. Present here, stable, and much better specified
than X11's valuator approach.

`pointer-gestures-unstable-v1` reports touchpad pinch and swipe as gestures the
compositor recognised, rather than as raw touch to be interpreted — which means
[`gesture.cst`](../input.md)'s own state machine is for touchscreens, and
touchpads come pre-analysed.

---

## Text input

`text-input-unstable-v3` is small, modern, and does what XIM does with a fraction
of the surface: preedit string, commit, and cursor position. A text field a
Japanese user can type into costs far less here than on X11.

---

## Order of work

Alongside the Wayland window backend, which shares the connection.

1. **`seat`**, pointer and keyboard.
2. **`keymap`**, with libxkbcommon first.
3. **`relative` and constraints**, since mouselook is the thing that does not
   work without them.
4. **Touch, then `tablet` and `gestures`.**
5. **`text-input`**, when `ui/` needs a field.
6. **Our own XKB interpretation**, to drop the dependency.
