# PLY

The Stanford triangle format: a header declaring named properties, then the data
in ASCII or binary. What 3D scanners, photogrammetry tools and research code
emit.

```
ply/
  ply.cst   header and both body encodings
```

---

## Why have it

Nothing in a game pipeline produces PLY, and that is the point: **scans do.** A
program that reads it can open the output of a photogrammetry run, a depth
camera, or an academic tool, none of which will ever export glTF.

It is also the natural format for **point clouds**, which are vertices with no
faces at all — something the other formats here cannot represent and that
[`render/`](../render/render.md) can draw as points.

---

## Shape

The header is text regardless of the body, and it is self-describing:

```
ply
format binary_little_endian 1.0
element vertex 34835
property float x
property float y
property float z
property uchar red
...
element face 69666
property list uchar int vertex_indices
end_header
```

So a parser reads the property declarations and builds a reader for exactly that
layout — which is more general than OBJ's fixed grammar and simpler than glTF's
accessors, because the description sits right above the data.

Three body encodings: ASCII, little-endian binary, big-endian binary. All three
are the same code with a different reader.

Properties are arbitrary and files carry whatever the tool wanted: confidence
values, intensity, normals, curvature. The loader takes what maps onto a vertex
layout and reports the rest as ignored rather than failing.

---

## Order of work

After OBJ, and cheaper than it — the header does most of the work. Worth having
whenever something needs to open a scan.
