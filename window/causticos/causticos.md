# CausticOS windows

A window on CausticOS is a client of its window manager: a channel carrying
32-byte messages and a segment the program draws into, protocol 4 as the
causticos repository's `userspace/lib/wmproto` defines it. `proto.cst` speaks
it; `backend.cst` is the window.

```
causticos/
  proto.cst     the messages, packing, a reader that assembles them
  backend.cst   the window: surface, frames, input, popups, clipboard, drops
```

## The surface

The program draws into memory of its own, width × height with no gap between
rows — what a target is. The window manager reads a segment whose rows are as
long as the screen is wide, so that a resize changes how much of it is used,
not where rows start; `submit` copies the frame in. The segment holds two
buffers: one shown, one written. A frame goes up with `PRESENT`, and the next
is written once `FRAME` says the other buffer is free; a submit that comes
sooner waits for it and is the one shown. What is copied is this frame's
damage and the last one's, which the buffer written now has not seen.

## Windows, dialogs, popups

The first window is the key the program was started with (`KEY_WM`). Every
other one — a second window, a dialog, a popup — is asked for on its owner's
channel (`WINDOW_NEW`) and its own channel comes back there; opening one waits
for that answer, and for `WELCOME`. A popup is placed by the window manager
against a rectangle of its owner, with `positioner.cst`'s anchor, gravity and
adjustments, which are the protocol's own numbers; `POPUP_PLACED` says where.

## The frame

The toolkit draws its own title bar. `SET_FRAME a=0` asks the window manager
for none of its own; a press on the toolkit's title bar or edge becomes
`MOVE_RESIZE` (the drag is then the window manager's, and the press is said to
be up, `BUTTON_TAKEN`), its buttons `SET_STATE`, and `STATE` tells it when it
is maximised or minimised.

## Input

`KEY` carries what the layout made of the key — a code point, or a named key —
its modifiers and its set-1 scancode. The key a program sees is the named key
or the character's (a capital is its letter's key); a shortcut is matched
against the letter where a US keyboard has it, from the scancode; what it
typed comes as `TEXT` unless Ctrl, Alt or Super were held. The pointer comes as
a button mask and a position: `BUTTON_DOWN`/`UP` for each bit that changed,
`MOTION`, `ENTER` and `LEAVE` at the content's edge (`POINTER_TRACK` asks for
motion with no button held). An input method's composition is `IME_PREEDIT`,
its result `IME_COMMIT`.

## What it does not have

No primary selection, and the clipboard is text: the window manager keeps
one string. No drag source yet — drops are received. No transparency: the
window manager composes opaque content. No scale: one pixel is one pixel.

## Fonts

The toolkit's text comes from `/fonts` (`window/fonts.cst`), which the
desktop profile fills with Noto Sans and Noto Sans Mono. No index is kept: the
root lives in memory and is made again at every boot.

## Testing

`tools/causticos/run.sh` builds tests for `caustic-x86_64`, puts them on a
root image beside the repository's `testdata/` and runs them in one headless
boot of QEMU.
