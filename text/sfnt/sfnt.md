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
  var.cst      variable fonts: fvar, avar, HVAR
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

---

## Order of work

First of the layer — nothing else can start without a table directory and a
`cmap`.

1. **Table directory, `head`, `hhea`, `maxp`, `hmtx`.**
2. **`cmap` formats 4 and 12**, which is every font that matters.
3. **`name`**, so faces can be enumerated and chosen.
4. **WOFF/WOFF2**, once Brotli is wired up from caustic-compact.
5. **Variable font axes**, when a design calls for one.
