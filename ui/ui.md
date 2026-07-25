# The UI layer

Widgets: buttons, sliders, text fields, panels, menus, trees, tables. Drawn
through [`render/`](../render/render.md)'s 2D family, typeset by
[`text/`](../text/text.md), fed by [`input/`](../input/input.md).

```
ui/
  ui.cst        hub
  context.cst   the frame: begin, end, the id stack, what has focus
  layout.cst    stacking, rows, columns, grids, spacing
  widget.cst    button, label, checkbox, radio, slider, progress, separator
  edit.cst      text fields, with selection, caret and IME
  container.cst window, panel, collapsing header, tab bar, scroll area
  list.cst      lists, trees, tables
  menu.cst      menu bar, context menu, popup
  draw.cst      the primitives widgets are built from, over render/draw2d
  theme.cst     colours, spacing, fonts, rounding
```

Neither SDL nor raylib has one — raygui is a separate project and Dear ImGui is
the reference everyone actually uses. So this is the layer with no comparison to
measure against, and the most opinion per line.

---

## Immediate mode

Decided already, and worth restating because everything here follows from it:

```cst
if (ui.button(&ctx, "Save")) { save(&doc); }
```

There is no button object. The call draws it, tests the pointer against it, and
returns whether it was clicked, every frame. The widget tree is a function of the
program's data because it is *rebuilt from* the program's data, so the two cannot
disagree — which is the entire class of bug that retained-mode UI spends its
machinery preventing.

The costs are real and worth stating rather than discovering: it redraws every
frame, and both animation and accessibility need something the model does not
obviously provide. Both are answered below, and neither turns out to require a
second mechanism. Against a retained toolkit's ~500k lines for the same widget
set, at maybe 1% of that, it is the right trade for a framework — and it is what
[`render/`](../render/render.md)'s decision that state travels with the work was
already pointing at.

---

## Identity is the hard part

Immediate mode's one genuinely difficult problem, and the place implementations
diverge.

The library must recognise the same widget across frames — to know that *this*
button is the one being held, that *that* field has the caret. With no objects,
identity comes from a generated id, usually hashing the label with the enclosing
scope. Which breaks in exactly the ways you would expect:

- Two buttons labelled "OK" in the same panel collide, and pressing one presses
  both.
- A list whose items reorder makes focus jump to whatever now occupies the slot.
- A label that changes with state — `"Pause"` becoming `"Resume"` — is a
  different widget as far as the id is concerned, so a click in progress is lost.

The answers are an explicit id when the label is not unique, an id stack that
scopes children under their container, and a way to push a loop index. None of
that is optional, and a UI layer that discovers it late has to change every call
site.

---

## What it needs from the layers below

**From `render/`:** the 2D family entirely — sprites for icons, `shapes2d` for
frames and fills, scissor for clipping, and layer ordering so a popup draws over
what it covers. `ui/` adds no drawing of its own; `draw.cst` is a vocabulary of
rounded rectangles and borders composed from what is already there.

**From `text/`:** glyph layout, and the three questions a text field asks — where
the caret sits for a byte offset, which offset is under a point, and what a
selection rectangle looks like across a bidirectional run. Those are much easier
to answer while layout is being built than afterwards, which is why they are
named in `text/`'s note.

**From `input/`:** events rather than sampled state, because a UI cares about
transitions — pressed, released, dragged — and losing a click that happened
between two frames is a bug users report as "it sometimes doesn't work". Text
input, dead keys and IME composition come from there too, since a text field that
a Japanese user cannot type into is not a text field.

---

## Layout is immediate too

Widgets are placed as they are called, in one pass, taking the space they ask
for and advancing a cursor. No measure pass, no deferred placement, no second
traversal.

That is the same decision as everything else here: the call does the work, and
reading it tells you what happened. It also means a widget's position is known
at the moment it is called, so hit-testing against the pointer can happen right
there rather than being deferred to a later phase.

The cost is real and worth naming: **a row cannot be centred if its total width
is only known once the row has ended.** Anything whose placement depends on a
sibling that has not been called yet needs the program to supply a size, or to
compute it and pass it in.

Where that is not enough, the answer is an explicit measure — the program asks
what a piece of content would occupy, then lays it out with the number in hand.
That is a function a caller invokes, not a hidden phase the library runs, so it
stays consistent with the rest.

---

## Per-id state, which immediate mode never actually avoided

The model is usually described as keeping no state. It does not keep the *widget
tree*, which is the part that matters — but it has always kept a small table
keyed by id: which widget is being held, which has keyboard focus, where a
scroll area is scrolled to, what a text field has selected.

Naming that table makes two things fall out that otherwise look like problems.

**Animation.** A panel that slides open has to remember how far open it is. That
is another field in a table that already exists, not a new mechanism and not a
retreat toward retained mode. A program that wants control instead passes its own
`t` and drives the animation itself, which stays the more explicit path and is
always available.

**Cheap persistence for expensive widgets.** A table with ten thousand rows does
not need its layout recomputed every frame; the row height and the scroll offset
live in the same table, and only the visible rows are built.

The table is bounded and its size is stated, for the same reason the draw queue
and the event queue are.

## Accessibility

Immediate mode has nothing persistent for a screen reader to walk, and *"not a
toy — real software"* makes ignoring that uncomfortable.

Implementing it properly means AT-SPI over D-Bus on Linux and UI Automation over
COM on Windows — a platform surface comparable to a window backend, for a layer
that does not exist yet. So not now.

What is decided now is that it stays possible: **the id stack is the tree.**
Immediate mode does have hierarchy — a widget's id is scoped by its container —
it simply does not persist it. So the context can be told to record a node per
widget as it goes, and when it is not told, the recording costs nothing. The
platform bridges become backends later, against a tree that was already there.

Deciding this late instead would mean discovering that ids were generated in a
way that cannot express hierarchy, and changing every call site.

## Theme

Colours, spacing, rounding, borders and font choices in one struct the context
carries, rather than constants scattered through widgets. That is what makes a
program able to look like itself instead of like the library, and it costs
nothing to do from the start and a rewrite to add later.

---

## Not now, and deliberately

**Multi-window and docking.** Dear ImGui added both after the fact and it
reshaped its architecture — viewports turn one context into several, each with
its own platform window and render target. Doing it later would cost the same
rework here, and doing it now would cost it before there is a single working
widget. Neither is worth it yet.

**A retained escape hatch** — a path where a widget keeps its own state and
rebuilds only what changed. The per-id table above covers the case that motivates
it, which is expensive widgets, without a second model to maintain.

---

## Order of work

Blocked on `render/`'s 2D path and on `text/` reaching layout and hit-testing —
`ui/` cannot start before either.

1. **`context` and the id stack**, which everything else assumes.
2. **`draw` and `theme`**, over `render/draw2d`.
3. **`layout`**, then button, label, checkbox and slider — enough to be useful.
4. **`container`**: panels, scroll areas, tabs.
5. **`edit`**, once `text/` can answer caret and hit-testing, and `input/` can
   deliver composed text.
6. **`list` and `menu`**, which are the widgets that most need the id stack to be
   right.
