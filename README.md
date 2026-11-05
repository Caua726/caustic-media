# caustic-media

**Windowing, input, graphics and audio for [Caustic](https://github.com/Caua726/Caustic) — roughly what SDL is for C, plus a renderer and a UI layer.**

![version](https://img.shields.io/badge/version-0.0.2-blue)
![status](https://img.shields.io/badge/status-8%20of%2010%20layers%20built-yellow)
![license](https://img.shields.io/badge/license-MIT-blue)

> **Implementation status varies by layer and platform.** The software
> renderer, image/text stack and retained toolkit have working code and tests;
> the three toolkit examples are exercised on X11, on a seated Weston/Wayland
> software compositor and on Windows under isolated Wine, each driven with
> the system's own keyboard and pointer input. External Wayland clipboard,
> DnD and installed IME interaction remain unverified. On Windows the file
> dialog, known folders, one instance and opening links are native; the
> toolkit's menus in native popups, OLE DnD, UI Automation and notifications
> are not there yet, and Wine is not proof of real-Windows behaviour.
>
> The architecture examples below also include designed interfaces that are
> not implemented — Vulkan, OpenGL and Direct3D devices, `3d/`, and every
> audio backend but ALSA. See **Status** and each module's **Current state**
> before treating a platform or API as supported.

## Layers

Each layer is meant to stand on its own and hand out the handle of the layer
below it, so a program can take the whole stack, take part of it, or take a
window and drive the GPU itself.

| Layer | Holds | State |
|---|---|---|
| `math/` | vectors, matrices, quaternions, geometry, colour, curves — pure | **done**, tested |
| `gpu/software/` | the software rasteriser | **done**, tested |
| `window/` | window, display, platform | X11, Wayland (direct socket) and Win32 backends, tested on private servers and Wine; see each backend's current state for what remains |
| `gpu/` (device) | the portable device, in the shape of wgpu | built over the software backend, tested; Vulkan bindings generated, vk, gl, d3d devices designed |
| `input/` | keyboard, mouse, touch, pen, gamepad, haptics, sensors | cooked events, their queue, key ids and fed state built and used by every window backend; devices of its own designed |
| `render/` | the framework: meshes, sprites, materials, cameras, a frame | the 2D family built and tested — what the toolkit draws with; 3D pieces over the software device |
| `3d/` | models, meshes, skeletons, animation | design note only |
| `image/` | pixels on the CPU: load, save, transform, generate, atlas | built, tested; decoding through caustic-image |
| `text/` | fonts, glyph atlases, shaping, layout | built, tested: faces, outlines, variable fonts, shaping held to HarfBuzz with the scripts that join and reorder (Arabic, Indic, Hangul, Thai, Khmer, Myanmar, the Universal Shaping Engine), bidi layout, system fonts |
| `audio/` | device, mixer, music, positional | device, sample conversion and a direct ALSA backend; the other backends a design note each |
| `ui/` | a toolkit for applications, drawn by us; immediate mode beside it in `ui/immediate/` | **built**, tested: three example programs on X11, Wayland and Windows; immediate-mode prototype, tested |
| `dbus/` | the D-Bus wire protocol, connections, objects | **built**, tested against busctl |

The shape the whole thing is aiming at:

```
math/     vectors, matrices, quaternions, geometry, colour, curves   — pure
window/   window, display, platform:  x11 | wayland | kms | win32
input/    keyboard, mouse, touch, pen, gamepad, haptics, sensors
            cooked from the display server, or raw from evdev
gpu/      the device, in the shape of wgpu:  vk | gl | d3d | software
render/   the framework: meshes, sprites, materials, cameras, a frame
3d/       models, meshes, skeletons, animation
image/    pixels on the CPU: load, save, transform, generate, atlas
text/     fonts, glyph atlases, shaping, layout
audio/    device, mixer, music, positional:  alsa | wasapi | causticos
ui/       a toolkit for applications, drawn by us;  immediate/ for HUDs and tools
```

Each has a design note beside it recording what it holds and why:
[`math/`](math/math.md) · [`window/`](window/window.md) · [`input/`](input/input.md) ·
[`gpu/`](gpu/gpu.md) · [`render/`](render/render.md) · [`3d/`](3d/3d.md) ·
[`image/`](image/image.md) · [`text/`](text/text.md) ·
[`audio/`](audio/audio.md) · [`ui/`](ui/ui.md), with one per backend under
`window/`, `input/`, `gpu/`, `3d/`, `text/` and `audio/`.

`math/` and `gpu/software/` import nothing below them, so a program that only
needs geometry or software rendering still builds with no external dependency.

The split between `gpu/` and `render/` is deliberate. `gpu/` is a portable GPU —
buffers, pipelines, submission, compute — at the level the hardware works, and it
owns every backend including software. `render/` is a client of it, the way a
game framework is a client of wgpu: it decides what to draw and has no backends
of its own. A program that wants to drive Vulkan itself takes `gpu/`, or reaches
past it to the raw bindings, and leaves the rest.

## Choosing a backend

Every layer that has backends lets the program pick, and the choice happens at
two levels.

**What is in the binary** is decided in the Causticfile — see *What ships in a
binary* below. A backend that was not compiled in costs nothing and cannot be
selected.

**What is used** is decided by the program, at run time, among those:

```cst
// let the library decide
let is gpu.device.Device as d = gpu.open(&surface, gpu.AUTO);

// or name it — per platform, per program, per device
let is gpu.device.Device as d = gpu.open(&surface, gpu.VULKAN);
let is media.window.Window as w = media.window.open_with(media.window.WAYLAND, ...);
let is media.input.Source as s = media.input.open_source(media.input.EVDEV);
```

A type lives in the module that declares it — `gpu.device.Device`, the way
`soft.target.Target` already reads — because Caustic's `type` alias does not
cross a module boundary. Functions do forward, so the calls stay short.

So Vulkan on one platform and OpenGL on another is a program-level decision, not
a build-level one, and nothing prevents both from being compiled in. Backends
also **coexist**: two GPU devices on different backends, or a window whose
keyboard comes from the display server while its mouse comes from evdev, are all
representable.

`AUTO` picks in a documented order per layer, listed in that layer's note.

## Three ways to reach the GPU

```cst
use "caustic-media/gpu/gpu.cst" as gpu;
use "caustic-media/window/x11/x11.cst" as x11;

let is x11.window.Window as win = x11.open("app", 1280, 720);

// The window supplies its native handles and a way to put pixels on itself.
// gpu/ takes them as data and never imports window/ — which is what keeps a
// software-rendering binary free of libX11.
let is gpu.swapchain.Surface as surf = x11.surface_for_gpu(&win, fn_ptr(blit), &win);

// let the library choose
let is gpu.device.Device as a = gpu.open(&surf, gpu.AUTO);

// or name the backend
let is gpu.device.Device as b = gpu.open(&surf, gpu.VULKAN);

// or skip the abstraction and drive Vulkan yourself on our window
let is *u8 as vksurface = gpu.vk.surface_from(&surf);
```

There is a fourth way, and it is the one CI uses: `gpu.open_headless(w, h,
AUTO)` gives a device with no screen behind it, rendering into textures the
program reads back. That is what makes the whole layer testable with no
`DISPLAY`.

Backends coexist: two devices on different backends can be alive at once.

## What ships in a binary

Which backends are compiled in is a build-time choice, made in the Causticfile
rather than by the compiler:

```
target "game" {
    src "main.cst"
    out "game"
    media_backends "all"          // default
    // media_backends "vulkan,soft"
    // media_backends "detect"    // only what this machine has — not portable
}
```

`caustic-mk` writes a small module of `with imut` constants beside the target,
and the branches for backends that were left out fold away before codegen — the
same mechanism that keeps a Linux build free of kernel32.

## Dependencies

`math/`, `gpu/software/`, `input/evdev` and the KMS path need nothing but the
kernel. The Vulkan, OpenGL, X11 and Wayland backends link the corresponding
system library, which is why they are opt-in and live behind their own
namespace: a program that never names them produces a static binary with no
`.dynamic` section, like any other Caustic program.

Decoding image files goes through
[caustic-image](https://github.com/Caua726/caustic-image), behind
[`image/`](image/image.md) and opt-in — a program that only generates or
transforms images links no decoder.

## Building

```sh
caustic-mk run test          # everything that needs no screen
caustic-mk run test-x11      # what does, under Xvfb: windows, the toolkit's loop, the examples
caustic-mk run test-wayland  # protocol catalog against wayland-scanner, clients on a private Weston
caustic-mk run test-win32    # Windows libraries and the native backend under a private Wine
caustic -q examples/cube.cst -o build/cube && ./build/cube
caustic -q examples/text_editor.cst -o build/text_editor --path .caustic/deps && ./build/text_editor
```

`test` is headless, the device included, so it runs with no `DISPLAY`;
`test-x11` starts an Xvfb of its own, and the D-Bus tests a private bus that
starts none of the desktop's services. The toolkit's examples —
`settings_form`, `text_editor`, `file_browser` — run as programs, and with
`--check` drive themselves the way a user would, their keys, clicks and
wheel sent through the X server to their own window.

Every server a test uses is its own: `window/x11/tools/run_headless.sh`
(Xvfb, answered by a client before the program starts),
`window/wayland/tools/run_wayland.sh` (headless Weston, or with
`WESTON_BACKEND=x11` a Weston nested in that Xvfb whose keyboard and pointer
are a real seat) and `tools/wine_session.sh` (a temporary Wine prefix on its
own Xvfb; `tools/run_wine.sh` starts one when run alone). Startup is bounded,
the program's exit status is the runner's, and a HUP/INT/TERM ends the
program's process group before the runner removes only what it created.

A program of the toolkit's imports a few hundred modules; the compiler has
to take more than 256 of them in one program (Caustic's build of 2026-10-06
does, an older one leaves the ones past the 256th unlinked).

Needs a standard library new enough to carry the float functions in
`std/math.cst` — `sin`, `cos`, `atan2`, `pow` and the rest. An older install
fails with "funcao nao encontrada no modulo" pointing at `min_f64` or a
trigonometric function.

The compiler looks for the stdlib beside its own binary first, at
`<dir of caustic>/../lib/caustic`, and only then at `/usr/local/lib/caustic`.
To build against a Caustic checkout without installing anything, point the
first location at it:

```
mkdir -p ../lib/caustic && ln -sfn ../../Caustic/std ../lib/caustic/std
```

Every sibling project then resolves `use "std/..."` to that tree.

## Status

The retained toolkit runs on X11, Wayland and Windows: three example
applications — a settings form, a text editor, a file browser — built from
its widgets, with real windows, the desktop's settings, its own fonts and
text, and accessibility (AT-SPI; not yet UI Automation on Windows).

| | |
|---|---|
| `math/` | vectors, matrices, quaternions, geometry, colour, curves — tested |
| `gpu/` | the portable device: buffers, textures, pipelines, commands, swapchain — tested |
| `gpu/software/` | the device implemented with no hardware, over a tested rasterizer: what the toolkit draws with |
| `render/` | the 2D family: frames, queues, shapes, images, glyphs, filled paths |
| `image/` | buffers, transforms, an atlas packer, PNG and others loaded through caustic-image |
| `text/` | fonts read whole (TrueType, CFF, CFF2, variable), rasterized, shaped as HarfBuzz does, laid out with bidi and line breaking, colour glyphs, the system's fonts indexed with fallback |
| `window/x11` | windows many to a connection, MIT-SHM presentation, events, keys and text, cursors, clipboard, drag and drop, monitors, XSETTINGS, dialogs, frames — libX11, Xext, Xrandr, Xi, Xcursor and Xfixes bound |
| `dbus/` | the D-Bus wire protocol, connections, objects and calls, our own |
| `ui/` | the toolkit: widgets, layout, events, painting, themes, animations, dialogs, choosers, menus, lists, trees, tables, drag and drop, AT-SPI, two languages, the application's loop, icons, the tray, notifications, one instance |
| `window/wayland` | direct socket backend, xkbcommon input, selections, DnD, text-input-v3; Weston backend/host smoke; three-example visual acceptance remains incomplete |
| `window/win32` | generated SDK/COM declarations, Win64 ABI bridge and native window backend behind the portable device; the toolkit and its three examples run under Wine, with the native file dialog, known folders, one instance and links opened by the shell; toolkit menus in native popups, OLE DnD, UI Automation and notifications remain |
| `input/` | cooked input every window backend translates into: keys with their place and repeat, text, the pointer, scrolling, cancels; sampled state as something fed |
| `audio/` | the device, sample conversion and a direct ALSA backend over kernel ioctls; the other eleven backends a design note each |
| Vulkan | generated bindings and their checks; the device is a design note, and the work it describes |

Each layer's note says what it holds now under its "Current state".

Two things measured rather than assumed, so they are not rediscovered later.

The device costs about 10% against calling the rasterizer directly — 1554 ms
against 1410 ms for 120 frames at 640×480 — which is the indirect call per
fragment plus decoding vertices through a declared layout.

And `window/x11` did not survive a window manager that ignores the requested
size. A tiling compositor grants 1366×768 for a 640×480 request; the backend
updated its width and height and rebuilt neither the scratch buffer nor the
XImage, so `present` wrote megabytes past the end of its mapping while the
server read every row at a stale stride — a crash and visible banding, both
silent in the code. It now rebuilds both on `ConfigureNotify`, and the frame
the server holds is byte-for-byte what was sent, checked with `XGetImage`
rather than by looking.

## Next

The toolkit first, on all three platforms; then the GPU; the rest waits.

1. **Windows parity** — UI Automation, notifications, and a run on real Windows
   rather than Wine.
2. **Wayland acceptance** — the three examples checked whole, and the
   clipboard, drag and drop and input method against other programs.
3. **After the toolkit** — the Vulkan device, so the 2D family draws on the
   GPU ([`gpu/`](gpu/gpu.md), *Order of work*); `ui/immediate/` moved onto
   `render/`, `text/` and `input/`; KMS as a window backend.

Not now, and written down so it is not started by accident: `3d/`, the
OpenGL and Direct3D devices, and every audio backend past ALSA — each
layer's *Order of work* says in what order they come
([`audio/`](audio/audio.md), [`gpu/`](gpu/gpu.md)).

## License

MIT — see [LICENSE](LICENSE).
