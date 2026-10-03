# The text layer

Fonts: parsing them, rasterizing glyphs, packing an atlas, and turning a string
into positioned quads that [`render/`](../render/render.md) draws like any other
sprite.

```
text/
  text.cst      hub
  atlas.cst     glyph cache over image/'s packer

  fonts/        the system's fonts found and chosen; chains   fonts/fonts.md

  sfnt/         the container: tables, cmap, metrics    sfnt/sfnt.md
  outline/      glyf quadratic and CFF charstrings      outline/outline.md
  raster/       outline to coverage, and SDF            raster/raster.md
  shape/        codepoints to positioned glyphs         shape/shape.md
  layout/       runs, breaking, bidi, caret             layout/layout.md
  color/        emoji, and its four standards           color/color.md
```

Six of those have notes of their own, because each is a different kind of
problem: a binary container, two unrelated outline models, a rasterizer with
unusually high quality standards, a table-driven substitution engine, a text
algorithm, and a compatibility mess.

---

## What this layer does not do

Text is three problems stacked, and only the innermost is about fonts.
[caustic-unicode](https://github.com/Caua726/caustic-unicode) already carries
the outer two — 10450 lines covering `utf`, `normalize`, `segment`, `linebreak`,
`bidi`, `width`, `case` and `collate`.

| Problem | Where it lives |
|---|---|
| What *is* a character — encoding, normalisation, grapheme clusters | caustic-unicode |
| Where a line may break, which direction the text runs | caustic-unicode |
| What a glyph looks like and where it sits | **here** |

So `layout.cst` asks caustic-unicode where the break opportunities are and which
runs are right-to-left, and spends its own effort on the geometry. Reimplementing
UAX #14 line breaking or UAX #9 bidi would be writing something that already
exists in the ecosystem, correctly, against the current Unicode version.

Text *input* — dead keys, IME composition, candidate lists — is
[`input/`](../input/input.md)'s, not this layer's. This one draws; that one
receives.

---

## Parsing and rasterizing fonts ourselves

The references are FreeType at 825 KB and 220 symbols, and HarfBuzz at 1252 KB
and 551. Both are large because they carry decades of format edge cases, dozens
of scripts, and hinting bytecode interpreters.

We parse and rasterize ourselves, for the reason everything else here is written
rather than linked: an sfnt file is a documented binary container of tables, and
none of it needs a library.

**`sfnt.cst`** reads the container: `head`, `maxp`, `cmap` for codepoint to glyph
index, `hmtx` for advances, `loca` and `glyf` for TrueType outlines or `CFF ` for
PostScript ones, `kern` or `GPOS` for pair adjustment, `name` for identifying the
face. `.ttc` collections and WOFF2 are the same container with a wrapper.

**`outline.cst` and `raster.cst`** turn a glyph into coverage. A TrueType outline
is quadratic B-splines; CFF is cubic. Filling either is a scanline pass with a
non-zero or even-odd rule — the same shape as the triangle rasterizer already in
`gpu/software`, over curves rather than edges, and analytically anti-aliased
rather than sampled.

**Hinting is skipped.** It is a bytecode interpreter whose purpose is making stems
land on whole pixels at low resolution, and its value has fallen with pixel
density. Vertical stem snapping and gamma-correct blending get most of the
crispness for a fraction of the machinery. If it turns out to matter on a 96 DPI
screen, it can be added; starting with it would be starting with the hardest part
of FreeType.

---

## Shaping, in two stages

Shaping maps a sequence of codepoints to positioned glyphs, and how hard it is
depends entirely on the script.

**Stage one covers Latin, Greek, Cyrillic and CJK**: look up each codepoint in
`cmap`, apply kerning pairs, advance. That is most of the text most programs draw
and it is a few hundred lines.

**Stage two is OpenType `GSUB`/`GPOS`**: ligatures, contextual alternates, mark
positioning, and the substitution rules that make Arabic letters join and
Devanagari reorder. This is where HarfBuzz's bulk lives, and it is not optional
for those scripts — Arabic rendered without it is unreadable, not merely ugly.

The staging is honest rather than a limitation dressed up: a program drawing
English gets working text early, and a program that needs Arabic knows what is
missing rather than getting mojibake.

---

## The atlas

Glyphs are rasterized once at a size and cached in an atlas, which
[`image/`](../image/image.md) packs — the reason that packer lives there rather
than here is that `render/`'s sprite batching needs the same one.

Two things the cache has to get right:

**Eviction.** A program that renders at many sizes, or shows a lot of CJK, will
overflow a fixed atlas. Growing is one answer, evicting least-recently-used is
another, and a second atlas page is a third. Whichever, the failure mode must not
be silently dropping glyphs.

**Subpixel positioning.** A glyph rendered at x=10.0 and at x=10.3 differs. Text
that snaps every glyph to whole pixels has visibly uneven spacing; text that
caches a variant per subpixel offset multiplies atlas usage. The usual answer is
quantising to a few offsets, and it should be a knob rather than a constant.

**SDF is the alternative worth naming.** A signed distance field caches one
resolution and scales, which makes arbitrary size and rotation cheap and is how
text in a 3D world is usually done. It costs sharpness at small sizes, so it
complements the direct path rather than replacing it.

What `atlas.cst` does: pages of `image/`'s atlas, borrowed — the same ones
`render/coverage.cst` gives filled paths, so that one set of textures holds
both — a table sized at open, and pages added until a number fixed at open,
then the least recently used taken back whole. A glyph is keyed by a number
the caller gives each face at each instance, its size to the bit, and its pen
on a grid of steps a pixel, four across and one down unless told otherwise.
Nothing used in the frame being drawn gives way, so a glyph with no place is
`NO_ROOM`, said; a space is remembered as nothing to draw, an unreadable glyph
as never. Colour glyphs are kept apart, on RGBA pages, written by their maker
into the place the cache gives them.

---

## Fonts are chains, not files

A font handle is a **fallback chain**, not one face. No single font covers
Unicode: the program asks for text, and the first face that has each glyph
provides it. Missing everywhere means the notdef box, which should look like a
missing glyph rather than like nothing.

Fontconfig exists for finding faces by name and is 485 KB. Enumerating
`/usr/share/fonts` — 4459 files on this machine — and reading each `name` table
does the same job with no dependency, and cached, once.

That is [`fonts/`](fonts/fonts.md): the system's font files walked and every
face described once — names, weights, widths, styles, which pages of Unicode
it reaches — kept in an index file that the next start brings up to date in
90 ms; a family's face chosen by CSS's matching; and chains that put behind
the faces asked for the faces that draw what those lack — the family's own
other faces and its companions first, the preferred CJK families in the
locale's order and the emoji ones, then any face, nearest the request.

---

## Layout

`layout.cst` takes a string, a font chain, a width and an alignment, and returns
positioned glyphs plus the box they occupy. Wrapping happens at break
opportunities caustic-unicode identifies rather than at spaces, because that is
the difference between correct and nearly-correct in most of the world's
languages.

It also answers the questions a text field asks: where is the caret for this
byte offset, which offset is under this point, what does the selection rectangle
look like across a bidi run. `ui/` needs all three, and they are much easier to
answer while the layout is being built than afterwards.

---

## Order of work

1. **`sfnt` and `outline`** — read a face, get a glyph's curves.
2. **`raster` and `atlas`** — glyphs on screen at one size. Done.
3. **Shaping**, both stages: every `GSUB` and `GPOS` lookup, the legacy `kern`
   table, the fallbacks — done, held to what HarfBuzz gives
   ([`shape/shape.md`](shape/shape.md)). The scripts with shapers of their
   own (Arabic joining, Indic reordering, Hangul, Thai, Khmer, Myanmar, USE)
   are [B].
4. **Layout**, with caustic-unicode for breaking and bidi — done
   ([`layout/layout.md`](layout/layout.md)).
5. **Caret and hit-testing**, which `ui/` blocks on — done, in
   `layout/cursor.cst`.
6. **Colour glyphs**: bitmaps (`CBDT`/`CBLC`) and layers (`COLR` version 0,
   `CPAL`) — done ([`color/color.md`](color/color.md)); `sbix` and `COLR`
   version 1 are [B].
7. **SDF**, when text in a 3D scene does.
