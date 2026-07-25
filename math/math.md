# The math layer

Vectors, matrices, quaternions, geometry, colour and curves. **The only layer
that is finished**, and the only one every other layer depends on.

```
math/
  math.cst      hub
  vec.cst       Vec2, Vec3, Vec4
  mat.cst       Mat3 and Mat4, transforms, projections, the normal matrix
  quat.cst      rotation, slerp, conversion to and from Mat4
  scalar.cst    remap, smoothstep, angle wrapping, tolerant comparison
  rect.cst      axis-aligned 2D rectangles, layout splitting
  color.cst     linear colour, the sRGB transfer, packing, HSV, compositing
  geom.cst      ray, plane, AABB, sphere, triangle, frustum, intersection
  curve.cst     easing functions and splines
```

Pure: it imports `std/math` for the scalar transcendentals and nothing else. No
allocator, no syscalls, no external library — it builds and runs on every Caustic
target, including wasm and CausticOS.

2516 lines, and 85 checks across two test files.

---

## The conventions, and where they bite

Every one of these is a choice, and a caller that assumes the other answer
produces geometry that is subtly and consistently wrong.

**Right-handed, Y up, camera looking down −Z.** So `v3_cross(right, up)` is `+Z`,
and `v3_forward()` is `(0, 0, −1)`. The cross product's sign is what the test
suite pins first, because a flipped handedness shows up much later as inverted
winding and back-faces where front-faces should be.

**Mat4 is column-major**, element (row *r*, column *c*) at `m[c*4 + r]`, so a
matrix reaches OpenGL or Vulkan without transposing. The translation therefore
sits in `m[12..14]`.

**Transforms compose right to left.** `m4_mul(a, b)` applies `b` first, matching
how the mathematics is written, so a model-view-projection is
`m4_mul(proj, m4_mul(view, model))`.

**Both projection conventions exist.** `m4_perspective` produces OpenGL's clip
range, z in [−1, 1]; `m4_perspective_zo` produces the [0, 1] that Vulkan and
Direct3D want. Getting this wrong produces no error — geometry is quietly clipped
at the near plane — which is why both are present rather than one plus a note.

**Colour is linear**, always, unless a function says otherwise. See below.

---

## Colour is the one that looks like a detail and is not

An image file and a display are sRGB-encoded: 0.5 means "half as bright as the
display can show", not "half the light". Adding, multiplying or interpolating
those encoded values is arithmetic on the wrong quantity.

It is why naively blended gradients go muddy in the middle, why alpha-composited
edges darken, and why text rendered without it looks too thin or too heavy — a
point [`text/raster`](../text/raster/raster.md) returns to.

So `color.cst` keeps components linear and provides the transfer explicitly. The
curve is the **piecewise sRGB function**, not the 2.2 power law often used in its
place: the two diverge near black, which is exactly where banding is visible.

---

## The normal matrix

`m3_normal_matrix` exists because under a non-uniform scale the model matrix
stretches a normal along with the surface, leaving it no longer perpendicular.
The inverse transpose undoes precisely that stretch. For a rotation alone it
equals the rotation, so it is never wrong to use.

Lighting on a squashed mesh is wrong without it, in a way that looks like a
shading bug rather than a transform bug.

---

## Tests check identities, not remembered numbers

That distinction did real work. A table of expected outputs cannot catch a
consistently wrong convention, because a consistently wrong implementation
reproduces its own numbers.

So the tests assert properties: a transform composed with its inverse is the
identity, x cross y is z, a rotation preserves length, a quaternion and the matrix
built from it move a vector identically, slerp lands on its endpoints, a ray hit
put back into the ray lands on the surface, a colour conversion returns where it
started, a normal stays perpendicular to a squashed face.

Two of the failures they produced were **wrong expectations rather than wrong
code** — the sign of an orthographic projection's depth, and where the middle of a
receding floor falls on screen. Both were the tests being corrected, which is the
argument for writing them this way.

---

## What lives here versus elsewhere

`std/math.cst` carries the **scalar** functions — `sin`, `cos`, `atan2`, `exp`,
`log`, `pow`, `floor`, `fmod` — because they are general-purpose: a physics
simulation, a plotting tool and a signal processor all need them and none of them
is graphics. Several were added to the standard library for this layer, and they
belong there.

Linear algebra for rendering lives here. So does `geom`, because ray-triangle
intersection and frustum culling are graphics questions even though they are pure
mathematics.

**Atlas packing does not.** It is bin packing, and it lives in
[`image/`](../image/image.md) with the atlas it produces — the alternative was
putting the packer somewhere with no relationship to its own output.

---

## Status

Done, and used. `gpu/software`'s rasterizer, the X11 backend and the cube example
all run on it. Additions will come from the layers above asking — noise for
procedural generation, and whatever `ui/` needs for layout arithmetic — rather
than from a plan here.
