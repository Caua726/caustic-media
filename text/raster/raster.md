# Rasterizing glyphs

Outlines to coverage. The same problem the triangle rasterizer in
[`gpu/software`](../../gpu/software/software.md) solves, over curves instead of
edges, and with quality standards that are much higher because text is what a
reader looks at directly.

```
raster/
  flatten.cst   curves to line segments, adaptively
  fill.cst      scanline coverage with a winding rule
  sdf.cst       signed distance fields
  gamma.cst     the blend that makes text the right weight
```

---

## Coverage, not samples

The straightforward implementation supersamples: rasterize at 4× and average.
It works, it is slow, and it is not what a text rasterizer should do.

The better approach computes **exact area coverage per pixel** analytically —
accumulating signed area as edges cross each scanline, then integrating along the
row. It is one pass, it is exact rather than approximate, and it is what FreeType
and every serious rasterizer does. The idea is not complicated; getting the
accumulation right at contour boundaries is where the care goes.

Curves are flattened to line segments first, **adaptively**: the number of
segments follows the curvature and the size being rendered, because a glyph at 8
pixels and the same glyph at 200 need very different subdivision, and using one
constant wastes work at one end and shows facets at the other.

The winding rule is non-zero, which is what both outline formats assume — it is
how a counter inside an `o` becomes a hole.

---

## Gamma is not a detail

This is the single thing that most affects whether text looks right, and it is
invisible until compared side by side.

Coverage is a linear quantity. Blending it into an sRGB-encoded framebuffer as if
the values were linear makes light text on dark backgrounds look too thin and
dark text on light backgrounds too heavy. The fix is blending in linear space,
which [`math/color`](../../math/) already provides the conversion for and which
[`image/`](../../image/image.md) already argues for at length.

Some rasterizers additionally apply **stem darkening** — a deliberate thickening
that compensates for the perceptual thinning of unhinted outlines at small sizes.
It is a correction for not hinting, and worth having for the same reason hinting
is worth skipping.

---

## Hinting is skipped, deliberately

TrueType hinting is a bytecode interpreter — another stack machine, larger than
CFF's — whose purpose is snapping stems to whole pixels at low resolution. It is
a large part of FreeType's 825 KB.

Its value has fallen with pixel density, and its absence is correctable: vertical
stem alignment as a heuristic, plus stem darkening and gamma-correct blending,
recover most of the crispness. If it turns out to matter on a 96 DPI screen, it
can be added; starting with it would mean starting with the hardest part of the
reference implementation for a benefit that is shrinking.

---

## SDF, for when a texture is not enough

A signed distance field stores distance-to-edge instead of coverage, so one
rasterization scales, rotates and transforms without re-rendering. That is how
text in a 3D scene is done, and how a UI that zooms avoids a glyph cache per zoom
level.

It costs sharpness at small sizes and it cannot represent sharp corners — an `A`'s
apex rounds off. **Multi-channel SDF** fixes the corners at three times the
memory, and is what a program that cares would use.

So it complements the direct path rather than replacing it: direct rasterization
for UI text at a known size, SDF for text in a world.

---

## Order of work

1. **`flatten` and `fill`** — glyphs on screen, with exact coverage.
2. **`gamma`**, immediately after, because text that looks wrong from the start
   trains everyone's eye to accept it.
3. **Subpixel positioning**, quantised — see the atlas discussion in
   [`../text.md`](../text.md).
4. **`sdf`**, when text goes into a 3D scene.
