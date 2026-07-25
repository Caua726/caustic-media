# The render layer

What to draw. [`gpu/`](../gpu/gpu.md) is the portable device — buffers,
pipelines, submission, compute — and `render/` is a client of it, the way a game
framework is a client of wgpu.

The comparison that fits is raylib, and the comparison is about **scope**, not
mechanism: meshes, sprites, cameras, materials, a frame. raylib reaches that
scope through implicit global state, and this layer cannot, for reasons that are
written down rather than inherited.

`render/` has **no backends of its own**. Everything portable about reaching
hardware is settled in `gpu/`, including software. This layer is one
implementation.

---

## State travels with the draw

raylib's shape is a stack of modes:

```c
BeginShaderMode(shader);
    DrawModel(model, pos, 1.0f, WHITE);   // which shader? not on this line
EndShaderMode();
```

Reading the draw does not tell you what the draw does. Caustic's stated position
is the opposite:

> **No magic, no implicit.** Every operation is visible. No hidden allocations,
> no implicit conversions, no runtime surprises.

So there is no mode stack, and no pipeline-object ceremony either. The state
comes with the work:

```cst
render.draw(&r, mesh, material, transform)
```

The material *is* the state. Nothing to push, nothing to forget to pop, and the
line explains itself.

This buys something beyond readability: **if no draw depends on state set
earlier, the renderer may reorder freely.** Sorting for transparency and grouping
by material stop being risks and become plain optimisations.

---

## The unit of work is a draw, not a primitive

One triangle per call cannot be this layer's interface: a GPU backend
implementing it would issue a draw call per triangle. The reverse fails too — an
interface shaped like command-buffer recording turns the whole framework into
ceremony for no gain.

A draw is geometry, plus how to shade it, plus where to put it. That is the level
both a real GPU and a software rasterizer implement well.

---

## One engine, two ways to draw

There is no `2d/` module: 2D is not a separate domain. It is not a special case
of 3D either, and pretending otherwise produces a UI layer that fights the
renderer:

| | 3D | 2D |
|---|---|---|
| Ordering | depth buffer, submission order free | **layer index**, submission order matters |
| Batching | by material | **by texture atlas** |
| Clipping | frustum | **scissor rectangle**, nested |
| Depth | per-fragment test | usually none |

So: one engine, one resource model, one frame — and two families of draw that
differ in how they are ordered, batched and clipped. Not two APIs, and not one
API pretending they are the same thing.

**The mode belongs to the pass, not to the renderer.** A 3D game needs both
anyway, since its HUD is 2D, so a renderer-level mode would exist only to
disable something — and *"if a feature adds complexity without clear benefit, it
doesn't go in"*. A 2D game is then simply a program that never opens a 3D pass,
and pays for nothing it does not open. Whether a depth buffer exists is
something the pass declares, which is the same rule as the material: state
travels with the work.

**A mesh can be drawn in a 2D pass.** That is what "load a model as a 2D asset"
means: the geometry is 3D, but ordering, batching and clipping stay 2D, so the
game stays entirely 2D. It costs nothing — the same draw with a different
ordering rule — and it is what isometric games actually want. Pre-rendering a
model to a sprite needs no support from this layer at all: a program renders to
a texture and uses the texture.

---

## Resources are declared, not inferred

**Vertex layouts are declared.** raylib's `Mesh` carries eleven optional arrays,
null when unused, so every consumer branches on what happens to be present —
behaviour depending on invisible state, which is the thing the philosophy
rejects. A declared layout says what is there.

Named presets keep that from becoming ceremony. A preset is not magic: it is a
named constant that expands to an explicit layout, and you can read which.

**Materials carry declared parameters**, not fixed slots. raylib's
`MaterialMap[]` model misrepresents any surface that does not fit its slots,
which collides with *"not a toy — real software"*.

Shaders arrive as SPIR-V; see [`gpu/gpu.md`](../gpu/gpu.md) for why that is the
currency and why it comes in precompiled for now.

---

## Who owns which state

| Material — *how this thing looks* | Pass — *where we are drawing* |
|---|---|
| shader, textures, uniform values | viewport, scissor |
| blend mode | clear values |
| cull mode | whether depth exists |
| depth test mode | render target |

The line is ownership. Blend and culling are properties of the surface; viewport
and scissor are properties of the target.

Putting viewport in the material would repeat it on every draw for no reason.
Putting blend in the pass would make opaque and transparent geometry
unrepresentable in one pass, which no real scene survives.

---

## How things fail

Caustic has no exceptions, and `Result` in every signature would make each call
site a two-line check without an operator to chain them — complexity out of
proportion to the benefit. Aborting is worse: a game should not die because a
texture did not fit.

- **Creation can fail.** It returns a handle that may be invalid, and
  `render.last_error(&r)` says why.
- **Frame operations never fail.** Drawing with an invalid handle is a silent
  no-op.
- **Device loss is queryable state**, not a return value.

The hot path therefore carries no error handling at all, and the setup path
carries explicit checks — which is how graphics APIs behave in practice.

---

## Camera

The matrices belong to `math/`; the controller belongs here, because it produces
the view and projection the renderer consumes, and because both families of draw
need one.

`math/geom` already builds a frustum from a view-projection and tests boxes and
spheres against it, so culling assembles from pieces that exist.

---

## Threading

Single-threaded by default. A program that wants to record draws from several
threads opts in.

*Open:* whether opting in makes handles safe for concurrent use, or requires the
program to partition resources per thread. This decides who owns the queue and
whether there is a lock, and it is expensive to answer late.

---

## Allocation

*"No hidden allocations."* If draws are queued before being sorted and
submitted, the queue is **explicitly sized** — the program says how many draws
fit, or supplies the memory, the way `Target` already works in the software
rasterizer today.

---

## Modules

```
render/
  render.cst      hub
  renderer.cst    creation over a gpu.Device, last_error, device-loss state
  frame.cst       frame boundaries, passes, targets, clear
  queue.cst       the draw queue: explicit sizing, sorting, batching, submit
  mesh.cst        declared vertex layouts and their presets, mesh handles
  material.cst    material handles, declared parameters, surface state
  texture.cst     texture handles and formats, created from an image.Image
  camera.cst      2D and 3D camera controllers
  draw3d.cst      the 3D family
  draw2d.cst      the 2D family: sprites, atlas batching, layer, scissor
  shapes2d.cst    rectangles, circles, lines, polygons
  debug.cst       wireframes for the shapes math/geom already describes
```

No subdirectories, and that is the sign the split with `gpu/` landed in the
right place: with backends owned one level down, there is nothing here to
separate by platform.

### What each one holds

**`renderer.cst`** — creation over a `gpu.Device`, and the state that outlives a
frame: `last_error`, whether the device has been lost, the resource tables the
handles index into. Nothing draws here.

**`frame.cst`** — the frame boundary and passes. A pass declares its kind (2D or
3D), its target, whether it has depth, its clear values, and its viewport. That
declaration is what lets a 2D-only program pay for no depth buffer, and what
makes rendering to a texture a pass with a different target rather than a concept
of its own.

**`queue.cst`** — where draws accumulate between `pass_begin` and `pass_end`, and
where they are sorted and collapsed before submission. Opaque front-to-back,
transparent back-to-front, consecutive draws sharing a material merged. Its
capacity is set by the program, since a queue that grows on its own is the hidden
allocation the philosophy forbids.

**`mesh.cst`** — vertex layout declarations and the presets that keep them from
being ceremony, plus mesh handle creation and upload through `gpu/buffer`. A
preset is a named constant that expands to an explicit layout, and you can read
which.

**`material.cst`** — material handles, their declared parameters, and the state
that belongs to a surface: blend, cull, depth comparison. Also the built-in
materials, which exist so the software backend can implement the common cases as
native code instead of interpreting a shader per fragment.

**`texture.cst`** — handles, formats, sampler settings, upload. Creating a
texture *usable as a render target* is here, because a GPU wants that declared up
front; rendering into it is `frame.cst`'s business. A texture is created from an
[`image.Image`](../image/image.md), which is where pixels live on the CPU and
where loading, transforming and mipmap generation happen — this layer never
touches a file format.

**`camera.cst`** — perspective and orthographic setup, orbit and first-person
controllers, 2D pan and zoom, and the frustum that `math/geom` already knows how
to build and test against.

**`draw3d.cst`** — `draw(mesh, material, transform)`, and the instanced form.
Ordering is by depth, so submission order does not matter.

**`draw2d.cst`** — sprites, atlas batching, layer ordering and the scissor stack.
Submission order matters here, and layers are how a program controls it without
depending on it.

**`shapes2d.cst`** — rectangles, rounded rectangles, circles, lines, polylines,
polygons, arcs. Filled and outlined. Geometry generated straight into the batch.

**`debug.cst`** — wireframes for the shapes `math/geom` already describes: AABB,
sphere, ray, frustum, plus axes and a ground grid. Depth-tested or always on top,
because both are useful and for different reasons.

### Three placements worth their reasoning

**`shapes2d` is its own file, not part of `draw2d`.** They are the same family —
same queue, same layer ordering, same scissor — but a different concern: a sprite
is driven by a texture, a shape is geometry generated on the spot. Generating
straight into the batch is also what avoids minting a mesh handle per rectangle
drawn, which is the whole reason immediate 2D drawing is worth having.

**`debug` belongs here, not to `3d/`.** What it draws are `math/geom` types —
boxes, spheres, frusta — and this layer already depends on `math/`. `3d/` is about
content: files, meshes, skeletons. A wireframe box is none of those, and
visualising a frustum has to work in a program that loads no models at all.

**Rendering to a texture is a property of the pass**, which the ownership table
above already decided: the render target belongs to the pass. `texture.cst` only
has to be able to create a texture *usable* as a target, since a GPU wants that
declared at creation. Drawing into it is `pass_begin` with a different target.

---

## What is not here

- **Compute, tessellation, geometry stages.** They live in `gpu/`, which is the
  level at which they are honest. A program that needs them drops one level.
- **Backends.** `gpu/` owns them, software included.
- **A scene graph.** This is a framework, not an engine: `render/` receives
  draws, not a tree it walks. One can be built on top later without reshaping
  anything here, precisely because the unit of work is a batch.
- **Model loading and animation.** `3d/` turns files into meshes and materials;
  `render/` only knows the handles it was given.
- **Glyphs.** `text/` produces quads and an atlas; `render/` draws them like any
  other sprite.
- **Pixels on the CPU.** [`image/`](../image/image.md) owns loading,
  transforming, generating and atlas packing; `render/` receives an `Image` and
  uploads it.

---

## Current state

`render/software/` holds a working rasterizer — target, triangle fill, depth
buffer, perspective-correct interpolation, top-left fill rule. Under the
structure above it belongs to `gpu/software` as the implementation of a device
backend, with a device-shaped surface around it: buffers, pipelines, submission.

Moving it is the first piece of work here.
