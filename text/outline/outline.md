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

## Bounds

Every limit is there for a malformed or hostile file, and each is refused
rather than cut short — a glyph is drawn whole or not at all:

- a path holds what it was opened for and is never grown;
- TrueType: composites nest at most 16 deep and a glyph has at most 4096
  components; the points of a glyph fit the reader's 65536, more than a simple
  glyph can have;
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
store) and the hub `outline.cst`. CFF2 is drawn at the default instance: `blend`
keeps the values, and the deltas wait for the variable-font step.

`outline_test` holds every glyph of every face in
[`../testdata`](../testdata/README.md) — 2348 of them, real fonts and fonts made
to exercise every composite form and charstring operator — to what fontTools
draws (`*.outline.ref`, written by `../tools/outline_reference.py`). Then the
malformed: points, flags and contour ends corrupted, composites that loop, fan
out or nest past the limit, charstrings with every operator given the wrong
number of arguments, subroutines that call themselves, INDEXes and DICTs out of
order or past their table, FDSelect pointing nowhere, and every font cut at
every length against an unreadable page, so a read one byte too far faults.
Mutation-tested: every mutant killed, the checks no test could tell apart
removed.

## Order of work

1. **`path` and `glyf`**, including composites — the larger share of fonts, and
   the simpler format. Done.
2. **`cff`**, with a bounded interpreter, because a quarter of installed fonts
   need it and every professionally-typeset document uses one. Done, CFF2 with
   it.
3. **Variations**: `gvar` deltas for TrueType, `blend`'s deltas for CFF2,
   alongside variable font support.
