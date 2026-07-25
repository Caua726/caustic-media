# Layout

A string becomes lines of positioned glyphs in a box. Where
[`../shape/shape.md`](../shape/shape.md) answers what the glyphs are, this
answers where they go — and it is the part [`ui/`](../../ui/ui.md) talks to.

```
layout/
  run.cst      splitting a string into script, direction and font runs
  line.cst     breaking, and fitting shaped runs into a width
  align.cst    left, right, centre, justify
  bidi.cst     reordering visual runs from logical order
  cursor.cst   caret positions, hit-testing, selection rectangles
  vertical.cst CJK vertical writing
```

---

## Runs come before shaping

A string is not shaped as one unit. It is split into runs that are uniform in
three things — **one script, one direction, one font** — because a shaper needs
all three fixed, and because a fallback chain means different parts of a sentence
may come from different faces.

All three splits come from
[caustic-unicode](https://github.com/Caua726/caustic-unicode): script
identification, the bidirectional algorithm's embedding levels, and the grapheme
boundaries that keep a cluster from being split across runs.

That ordering is why `run.cst` is first in this directory and why shaping has no
opinion about Unicode.

---

## Breaking is not splitting on spaces

Line breaking is UAX #14, and caustic-unicode implements it. It matters because
the naive version is wrong in most of the world: Chinese and Japanese break
between almost any two characters and have no spaces; Thai has no spaces and
needs dictionary-based segmentation; and even in English, a break may not fall
after an opening bracket or before a closing one.

So this layer asks for the break **opportunities** and decides which to use given
a width — which is a different and much simpler problem, and one where the
classic choice appears: greedy fitting, or Knuth-Plass paragraph optimisation
that minimises raggedness across the whole paragraph.

Greedy is what a UI needs. Knuth-Plass is what a document renderer needs, and it
is worth naming as a thing that can be added later without disturbing the rest.

---

## Bidirectional text reorders after breaking

The bidi algorithm produces embedding levels over the logical string;
reordering into visual order happens **per line**, after the break points are
known, because a run split across two lines reorders independently on each.

Getting the order wrong is the visible failure. Getting the *edges* wrong is the
subtle one: where an Arabic run meets a Latin one, the boundary belongs to a
level rather than to either side, and a caret placed there has two valid positions
— one for each direction.

---

## The three questions a text field asks

`ui/`'s text field needs exactly these, and answering them afterwards from
positioned glyphs is much harder than recording them while laying out:

- **Where is the caret for this byte offset?** Not per glyph — a grapheme cluster
  may be several codepoints and one caret stop, and a ligature is one glyph with
  a caret position inside it.
- **Which offset is under this point?** The inverse, including the half-glyph rule
  that puts the caret on the nearer side.
- **What does the selection look like?** In bidirectional text, a logically
  contiguous selection can be **two or more disjoint rectangles**, which is the
  thing implementations get wrong and users notice immediately.

These are why `cursor.cst` exists as its own file rather than being a couple of
functions on the side.

---

## Vertical writing

CJK set vertically is not rotated horizontal text: some glyphs rotate, some do
not, punctuation moves to different positions in the em box, and the line advance
runs horizontally. `vertical.cst` is small but it cannot be faked with a
transform, and pretending otherwise produces text a reader can see is wrong.

Not urgent, and worth naming so that the line model is not built in a way that
assumes horizontal.

---

## Order of work

1. **`run` and `line`**, greedy, horizontal, left-to-right — enough for a label.
2. **`align`.**
3. **`cursor`**, which `ui/`'s text field blocks on entirely.
4. **`bidi`**, once a script that needs it is supported by shaping.
5. **`vertical`** and Knuth-Plass, when something asks.
