# hidraw

`/dev/hidraw*` — HID reports straight from the device, with nothing interpreting
them. What evdev and XInput cannot describe.

```
hidraw/
  device.cst    enumeration by vendor and product
  report.cst    reading and writing raw reports
  descriptor.cst  parsing the HID report descriptor
  backend.cst   the devices that need it
```

Seven such devices on the development machine, alongside 23 evdev nodes for the
same hardware — because they are two views of the same devices, at different
levels.

---

## Why a raw path exists at all

evdev and XInput present a *normalised* device: buttons, axes, a standard layout.
That normalisation is the point, and it is also lossy. What it drops:

- **Gyroscope and accelerometer** on a DualShock or Switch controller. The
  kernel exposes the buttons and sticks; the motion sensors are in reports evdev
  does not map.
- **Battery level and charging state**, on controllers that report it in a
  vendor-specific way.
- **Rumble that is not simple rumble** — trigger haptics, the adaptive triggers
  on a DualSense, the speaker.
- **Lightbar and LED colour.**
- **Anything a device does that no standard covers**, which is most of what makes
  a controller distinctive.

So this is not an alternative to evdev — it is the layer underneath, used
alongside it for the same device: buttons from evdev, gyro from hidraw.

---

## Report descriptors, and why parsing them is optional

Every HID device carries a **report descriptor**: a byte-code description of what
its reports contain, which is how a generic driver knows that byte 3 bit 2 is a
button. Parsing it properly means implementing the HID specification's item
encoding, usage pages and collections.

Two approaches, and both are legitimate:

**Parse the descriptor** and handle any device generically. Correct, and
considerable work for a class of device where the generic answer is already
available through evdev.

**Match on vendor and product**, and use a hand-written layout per controller.
Which is what the interesting cases actually need — the DualShock's gyro is at a
known offset because someone reverse-engineered it, not because the descriptor
said so.

The second is what SDL does with `hidapi`, and it is honest: the devices worth
reaching this way are worth reaching specifically.

---

## The same thing on Windows

Windows has `hidsapi` and `HidD_*` functions serving the same purpose, so this
backend has a direct counterpart — and the code that decides "byte 3 is the
gyro's X axis for this controller" is shared, since it is about the hardware
rather than the operating system.

That makes the per-device knowledge the valuable part and the platform access the
easy part, which is unusual in this library and worth structuring for: the
descriptor tables live in `report.cst`, the platform access does not.

---

## Permissions

`/dev/hidraw*` is root by default on many distributions — stricter than
`/dev/input`, since raw HID access reaches devices that are not input at all.
udev rules are the usual answer, and this is another place where failing loudly
with a fixable message beats returning nothing.

---

## Order of work

Last of the input backends. Nothing needs it until a program wants what a
controller offers beyond buttons and sticks.

1. **`device` and `report`** — enumerate by vendor and product, read reports.
2. **A layout for one controller**, proving the shape.
3. **The Windows counterpart**, sharing the layout tables.
4. **Descriptor parsing**, only if generic devices turn out to matter.
