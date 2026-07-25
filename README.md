# caustic-media

Windowing, input, graphics and audio for [Caustic](https://github.com/Caua726/Caustic).

Roughly what SDL is for C, plus a renderer and a UI layer: one library rather
than a repository per platform API.

## Layers

Each layer stands on its own and hands out the handle of the layer below it, so
a program can take the whole stack, take part of it, or take a window and drive
the GPU itself.

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
ui/       immediate-mode widgets, drawn through render/
```

Each has a design note beside it recording what it holds and why:
[`window/`](window/window.md) · [`input/`](input/input.md) ·
[`gpu/`](gpu/gpu.md) · [`render/`](render/render.md) · [`image/`](image/image.md),
with one per backend under `window/` and `gpu/`.

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
let is media.gpu.Device as d = media.gpu.open(&win, media.gpu.AUTO);

// or name it — per platform, per program, per device
let is media.gpu.Device as d = media.gpu.open(&win, media.gpu.VULKAN);
let is media.window.Window as w = media.window.open_with(media.window.WAYLAND, ...);
let is media.input.Source as s = media.input.open_source(media.input.EVDEV);
```

So Vulkan on one platform and OpenGL on another is a program-level decision, not
a build-level one, and nothing prevents both from being compiled in. Backends
also **coexist**: two GPU devices on different backends, or a window whose
keyboard comes from the display server while its mouse comes from evdev, are all
representable.

`AUTO` picks in a documented order per layer, listed in that layer's note.

## Three ways to reach the GPU

```cst
use "media/media.cst" as media;

let is media.window.Window as win = media.window.open("app", 1280, 720);

// let the library choose
let is media.gpu.Device as a = media.gpu.open(&win, media.gpu.AUTO);

// or name the backend
let is media.gpu.Device as b = media.gpu.open(&win, media.gpu.VULKAN);

// or skip the abstraction and drive Vulkan yourself on our window
let is *u8 as surface = media.gpu.vk.surface(&win);
```

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

```
caustic -q math/math_test.cst -o build/math_test && ./build/math_test
caustic -q math/geom_test.cst -o build/geom_test && ./build/geom_test
```

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

Early, and the first milestone is met: a shaded cube turns in a window, drawn
entirely on the CPU, with nothing between the geometry and the screen but our
own code.

| | |
|---|---|
| `math/` | vectors, matrices, quaternions, geometry, colour, curves — tested |
| `gpu/software/` | rasterizer with a depth buffer, perspective-correct, tested |
| `window/x11` | opens, presents and reads input — 22 of libX11's 774 functions |
| everything else | a design note, and the work it describes |

The next pieces are the ones the notes call for first: reshaping `window/x11`
to the pull model before a second backend exists, and wrapping the rasterizer in
a device so `gpu.open(SOFTWARE)` reaches it.

## License

MIT — see [LICENSE](LICENSE).
