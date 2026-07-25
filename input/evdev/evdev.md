# evdev

`/dev/input/event*` — the kernel's own input interface, read directly. **No
library, no display server, no permission from anything but the `input` group.**

```
evdev/
  device.cst    enumeration, capabilities, hotplug
  read.cst      the event stream
  ff.cst        force feedback
  backend.cst   input/source.cst implemented over them
```

This is the backend that makes [`input/`](../input.md) a layer rather than an
interpretation of window events: **no display server delivers a gamepad**, and
this is where one comes from on Linux.

---

## What it carries

`linux/input-event-codes.h` declares **527 keys, 123 buttons and 46 axes**, and
`linux/input.h` **36 ioctls**. Every input device on the machine appears here —
23 event nodes on the development machine, from the keyboard and mouse down to
the power button and the HDMI audio jacks' presence detection.

The event structure is four fields: a timestamp, a type, a code and a value. A
key press is `EV_KEY` with the keycode and 1; a mouse move is two `EV_REL`
events; a touchscreen contact is a cluster of `EV_ABS` ending in `EV_SYN`. That
last one matters — `EV_SYN` marks the boundary of an atomic update, and a reader
that acts on events before the sync sees a pointer at half its new position.

---

## Raw is the point, and so is global

What arrives here is what the hardware sent: **unaccelerated mouse counts,
physical scancodes, unfiltered key repeat, no focus.** That is the whole reason
this path exists alongside the display server's, as [`../input.md`](../input.md)
describes.

Global also means a program reading evdev sees keystrokes typed into other
windows. That is a real capability and a real hazard, and it is why the `input`
group exists — the permission is the boundary, not the API.

`open_raw` failing for lack of permission must therefore say so plainly, since
"add yourself to the `input` group" is a fix a user can act on and silence is not.

---

## Capabilities, before reading anything

A device does not announce what it is. `EVIOCGBIT` reports which event types and
codes it supports, and the classification follows from that: something with
`EV_KEY` and `BTN_LEFT` and `EV_REL` axes is a mouse; something with
`ABS_MT_POSITION_X` is a multitouch surface; something with `BTN_GAMEPAD` and
absolute axes is a gamepad.

Getting this wrong is how a program ends up treating a laptop's lid switch as a
game controller — every device in `/dev/input` looks the same until asked.

`EVIOCGNAME`, `EVIOCGID` (bus, vendor, product, version) and the `uniq` identify
it, and the vendor/product pair is the key into the controller database described
in [`../input.md`](../input.md).

---

## Hotplug

New devices appear as new nodes. Watching for them means either polling the
directory, or listening on a netlink socket for the kernel's uevents — the
latter is what `udev` does and it is a socket read rather than a library.

A gamepad plugged in mid-game should appear without restarting, so this is not
optional polish.

---

## Force feedback

`EV_FF` and 37 constants in `input.h`: rumble, periodic, constant force, spring,
damper. Uploading an effect with `EVIOCSFF` and playing it by writing an event is
the whole interface, and it is how a controller vibrates on Linux without any
library at all.

That makes [`haptic.cst`](../input.md) genuinely dependency-free on this
platform, which the Windows equivalents are not.

---

## Order of work

Third in the layer, after the event model and the sampled-state helper — and the
first backend that brings something no other path can deliver.

1. **`device`** — enumerate, classify, identify.
2. **`read`**, respecting `EV_SYN` boundaries.
3. **Gamepad classification and the mapping database.**
4. **Hotplug over netlink.**
5. **`ff`.**
