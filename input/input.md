# The input layer

Keyboard, mouse, touch, pen, gamepad, haptics and sensors — from every source a
platform offers, including the ones no window system delivers.

```
input/
  input.cst     hub
  source.cst    our abstraction: open a source, drain it, ask what it can do
  event.cst     cooked input and the explicitly sized queue it arrives in
  keys.cst      key ids (the layout's keysyms) and modifier bits
  keyboard.cst  keys, scancodes and keysyms, modifiers, layout
  mouse.cst     buttons, position, wheel, relative motion
  touch.cst     fingers
  pen.cst       pressure, tilt, proximity, barrel buttons
  gamepad.cst   buttons, axes, the standard layout
  mapping.cst   the controller database
  haptic.cst    rumble and force feedback
  sensor.cst    accelerometer, gyroscope
  text.cst      what a text field tells an input method: purpose, hints, limit
  state.cst     sampled state, fed from events
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

`window/` translates what the display server delivers — an `XEvent`, a Wayland
callback, a `MSG` — into `input/event.cst`'s cooked events as it arrives, one
queue per window (`poll_input`); what happens to the window itself (resized,
focused, closed, dropped on) stays `window/event.cst`'s (`poll_event`).

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

So the primitive is a queue you drain, and nothing is lost — or, when the
queue was full, a `CANCEL` says how much was:

```cst
let is ie.Event as ev;              // use "caustic-media/input/event.cst" as ie;
while (window.poll_input(&win, &ev) == 1) {
    // every event, in order
}
```

The queue is **explicitly sized**, since one that grows on its own is the hidden
allocation the same sentence forbids. The same rule the draw queue follows in
[`render/`](../render/render.md).

**Sampled state still exists — as something you feed.** `state.cst` is an object
the program creates and hands events to, then queries:

```cst
state.new_frame(&st);               // motion and scrolling are per frame
while (window.poll_input(&win, &ev) == 1) { state.feed(&st, &ev); }
if (state.key_down(&st, keys.SPACE) == 1) { ... }
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

## Current state

Built: the cooked event model and its queue (`event.cst`), the key ids and
modifier bits every backend names keys with (`keys.cst`), and sampled state as
something fed (`state.cst`). Each window backend translates its platform's
input into a queue of its own — X11 core events with XLookupString/XIM, Wayland
`wl_keyboard`/`wl_pointer`/text-input-v3 with xkbcommon, Win32 messages and
IMM — and `window.poll_input` hands it out; the backends keep no sampled input.

What the queue promises:

- **Kinds:** `KEY_DOWN`/`KEY_UP` (key id, shortcut, physical place, the
  platform's repeat), `TEXT` (committed UTF-8, `from_key` when typed by the
  key just before it), `PREEDIT`, `DELETE_SURROUNDING`, `BUTTON_DOWN`/`UP`,
  `MOTION`, `ENTER`, `LEAVE`, `SCROLL` (lines, whole steps, 120ths of a notch)
  and `CANCEL`.
- **Text is the queue's**, copied in when pushed and valid until the next
  `next()`: no pointer into a platform buffer outlives its translator.
- **Fixed size.** Motion waiting at the end absorbs newer motion; anything else
  that does not fit is refused and counted, and once what came before is taken
  a `CANCEL` (`CANCEL_LOST`, with the count) says so — a lost release is never a
  key held forever.
- **Focus loss is a `CANCEL`** (`CANCEL_FOCUS`): everything held is let go,
  keys and buttons, and a release that comes after says nothing.
- **A press the window system takes** — a move or resize it follows from there
  — ends in `BUTTON_UP` with detail `BUTTON_TAKEN`: it clicks nothing, and the
  release the system keeps never arrives.
- **Text is only text.** Control characters, and what a key held with
  Control, Alt or Super types, are never `TEXT`; AltGr's characters are.
- **Keys across layouts.** `key` is the key in the layout in use, at its first
  level (`a`, not `A`; Cyrillic es in Russian); `shortcut` is the key a
  shortcut is matched against — the same where it is Latin or a function key,
  else the same key's Latin letter read off another layout of the keymap
  (X11's XKB groups, Wayland's xkbcommon layouts; on Windows the virtual key's
  letter, which no layout changes). Nothing is guessed from keycodes: a key no
  layout makes Latin keeps its own.
- **Repeat is said by every platform**: X11's detectable auto-repeat, Windows'
  bit 30, and on Wayland the client's own timer at the compositor's
  `repeat_info` rate and delay, cancelled when the key is let go, the focus
  leaves, the keyboard goes or the keymap is replaced; `window/wait.cst` and
  the host sleep no longer than the next repeat (`device.input_due`).

`ui/host.cst` feeds a window's queue to its router: a key's `TEXT` is typed
unless the router took that key; text an input method or another program sent
is typed whatever is held. With Control or Super held the router is told the
key's `shortcut`, so Ctrl+C, Ctrl+A and Ctrl+V work in a Cyrillic layout; with
Alt alone, the layout's key, which mnemonics mean.

Proved with real platform input: `keyboard_x11_test` (XTest, us/ru/de groups:
keys, Ctrl+C in Russian, Caps Lock, AltGr, the focus taken by another
program's window while a key is held), `keyboard_wayland_test` (XTest through
Weston nested in Xvfb with US international, Russian and German: Ctrl+C in
Russian, Cyrillic text, the client's repeat, a dead acute composing é, Caps
Lock, AltGr), `keyboard_win32_test` (SendInput under Wine: scan codes,
repeat, Caps Lock, Alt, Ctrl+C with a Russian layout loaded) and
`host_x11_test` (Ctrl+A selecting an entry with the Russian group in use).
Wine translates SendInput's keys to text through its X keymap whatever layout
is loaded, so a Russian or German layout's text on Windows awaits real
Windows. X11 without an input method types only Latin-1 and composes no dead
key (XLookupString); the toolkit opens one whenever a text field has the
keyboard, so there any layout's text and dead keys come through it.

Input methods: on X11 the display's XIM — on the spot where the method
offers it (the composition as `PREEDIT`, with `hl_start..hl_end` the part
being converted, which the entry and text view show as selected), over the
spot otherwise, Xlib's own local method with no server — told the caret as
its spot, reset when the field gives the composition up. Proved with the
installed Fcitx5 (`xim_x11_test` on and over the spot, `ime_x11_test` an
entry composing and committing, its panel at the entry's caret, the
composition given up when the focus moves) and with Xlib's local method (a
dead key composing é).

What the field is, told the method (`text.cst`, `device.text_input_context`
from the host for the field with the keyboard): its text around the caret —
the caret's paragraph, at most 3999 bytes of whole characters, the selection
in it as far as it fits (`ui/access.cst`'s `text_around`) — and its purpose
and hints from its role (a password, many lines). A password's text is never
told; on X11 and Windows the method does not even get its keys. On Wayland,
text-input-v3 follows the protocol's rules: enable only with the text-input
focus and everything told again after each enter, state told when it changed
and held while a `done` is behind the commits, the change's cause (the
method's commit or not), `done` applied as delete, commit, then the
composition (its cursor range the part being converted), a composition not
sent again gone, malformed strings ignored, and a method's commit never cut.
A key's text is typed whatever the method: a compositor's method grabs the
keys it composes with. On Windows, IMM32: an input context only while a field
that is not secret has the keyboard, the composition with its caret and its
target clause (`GCS_COMPATTR`) as the converted part, candidates under the
caret and never over it (`CFS_EXCLUDE`), the method's own composition window
kept hidden, `CPS_CANCEL` to reset, and the field's text with the
composition in it for a method that asks (`IMR_DOCUMENTFEED`), the caret's
place on the screen (`IMR_QUERYCHARPOSITION`). Deleting around the caret
(`DELETE_SURROUNDING`) is counted from the selection's ends and keeps it.

Proved: `wayland/text_input_test` on the wire (order, serials, malformed and
boundary strings, a password, leave and enter, reset);
`text_input_wayland_test` on KWin (keys type with text input on — they did
not before — and the installed Fcitx5, as KWin's input method, told the
field's text, selection and kind, and a password's kind but not its text);
`ime_win32_test` through Wine's IMM32 (composition, commit, reset, no context
without a field or in a password's, the document asked) and
`program_win32_test` (an entry of a toolkit program composing and
committing through it). Not proved here: an installed method composing on
Wayland — KWin nested in Xvfb never hands XTest's keys to its input method's
grab, a GTK client's neither — and Wine's IMM32 keeps no attributes it is
given, so the target clause is checked from attributes as a method sets
them.

Still to come, in this order: continuous scrolling (axis source, stop,
value120 on Wayland); touch, pen and gestures; gamepads, raw devices and the
action mapping.
