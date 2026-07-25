# OBJ

Wavefront's format from 1992: text, no compression, no skeleton, no animation.
Every tool exports it and it can be written by hand, which is exactly what it is
for here.

```
obj/
  obj.cst   the mesh
  mtl.cst   the companion material file
```

---

## What it is good for

**Debugging.** A cube in OBJ is eight lines of coordinates and six of faces, so
a suspicious loader can be tested against a file whose contents are visible. That
is worth more than it sounds when the alternative is a binary blob from Blender.

**Reach.** Every modelling tool, every scanner, every converter emits it. A
program that reads OBJ can open something from anywhere.

**Cost.** A few hundred lines. The parser is line-oriented and the grammar is a
handful of keywords.

---

## What it cannot do

No skeleton, no animation, no morph targets, no scene hierarchy beyond named
groups. Its material file, `.mtl`, predates PBR entirely — ambient, diffuse and
specular colours with a shininess exponent, from a lighting model nobody uses
now.

So mapping `.mtl` onto [`render/`](../render/render.md)'s materials is a
best-effort translation, and the honest one is: diffuse becomes base colour,
shininess becomes an inverted roughness, everything else is dropped. Reported,
like every other lossy mapping in this layer.

---

## The parts that trip parsers

Three, and all are common in real files:

**Faces can be polygons**, not just triangles, and are triangulated on load. Fans
work for convex faces and fail visibly for concave ones — which do appear.

**Indices are 1-based and can be negative**, where negative counts backward from
the current end of the list. A parser that assumes 0-based positive indices
appears to work until it meets a file that uses either.

**The three index streams are independent.** `f 1/2/3` is position 1, texture
coordinate 2, normal 3, and the same position may appear with different UVs. So
loading means building unique combinations, which is the one place OBJ costs more
than it looks.

---

## Order of work

After the mesh model is settled, since it has no say in shaping it. An afternoon.
