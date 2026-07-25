# Colour fonts

Emoji, and the four incompatible standards for putting colour in a font. Every
one of them is in use.

```
color/
  cbdt.cst    embedded bitmaps — Google's, and what Noto Color Emoji uses
  sbix.cst    Apple's bitmaps
  colr.cst    layered vector glyphs, v0 and v1
  svg.cst     SVG documents embedded in the font
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

## Order of work

Last of the text layer. Text works without any of it, and every part of it
depends on shaping and the atlas already working.

1. **CBDT/CBLC**, because it is what is installed.
2. **COLR v0**, which is small and scales.
3. **sbix**, for fonts from Apple.
4. **COLR v1**, when a font needs gradients.
5. **SVG**, never, unless something changes.
