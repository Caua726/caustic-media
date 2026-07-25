# Shaping

Codepoints to positioned glyphs. The largest single piece of work in this layer,
and the one where difficulty depends entirely on the writing system.

```
shape/
  simple.cst    cmap lookup, kerning, advance — Latin, Greek, Cyrillic, CJK
  feature.cst   OpenType feature tags and how a script selects them
  gsub.cst      substitution: ligatures, alternates, joining, reordering
  gpos.cst      positioning: pair kerning, marks, cursive attachment
  script/       the per-script rules that drive the tables
```

HarfBuzz is 1252 KB and 551 symbols. Nearly all of that is this file's subject.

---

## Two stages, and the first covers most text

**Simple shaping** is a `cmap` lookup per codepoint, a kerning pair adjustment,
and an advance. That is correct for Latin, Greek, Cyrillic and CJK — most of the
text most programs draw — and it is a few hundred lines.

A program that draws English, Portuguese, Russian or Japanese is finished here.

**OpenType shaping** is `GSUB` and `GPOS`, and it is not an enhancement for the
scripts that need it. Arabic without it is a row of disconnected letters, which
is not ugly but unreadable. Devanagari without it puts vowel signs in the wrong
place, changing what the word says.

Staging it this way is honest rather than a limitation dressed up: a program gets
working text early, and a program that needs Arabic knows precisely what is
missing instead of shipping something wrong.

---

## What GSUB and GPOS actually are

Both are the same machinery pointed at different problems: a set of **lookups**,
each a table of rules, selected by script, language and feature tag, applied in
order.

**GSUB substitutes glyphs.** One for one (a small-caps `a`), one for many
(decomposing a ligature), many for one (`f`+`i` becoming `fi`), or contextually —
this glyph becomes that one *when preceded by* something. Arabic joining is four
contextual substitutions: initial, medial, final and isolated forms of every
letter.

**GPOS moves them.** Pair kerning that is richer than the old `kern` table,
attaching a mark to a base glyph at a defined anchor, attaching marks to other
marks, and cursive attachment where one glyph's exit point meets the next one's
entry — which is how Arabic's baseline flows.

The rule formats are where the volume is: coverage tables, class definitions,
chained context in three formats each. It is not conceptually hard; it is a lot
of carefully specified table walking, and every format has to be right or a font
silently renders wrong.

---

## Scripts, in tiers of what they demand

| | What it needs |
|---|---|
| **Latin, Greek, Cyrillic** | nothing beyond kerning. Ligatures and small caps are optional polish |
| **CJK** | nothing for horizontal text. Vertical writing needs alternate forms and rotation |
| **Hebrew** | marks positioned by GPOS, and right-to-left ordering from layout |
| **Arabic** | **mandatory**: joining forms via GSUB, cursive attachment via GPOS |
| **Indic** — Devanagari, Bengali, Tamil and the rest | **the hardest**: syllables are reordered before the tables are applied, by rules specific to each script |
| **Thai, Khmer, Myanmar** | mark stacking with reordering, without Indic's syllable model |

Indic is where a shaping engine earns its size. The reordering is not in the font
— it is in the *shaper*, per script, and it happens before GSUB sees the string.
That is why HarfBuzz has a file per script family rather than one general
algorithm, and it is why `script/` is a directory here rather than a function.

---

## Where the boundary with caustic-unicode sits

[caustic-unicode](https://github.com/Caua726/caustic-unicode) answers what the
characters *are*: normalisation, grapheme clusters, script identification,
bidirectional levels. This layer answers what the glyphs *are*.

So shaping receives a run that is already one script, one direction and one font,
because splitting the string into such runs is `layout`'s job using
caustic-unicode's answers. That division keeps this file from having any opinion
about Unicode itself, which is the only way it stays tractable.

---

## Order of work

1. **`simple`** — cmap, kern, advance. Text on screen for most of the world.
2. **`feature`**, the tag and lookup selection machinery, which both tables need.
3. **`gpos`** before `gsub`: mark positioning fixes accented text in fonts where
   composites do not, and it is the smaller of the two.
4. **`gsub`**, then **Arabic**, which is the first script that needs both.
5. **Indic**, which is a project of its own and should be treated as one.
