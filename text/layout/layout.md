# Layout

A string becomes lines of positioned glyphs in a box. Where
[`../shape/shape.md`](../shape/shape.md) answers what the glyphs are, this
answers where they go — and it is the part [`ui/`](../../ui/ui.md) talks to.

```
layout/
  layout.cst   the hub: text, styles and a box in; lines of glyphs out
  font.cst     what text is set in: faces found for characters, opened at a size
  items.cst    paragraphs, bidi levels, scripts, faces: the runs shaping takes
  lines.cst    breaking into lines, fitting, ellipsis, bidi reordering, alignment
  cursor.cst   carets, hit-testing, selection — the questions a text field asks
```

Everything here is in pixels, as `f64`, as `ui/` measures: a style says how
many pixels its em is, and font units are scaled to it.

---

## Runs come before shaping

A string is not shaped as one unit. It is split into runs that are uniform in
what the shaper needs fixed — **one script, one direction, one face, one
style** — because a fallback chain means different parts of a sentence may come
from different faces.

- **Paragraphs** end at a paragraph separator, a line feed, a carriage return
  (with the line feed after it) or a next line; each is laid out on its own,
  in its own direction.
- **Directions** are the bidirectional algorithm's levels, caustic-unicode's
  UAX #9, over each paragraph — its base direction given, or found from its
  first strong character.
- **Scripts** come from caustic-unicode's Script property; characters common
  to scripts (spaces, digits, punctuation) and inherited ones (marks) take the
  script of the text before them, or after them at the start.
- **Faces**: the style's font finds the first face with a glyph for each
  grapheme's first character, and the grapheme stays in it when the face has
  the rest.

## Breaking is not splitting on spaces

Line breaking is UAX #14, and caustic-unicode implements it: Chinese and
Japanese break between almost any two characters and have no spaces, and even
in English a break may not fall after an opening bracket or before a closing
one. This layer asks for the break **opportunities** and fits greedily: each
line takes as many as fit the width, a word longer than a line is broken
between graphemes, and spaces at the end of a line hang past it. (Knuth and
Plass's paragraph optimisation is what a document renderer needs, and can
come later without disturbing the rest.)

A word broken between graphemes is a last resort, so it does not make the
text any narrower: the **least width** a layout reports is its widest stretch
between two opportunities, the spaces before each left out — CSS's
min-content, what a wrapping label can be given without breaking a word.

A line ends where a break falls between two clusters the shaper said are
**safe to break** between: the glyphs on each side stay as they were shaped.
Where it is not safe — inside a kerned pair, a contextual form — each side is
shaped again on its own.

A text may be held to a number of lines; the last then ends in an **ellipsis**
(U+2026, in the style of what it follows) when there is more. A **tab** moves to
the next multiple of the tab width from the start of the line.

## Bidirectional text reorders after breaking

The levels are resolved over the logical paragraph; reordering into visual
order happens **per line**, after the break points are known, because a run
split across two lines reorders independently on each (UAX #9's L1 and L2:
trailing whitespace back at the paragraph level, then the highest levels
reversed first).

## Lines

A line is as tall as the tallest face on it — its ascender, descender and
line gap at its size (`OS/2`'s typographic values when the face says to use
them, `hhea`'s otherwise) — times the line height asked for. Lines are aligned
to the start or the end of the paragraph's direction, to the left, the right,
the centre, or justified: the space left on a line, but the last of a
paragraph, shared between its spaces.

Underline and strikethrough are where each face puts them (`post`, `OS/2`), at
the thickness it gives, along the glyphs of the spans that ask.

## The three questions a text field asks

`ui/`'s text field needs these, and answering them afterwards from positioned
glyphs is much harder than recording them while laying out:

- **Where is the caret for this byte offset?** Not per glyph — a grapheme
  cluster may be several codepoints and one caret stop, and a ligature is one
  glyph with caret positions inside it, shared out by its graphemes.
- **Which offset is under this point?** The inverse, including the half-glyph
  rule that puts the caret on the nearer side.
- **What does the selection look like?** In bidirectional text, a logically
  contiguous selection can be **two or more disjoint rectangles**.

Where an Arabic run meets a Latin one, the boundary belongs to a level rather
than to either side, and a caret placed there has two valid positions — one
for each direction; the caret says which it is and where the other is. Which
one is the position's: upstream it goes with the text before it, as at a
wrap it is the end of the line before. Moving left and right goes from place
to place as they are drawn, so each stop inside a run of the other direction
is reached from either side. Moving up and down keeps the x the caret had;
moving by word uses caustic-unicode's word boundaries, by character its
grapheme boundaries.

## Vertical writing

CJK set vertically is not rotated horizontal text: some glyphs rotate, some do
not, punctuation moves to different positions in the em box, and the line
advance runs horizontally. That is [B], and the line model keeps an axis so as
not to assume horizontal.

---

## Tests

`layout_test` sets text in the test fonts (`text/testdata`) through a font of
those faces and checks what layout promises: where lines break — before a
width is passed, at opportunities, by graphemes when a word is too long and
as many as fit, giving back a grapheme when a part shaped alone is wider, at
paragraph ends; the least width, the widest word of any paragraph whatever
the width; that each line's glyphs are the shaper's for that stretch,
features ranged per paragraph, an item that shapes to nothing; alignment of
every glyph, line and decoration, justification of every space, the
ellipsis in the style of what it follows, tabs at and between stops, line
heights; bidirectional lines in visual order; decorations, side by side and
not under hanging spaces; paragraphs, scripts, faces and styles found for
each character, the face found again where a style changes inside a
grapheme; more faces at sizes than can be open, refused; a long text, and
one whose memory ends where it does. For the cursor: every caret stop
round-trips through hit-testing; moving right and left visits each stop
once, the caret moving the one way — through wrapped lines, runs of both
directions and marks; graphemes and words from any offset, inside a
character too; and selections cover exactly what they select.

Mutation testing changed every comparison, sum, constant and return value of
the layout's modules and the cursor's, one at a time; a case was added
wherever a change went unseen and could be seen — one of them showed moving
right and left skipping the inside of a run of the other direction, where
an offset's two places meet, which was then put right. Of the 282 changes
that still go unseen, most are of kinds no output shows: memory refused and
faces that cannot be opened; sentinels any negative value serves;
capacities; one past an end where nothing is read; equal values and ties; a
first guess at a line's end, too short, that measuring corrects; text shaped
again where it need not be, to the same glyphs; what no test font has (a
line gap, a font without strikeout metrics). Some 25 are paths no case tells
apart yet: tabs in a right-to-left line, spaces hanging over several parts,
a justified line of several parts, trailing whitespace of another level than
its paragraph's, a line given back to an opportunity once shaped alone, an
ellipsis wider than the box.

## Current state

Text is itemized by paragraph, bidi level, script, face and style; broken at
caustic-unicode's opportunities, by graphemes when a word is wider than the
box, at most `max_lines` lines with an ellipsis, its least width the widest
stretch between opportunities; reshaped where a break falls
inside what the shaper marked unsafe; reordered with bidi per line; aligned
(start, end, left, right, centre, justified on spaces), with tabs and line
heights from the faces' metrics; underlined and struck through from `post`
and `OS/2`. The cursor answers carets (two at a direction boundary), hit
tests, selection rectangles across bidi runs, and moves by graphemes, words,
visual left and right, and lines with a preferred x.

Vertical writing and hyphenation are [B].
