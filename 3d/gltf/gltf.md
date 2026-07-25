# glTF 2.0

The format the industry converged on for runtime delivery, and the reason this
layer exists in the shape it does.

```
gltf/
  json.cst      the scene description
  glb.cst       the single-file binary container
  buffer.cst    buffers, buffer views, accessors — where vertex data lives
  scene.cst     nodes, hierarchy, transforms
  material.cst  metallic-roughness PBR, mapped onto render/'s
  animation.cst samplers and channels
  skin.cst      joints and inverse bind matrices
  loader.cst    it all, into a 3d.Model
```

---

## Why this one

Khronos designed it as the delivery format rather than the authoring format,
which is exactly the distinction that matters here: it is meant to be read
quickly by a runtime, not edited. So the data is already in the shape a GPU
wants — vertex buffers, index buffers, and a scene graph that is a flat array of
nodes rather than a document tree.

It carries in its base specification everything a model needs at runtime: PBR
materials, skinning, keyframe animation, morph targets. No extension is required
to load a character that walks.

And it is unencumbered — a published specification, exporters from Blender, Maya,
3ds Max, Substance and every engine, and a conformance suite of sample models to
test against.

---

## Two containers, one format

**`.gltf`** is JSON, with buffers and images referenced as separate files or
embedded as base64 data URIs. Readable, debuggable, and awkward to ship.

**`.glb`** is the same JSON with the binary appended in one file: a 12-byte
header then a sequence of chunks, the first JSON and the second raw bytes. This
is what a program ships, and the parser is a dozen lines on top of the JSON path.

Both are read here; only `.glb` should be recommended.

---

## Accessors are the interesting part

glTF does not store "vertices". It stores **buffers** — opaque byte ranges —
divided into **buffer views** with a stride, read through **accessors** that give
a component type, a count and a shape.

That indirection is what makes it flexible and what makes the loader real work:

- Attributes may be **interleaved or not**, and the same buffer view may be
  shared by several accessors at different offsets.
- Component types vary: positions are almost always `float`, but normals may be
  normalised bytes and indices may be 8, 16 or 32 bits.
- Accessors may be **sparse** — a base of zeroes with a list of overrides —
  which is how morph targets stay small.

So the loader reads what a file declares and converts it into the layout
[`render/`](../render/render.md) was given, as described in
[`../3d.md`](../3d.md). A file that happens to match a preset costs a copy; a file
that does not costs a conversion, and both are known rather than guessed.

---

## Extensions, and which are worth it

The base specification is enough for most models. A handful of extensions are
common enough to matter:

| | What it does | Worth it |
|---|---|---|
| `KHR_texture_transform` | UV offset/scale per material | yes, cheap, and common |
| `KHR_materials_emissive_strength` | HDR emissive | yes, trivial |
| `EXT_mesh_gpu_instancing` | per-instance transforms | yes, once `render/` has instanced draws |
| `KHR_draco_mesh_compression` | compressed geometry | **a decoder of its own** — see below |
| `KHR_materials_*` (clearcoat, transmission, sheen) | advanced PBR | only with a shader that uses them |
| `KHR_lights_punctual` | lights in the file | maybe — lighting is the program's, not the file's |

**Draco is the one with a real cost.** It compresses mesh geometry substantially
and it is a full codec — quantisation, prediction, entropy coding — that Google
maintains in C++. Files using it fail to load without it, and a growing number
do. It is a project on its own scale, and it is worth naming as such rather than
discovering it inside a loader.

Unknown extensions listed in `extensionsRequired` mean the file cannot be loaded
correctly, and saying so is better than loading it wrongly.

---

## What the loader must not do quietly

Three places where a glTF loader can be silently wrong, and each is worth an
explicit report:

- **Dropped material channels**, since `render/`'s material set is smaller than
  PBR. Named in [`../3d.md`](../3d.md).
- **Unsupported extensions**, especially required ones.
- **Coordinate handedness and units.** glTF is right-handed, Y-up, metres — which
  matches `math/`'s convention, so nothing needs converting. That it matches is
  worth stating, because a loader that silently converts when it should not is a
  bug that appears only in mirrored geometry.

---

## Order of work

1. **JSON in `std/`**, promoted from the LSP's.
2. **`.glb` container and the JSON structure** — enough to enumerate what a file
   contains.
3. **Accessors and buffer views**, which is where the real work is.
4. **Static meshes and materials** — a model on screen.
5. **Skins and animations.**
6. **The cheap extensions**, then instancing alongside `render/`'s support.
