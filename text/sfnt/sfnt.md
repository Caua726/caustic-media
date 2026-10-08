# sfnt

The container every modern font is in: a table directory followed by tables, each
a self-contained block found by a four-byte tag. TrueType, OpenType, `.ttc`
collections and the WOFF wrappers are all this with different outlines or a
different envelope.

```
sfnt/
  table.cst    the directory, checksums, locating a tag
  head.cst     head, hhea, maxp, OS/2 — the metrics every face needs
  cmap.cst     codepoint to glyph index
  name.cst     family, style, and identifying a face
  hmtx.cst     advances and side bearings
  woff.cst     WOFF and WOFF2 wrappers
  var.cst      variable fonts: fvar, avar, the variation store, HVAR, MVAR
```

---

## The tables that are not optional

A face needs `head` (units per em, bounding box, index format), `hhea` and
`hmtx` (advances), `maxp` (counts), `cmap` (what character maps to what glyph),
and `name` (what to call it). `OS/2` carries the typographic metrics a layout
engine should prefer over `hhea`'s, which is one of those places where two tables
disagree and the right answer is a convention rather than a rule.

`glyf`+`loca` or `CFF `/`CFF2` carry the outlines, and which one is present
decides everything about [`../outline/outline.md`](../outline/outline.md).

---

## cmap is several formats wearing one name

Mapping a codepoint to a glyph index sounds like one operation and is a dispatch
over subtable formats, of which four matter:

| Format | What it is | Where |
|---|---|---|
| **4** | segmented ranges, 16-bit | the Basic Multilingual Plane — nearly every font |
| **12** | segmented ranges, 32-bit | anything with emoji or rare CJK |
| **6** | a dense array | small legacy fonts |
| **14** | variation selectors | the sequences that pick a specific CJK or emoji form |

A font carries several subtables for several platform and encoding pairs, and the
loader picks: format 12 with Unicode encoding if present, otherwise format 4.
Choosing wrong means everything above U+FFFF silently maps to nothing.

### Coverage

Finding a font for a character ([`../fonts`](../fonts/fonts.md)) needs to know,
of every face on the system, which characters it has — without opening 4000
files for each character. `cmap.pages` gives the **pages** of 256 characters a
subtable reaches, as 544 bytes of bits, and `cmap.coverage` the face's: exactly
the pages where some character maps to a glyph, as `lookup` and `glyph` would
answer. Segments and groups are read by arithmetic where they can be and
character by character where a glyph array decides; each character is looked at
once at most and each page left at its first hit, so a malformed table of
overlapping segments costs no more than a valid one — its pages then a subset of
what `lookup` finds.

---

## WOFF2 is nearly free here

`.woff` and `.woff2` are the same sfnt tables in a compressed envelope, which
matters because that is what a font served over the web is.

WOFF uses zlib. **WOFF2 uses Brotli — and
[caustic-compact](https://github.com/Caua726/caustic-compact) already has
`brotli.cst`**, so the expensive part of that format is already written in the
ecosystem. What remains is WOFF2's table transformation, where `glyf` and `loca`
are stored in a rearranged form to compress better, and that is a documented
transform rather than a codec.

---

## Variable fonts

One file, an axis of variation — weight, width, optical size — and instances
along it. `fvar` declares the axes, `gvar` the per-glyph deltas, `HVAR` the
metric deltas, `avar` a non-linear remapping of the axis.

Increasingly common, and worth naming now for one reason: **a variable font
rendered at its default instance looks correct**, so it is safe to ignore
initially and support later without anything looking broken in the meantime. That
is unusual and worth taking advantage of.

### Coordinates, as HarfBuzz computes them

An instance is chosen in the axes' own units — weight 650, optical size 20 — and
everything downstream works in **normalized coordinates**: -1 at an axis's
minimum, 0 at its default, +1 at its maximum. `var.cst` gets from one to the other
exactly as HarfBuzz 14 does — in single precision, as it does — so a glyph here is
the glyph every HarfBuzz-based program draws:

1. the value clamped to the axis's range and scaled to -1..+1 on its side of the
   default, rounded to 16.16 fixed point;
2. `avar`'s segment maps, interpolated in single precision — with HarfBuzz's
   answers for maps the specification does not allow: several pairs from one
   value, doubled ends, values outside every pair;
3. `avar` version 2's deltas, weighed at the coordinates step 2 gave rounded to
   2.14, added and clamped;
4. rounded to 2.14 — an integer in -16384..16384, halves rounded up.

Nothing after that sees an axis's own units again. The reference tool records
HarfBuzz's own coordinates for each test instance, and `var_test` must match them
exactly.

Normalization, segment interpolation and store accumulation now use native
`f32` arithmetic, with no memory-rounding compiler workaround. The public
`f64` design/delta API is unchanged. The pinned four-font references and the
single/double precision boundaries pass at O0, O1 and O2.

### One variation store, four readers

`HVAR` (advance widths), `MVAR` (ascender, x-height, underline and the rest),
`avar` 2 and `CFF2`'s `blend` all store their deltas the same way: an **item
variation store** — regions of the design space, and rows of deltas weighed by
how far the coordinates are into each region. `var.cst` reads it once for all
four, weighing regions as HarfBuzz does and summing in single precision; where
HarfBuzz departs from the specification — a coordinate of 0 weighs nothing even
in a region the specification would ignore — this follows HarfBuzz.

Deltas come back unrounded; where HarfBuzz rounds — an advance width, a line
metric — the caller does, so the layout can keep the fraction when it wants it.

### Optical size

A font with an `opsz` axis has shapes drawn for each size — thinner hairlines
and tighter spacing large, sturdier and looser small. Left unset, `opsz` follows
the text's size **in CSS pixels**, as `font-optical-sizing: auto` does in every
browser; a program that sets it explicitly keeps its value.

### Bounds

At most 64 axes — the most any shipping font has is in the teens. A font with
more is read at its default instance, which, as above, looks right.

---

## Current state

`table.cst` (the directory, single faces and `.ttc` collections, reads checked
against the file), `head.cst` (`head`, `hhea`, `maxp`, `OS/2`, `post`, and the
line metrics by the browsers' convention), `hmtx.cst`, `cmap.cst` (formats 0, 4,
6, 10, 12 and 13, Windows Symbol, format 14's variation sequences, and the pages
a mapping reaches), `name.cst`
(UTF-16 and Mac Roman to UTF-8, the record an English-reading user should see)
and the legacy `kern.cst`; `sfnt.open` does it all. `sfnt_test` holds every face
of the fonts in [`../testdata`](../testdata/README.md) to what fontTools reads
from them — every table field, name, subtable mapping, advance and kerning pair,
and for the mappings every character of Unicode not listed mapping to nothing,
and every subtable's pages — then subtables made by hand for the pages at every
edge (a page's first and last character, groups past the glyphs or past
Unicode, overlapping segments held to their bound by a count of the work), then
reads fonts cut at every length and with their counts corrupted, laid
against an unreadable page so a read one byte too far faults. Mutation-tested:
every mutant killed, the checks no test could tell apart removed.

`var.cst` reads variable fonts: `fvar`'s axes and named instances, an instance
chosen by axis values or by name, `opsz` from the text's size, HarfBuzz's
coordinates for it (`avar` versions 1 and 2), the item variation store and
its delta-set index maps, `HVAR`'s advance deltas and `MVAR`'s line metrics.
`var_test` holds the four variable fonts of the test data to HarfBuzz at chosen
instances — every coordinate exactly, every advance and `MVAR` delta as
HarfBuzz rounds it — then `fvar`, `avar` and the stores written by hand: every
special case of HarfBuzz's segment maps, inputs where single and double
precision part, every form of index map, offsets and counts past their tables.
Mutation-tested as the rest.

## Order of work

First of the layer — nothing else can start without a table directory and a
`cmap`.

1. **Table directory, `head`, `hhea`, `maxp`, `hmtx`.**
2. **`cmap` formats 4 and 12**, which is every font that matters.
3. **`name`**, so faces can be enumerated and chosen.
4. **WOFF/WOFF2**, once Brotli is wired up from caustic-compact.
5. **Variable font axes**, when a design calls for one. Done, with the
   variation store's readers — the outlines' deltas are in
   [`../outline`](../outline/outline.md).
