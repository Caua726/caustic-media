# Shaping

Codepoints to positioned glyphs. The largest single piece of work in this layer,
and the one where difficulty depends entirely on the writing system.

HarfBuzz is 1252 KB and 551 symbols. Nearly all of that is this file's subject —
and what is here is HarfBuzz's own algorithm, ported: a program shaping text here
gets the glyphs, clusters and positions HarfBuzz 14.5 gives for the same font,
text and features, held to it exactly by the tests. Where HarfBuzz departs from
the OpenType specification, this follows HarfBuzz, which is what fonts are
tested against.

```
shape/
  shape.cst     the hub: a run of text in, glyphs, clusters and positions out
  buffer.cst    the glyphs being shaped: the input, the output, clusters
  props.cst     what shaping needs of Unicode: categories, classes, ignorables
  normalize.cst decomposed and composed as the font can draw them
  tag.cst       scripts and languages as OpenType's tags
  languages.cst BCP 47 to OpenType, generated from HarfBuzz (tools/make_tag_table.py)
  ot.cst        what GSUB and GPOS share: coverage, classes, scripts, features
  map.cst       the plan: which lookups, on which glyphs, in which order
  apply.cst     matching: skipping, input, context, nested lookups
  gsub.cst      substitution, its eight lookup types
  gpos.cst      positioning, its nine lookup types, and attachment
  kern.cst      the legacy kern table
  fallback.cst  what is done when the font does not: spaces, marks
```

---

## Two stages, and the first covers most text

**Simple shaping** is a `cmap` lookup per codepoint, a kerning pair adjustment,
and an advance. That is correct for Latin, Greek, Cyrillic and CJK — most of the
text most programs draw.

**OpenType shaping** is `GSUB` and `GPOS`, and it is not an enhancement for the
scripts that need it. Arabic without it is a row of disconnected letters, which
is not ugly but unreadable. Devanagari without it puts vowel signs in the wrong
place, changing what the word says.

Both stages are HarfBuzz's **default shaper** — the one it uses for Latin,
Greek, Cyrillic, Han, kana and every script without a shaper of its own — and
that is what is here, in both directions. The shapers that reorder or join
(Arabic, Indic, Hangul, Hebrew's presentation forms, Thai, Khmer, Myanmar, the
Universal Shaping Engine) are the [B] part of the plan: a run of those scripts
is shaped by the default shaper meanwhile, which is right for their fonts'
ligatures and marks and wrong where the script needs a shaper's rules.

---

## The pipeline

In HarfBuzz's order, each step its function there:

1. **Unicode properties** of each character: general category, combining class
   as HarfBuzz modifies it, default ignorables, and the continuations that hold
   a grapheme together — marks, ZWJ sequences, emoji modifiers, flags.
2. **Clusters** merged per grapheme; a run in the opposite of its script's
   direction reversed by grapheme.
3. **Normalization**: each grapheme decomposed as far as the font can draw the
   pieces, marks reordered by class, and recomposed where the font has the
   composed character — so `e` + `◌́` and `é` come out alike.
4. **Mirroring** for right-to-left text, and **masks**: which features apply to
   which glyph — automatic fractions around U+2044.
5. **Glyphs** from `cmap`, variation selectors through format 14.
6. **Glyph classes** from `GDEF`, or from the general category without it.
7. **GSUB**, lookups by feature and stage (below).
8. **Advances** from `hmtx`, varied by `HVAR` or the outline's phantom points.
9. **GPOS**, then marks' advances zeroed, attachment offsets resolved; the legacy
   `kern` table where `GPOS` has no kerning.
10. **Default ignorables** made invisible; spaces the font lacks made from its
    space; a right-to-left run reversed into visual order.

Positions are integers in font units, rounded where HarfBuzz rounds at a scale
of one unit per unit — variation deltas, anchors. A layout scales them to its
size.

## Features, lookups, stages

The default features, as HarfBuzz enables them: `rvrn`; then `ltra`, `ltrm`
or `rtla`, `rtlm`; `frac`, `numr`, `dnom` around a fraction slash; `ccmp`,
`locl`, `rlig`, `mark`, `mkmk`, `abvm`, `blwm`; and for horizontal text
`calt`, `clig`, `curs`, `dist`, `kern`, `liga`, `rclt`. A program may turn any
on or off, over the whole run.

The script's tags chosen as HarfBuzz chooses them, falling back to `DFLT`,
`dflt` and `latn`; the language's by HarfBuzz's table of BCP 47 to OpenType,
generated from its source. Each feature's lookups, from the language system or
`FeatureVariations` at the face's instance, are applied in stages: within a
stage, every lookup once, in lookup order, to the glyphs whose mask has its
feature.

---

## Clusters

Every glyph carries the byte offset in the UTF-8 text of the first character of
its **cluster** — the smallest stretch of text and of glyphs that map onto each
other. Clusters are graphemes at least, merged further where a ligature or a
contextual substitution joins characters; they are what the caret moves over
and what selection highlights, and they are in the output from the first day.

---

## Where the boundary with caustic-unicode sits

[caustic-unicode](https://github.com/Caua726/caustic-unicode) answers what the
characters *are*: normalisation, grapheme clusters, script identification,
bidirectional levels. This layer answers what the glyphs *are*.

So shaping receives a run that is already one script, one direction and one font,
because splitting the string into such runs is `layout`'s job using
caustic-unicode's answers.

---

## What is not HarfBuzz's

- **Unsafe to concatenate** is not produced, nor the buffer flag that asks for
  it. Which glyphs HarfBuzz marks so depends on how it caches lookups and on
  fast paths that skip rules early, not only on the font and the text; only
  *unsafe to break*, which does not, is marked here, exactly as HarfBuzz marks
  it. *Safe to insert tatweel* is the Arabic shaper's.
- HarfBuzz's **digests** — filters that skip a lookup when no glyph in the
  buffer could start or continue it — and its rule-set fast paths are left
  out: they change how fast a lookup runs, never what it does, now that
  unsafe to concatenate is not kept.
- The **Unicode version** is caustic-unicode's, 16; HarfBuzz 14.5 has 18. The
  test strings use characters the two agree on.
- A **malformed table** is read as far as it can be, every read bounded by
  the table; HarfBuzz's sanitizer instead rejects or patches it before
  shaping. Fonts that pass the sanitizer shape the same either way.

## Current state

The pipeline, steps 1 to 10, with GSUB: the default features and a run's own,
over all of it or part of it; scripts and languages selected as HarfBuzz
selects them, required features, `FeatureVariations`; the eight substitution
types with nested lookups, ligatures across marks, lookup flags and mark
filtering sets — of which the test fonts reach single, multiple, ligature and
chained context substitution, the rest waiting for the tool's font. Spaces and
marks placed by fallback when the font has no `GPOS`. Every case of the tests
below gives HarfBuzz's glyphs, clusters, flags and positions.

To come, in order: `GPOS`; a font made by the tool with every lookup type,
format and flag; the legacy `kern` table applied; `cmap` subtables chosen in
HarfBuzz's order (symbol first, the Macintosh ones) and format 14's default
variations before the others; the extents of colour glyphs for fallback
marks. A plan is made for each run — HarfBuzz caches them, keyed by face,
script, language, direction and features; a cache is for when layout needs
one.

## Tests

`text/tools/shape_reference.py` shapes strings with HarfBuzz, through uharfbuzz,
and writes what it gives — glyph, cluster, glyph flags, advances, offsets —
beside each test font (`*.shape.ref`): the core pipeline with every feature
off, then GSUB alone — `GPOS`'s features off — at the variable fonts'
instances too; to come, everything on, and a font made by the tool with every
lookup type, every format and every lookup flag. `shape.ttf`, made by the
tool, has no `GDEF` or `GSUB` and an empty `GPOS`, for the classes synthesized
from Unicode and for spaces and marks the font does not place. `shape_test`
must give the same, exactly.
