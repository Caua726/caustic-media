# Outlines

A glyph's shape, and there are two entirely different ways a font stores one.
Both are common enough that neither is optional: on this machine, **3249 `.ttf`
files carry TrueType outlines and 955 `.otf` files carry CFF**.

```
outline/
  outline.cst   the hub: a face's glyphs as paths, whichever the format
  glyf.cst      TrueType: quadratic curves, composite glyphs
  cff.cst       PostScript charstrings, CFF and CFF2: a stack machine, cubic curves
  path.cst      the common representation both produce
```

---

## TrueType: quadratic, and points that may not be there

`glyf` stores contours as points flagged on-curve or off-curve, forming quadratic
B-splines. The compact part is that **two consecutive off-curve points imply an
on-curve point midway between them**, so it is not a straightforward list of
segments — reconstructing the curve means walking the flags and synthesising the
implied points.

**Composite glyphs** are the other half. `é` is not drawn; it is a reference to
`e` and a reference to the acute accent, each with a transform. Accented Latin,
much of Cyrillic and Greek, and every font's punctuation variants are composites,
so this is a common path rather than an exotic one. Components can nest, and a
loader needs a depth limit for the malformed files that make them cycle.

---

## CFF: a bytecode, not a data structure

`CFF ` is compressed PostScript, and a glyph is a **charstring** — a program for
a stack machine with about forty operators. Reading one means executing it:
`rrcurveto` pushes a cubic segment, `hstem` declares a hint, `callsubr` jumps into
a shared subroutine.

Three consequences:

- **Cubic curves**, not quadratic, so `path.cst` has to represent both or convert
  one to the other. Converting cubic to quadratic is lossy and needs a tolerance;
  keeping both is simpler and the rasterizer flattens either.
- **Subroutines** mean shared shapes are stored once, so a naive reader that does
  not implement `callsubr` and `callgsubr` produces empty glyphs for most of a
  font rather than failing loudly.
- **The interpreter must be bounded.** A charstring is data from a file, and an
  unbounded stack machine reading untrusted input is exactly the shape of a
  vulnerability.

`CFF2` is the variable-font form: the same machine with the hinting operators
removed and blend operators added.

---

## One path type out of two sources

Both produce the same thing for [`../raster/raster.md`](../raster/raster.md): a
set of closed contours of line, quadratic and cubic segments, in font units.

Keeping the representation common is what stops the outline format from leaking
into everything downstream. The rasterizer should not know whether a glyph came
from `glyf` or `CFF `, and neither should the atlas.

---

## Drawn the way fontTools draws

Two readers of the same glyph can draw different contours and both be right —
a TrueType contour can start at any of its points, an implied midpoint can be
emitted or not. The tests compare segment by segment, so the readers draw
exactly as fontTools does:

- **TrueType** contours start at their first on-curve point and leave the line
  back to it to the close; two off-curve points meet at their midpoint; a
  contour of off-curve points alone starts at the midpoint of its last and first.
- **Composites** place a component the way Microsoft's rasterizer does — scaled,
  then moved — unless the font sets `SCALED_COMPONENT_OFFSET` without
  `UNSCALED_COMPONENT_OFFSET`, Apple's way, in which the offset is scaled too.
- **CFF**: every moveto starts a contour and closes the one before; a drawing
  operator with no moveto before it starts one where the pen is; the end of the
  glyph closes the last.

## Variations

A variable font's glyph is its default outline moved by deltas, one set per
region of the design space, weighed by how far the instance's coordinates are
into each (see [`../sfnt/sfnt.md`](../sfnt/sfnt.md)).

**TrueType: `gvar`.** Each region's deltas name the points they move; a point
left out is moved by **interpolation** (IUP) between the nearest moved points of
its contour, on each axis separately — or not at all when its contour has none.
Four **phantom points** follow a glyph's own: its left and right side bearings'
ends, and the top and bottom for vertical text. Deltas move them too, which is
how an instance's side bearings and advance change, and the glyph is drawn
with **the first phantom point at the origin** — as FreeType and HarfBuzz draw
it, at the default instance as well. A composite's "points" are its components'
offsets; each component is varied as a glyph of its own at the same
coordinates, and a component marked `USE_MY_METRICS` gives the composite its
phantom points, as in HarfBuzz.

How much each region weighs, which points a tuple names and how the rest are
inferred all follow HarfBuzz, malformed data included: data that cannot be read
from its start — the table, a glyph's header, its shared point numbers — leaves
the glyph as it is; a tuple that cannot be read fails it; a tuple weighing
nothing is passed over unread; a point named twice moves by both deltas, one
past the glyph's by none. The default instance is never varied, even by a
region that would count there.

**CFF2: `blend`.** The charstring carries each value's deltas inline; `blend`
weighs them by the regions its `vsindex` names — once in a charstring, before
any blend, as the specification and HarfBuzz have it. The regions are weighed
by the first blend with values, which bounds them: the stack holds the deltas.

**Advances**, as HarfBuzz finds them: `hmtx`'s at the default instance; moved by
`HVAR` when the font has it; otherwise, when it has `gvar`, the distance between
the first two phantom points — half the em for a glyph that cannot be read, as
HarfBuzz has it; otherwise `hmtx`'s. They come back unrounded; rounded half away
from zero and no less than 0, they are HarfBuzz's. A glyph this reader refuses
that HarfBuzz reads as empty — a `loca` entry out of order, a glyph shorter than
its header, a component past the glyphs — is half the em there and its own
width in HarfBuzz; and a `vsindex` the store does not have fails the glyph
here, where HarfBuzz blends it by nothing.

## Bounds

Every limit is there for a malformed or hostile file, and each is refused
rather than cut short — a glyph is drawn whole or not at all:

- a path holds what it was opened for and is never grown;
- TrueType: composites nest at most 16 deep and a glyph has at most 4096
  components, counted composite by composite before any is loaded; the points
  of a glyph fit the reader's 65536, more than a simple glyph can have; a
  `gvar` tuple names at most as many points as the reader has room for, more
  than a tuple can name;
- CFF: 48 values on the stack (513 in CFF2), subroutines nested at most 10 deep,
  65536 operators per glyph — which also bounds a fan-out of subroutines that
  each call the next several.

## Current state

`path.cst`, `glyf.cst` (simple glyphs; composites with offsets, scales, 2×2
transforms, point matching and both offset conventions, expanded into points
before drawing because matching needs them), `cff.cst` (INDEX, DICT, Private
DICTs and subroutines with their biases; CID-keyed fonts, FDSelect formats 0, 3
and 4, each glyph's font dict read with the glyph; Type 2 charstrings — widths,
hints and masks, the flex family, `seac` accents found through the Standard
Encoding and the charset; CFF2's `blend` and `vsindex` held to the variation
store) and the hub `outline.cst`, all at any instance of a variable font:
`gvar`'s deltas with IUP and phantom points, composites varied component by
component, `blend`'s deltas weighed, and each glyph's advance (`advance`).

`outline_test` holds every glyph of every face in
[`../testdata`](../testdata/README.md) — 2366 of them, real fonts and fonts made
to exercise every composite form and charstring operator — to what fontTools
draws (`*.outline.ref`, written by `../tools/outline_reference.py`); and the four
variable fonts at chosen instances — 1327 glyphs more, with their advances — to
what fontTools draws there, each glyph also drawn by HarfBuzz and every advance
HarfBuzz's once rounded (`*.var.ref`, by `../tools/var_reference.py`). Then the
malformed: points, flags and contour ends corrupted, composites that loop, fan
out or nest past the limit, charstrings with every operator given the wrong
number of arguments, subroutines that call themselves, INDEXes and DICTs out of
order or past their table, FDSelect pointing nowhere; `gvar` written by hand,
tuple by tuple — every kind of region at its edges, point numbers and deltas
malformed one way at a time, shared and private, the table and a glyph's data
cut short; a variation store claiming more regions than a blend could weigh;
and every font cut at every length against an unreadable page, so a read one
byte too far faults. Mutation-tested: every mutant killed but four that cannot
be — a comparison at the peak the code never reaches with the peak, IUP's at
either end of an interpolation that gives the same there, and the flag keeping
CFF2's weights from being found twice — the checks no test could tell apart
removed.

## Order of work

1. **`path` and `glyf`**, including composites — the larger share of fonts, and
   the simpler format. Done.
2. **`cff`**, with a bounded interpreter, because a quarter of installed fonts
   need it and every professionally-typeset document uses one. Done, CFF2 with
   it.
3. **Variations**: `gvar` deltas for TrueType, `blend`'s deltas for CFF2,
   alongside variable font support. Done.
