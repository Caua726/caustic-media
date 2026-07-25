# The input layer

Keyboard, mouse, touch, pen, gamepad, haptics and sensors — from every source a
platform offers, including the ones no window system delivers.

```
input/
  input.cst     hub
  source.cst    our abstraction: open a source, drain it, ask what it can do
  event.cst     the event types and the queue they arrive in
  keyboard.cst  keys, scancodes and keysyms, modifiers, layout
  mouse.cst     buttons, position, wheel, relative motion
  touch.cst     fingers
  pen.cst       pressure, tilt, proximity, barrel buttons
  gamepad.cst   buttons, axes, the standard layout
  mapping.cst   the controller database
  haptic.cst    rumble and force feedback
  sensor.cst    accelerometer, gyroscope
  text.cst      text input and IME composition
  state.cst     sampled state, derived from events
  action.cst    action mapping, derived from events
  gesture.cst   tap, drag, pinch, hold — derived from touch

  x11/      core events + XInput2                  x11/x11.md
  wayland/  seat, keymap, relative pointer, tablet  wayland/wayland.md
  win32/    messages, Raw Input, XInput             win32/win32.md
  evdev/    /dev/input/event* — raw, global         evdev/evdev.md
  hidraw/   /dev/hidraw* — what evdev cannot say    hidraw/hidraw.md
```

---

## Gamepads do not come from the window system

This is what makes `input/` a layer rather than an interpretation of
[`window/`](../window/window.md)'s events.

X11 and Wayland deliver keyboard, mouse and touch. **Neither delivers a
gamepad.** On Linux that is evdev — `/dev/input/event*`, with a `js0` present on
the development machine right now. On Windows it is XInput or GameInput, separate
DLLs from the windowing API.

So `input/` has platform code of its own, and the split with `window/` is:

| Device | Where it comes from |
|---|---|
| keyboard, mouse, touch | `window/` — the display server delivers it |
| gamepad, joystick, haptics, sensors | **`input/`'s own backends** |

`window/event.cst` hands over the platform event as it arrived — an `XEvent`, a
Wayland callback, a `MSG`. `input/x11` knows how to read an `XEvent`;
`window/x11` does not need to.

---

## Cooked and raw are different inputs, not two ways to get the same one

The reason both a display-server path and an evdev path exist is that they carry
different things:

| | Through the display server | Through evdev |
|---|---|---|
| Focus | respected — nothing arrives unless the window is active | **global**, arrives regardless |
| Mouse | accelerated by the desktop's curve | **raw sensor counts** |
| Key repeat | filtered or synthesised by the server | raw |
| Layout | translated — ABNT2 gives you the character | physical scancode |
| Permission | none | the `input` group |
| Latency | through the server | straight from the kernel |

A game wants both, for different jobs: menus and text from the cooked path,
mouselook and competitive input from the raw one. That is what
`SDL_SetRelativeMouseMode` and Windows Raw Input exist for.

**The program chooses, explicitly, and they are separate objects.**

```cst
let is input.Source as cooked = input.open(&window);       // focus-aware
let is input.Source as raw    = input.open_raw();          // global, needs permission
```

Both can be open at once. Nothing merges them, because merging risks double
events and stuck keys, and because a program that cannot tell where an event came
from cannot reason about it. Every event carries its source.

`open_raw` fails honestly when permission is missing, rather than silently
producing nothing.

---

## Events are the primitive

The philosophy decides this one:

> **No magic, no implicit.** Every operation is visible. No hidden allocations,
> no runtime surprises.

`IsKeyDown(SPACE)` is implicit by construction: something polled the platform
behind you, at a moment you did not choose. Two presses in one frame vanish
silently, which is precisely a runtime surprise. `GetMouseDelta` implies someone
kept last frame's position for you — hidden state.

So the primitive is a queue you drain, and nothing is lost:

```cst
while (input.next(&source, &ev) == 1) {
    // every event, in order, with its source
}
```

The queue is **explicitly sized**, since one that grows on its own is the hidden
allocation the same sentence forbids. The same rule the draw queue follows in
[`render/`](../render/render.md).

**Sampled state still exists — as something you feed.** `state.cst` is an object
the program creates and hands events to, then queries:

```cst
input.state_feed(&st, &ev);
...
if (input.state_key_down(&st, KEY_SPACE) == 1) { ... }
```

That is the ergonomics of `IsKeyDown` with the mechanism visible. You can see
where the state came from, and a program that wants two independent states — one
for the game, one for a replay — gets them for free.

`action.cst` and `gesture.cst` work the same way: they are fed, they derive.

---

## Scancode and keysym are not the same key

Worth stating because getting it wrong is a bug users report and developers
cannot reproduce.

- **Scancode** is the physical key position. `W` is the same key on QWERTY and
  AZERTY, so WASD movement should bind to scancodes or it becomes ZQSD in France.
- **Keysym** is what the key produces given the current layout. A shortcut the
  user reads as "Ctrl+S" should bind to keysyms.
- **Neither is a character.** That is `text.cst`'s job — see below.

Both are carried in a key event. Neither is the default, because there is no
correct default: a movement binding and a shortcut binding want opposite answers.

---

## Text input is not key input

A key press is not a character. Between them sit the layout, dead keys — where
`´` then `a` produces `á` and neither keystroke alone produces anything — and
input methods, where a Japanese or Chinese user composes over several keystrokes
with a candidate list on screen.

SDL carries three separate event types for this, and it is right to: text
arrives, text is being *edited* but not committed, and candidates are being
offered.

So `text.cst` is its own thing, fed by the platform's text machinery — XIM or
`ibus` on X11, the `text-input` protocol on Wayland, `WM_CHAR` and the IME API on
Win32 — and it produces committed text, a composition string, and a candidate
list. `ui/` needs all three to have a text field a Japanese user can type in.

This lives here rather than in `text/`, which is fonts, atlases and layout.
Codepoint handling itself belongs to neither:
[caustic-unicode](https://github.com/Caua726/caustic-unicode) already carries
UTF conversion, normalisation, grapheme segmentation and bidi.

---

## The controller database

A gamepad reports raw button and axis indices, and they differ per model: button
0 is not "A" on every controller, and the sticks are not always axes 0–3. Without
a mapping, every program grows a table of exceptions per device.

SDL solves this with a community-maintained database keyed by device GUID —
**274 entries are embedded in the copy of libSDL3 on this machine**. It is data,
not code, and it is the same data everyone needs.

So: **carry it, in SDL's format.** The format is a line per controller keyed by
GUID, it is public, and it is maintained by people who own the hardware. Parsing
it is trivial and a program can add its own entries at run time, which is how an
unrecognised controller gets fixed without waiting for us.

Reimplementing the mapping ourselves would mean owning a hardware compatibility
table, which is a maintenance commitment with no upside.

---

## Action mapping

Neither SDL nor raylib has this, and every game rebuilds it: "the JUMP action is
Space, or gamepad South, or the left trigger past a threshold".

`action.cst` binds `(device, control)` pairs to named actions and derives their
value from the event stream — a bool for buttons, a scalar for axes and
triggers, a vector for sticks and for WASD treated as one. It is fed like the
other derived layers, so nothing happens invisibly.

It is opinion, and it is the one piece of opinion here worth having: rebinding
keys, supporting a gamepad and a keyboard at once, and dead zones are things
every program needs and no program enjoys writing twice.

---

## Gestures

Tap, double tap, hold, drag, swipe, pinch and rotate. raylib has them; SDL leaves
you the raw fingers.

They belong here, derived from touch events like everything else in this layer's
second half, because `ui/` needs tap and drag at minimum and deriving them from
fingers is a state machine nobody should write twice.

---

## What each backend must answer

Not every source has every device, and pretending otherwise means a program
finds out by getting nothing. `source.cst` answers what a source can do, the way
`gpu/device.cst` reports capabilities.

| | keyboard | mouse | touch | pen | gamepad | haptics |
|---|---|---|---|---|---|---|
| x11 | core | core | XInput2 | XInput2 | — | — |
| wayland | seat | seat | seat | tablet protocol | — | — |
| win32 | messages | messages + Raw Input | messages | messages | XInput | XInput |
| evdev | yes | yes | yes | yes | **yes** | yes |
| hidraw | — | — | — | — | reports | reports |

evdev is the only one that covers everything, which is why it is not merely the
raw path but also the gamepad path on Linux.

---

## Order of work

1. **Event types and the queue**, with `x11` reading `window/`'s events — enough
   to replace the sampled fields the window backend carries today.
2. **`state.cst`**, so the ergonomics arrive with the mechanism.
3. **evdev**, which brings gamepads and the raw path at once, and is the backend
   KMS has no alternative to.
4. **The controller database and `action.cst`.**
5. **Text input**, when `ui/` needs a text field.
6. **wayland and win32**, alongside their window backends.
