# Rasterizing glyphs

Outlines to coverage. The same problem the triangle rasterizer in
[`gpu/software`](../../gpu/software/software.md) solves, over curves instead of
edges, and with quality standards that are much higher because text is what a
reader looks at directly.

```
raster/
  raster.cst    the hub: a path's box under a transform, its coverage, subpixel positions
  flatten.cst   curves to line segments, adaptively
  fill.cst      area coverage with a winding rule
  sdf.cst       signed distance fields (later)
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

Precisely: a pixel's coverage is the path's winding number integrated over the
pixel's square — its magnitude clamped to 1 under the non-zero rule, folded into
0..1 under even-odd — written as a byte, halves up. That is the area covered
wherever contours do not overlap within the pixel, which is everywhere but a few
pixels of a glyph whose contours overlap (as a variable font's do); there it is
what FreeType and every analytic rasterizer compute too. Each line adds its
area to the cells it crosses as differences along the row, and one sum along
the row finishes them — font-rs's accumulation, in doubles.

Curves are flattened to line segments first, **adaptively**: the number of
segments follows the curvature and the size being rendered, because a glyph at 8
pixels and the same glyph at 200 need very different subdivision, and using one
constant wastes work at one end and shows facets at the other. The points are
transformed before flattening, so the count is chosen in pixels: Wang's formula,
from the control points' second differences, gives the fewest evenly spaced
chords that stay within a 64th of a pixel of the curve.

The winding rule is non-zero, which is what both outline formats assume — it is
how a counter inside an `o` becomes a hole. Even-odd is there for the paths
that are not glyphs: the filler is public, for icons and `render/`'s shapes,
under any affine transform.

---

## Gamma is not a detail

This is the single thing that most affects whether text looks right, and it is
invisible until compared side by side.

Coverage is a linear quantity. Blending it into an sRGB-encoded framebuffer as if
the values were linear makes light text on dark backgrounds look too thin and
dark text on light backgrounds too heavy. Blending in linear space fixes it —
and makes text look unlike text everywhere else on the same desktop, because
browsers and most toolkits blend in sRGB. The toolkit's decision is theirs:
coverage is blended in sRGB, as everything `render/` draws is, so that text here
looks like text next to it. A **contrast and gamma adjustment** of the coverage
itself — what FreeType's and Skia's text gamma do — is the knob for the
difference, and comes later.

Some rasterizers additionally apply **stem darkening** — a deliberate thickening
that compensates for the perceptual thinning of unhinted outlines at small sizes.
It is a correction for not hinting, and worth having for the same reason hinting
is worth skipping; it comes later too.

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

## Subpixel positions

A glyph drawn with its pen at x = 10.3 is not the glyph at 10.0 moved: its
coverage differs. The transform takes any offset; `subpixel()` puts a pen
position on a grid of so many steps a pixel — the whole pixel, and which step —
so that a cache keeps a few renderings of each glyph rather than one per
position. How many is the caller's knob: 1 is whole pixels, 4 is quarters.

## Bounds

A box is at most 32768 pixels on a side and begins within 2^30 of the origin;
a path with a point that is not finite has none. A rasterizer holds the cells it
was opened for — max_w + 2 a row, max_h rows — and refuses a box of more, with
nothing written. A curve is cut into at most 1024 chords, more than any curve
within a box the size of a screen needs. A line outside the canvas is cut to it,
a NaN to 0, so that nothing the filler is given reaches outside its cells.

---

## Current state

`flatten.cst`, `fill.cst` and the hub `raster.cst`: a path's box under an affine
transform, its coverage under either rule into any bitmap or region of one, a
glyph's transform from font units, and subpixel positions. `raster_test` holds
the rasterizer to `../testdata/raster.ref`, written by
`../tools/raster_reference.py`, which computes the same coverage another way —
the same chords, every contour clipped to every pixel and its area taken — and
checks each glyph against FreeType's unhinted rendering: 168 paths and glyphs,
shapes of every awkward kind under both rules, glyphs of TrueType, CFF and
CID-keyed fonts at fractional sizes and positions, slanted and turned, every
byte exact but where the coverage is a half level, which arithmetic of any
precision may round either way. Then the refusals, strides, NaNs and lines
outside the canvas by hand. Mutation-tested: every mutant killed but the early
return for flat lines, which would otherwise add nothing.

## Order of work

1. **`flatten` and `fill`** — glyphs on screen, with exact coverage. Done.
2. **Subpixel positioning**, quantised — see the atlas discussion in
   [`../text.md`](../text.md). Done; the cache keeps the renderings.
3. **Contrast and gamma for text, stem darkening, LCD subpixel coverage** — the
   corrections, later.
4. **`sdf`**, when text goes into a 3D scene.
