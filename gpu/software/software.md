# Software

A GPU implemented in software: the same device as Vulkan, OpenGL and Direct3D,
with no driver and no hardware underneath.

```
software/
  target.cst    ✅ the framebuffer view — colour, depth, clear, lines, rects
  raster.cst    ✅ triangle rasterization with a depth buffer
  backend.cst   gpu/device.cst implemented over them
```

**This is the only backend that keeps a binary fully self-contained.** Every
other one links something written by someone else. It is also what makes the
library work on CausticOS, in a VM with no acceleration, over a remote session,
and in CI.

Nothing is beyond it. It can implement compute, tessellation and geometry stages
— it is code, and the constraint is speed rather than capability. Where it is
slow, that is a fact to state, not a feature to withhold.

---

## What exists

`target.cst` and `raster.cst` work today and are tested:

- Framebuffer as a **view over memory the caller owns** — nothing allocates, so
  the same code fills a heap block, a static array, or a window's back buffer
- Triangle fill with a depth buffer, back-face culling, bounding-box traversal
- **Perspective-correct interpolation** — weights divided by w and renormalised
- **Top-left fill rule**, so a shared edge is covered exactly once
- Near-plane rejection for triangles crossing the eye

The tests check invariants rather than pixels: coverage against area, a seam
covered once, the nearer surface winning either submission order, one winding
surviving culling.

What is missing is the shape around it. Today it is a *renderer* — you hand it
triangles. It has to become a *device* — you hand it buffers, a pipeline, and a
draw command.

---

## What a device means without hardware

**Buffers** are the easy part: a buffer is memory, upload is a copy, mapping is
returning the pointer. No staging, no transfer queue, no visibility rules.

**Textures and samplers** are memory plus a fetch function. Filtering, wrapping
and mip selection are code paths, and the sampler is a small struct the fetch
function reads. Mip generation is a loop.

**Pipelines** are the interesting one. A pipeline elsewhere is driver state; here
it is a **set of function pointers plus a state struct**: the vertex stage, the
fragment stage, the blend function, the depth comparison, the cull mode.
`bind_pipeline` swaps which functions the rasterizer will call. Caustic has
`fn_ptr` and `call`, so this is direct.

**Command buffers** are a recorded list, and submission walks it. Since there is
no asynchronous hardware, submission can execute immediately — but recording
should stay a real list rather than executing on the spot, so the behaviour a
program sees matches the GPU backends.

**Synchronisation** is nearly free: fences signal at submission, semaphores are
no-ops, barriers do nothing. It matters that these exist and do the right thing
rather than being absent, because a program written against the device must not
have to know which backend it is on.

---

## Shaders

Two paths, and the split is what keeps the common case fast.

**Built-in materials run as native code.** `render/`'s unlit, textured and lit
materials have hand-written fragment functions here — no interpretation, no
translation. This covers most of what a program draws.

**Custom SPIR-V is interpreted.** A dispatch per instruction per fragment: a
50-instruction fragment shader at 640×480 is 15 million interpreter steps a
frame. It is correct, it is slow, and a program choosing a custom shader on the
software backend is choosing that knowingly.

An interpreter is the honest starting point. Compiling SPIR-V to native at run
time would be faster and would mean linking a compiler into the renderer, which
this library does not do and does not currently plan to — see
[`../gpu.md`](../gpu.md).

---

## Compute, tessellation, geometry

They are in `gpu/`, so they are here.

**Compute** is a dispatch over a 3D grid of workgroups, each running the compute
stage over storage buffers. Without hardware it is a loop — and the obvious place
for the thread pool, since workgroups are independent by definition. Barriers
within a workgroup need the invocations to actually interleave, which is the one
place the emulation is genuinely awkward rather than merely slow.

**Tessellation and geometry** stages generate primitives before rasterization.
In software they are a stage that expands the vertex stream, which is
structurally simpler here than on hardware.

---

## Where the time goes, and threading

A software rasterizer is bound by fragments. The parallelism that matters is
**tiles**: split the target into regions, and each thread rasterizes the
primitives that touch its own region. No locking, since no two threads write the
same pixel, and the depth buffer stays correct because a tile is owned outright.

Caustic has threads, atomics and a thread-safe allocator, so this is available
rather than aspirational. It stays behind the same opt-in as the rest of the
library's threading: single-threaded by default.

Before that, the cheap wins are worth naming: the current rasterizer walks the
full bounding box testing three edge functions per pixel, when the edge functions
are linear and can be stepped incrementally.

---

## Order of work

1. **`backend.cst`** — the device shell: buffers, textures, pipelines as function
   pointer sets, command recording, submission. This is what makes the existing
   rasterizer reachable through `gpu.open(SOFTWARE)`.
2. **Built-in material fragment functions**, so `render/`'s common path is native.
3. **SPIR-V interpreter**, for custom shaders and for compute.
4. **Tiled multithreading**, once there is something worth parallelising.
