# Immediate mode

The lightweight half of [`ui/`](../ui.md): widgets that exist only for the
duration of the call that draws them. What Dear ImGui and raygui are — for
HUDs, debug panels, editors inside a game, tools that live for an afternoon.
Applications with windows full of forms, lists and menus are what the toolkit
in [`../ui.md`](../ui.md) is for.

```
immediate/
  immediate.cst  hub: the context, the frame, the widgets
  draw.cst       rectangles, borders and text, over the software target today
  font.cst       an 8×12 bitmap font — unused, see below
  immediate_test.cst
```

---

## The model

```cst
if (ui.button(&ctx, "Save", x, y, w, h) == 1) { save(&doc); }
```

There is no button object. The call draws it, tests the pointer against it, and
returns whether it was clicked, every frame. The widget tree is a function of the
program's data because it is *rebuilt from* the program's data, so the two cannot
disagree — which is the entire class of bug that retained-mode UI spends its
machinery preventing.

The costs are real: it redraws every frame, nothing persists for a screen reader
to walk, and anything that needs memory across frames — a held slider, a caret,
a scroll offset — needs somewhere to keep it. That last one is the identity
problem below, and it is why the toolkit is a separate design rather than this
one grown larger.

---

## Identity is the hard part

The library must recognise the same widget across frames — to know that *this*
slider is the one being dragged, that *that* field has the caret. With no
objects, identity comes from a generated id, usually hashing the label with the
enclosing scope. Which breaks in exactly the ways you would expect:

- Two buttons labelled "OK" in the same panel collide, and pressing one presses
  both.
- A list whose items reorder makes focus jump to whatever now occupies the slot.
- A label that changes with state — `"Pause"` becoming `"Resume"` — is a
  different widget as far as the id is concerned, so a click in progress is lost.

The answers are an explicit id when the label is not unique, an id stack that
scopes children under their container, and a way to push a loop index. None of
that is optional, and a layer that discovers it late has to change every call
site.

## Per-id state

The model is usually described as keeping no state. It does not keep the
*widget tree* — but it has always kept a small table keyed by id: which widget is
being held, which has keyboard focus, where a scroll area is scrolled to. The
table is bounded and its size is stated, for the same reason the draw queue and
the event queue are. Animation is another field in it: a panel that slides open
remembers how far open it is.

## Layout is immediate too

Widgets are placed as they are called, in one pass, taking the space they ask
for and advancing a cursor. No measure pass, no deferred placement.

The cost: **a row cannot be centred if its total width is only known once the
row has ended.** Anything whose placement depends on a sibling not yet called
needs the program to supply a size, or to measure first and pass it in. That
limit is acceptable for a debug panel and is exactly what the toolkit's two-pass
layout exists to remove.

## Input is events, not samples

A widget cares about transitions — pressed, released, dragged — so it is fed
[`input/`](../../input/input.md)'s events for the frame, not the pointer's
position and button state when the frame began. A click that starts and ends
between two frames is otherwise lost, and users report that as "it sometimes
doesn't work".

---

## Where the code is

`immediate.cst` is a working prototype — `examples/app_studio.cst` is built on
it — that predates the design above and has none of its structure yet:

| | designed | now |
|---|---|---|
| Identity | id stack, explicit ids, per-id table | **none**: hit-test against the rect each call |
| Input | events from `input/` | the pointer's position and button, sampled once per frame |
| Layout | a cursor advanced per widget | absolute coordinates passed by the program |
| Drawing | through `render/`'s 2D family | straight into a `gpu/software` target, so not on a GPU backend |
| Text | `text/` layout, UTF-8 | the stdlib's 8×16 bitmap font, one glyph per byte |
| Theme | a struct the program fills | colours fixed in `context_default` |

Consequences worth knowing before using it: a slider stops following the pointer
the moment the pointer leaves its track, because nothing records that it is the
one being dragged; `min == max` divides by zero; accented text draws as two
glyphs per character.

`font.cst` is not used — `draw.cst` takes the stdlib font — and several of its
glyphs are malformed (`)` and `]` are a single vertical stroke). It is kept only
until the stdlib font is replaced by `text/`.

The test checks the one thing a widget must never do whatever it is given, which
is write outside the target: `tgt.put` is unchecked by design, and
`waveform_display` once wrote past the end of the framebuffer when placed below
it.

## Order of work

1. **Context, id stack and the per-id table**, which every widget after this
   assumes.
2. **Events in**, from `input/`, replacing the sampled pointer.
3. **A layout cursor**, so programs stop passing coordinates.
4. **Drawing through `render/draw2d`**, once it exists, so the same widgets run
   on a GPU backend.
5. **Text through `text/`**, which is when `font.cst` goes.

## Current state

A working prototype, tested, that draws through `gpu/software` with its own
small font: the immediate-mode layer of `ui/` as this note designs it, before
`render/`'s 2D family and `text/` existed. It is not yet moved onto them; the
retained toolkit beside it (`ui/ui.md`) is where the work went, and the move
comes after it (the order of work above).
