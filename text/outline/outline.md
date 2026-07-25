# Outlines

A glyph's shape, and there are two entirely different ways a font stores one.
Both are common enough that neither is optional: on this machine, **3249 `.ttf`
files carry TrueType outlines and 955 `.otf` files carry CFF**.

```
outline/
  glyf.cst      TrueType: quadratic curves, composite glyphs
  cff.cst       PostScript charstrings: a stack machine, cubic curves
  cff2.cst      the variable-font form
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

## Order of work

1. **`path` and `glyf`**, including composites — the larger share of fonts, and
   the simpler format.
2. **`cff`**, with a bounded interpreter, because a quarter of installed fonts
   need it and every professionally-typeset document uses one.
3. **`cff2`**, alongside variable font support.
