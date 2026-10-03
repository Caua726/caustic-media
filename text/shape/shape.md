# Shaping

Codepoints to positioned glyphs. The largest single piece of work in this layer,
and the one where difficulty depends entirely on the writing system.

This is our own shaper, written from the specifications — OpenType's `GSUB`,
`GPOS`, `GDEF` and common tables, its script and language tag registries,
Unicode's normalization (UAX #15) and character properties through
[caustic-unicode](https://github.com/Caua726/caustic-unicode). No other
shaper's code or tables are in it. HarfBuzz is what most programs shape with,
so the tests run it — through uharfbuzz, as a black box — and hold our output
to its glyphs, clusters, advances and offsets, so that text set here looks as
it does elsewhere.

```
shape/
  shape.cst     the hub: a Shaper, a Text in, glyphs with clusters and positions out
  glyphs.cst    the run being shaped: its records, the passes that rewrite it,
                clusters, where it may not be broken
  chars.cst     what shaping asks of Unicode: categories, classes, ignorables
  compose.cst   characters decomposed and composed as the font can draw them
  scripts.cst   ISO 15924 scripts as OpenType tags, and their directions
  langs.cst     BCP 47 languages as OpenType tags
  tables.cst    the data for both, generated (tools/make_shape_tables.py)
  otl.cst       OpenType layout's common tables, read where they lie
  plan.cst      the features a text asks for, as lookups to apply, in order
  seq.cst       matching a lookup's sequences, and lookups nested in context
  subst.cst     GSUB: the eight substitution types
  pos.cst       GPOS: the nine positioning types, and attachments resolved
  kern.cst      the legacy kern table
  fallback.cst  what the font does not do: spaces it lacks, marks it does not place
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

Both are here for every script without rules of its own — Latin, Greek,
Cyrillic, Han, kana, Thaana and the rest — in both directions. The scripts
whose shaping reorders or joins (Arabic joining, Indic reordering, Hangul
composition, Thai, Khmer, Myanmar, the Universal Shaping Engine's) are the [B]
part of the plan; a run of them is shaped meanwhile by the same rules, which
is right for their fonts' ligatures and marks and wrong where the script needs
more.

---

## The run

A text arrives as UTF-8 with the paragraph around it. The part to shape becomes
one record per character, each with its **cluster**: the byte offset of the
first character it came from. Then glyphs replace characters — one for one,
several for one, one for several — and records keep their clusters, joined
where glyphs join: the glyphs of a ligature, and every glyph that already
shared a cluster with one of them, take the smallest. Clusters therefore stay
in the text's order, and are what a caret moves over and selection covers.

Steps that rewrite the run read it from one array and write it into another,
then swap them: what was written is the past a rule may look back at, what is
still to read the future it looks ahead to.

## The steps

1. **Characters.** A byte that does not begin a complete, well-formed UTF-8
   sequence reads as U+FFFD by itself. Each character's general category,
   combining class and whether it is a default ignorable come from
   caustic-unicode and Unicode's own data.
2. **Graphemes.** Marks, emoji modifiers, the second of a pair of regional
   indicators, a ZWJ and the pictograph after it, tags and the halfwidth kana
   sound marks continue the character before them, which a letter or a space
   always begins; a grapheme is one cluster from here on.
3. **A mark first.** A text that begins with a mark is given a dotted circle
   to carry it, when the font has one.
4. **Direction.** A run whose direction is not its script's own — Latin set
   right to left, Hebrew left to right — is shaped in the script's, its
   graphemes reversed, and turned back at the end; digits and flags alone in a
   right-to-left script read left to right and are left as they are.
5. **Masks**: which features apply to which glyph — the text's own over the
   stretches it asked, and automatic fractions where digits stand on both
   sides of U+2044.
6. **Mirroring.** Right to left, a character with a mirrored form takes it
   when the font has it; the rest are left to the font's `rtlm`.
7. **Composition.** Within each grapheme, a character the font cannot draw is
   decomposed into what it can, marks are put in canonical order, and a base
   and mark the font has a single character for are composed into it — so
   `e` + `◌́` and `é` come out alike, whichever the font has. A space the font
   lacks is drawn with its space, and a non-breaking hyphen with its hyphen.
8. **Glyphs** from `cmap`; a variation sequence through format 14, its
   selector then taken out — one the face does not know stays, drawn as
   nothing.
9. **Classes**: base, ligature, mark, and a mark's attachment class, from
   `GDEF`; without it, the non-spacing marks are marks and the rest bases.
10. **Substitution**: the plan's `GSUB` lookups (below).
11. **Advances** from `hmtx`, varied by `HVAR` or the outline's phantom
    points; spaces the font lacked made their width — an em, its fractions,
    a figure's, a full stop's.
12. **Positioning**: `GPOS` when the font has it; the `kern` table when
    `GPOS` has no kerning — each adjustment shared between the pair's two
    glyphs; marks then have no advance of their own, attachments become
    offsets, and a font with no `GPOS` has its marks placed by their class
    around the base (fallback.cst).
13. **Order**: a right-to-left run is reversed into the order it is drawn in.
14. **Ignorables**, which draw nothing, become the font's space with no
    width — or are taken out, if the text asks.

Positions are integers in font units: a layout scales them to its size.

## Features and lookups

The features turned on unless a text turns them off are the ones the
OpenType specification lists for every script: `rvrn` first, alone, so that
a variable font's substitutions see the glyphs before anything else does;
`ltra`, `ltrm` or `rtla`, `rtlm`; `frac`, `numr`, `dnom` around a fraction
slash between digits; `rand`; `ccmp`, `locl`, `rlig`, `mark`, `mkmk`, `abvm`,
`blwm`; and for horizontal text `calt`, `clig`, `curs`, `dist`, `kern`,
`liga`, `rclt`. A text adds its own, each over all of it or over a stretch of
clusters, with a value — 0 off, 1 on, more choosing an alternate.

The script's tags (`latn`; `dev2` then `deva`; `kana` for both kana) select a
script in each table, or `DFLT`, `dflt`, `latn`; the language's tags select a
language system, or the script's default. A feature is found in the selected
language system, through `FeatureVariations` at a variable font's instance.
Each found feature has its own bits in every glyph's mask, set over the
clusters it was asked for; features on everywhere with value 1 share one.

The lookups of the found features are applied in `LookupList` order, each
once, to the glyphs whose masks hold one of the features that asked for it —
in two stages for `GSUB`, `rvrn`'s and then the rest. The required feature of
a language system is always on.

## Matching

A lookup's flags say which glyphs it does not see: bases, ligatures, marks,
marks of another attachment class or outside a mark filtering set. Beyond
those, a default ignorable is passed over when it is not what the rule asks
for — except that a zero width non-joiner stops a ligature; the hidden ones
(tag characters, Mongolian free variation selectors, and a combining
grapheme joiner that keeps two marks from being reordered — the first of a
class above the second's) stop substitutions, which must see them,
but not positioning; and a zero width joiner stops `mark` and `mkmk`, which
handle joiners themselves. Variation selectors are not hidden: one the font
answers through its cmap is gone by then, and one it does not is passed over
like any default ignorable.

A ligature remembers which of its components each following mark belongs to,
so that `mark` attaches it to the right one, and is not formed across a mark
that belongs to another ligature's component. Context rules apply their
lookups at positions of the sequence they matched; a nested lookup may add or
remove glyphs, and the positions after it move with them. Nesting stops at 64
levels, and work is bounded by the text's length, so that no font makes
shaping loop.

## Where text may be broken

A layout breaks a shaped paragraph into lines without shaping it again where
it can. So every glyph says whether the text is **unsafe to break** before its
cluster: whether shaping the two sides apart could give different glyphs or
positions. Here it is so where anything joined the clusters on both sides —
a rule's match with its context, a ligature, a pair kerned, a mark attached,
a cursive connection, a fraction — and the tests check what it promises: at
every boundary not so marked, the two sides shaped apart give exactly the
whole.

---

## Where the boundary with caustic-unicode sits

caustic-unicode answers what the characters *are*: categories, normalization,
grapheme clusters, script identification, bidirectional levels. This layer
answers what the glyphs *are*.

So shaping receives a run that is already one script, one direction and one
font, because splitting a paragraph into such runs is `layout`'s job, using
caustic-unicode's answers.

---

## Tests

`text/tools/shape_reference.py` shapes strings with HarfBuzz, through uharfbuzz,
and writes its glyphs, clusters, advances and offsets beside each test font
(`*.shape.ref`): the steps above with every feature off, then substitution
with positioning's features off, then everything, at the variable fonts'
instances too. `shape_test` must give the same, exactly, and its own
unsafe-to-break marks must keep their promise in every case.

`shape.ttf`, made by the tool, has no `GDEF` or `GSUB` and an empty `GPOS`:
classes from Unicode, and spaces and marks the font does not place;
`shape-plain.ttf` the same with no `GPOS` and a legacy `kern` table, for the
fallback and for kerning by the old table; `lookups.ttf` every lookup type,
format and flag, lookups of several subtables and rule sets of several
rules in which an earlier one passes over what a later one takes, nested
lookups that grow and shrink an input that is not side by side, attachment
chains past their limit; `varlookups.ttf` lookups an instance chooses, and
a `GDEF` with no classes. Where the fallback places marks, `shape_test`
holds them to what it promises — centred, on an edge, clear or touching,
stacked — instead of to HarfBuzz.

`parts_test` checks the parts on their own: characters, script and language
tags, UTF-8, Hangul, the run's bounds and the way a pass grows and seeks,
how a plan sorts and merges its lookups and picks a script's or a
language's second tag, where text is exactly unsafe to break, nesting.

Mutation testing changed every comparison, sum, constant and return value
in the shaper's modules — 2053 changes — one at a time, and ran both tests
on each; a case was added wherever a change went unseen and could be seen.
Of the 358 that still go unseen, most are of kinds no output shows: guards
against malformed fonts (offsets out of range, formats that do not exist,
indices past their arrays); bounds no font or Unicode data reaches (64
items, decompositions nine deep, sums past 2^31); a loop reading one past
its end, where what it reads is not used; capacities grown more or less
eagerly; sentinels any negative value serves; equal values clamped either
way; text marked unsafe to break more widely than needed, which keeps the
promise; memory refused, or a run that has already failed; changes that
land where the lookup has already moved on; and what no test font has
(`ltra` and its kin, a hinting device table in a variable font). Some 60
are paths a font could take that no case here tells apart, and so are not
yet held to HarfBuzz: a nested lookup deleting past the input it was given,
or a later record on the last glyph one added; marks after a ligature whose
last component is itself a ligature carrying marks; a grapheme joiner's
hiding where a lookup would see it; a rule's input begun on a ligature's
component; pieces of a multiple substitution counted into a ligature; marks
with advances of their own in a font without GPOS.

## Where this is not what HarfBuzz does

Most of what a shaper does is fixed by the specifications; the rest is
choices, and these are ours:

- **Marks keep Unicode's canonical order.** HarfBuzz reorders some marks by
  classes of its own (Arabic shadda first, some Tibetan and Tai Tham marks
  last); that belongs to the shapers of those scripts, which are [B].
- **Marks without GPOS** are placed by Unicode's combining classes around
  the base's ink, centred, a sixteenth of the em clear (fallback.cst);
  HarfBuzz has its own guesses. The tests hold ours to what they promise.
- **Unsafe to break** is ours: marked wherever a match, a ligature, a pair,
  an attachment or a fraction joins clusters, and checked by its promise.
  HarfBuzz's other flags — unsafe to concatenate, safe to insert tatweel —
  are not made.
- **Feature variations** are met by the one condition the specification
  defines, an axis range.
- **Language tags** come from the registry and ISO 639-3, without
  HarfBuzz's private BCP 47 extensions or its overrides.

Where a choice made no difference to anyone, HarfBuzz's was taken, so that
text set here looks as it does elsewhere: an ill-formed byte is one U+FFFD;
a variation selector that found its glyph is taken out; a legacy kerning
value is split between the pair; `rand` draws its numbers as HarfBuzz
does.

## Current state

Written from the specifications, HarfBuzz used only as a black box: the
steps above, `GSUB` and `GPOS` with every lookup type, format and flag, the
legacy `kern` table, the fallback for marks and spaces. Every case of the
tests gives HarfBuzz's glyphs, clusters, advances and offsets (but for the
marks the fallback places), and every cut the shaper says is safe gives the
whole. The scripts with shapers of their own are [B].

A plan is made for each text; HarfBuzz caches them by face, script,
language, direction and features — a cache is for when layout needs one.
