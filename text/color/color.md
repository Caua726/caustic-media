# Colour fonts

Emoji, and the four incompatible standards for putting colour in a font. Every
one of them is in use.

```
color/
  color.cst   the hub: which glyphs are in colour, drawn — and kept in the atlas
  cbdt.cst    embedded bitmaps — Google's, and what Noto Color Emoji uses
  colr.cst    layered vector glyphs: COLR's version 0 records, CPAL's palettes
  sbix.cst    Apple's bitmaps                                      [B]
  svg.cst     SVG documents embedded in the font                   never
```

---

## Four standards, and the reason there are four

They were designed independently, at the same time, by parties who did not agree:

| | Who | What it stores |
|---|---|---|
| **CBDT/CBLC** | Google | PNG bitmaps per glyph per size |
| **sbix** | Apple | PNG bitmaps per glyph per size, differently |
| **COLR/CPAL** | Microsoft | layers of ordinary glyphs, each with a palette colour |
| **SVG-in-OpenType** | Adobe/Mozilla | an SVG document per glyph |

**Noto Color Emoji on this machine uses CBDT/CBLC**, which makes it the one to
implement first — it is what a Linux system will have installed.

**COLR is the one that scales.** Its glyphs are vector, so they render at any
size like normal text; v0 is flat layered colour and v1 adds gradients,
compositing and transforms, which is enough to express modern emoji designs
without bitmaps. It is where the format is heading.

**SVG-in-OpenType is the one to refuse.** Supporting it means an SVG renderer
inside a font layer, which is a project larger than everything else in this
directory combined, for a handful of fonts.

---

## Bitmaps are the easy path and the wrong long-term one

CBDT and sbix both store PNGs, so decoding a colour glyph means calling into
[`image/`](../../image/image.md) — which already has a PNG decoder available
through caustic-image. That makes the bitmap formats genuinely cheap to support.

What they cost is scaling: a bitmap emoji at 200 pixels from a 128-pixel strike
is blurry, and there is no fixing it. Fonts ship a few strike sizes and the
renderer picks the nearest, which is fine for UI text and visibly poor for
anything large.

---

## Sequences are caustic-unicode's problem, not this one

An emoji is frequently not one codepoint. A family is five codepoints joined by
zero-width joiners; a skin tone is a base plus a modifier; a flag is two regional
indicators. The font maps the *sequence* to one glyph, through `cmap` format 14
and GSUB ligature rules.

So the machinery that recognises those sequences is grapheme clustering, which
[caustic-unicode](https://github.com/Caua726/caustic-unicode) already implements.
This directory receives a cluster and finds its glyph; it does not decide what a
cluster is.

That boundary is what stops emoji from leaking into every part of the text layer.

---

## Drawing them

Both kinds are drawn into RGBA8, **premultiplied** — what the atlas's colour
pages hold and a premultiplied blend draws — with the pen where the
rasterizer has it (pixels, y down, on the baseline):

- **Layers** are filled one over another, from the first: each layer's glyph
  rasterized as coverage, its palette colour times that coverage drawn over
  what is there. A layer of entry 0xFFFF is in the text's colour, so a glyph
  with one is a different bitmap for each colour it is drawn in: the caller
  keeps a font number for each with the atlas.
- **Bitmaps** come from the strike nearest the size — the smallest as large,
  made smaller rather than larger; or the largest — decoded through
  caustic-image, premultiplied, and resampled by area: each pixel the average
  of what it covers of the strike's. At the strike's own size and on whole
  pixels that is the image exactly.

`color.get` does either through the atlas (`atlas.find`, `put`, `refuse`),
on pages of RGBA8: a glyph not in colour, or one asked for on pages of
coverage, is never drawn.

## Order of work

Last of the text layer. Text works without any of it, and every part of it
depends on shaping and the atlas already working.

1. **CBDT/CBLC**, because it is what is installed. *Done.*
2. **COLR v0**, which is small and scales. *Done.*
3. **sbix**, for fonts from Apple.
4. **COLR v1**, when a font needs gradients.
5. **SVG**, never, unless something changes.

## Tests

`color_test` reads three fonts: a subset of Noto Color Emoji (CBDT, one
strike, index format 1, PNG images of format 17); a subset of Noto Znamenny
Musical Notation (COLR version 0, one palette); and `color.ttf`, made by the
tool for what those do not show — two strikes, the larger stored first,
through every index format (1 to 5) and image format (17, 18, 19), PNGs in
RGBA, RGB and grey, glyphs in one strike alone, a bearing as far as a byte
goes; glyphs of several layers overlapping, the text's colour, two palettes.
Each is held to `*.color.ref`, written by `text/tools/color_reference.py`:
every image's metrics and PNG as fontTools reads them, its pixels as Pillow
decodes them, the strike HarfBuzz chooses for a size, every layer and colour.
Drawing is held to what is known of it: layers on whole pixels are their
colours exactly, blended as the arithmetic says; a bitmap at its strike's
size is its pixels, at half the average of each four; at any size it keeps
its ink; a glyph in one opaque colour is its coverage; nothing is written
past a box; through the atlas, each glyph is on its page where the atlas
says, beside or under another.

Mutation testing changed every comparison, sum, constant and return value of
`cbdt.cst`, `colr.cst` and `color.cst`, one at a time; a case was added
wherever a change went unseen and could be seen. What the test still does not
see is of kinds no output shows: guards against malformed tables (lengths and
offsets out of range, formats that do not exist, an image's length that only
bounds it, a table of its header alone); two strikes of one size; a loop
reading one past the end of a list the glyph is always found in; capacities
grown more or less eagerly; sentinels any negative value serves; equal values
clamped or kept either way; coverage of zero blended, which changes nothing;
and memory refused.

## Current state

CBDT/CBLC and COLR version 0 with CPAL are read and drawn, alone or through
the atlas. sbix and COLR version 1 are [B]; SVG is refused.
