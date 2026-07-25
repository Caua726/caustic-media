# STL

Triangles and nothing else. No indices, no normals worth trusting, no colour, no
units. The format 3D printing runs on.

```
stl/
  stl.cst   both encodings
```

---

## Why have it

**3D printing**, entirely. Every slicer takes STL, every printable model is
distributed as one, and CAD tools export it as the lowest common denominator.

A program that reads it can open essentially any printable object in existence,
which is a large corpus for a very small parser.

---

## Shape

Two encodings for the same content:

**Binary** — an 80-byte header nobody agrees on, a triangle count, then 50 bytes
per triangle: a normal, three vertices, and two bytes some tools use for colour
and most ignore.

**ASCII** — the same thing spelled out in keywords. Larger by an order of
magnitude and still common.

Distinguishing them is the one trap: a binary file may begin with the ASCII
keyword `solid`, so sniffing the first word is wrong. Comparing the file size
against `84 + 50 * count` is the reliable test.

---

## What it does not have, and what that costs

**No shared vertices.** Every triangle carries its three positions in full, so a
cube is 12 triangles and 36 positions with no indication that the corners are the
same points. Loading means **welding** — merging positions within a tolerance —
or accepting a mesh with no smooth shading at all, since a vertex normal needs to
know which faces meet.

The tolerance matters: too tight and seams stay split, too loose and detail
collapses. It is a parameter rather than a constant.

**No units.** A file might be millimetres or metres or inches and does not say.
The program decides, and the loader should not pretend to know.

**Per-face normals that are often wrong or absent**, so they are recomputed from
the winding rather than trusted.

---

## Order of work

Last, and the shortest. Its value is entirely in what it unlocks — a printing or
CAD program — rather than in anything a renderer needs.
