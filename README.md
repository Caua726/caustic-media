# caustic-media

Windowing, input, graphics and audio for [Caustic](https://github.com/Caua726/Caustic).

Roughly what SDL is for C, plus a renderer and a UI layer: one library rather
than a repository per platform API.

## Layers

Each layer stands on its own and hands out the handle of the layer below it, so
a program can take the whole stack, take part of it, or take a window and drive
the GPU itself.

```
math/     vectors, matrices, quaternions, projections   — pure
window/   window + input:  x11 | wayland | win32 | causticos
gpu/      device access:  gl | vk
render/   the renderer, over either path:
            software/   rasterizer, z-buffer, texturing — pure
            gpu/        the same scene through gpu/
audio/    playback:  alsa | wasapi | causticos
ui/       immediate-mode widgets, drawn through render/
```

`math/` and `render/software/` import nothing below them, so a program that
only needs geometry or software rendering still builds with no external
dependency.

The split between `gpu/` and `render/gpu/` is deliberate: `gpu/` is the device
— buffers, pipelines, submission — and stops there, so a program that wants to
drive Vulkan itself takes `gpu/` and leaves the rest. `render/` is the opinion
on top, and it is the layer that can swap between hardware and software without
the calling code noticing.

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

`math/`, `raster/` and the CausticOS and KMS paths need nothing but the kernel.
The Vulkan, OpenGL, X11 and Wayland backends link the corresponding system
library, which is why they are opt-in and live behind their own namespace: a
program that never names them produces a static binary with no `.dynamic`
section, like any other Caustic program.

Textures and image files go through
[caustic-image](https://github.com/Caua726/caustic-image).

## Status

Early. `math/` is in. Next is `render/software/`, then one window backend —
enough for a spinning cube with no external dependency, which is the first
milestone.

## License

MIT — see [LICENSE](LICENSE).
