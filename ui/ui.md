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
frame, complex animation is awkward, and accessibility has nothing persistent to
expose. Against a retained toolkit's ~500k lines for the same widget set, at
maybe 1% of that, it is the right trade for a framework — and it is what
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

## Layout

Widgets need to be placed before they are drawn, and immediate mode makes that
harder than it sounds: at the moment a widget is called, the size of its siblings
is not yet known.

Three ways out, and the choice shapes the whole API:

- **Fixed pass** — the caller gives coordinates. Simplest, and painful for
  anything resizable.
- **Single-pass flow** — widgets stack in a direction and take the space they ask
  for. Enough for most panels, and cannot centre a row whose total width is not
  known until the row ends.
- **Two-pass** — measure, then place. Handles centring and proportional sizing,
  and costs a frame of latency or a second traversal.

Dear ImGui is essentially the second with escape hatches. Starting there and
adding a measure pass where it is needed keeps the common case simple, and the
decision is recorded here rather than made by accident in the first widget.

---

## Theme

Colours, spacing, rounding, borders and font choices in one struct the context
carries, rather than constants scattered through widgets. That is what makes a
program able to look like itself instead of like the library, and it costs
nothing to do from the start and a rewrite to add later.

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
