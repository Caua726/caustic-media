# The UI layer

Interfaces for applications: what GTK is for C and Qt is for C++. Widgets that
exist as objects, laid out by the toolkit, and drawn by this library itself on
every platform — so a program written against it looks and behaves the same on
X11, Wayland and Windows, and links no toolkit of anybody else's.

```
ui/
  ui.cst          hub
  app.cst         the application: its windows, each with its own tree
  tree.cst        widgets as nodes in a window's tree, and the handles to them
  widget.cst      the table of functions every kind of widget fills in
  geom.cst        sizes, constraints, and the layout record each widget carries
  layout.cst      measure, place, the pass, sharing room along an axis
  box.cst         rows and columns
  grid.cst        rows and columns at once
  stack.cst       one page at a time; children layered on each other
  scroll.cst      a viewport onto content larger than it
  splitter.cst    panes divided by handles the user drags
  event.cst       what a widget receives
  router.cst      where events go: hit-testing, capture, bubbling, hover,
                  focus, shortcuts, mnemonics, the modal scope
  timer.cst       things that happen later, on an explicit clock
  signal.cst      connecting a callback to what a widget emits
  model.cst       row and cell models behind lists, trees and tables
  damage.cst      what a frame has to repaint, found from the tree
  paint.cst       drawing the tree through render/'s 2D family
  painter.cst     what a widget's paint entry is handed
  style.cst       the theme: colours, metrics, fonts, per-state variants
  system.cst      the desktop's settings, as the theme and the timings
  text.cst        one text service: runs laid out and drawn
  access.cst      role, name, states, value and actions per widget
  widgets/        label, button, check, radio, entry, slider, progress,
                  list, tree, table, menu, tabs, dialog

  immediate/      the other model, for HUDs and tools   immediate/immediate.md
```

---

## Three ways to put an interface in a window

The promise from the start was a GUI *the library's way or the program's way*,
the same promise [`gpu/`](../gpu/gpu.md) makes about Vulkan. So there are three,
and none of them is a wrapper over another:

| | for | |
|---|---|---|
| **the toolkit** — `ui/` | applications: forms, lists, menus, dialogs, settings | this note |
| **immediate mode** — `ui/immediate/` | HUDs, debug panels, editors inside a game, throwaway tools | [`immediate/immediate.md`](immediate/immediate.md) |
| **direct** — `window/` and `render/` | a program that draws every pixel itself | [`../window/window.md`](../window/window.md) |

The direct path is SDL's: open a window, take its events, draw with `render/`,
`gpu/` or the raw bindings under them. It needs nothing from this directory, and
`ui/` adds no shared library to a binary that does use it — it is Caustic code
over `window/`, `input/`, `render/` and `text/`.

The toolkit and immediate mode share what sits under them — the 2D drawing, the
text, the events — and nothing else. They are two models, not one model with a
switch.

---

## Drawn by us

The toolkit paints every widget itself, as Qt does, rather than wrapping each
platform's own controls, as wxWidgets does. Four reasons, in order of weight:

**Linux has no native widget set.** X11 and Wayland draw nothing; "native" there
means GTK or Qt. Wrapping native controls on Linux therefore means linking GTK —
on this machine `libgtk-4.so.1` is 11806 KB, exports 5476 symbols and carries 44
`DT_NEEDED` entries of its own — which is the opposite of what every other layer
in this repository does.

**One behaviour everywhere.** A drawn widget has the same metrics, focus rules and
keyboard handling on every backend, so a dialog laid out on Linux is laid out on
Windows. Wrapped controls differ in size and behaviour per platform, and a
portable API over them ends up as their intersection.

**Testable like the rest.** A tree renders into a `gpu/software` target with no
display at all and is compared pixel for pixel — the way the rasteriser and the
window layer are tested now. A native control can only be tested through the
platform that draws it.

**It is what `window/` was built to support.** A toolkit that paints needs a
surface and events, and nothing else from the platform.

The costs, stated rather than discovered:

- **It does not look native unless the theme makes it.** So it follows the
  platform's conventions wherever they are observable: dialog button order,
  shortcut modifiers, scroll direction, double-click time, the system font and
  scale, and the light or dark preference — the XDG portal's `color-scheme` on
  Linux, `AppsUseLightTheme` on Windows.
- **Accessibility is ours to provide.** A native control brings its own; a drawn
  one is invisible to a screen reader until the toolkit exposes it. Qt carries
  the same obligation and meets it with AT-SPI on Linux and UI Automation on
  Windows, and so must this — see below.
- **Every widget is ours to get right**, the text entry most of all.

---

## Retained: widgets are objects

Applications are where immediate mode's costs land hardest, which is why the
toolkit is the other model:

- **State lives in the widget.** A text field holds its text, selection and undo
  history; a list holds its scroll offset and selection. Nothing has to be
  recognised across frames by hashing its label.
- **Layout takes two passes.** Measure, then arrange — so centring a row,
  aligning the columns of a form and wrapping text to the width it is given are
  ordinary rather than special cases.
- **Only what changed is redrawn.** An application is idle most of the time. A
  tree knows what is dirty, and on the software backend that is the difference
  between repainting a blinking caret and repainting 1920×1080.
- **The accessibility tree is the widget tree.** It does not have to be recorded
  on the side.

The cost is the one immediate mode was built to avoid: the tree and the
program's data can disagree. The answer is discipline about direction — signals
out, setters in, and models for anything large — rather than a binding
framework.

```cst
// The designed interface; it does not compile today.
let is ui.Window as w     = ui.window(&app, "Settings", 480, 320);
let is ui.Widget as form  = ui.grid(&w, ui.root(&w));
let is ui.Widget as name  = ui.entry(&w, form);
let is ui.Widget as save  = ui.button(&w, form, "Save");
ui.on_clicked(&w, save, on_save, cast(*u8, &doc));
```

---

## Widgets without classes

Caustic has structs, functions and typed function pointers — no classes, no
inheritance, no virtual dispatch. GTK builds an object system on top of C to get
those (GObject); Qt needs a code generator for its signals (moc). Neither is the
shape here.

A widget is **a node plus a kind**:

- the **node** is what every widget has: parent and children, geometry, flags
  (visible, enabled, dirty), style state;
- the **kind** is a table of functions — measure, arrange, paint, event — called
  through typed pointers, the pattern
  [`gpu/vk/bind/loader.cst`](../gpu/vk/bind/loader.cst) already uses for Vulkan
  commands;
- the **data** is a struct the kind owns: the text of an entry, the value and
  range of a slider.

A new kind of widget is a new table and a struct, and a program writes one
exactly as this library does. That is the extension point, and a toolkit without
one cannot grow it later.

**Handles, not pointers.** A program holds a `ui.Widget` — an index and a
generation into its window's node pool — so a destroyed widget's handle fails a
check instead of pointing into reused memory. The pool is bounded and sized when
the window is created; growing it is explicit, the same rule the draw queue and
the event queue follow.

---

## Layout

Two passes, run only over subtrees marked dirty:

1. **measure**, bottom-up: each widget reports minimum, preferred and maximum
   size for the constraint it is offered;
2. **arrange**, top-down: each container hands its children their rectangles.

The containers are **box** (a row or column, with spacing and stretch factors),
**grid** (forms), **stack** (one visible child: tabs, wizards), **overlay**
(children layered, each aligned on its own), **scroll area** and **splitter**.
That set covers what GTK's and Qt's layouts are used for in practice. The last
two are interactive — scrollbars, a draggable handle — and take their input on
the same protocol as any widget; what they look like comes with painting and
the theme.

**Height for width.** Wrapped text is taller the narrower it is, so a widget's
height is measured again at the width it is actually given, and a row or grid
that knows its width shares it out before asking its children how tall they
need to be. Without that, a paragraph in a row is measured on one line.

Rectangles are **relative to the parent**: moving a container — scrolling it,
moving the window — moves everything in it without laying anything out again.

Units are logical pixels. [`window/`](../window/window.md)'s `display.cst`
supplies the scale per monitor, and everything — theme metrics, font sizes,
borders — scales with it; a window moved to a monitor with a different scale is
laid out again.

**Not now:** constraint solving, the Cassowary algorithm Apple's Auto Layout is
built on. Nested boxes and grids reach the same result for nearly every form,
and a solver is hard to debug from a user's report.

---

## Events and signals

**In.** [`input/`](../input/input.md) delivers events — events rather than
sampled state, because a click that begins and ends between two frames must not
be lost. The toolkit routes them:

- **pointer**: hit-test the tree top-down, deliver to the deepest widget, bubble
  to its ancestors until one handles it. A press *captures* the pointer, so a
  drag that leaves the slider stays with the slider.
- **keyboard**: the window's shortcuts and mnemonics are resolved before the
  focused widget sees the key — except that the focused widget is asked first,
  with `SHORTCUT_OVERRIDE`, whether it wants the key itself, which is how an
  entry keeps Ctrl+A while the window has a shortcut for it. Then the context
  menu keys, then the key to the focused widget and up the tree, and only if
  nobody took it do Tab and Shift+Tab walk the focus chain.
- **text**: composed text and the IME's pre-edit string from `input/`'s
  `text.cst`, to the focused entry.

**Out.** A widget emits signals — clicked, changed, activated — and the program
connects a callback: a function pointer and a user-data pointer, called
synchronously. No signals named by string, no generated code, no delivery queued
across threads; a callback that wants to act later posts to the window's queue
itself.

---

## Models for large data

A list of ten thousand rows does not create ten thousand widgets. Lists, trees
and tables take a **model** — functions answering how many rows there are, what
is in a cell, and that something changed — and create widgets only for the rows
on screen, recycling them as it scrolls. That is Qt's model/view and GTK 4's list
models, for the same reason: it is the difference between a file manager and one
that freezes opening `/usr/lib`.

---

## Painting

Through [`render/`](../render/render.md)'s 2D family: `shapes2d` for frames and
fills, `draw2d` for icons and glyph atlases, nested **scissor** for clipping — a
scroll area clips its children — and **layers**, so a popup draws over what it
covers. `ui/` adds no drawing primitives of its own, and never draws into a
`gpu/software` target directly the way the immediate-mode prototype does now: the
toolkit has to run on whichever `gpu/` backend the program opened.

**Damage.** A widget that changes marks its rectangle dirty; the frame repaints
the union of dirty rectangles, clipped to them, and presents only that region
where the window backend allows it.

**Animation.** A widget can be drawn moved from where layout put it and faded
as a whole, without laying anything out again; tweens take those — and colours
and numbers a widget paints from — to a target over the theme's durations and
along its curve, stepped by a frame clock per window that wakes the loop only
while something moves. "Reduce motion" puts every value at its target at once.

---

## Text

All of it from [`text/`](../text/text.md): shaping and layout for labels,
wrapping to the arranged width, caret positions and hit-testing for entries,
selection rectangles across bidirectional runs.

**One text service per application** (`text.cst`). A `text/layout` Layout
carries a shaper and a table of open faces — tens of kilobytes — which a form
of fifty labels should not hold fifty times. The service owns one, lays out
whatever a widget asks for with it, and hands back a **run**: the glyphs —
each with its face named by font, index and size, not by a slot the next
layout may give to another — the lines' extent and the decorations. A run is
what a label keeps, measures by and draws. A widget whose text is edited or
selected — the entry, a selectable label — keeps a Layout of its own, because
the cursor's questions need everything a layout found.

Fonts come from the theme — a family, a size and a weight for each role of
text — through a resolver the program gives the service: a function from
family, weight and style to a `text/layout` Font. An application hands it
chains over the system's index (`text/fonts`), a test hands it the test fonts.

Glyphs are drawn from `text/`'s glyph cache: rasterized once at the size and
subpixel step they are drawn at — the window's scale included — onto
`render/coverage`'s R8 pages, and colour glyphs onto RGBA pages of the
service's own. A page's changed part goes to its texture as soon as a glyph is
drawn there, so a subtree painted into a texture of its own, which is flushed
before the frame is, finds its glyphs there.

The entry is the hardest widget in any toolkit — selection, undo, IME,
clipboard, caret movement through bidi text — and it waits for `text/` to reach
caret and hit-testing rather than being faked with a monospace bitmap font.

---

## Style

A **theme**: colours, metrics (padding, spacing, border widths, corner radii),
fonts, and a variant per state — normal, hover, pressed, focused, disabled,
selected. One struct the window carries, and a widget may override fields of its
own.

**Not CSS.** GTK 3 moved its theming to CSS and took on the cascade, selector
matching and invalidation that CSS engines are built around. Most of what an
application changes is a colour or a spacing, and a struct does that.

Light, dark and high contrast ship, with Adwaita-, Breeze- and Windows 11-like
themes beside them, chosen from the platform's preferences and switchable at run
time. Every theme holds its text to WCAG contrast — 4.5 to 1, 7 in high
contrast — whatever accent the user picked: text, the ring and the selection
are moved towards black or white until they read.

---

## Windows, popups and dialogs

Each top-level window is a `window/` window with its own tree. Menus, the list
of a combo box and tooltips are **popups**: separate windows positioned against
their parent — override-redirect on X11, `xdg_popup` on Wayland — because a popup
has to be able to extend past its parent's edge. `window/` already keeps several
windows on one connection (`window/x11/multi_test.cst`), which is what this
needs.

Dialogs are windows with a modal flag and a result. The file dialog is our own
first; the XDG desktop portal on Linux and the system dialog on Windows come
later, because users expect their own.

Clipboard and drag-and-drop come from `window/`, where the X11 side is already
done.

---

## Accessibility

Not optional for a toolkit meant for applications, and harder here than for a
native one, because nothing is inherited.

The retained tree makes the shape free: each widget reports a **role**, a
**name**, a **state** and its **actions**, and a bridge exposes the tree — AT-SPI
over D-Bus on Linux, UI Automation over COM on Windows. The bridges are a
platform surface comparable to a window backend, so they are not first. The
per-widget role, name and state are, because adding them afterwards means
touching every widget.

So a kind carries a role and an `access` entry from the first widget on
(`access.cst`): asked, it fills in the widget's name and description, its
states — checked, mixed, pressed, expanded, selected, read-only, required,
busy — its value and range where it has one, and the actions it answers to,
each of which a bridge can call through the same entry. Focusable, focused,
enabled and visible come from the tree, not from the kind. A widget with no
text of its own takes its name from the label that says `labelled_by` it.

---

## The first widgets

Each a kind with its data, its setters that say what they changed, its
signals and its role; drawn from the theme's variants and metrics, so a
theme change restyles them all:

- **label** — text in a style of the theme, or spans of styles; wrapping to
  the width it is given (height for width), at most a number of lines with an
  ellipsis, aligned; selectable on request, then with a caret's worth of
  layout and the selection's rectangles, copied with Ctrl+C. A mnemonic
  (`_Save`) underlines its letter while Alt is held and moves focus to what
  the label is for.
- **button** — text, an icon, or both; `clicked` on release inside it, on
  Space or Enter; the default button of a window answers Enter anywhere and
  is drawn in the accent, the cancel button Escape; a **toggle button** keeps
  a pressed state and says `toggled`. A **link** is a button drawn as text in
  the link colour, `activate_link` carrying its address.
- **check box** — on, off or mixed (three-state on request), with its label
  beside it, the whole of it hit; **radio** buttons in a group, one on,
  arrows moving within the group; a **switch** on or off, its knob sliding
  over the theme's duration.
- **slider** — a value in a range, by step and by page, dragged, clicked to
  jump or to page, moved by the arrows, Page Up and Down, Home and End;
  horizontal or vertical, right to left mirrored; marks on request. A
  **spin box** — an entry of a number with buttons and the same keys, the
  value held to range and step; a **progress bar** — a fraction, or a pulse
  going back and forth when there is none.
- **separator**, **image** (an image at its size or scaled to fit, keeping
  its proportions), **group box** (a frame with a title around one child),
  and the **tooltip**: text shown after the hover delay over the widget that
  has it, on the topmost layer, hidden by a press, a key or leaving it.

## What it needs from the layers below

| from | what | state |
|---|---|---|
| `window/` | windows, popups, several windows, clipboard, drag-and-drop, cursors, per-monitor scale | X11 done; Wayland and Win32 designed |
| `input/` | events; text input and IME composition | design note |
| `render/` | `draw2d`, `shapes2d`, scissor, layers | done |
| `text/` | shaping, layout, caret, hit-testing, colour glyphs | done; complex scripts later |

`input/` is still a design note: the router takes keys and text from
`window/`'s events as they are.

## Current state

**The tree** (`tree.cst`, `widget.cst`, tested by `tree_test.cst`): handles
with an era, the tree sized at open and never grown, children kept in order,
recursive destruction children-first with each kind's `destroy` entry, reparent
refusing cycles, inserting before a given sibling and moving among siblings
(`create_before`, `insert`, `move_before`, with `index_of` and `child_at`), hidden and disabled inherited from ancestors, and layout
invalidation that climbs to the root. It carries what the application lends
its widgets: the text service (`set_text_service`, a bare pointer so that the
tree does not bring the text stack into programs that show none), the
program's function for text copied to the clipboard or the primary selection
(`set_clipboard`, `copy`), which widget names which (`set_labelled_by`,
forgotten when the label goes), and the window's animator (`set_animator`).
A widget's event entry may ask for the focus for another
(`tree.request_focus`): the router gives it once the event is delivered,
by keyboard or pointer as the event was.

**The application** (`app.cst`, tested by `app_test`): one per program,
holding what no single window owns — the system's settings, which every
window's router takes, and the timers, one clock for one loop — and a bounded
table of windows named by handles with an era. Each window has its own tree
and its own router, its logical size, its scale and whether the platform says
it is active (a change repaints all of it); `app.frame` lays it out at its size
and collects what to repaint. `app.set_text_service` lends every window, open
and to come, the application's text service; each window's tree has its
animator. It is fed by calls, as the router is: the
platform loop that opens real windows and presents frames sits above it.

**Layout** (`geom.cst`, `layout.cst`, `box.cst`, `grid.cst`, `stack.cst`, each
with its test): margins, alignment, minimum and maximum, right to left,
height for width, a pass that skips what did not change, and box, grid, stack
and overlay. No container has a fixed ceiling on children: they work in a
scratch area that is part of the tree's mapping, sized from its capacity, and a
grid too sparse to fit says so with `LAYOUT_TOO_LARGE` instead of dropping
cells. Edges land on physical pixels: `layout.set_scale` gives the window's
scale, and every rectangle's edges — not its size — are rounded to that grid
relative to a parent already on it, so 100 split three ways at scale 1 is 33,
34 and 33 edge to edge, and at 1.5 is 50 physical pixels each (`snap_test`). The tests were checked against deliberate breakages — each rule
removed in turn — and catch every one.

**Scroll areas** (`scroll.cst`, tested by `scroll_test`): content measured
without a limit along an axis that scrolls and given the view's size along one
that does not; offsets clamped and on physical pixels, moving the content
without a layout pass and emitting `VALUE_CHANGED`; bars per axis NEVER, AUTO or
ALWAYS, overlay or classic — classic ones taking room, and settling which are
needed when one narrows the view for the other; thumbs sized by the share in
view. The wheel scrolls by lines and a touchpad by pixels, Shift turns it
sideways, and an area at its end hands the rest to an outer one; a press on a
thumb drags it, on the track pages; unwanted keys scroll; right to left starts
at the right with the bar on the left. Two things it needed from below arrived
with it: a kind can claim points before its children (the `hit` entry, so a
press on an overlay bar is the bar's), and `router.reveal` asks every ancestor
to bring a rectangle into view — sent when focus moves by keyboard. Damage is
clipped to the ancestors, so scrolling repaints the view, not the content's
whole extent.

**Splitters** (`splitter.cst`, tested by `splitter_test`): any number of panes
in a row or a column with a handle between each two. Each pane's size is kept
(in its layout record's `slot`), so a pane dragged wider stays wider; when the
splitter changes size the difference is shared from the kept sizes by stretch,
equally when nothing stretches. A handle moves only its two neighbours, never
past a minimum or maximum, and a press within `grab` of it takes it — the
splitter claims those points from the panes. It takes focus: the arrows along
its axis move the current handle, Home and End as far as it goes, F8 and
Shift+F8 go round the handles. Right to left puts the first pane on the right
and counts positions from there.

**Events** (`event.cst`, `router.cst`, `timer.cst`, tested by `router_test`,
`focus_test`, `shortcut_test` and `timer_test` with synthetic events): hit-testing
that respects clipping, bubbling, capture by whoever handles the press,
per-widget enter and leave kept as a chain so a destroyed widget does not
confuse it, click counting and the drag threshold from settings, scrolling that
bubbles, context menus by right click, Menu key or Shift+F10, focus with Tab
order, focus ring only after keyboard use, a modal scope, window shortcuts with
the override, mnemonics — what one chooses takes the focus when it can, as by
Tab, then hears `ACTIVATE`; Alt going down or up repaints the window — and
timers with ids that stay dead once cancelled.
A long press — a finger or pen held still for `long_press_ms`, or a mouse when
the settings ask — opens the context menu where the press was: the widget that
captured the press hears `POINTER_CANCEL`, and the release that follows is no
click. The router says when one is due and the application's loop ticks it,
since nothing arrives while a finger is still (`longpress_test`); the touch and
pen events themselves come from `input/`, which says which the pointer is with
`router.set_source`. The
router is fed by plain calls — `pointer_move`, `pointer_button`, `key`, `text` —
which is what `input/` will call.

**Signals and models** (`signal.cst`, `model.cst`, tested by `signal_test` and
`model_test`): callbacks connected per widget and signal, called in connection
order, with what happens mid-emission decided — connected then is not called,
disconnected then is not called, a destroyed widget ends the emission — and
connections of destroyed widgets reclaimed when the table fills. Models name
rows by stable ids the program chooses, answer by role, may be editable, and
tell their observers about inserts, removals, changes and resets.

**Damage** (`damage.cst`, the bookkeeping in `tree.cst`, tested by
`damage_test`): every widget remembers where it was last painted, and after
layout `damage.collect` compares that with where it is — so a widget that moved,
appeared, was hidden or reparented repaints both places without saying so, and
one whose look changed says so with `tree.invalidate_paint` or, from a setter,
`widget.changed(t, w, CHANGED_LAYOUT | CHANGED_PAINT)`. Destroying, disabling
and resizing the window are accounted for. The region is at most
`tree.MAX_DAMAGE` rectangles in window coordinates, clipped to the window:
overlapping and bordering ones merge, and past the limit a rectangle goes into
the one it enlarges least, so it can cover more than changed but never less.
`tree.paint_wanted` says whether a frame has anything to collect at all.

**Painting** (`paint.cst`, `painter.cst`, the `paint` entry, tested by
`paint_test` on a headless software device): each damage rectangle gets the
theme's window background and a walk of the tree — parent before children, a
subtree outside the rectangle not walked at all. Every widget is clipped to its
own rectangle cut by its ancestors' and the region, with one clip on the
canvas's stack at a time, so a tree forty deep clips right and a partial
repaint is pixel for pixel what a full one would be. A paint entry gets the
canvas, the theme, its rectangle, its clip and its state — hover, pressed,
focused, focus visible, focus within, disabled, backdrop, mnemonics shown
while Alt is held — and the router
repaints whatever widget those change for (focus within only for widgets that
ask, with `tree.F_FOCUS_WITHIN_LOOK`). A paint entry may set the painter's
`fg`, the colour what the widget holds draws its text in, and everything under
it is given that colour, as CSS's `color` is inherited: a button's label in the
button's text colour for its state. A widget on a higher layer
(`tree.set_layer`) escapes its ancestors' clips, draws above everything below
it and is hit-tested first: a popup inside the window. The focus ring is drawn
after the subtree, inside the edge, unless the widget drew its own. A clip a
paint entry forgets to pop is popped for it. `app.paint` paints a window with
its own router and active state, in the application's theme.

A widget with `tree.F_CACHE`, given a `paint.Cache`, is painted once into a
texture and put back while nothing inside it changes — moved as a whole, it
repaints the places it left and reached without one paint entry inside it
(`cache_test`, every frame compared with the same tree painted with no cache).
`damage.collect` makes the texture stale when anything inside changes other
than moving along with it. The texture holds premultiplied colour and goes back
with a premultiplied blend, so soft edges match painting directly. A texture
inside another's is painted first and put back into it, `paint.MAX_NEST` deep,
each on a canvas of its own opened when first needed; a change inside the inner
one makes both stale, the inner one moving within the outer only the outer.
Deeper than that, with a layered widget inside, or with no entry left, a widget
is painted directly. `app.set_cache` gives a window its cache.

**Moving and fading** (`tree.set_translation`, `tree.set_opacity`, tested by
`visual_test`): a translation moves a widget and its subtree from where layout
put it — painted, damaged and hit there, its window rectangle there
(`layout.visual_rect`), nothing laid out again. Opacity fades the subtree as one
picture: below 1 a widget goes through a texture of its own, as `F_CACHE` does,
put back with every channel scaled; changing only the opacity puts the same
texture back fainter, painting nothing inside it again. At 0 nothing of it is
drawn and it is still hit. With no cache to go through it is drawn as it is.

**Animation** (`anim.cst`, tested by `anim_test`; `math/curve.cst` for the
curves): tweens on a widget's opacity and translation, and on a colour or a
number the program owns, whose widget is repainted as it changes. Each starts
at the first tick after it is made, from the value as it is then; one made for
something that already has one replaces it from where it got to, so a reversed
fade turns back rather than jumps. A curve by name — the usual polynomial, sine
and exponential ones, CSS's `ease` family and Windows 11's Fluent curves as
cubic Béziers — or the theme's (`style.Motion.ease`: ease-out cubic for
Adwaita, in-out quadratic for Breeze, Fluent's point to point for Windows 11).
Colours move premultiplied, as CSS's do. Repeating, back and forth, for ever;
callbacks after each step and when done, which is never for one cancelled,
replaced or whose widget was destroyed — that one is dropped with nothing
written. Reduced motion, or a duration of 0, puts the value at its target at
once and still calls back at the next tick, so code that hides a widget when
its fade is done works with animations off. Every window has an animator
(`app.animator`): `app.next_due` asks for a frame an interval after the last
while anything runs and nothing otherwise, and `app.tick` steps them.

**The theme** (`style.cst`, tested by `style_test`): tokens — a palette by role,
a type scale, spacing, radii, borders, shadows, durations and an easing curve —
and the variants a widget picks from by its painter state (`style.pick`): controls, the suggested
and destructive actions, fields, rows, indicators and tracks, with hover and
press laid over whichever background applies, disabled fading everything and a
window in the backdrop quieting its text. Colours are sRGB as written
(`math.color.hex`). `style.finish` holds any palette — a preset, the user's
accent, the program's own colours — to the contrast floors; `style_test` checks
every theme against them. `style.Prefs` carries what the platform reports (dark,
high contrast, accent, the interface and monospace fonts, body size, text scale,
reduced motion) and `style.for_prefs` builds the theme from it; high contrast
keeps its own accent. Each tree has a theme (`tree.set_theme`, light when none
is set) and any widget can lay `style.Overrides` on its subtree — colours by
role, corner radius, text scale — with what depends on them derived again
(`tree.set_overrides`; `widget.theme` resolves a widget's). Painting resolves
the same on its way down, so every paint entry gets its widget's theme, and a
cached subtree too. `app.set_theme` and `app.set_prefs` switch every window at
once; the theme lives in the application's mapping, so the trees' pointers to
it survive the `App` being returned by value. Reading the preferences from the
platform is `window/`'s (`window/settings.cst`: GTK's files, XSETTINGS, the
portal over D-Bus); `system.cst` turns what it reads into `style.Prefs` and the
router's timings — double-click time and distance, the drag threshold, the
caret's blink — and `app.set_system` applies both to every window, keeping its
own copy so the theme's font names outlive the caller's record (`system_test`).
The Windows registry comes with the Win32 backend.

**Text** (`text.cst`, tested by `text_test`): one service per application,
on a device or with none for measuring alone, sized once (`text.Limits`: the
glyph pages and their side, the glyphs the caches remember, the faces open
for drawing at once). It lays out with its one `text/layout` Layout into the
run a widget keeps — one block holding the text itself, copied, and how it was
laid out: glyphs with their clusters and advances and their faces named by
font, face and size, lines, decorations, each style's colour, and the least
width the text can be given without a word broken. A run lays out its own text
again, as a theme change asks, and says where an underline of one character
goes, as a mnemonic's is drawn. Fonts come through the
program's resolver, asked once for each family, weight and style and
remembered by name — sixteen of them, the rest asked again each time; a style
of no family takes the theme's. Drawing rasterizes each glyph at the size and
quarter-pixel step it lands on at the canvas's scale, through `text/atlas` onto
`render/coverage`'s pages, colour glyphs — layers and bitmaps — onto RGBA
pages of the service's own, cached under the text's colour as well, since a
layer may be in it; the text's opacity is laid over the whole glyph. Each draw
sends its pages' changed parts before it returns. What finds no room — a full
glyph cache or page, every face slot drawn from in this frame — is said, and
drawn the next frame, when what was not used makes room. The test holds every
pixel to what the rasterizer draws for each glyph alone, at scale 1 and 2, with
the pen between pixels; colour glyphs to their palette's colours exactly.

**Accessibility** (`access.cst`, tested by `access_test`): every kind has a
role — forty-odd, close to AT-SPI's and UI Automation's — and an `access`
entry. `access.describe` asks the entry for the name and description, the
states, the value in its range and the actions the widget answers to, and lays
over them what the tree and the router know: visible, showing, enabled,
focusable, focused — those are theirs alone, whatever a kind says. The root is
the window. `access.act` asks a widget to do one of its actions, with a value
for setting one; one that is gone or disabled, or whose kind does not list the
action, is not asked. A widget with no name of its own takes its label's. The
bridges to the platform come later and read nothing else.

**Labels** (`widgets/label.cst`, tested by `label_test` on a headless software
device): text copied into the label's run and laid out through the text
service in the theme's text style for its role — caption, body, heading,
monospace and the rest — of its part of the window, so a theme change or an
override's text scale sets it again; spans of the program's styles over it,
their empty fields the label's. Measured as wide as its text would like and
as tall as its text at the width offered — rounded up to whole physical pixels,
as GTK rounds Pango's sizes, so a label given its own width on snapped edges
does not wrap its last word; wrapping, no narrower than its
widest word; at most a number of lines with an ellipsis, and then, not
wrapping, cut to its width and no narrower than the ellipsis. Drawn in the
theme's colour for its tone — text, secondary, accent, error, warning,
success, link — faded when disabled, quieter in the backdrop (in its normal
tone, in the colour what holds it gives instead), aligned across
by its alignment in the widget's direction and centred down. Its text is its
name to assistive technology, and the name of the widget it labels. A
mnemonic (`_Save`; `__` for an underscore) underlines its letter while Alt is
held; bound to the router, Alt and the letter focus what it labels.
Selectable on request, it keeps a `text/layout` Layout of its own beside its
run (`text.lay_layout`) for the cursor's questions: dragged across, by words
after a double click and paragraphs after a triple, extended with Shift, and
by keyboard once focused — arrows, words, lines keeping the x they began at,
Home and End — with Ctrl+A and Ctrl+C claimed ahead of the window's
shortcuts; what the pointer or Shift selects goes to the primary selection.
The selection is drawn behind its text in the theme's selection colours.

**Images** (`widgets/image.cst`, tested by `image_test`): a texture of the
program's at the size it says, centred in what the image is given, or scaled
to fit keeping its proportions (shorter when narrower); filtered or the
nearest texel taken, straight or premultiplied. A symbolic image is drawn in
the colour what holds it gives, else the theme's text colour; disabled, an
image fades as controls do unless it was given a colour. Its description is
its name to assistive technology.

**Buttons** (`widgets/button.cst`, tested by `button_test`): a label (with
its mnemonic) or an image inside the theme's padding, at least a control
tall, drawn from the control's variants — the suggested action's, the
destructive one's, or flat with only the overlays — its content in the
variant's text colour for its state. Clicked on a release inside it after a
press, the pointer free to leave and come back (looking at rest while it is
away); on Space, pressed looking while held, clicked on release; on Enter; on
its mnemonic; by assistive technology. A toggle button stays down and says
`TOGGLED`. The window's default button answers Enter wherever the focus is,
unless a focused button takes Enter, and is drawn in the accent; its cancel
button answers Escape; both give their keys back when destroyed. A link is
underlined text in the link colour, no padding, saying `ACTIVATED` with its
address and told visited once followed.

**Check boxes and radio buttons** (`widgets/check.cst`, tested by
`check_test`): one widget for both, as GTK 4 makes it — an indicator beside a
label with its mnemonic, the label past it in the widget's direction, the
whole of it clicked. A check box is on, off, or mixed when the program says or
as the third state a click goes through on request. Radios are joined in a
group, a ring: one chosen, choosing another unchooses it, a chosen one stays
chosen when clicked; only the chosen one takes Tab, so the group is one stop,
and the arrows — mirrored right to left — choose the next or the one before,
passing over what is disabled, and move the focus there; with none chosen
every one takes Tab. Clicked on a release inside after a press, on Space
released, on the mnemonic and by assistive technology; `TOGGLED` from every
radio a choice changes. Drawn from the theme's indicator variants: a rounded
square with a tick, or a bar when mixed; a circle with a dot.

**Switches** (`widgets/switch.cst`, tested by `switch_test`): a track the
theme's size, rounded at its ends and filled when on, with a knob that slides
to its end over the theme's fast duration through the window's animator — at
once with reduced motion or no animator — mirrored right to left. Clicked,
toggled by Space released or Enter as GTK's is, and by assistive technology;
named by the label beside it.

**Sliders** (`widgets/slider.cst`, tested by `slider_test`): a value in a
range held to its step (any value with none), on a track across — filled from
its start, from the right when right to left — or up and down with the least
at the bottom. A click on the track jumps there and the knob follows until
release; a press on the knob drags it as held; Shift and a click move a page
towards the click. The arrows move a step, mirrored right to left, Page Up
and Down a page (ten steps unless set), Home and End to the ends, the wheel a
step a notch or a tenth of one every pixel. Marks at the program's values,
below or beside the track. `VALUE_CHANGED` carries the value; assistive
technology reads the value, range and step and sets, raises and lowers it.

**Progress bars and separators** (`widgets/progress.cst`, `widgets/separator.cst`,
tested by `progress_test`): a bar filled from its start — from the right, right
to left — to a fraction held to 0 .. 1, or, when there is none, a pulse: a
quarter of the bar going back and forth for ever, one way in four of the
theme's slow durations through the window's animator, until a fraction is
set; told its fraction, or busy. A separator is a line the theme's border
thick in its separator colour, across or up and down, in the middle of what
it is given on whole physical pixels.

**Group boxes** (`widgets/group.cst`, tested by `group_test`): a title — a
label in the theme's strong body style, with its mnemonic — on top, then after
the theme's small space a frame, a rounded border in the theme's border colour,
around the content, the theme's medium space in from it; least and preferred
sizes both carried through. The title names the group to assistive
technology, and its mnemonic gives the focus to the first thing inside that
can take it.

**Tooltips** (`widgets/tooltip.cst`, tested by `tooltip_test`): one set per
window beside its tree and router, each widget's tip the program's text, told
the time by the program's loop and saying when it next wants to be (as the
animator does). The pointer resting on a widget with a tip — or inside one,
the nearest ancestor's — for the router's `tooltip_ms` (500 by default)
shows a bubble on a layer above everything, just below the pointer and kept
inside the window, in the theme's tooltip colours, its text in the caption
style wrapped to twenty-five of the body's ems; a tooltip to assistive
technology, named by its text. Leaving the widget, a press or a key (the
router counts them) hides it, and it does not show again until the pointer
leaves and comes back.

## For scale

Measured on this machine, as a sense of what the references weigh rather than a
target:

| | size | exported symbols |
|---|---|---|
| `libgtk-4.so.1` | 11806 KB | 5476 |
| `libQt6Widgets` + `libQt6Gui` + `libQt6Core` | 24049 KB | 25536 |

Both carry decades of widgets, platforms and compatibility. The goal is the
widget set applications actually use, done properly, not parity.

---

## Order of work

1. **The tree, the widget table, handles and layout** — testable with geometry
   alone, before anything is drawn.
2. **Event routing**: capture, bubbling, the focus chain — testable with
   synthetic events.
3. **Painting and damage**, once `render/draw2d` exists.
4. **The first widgets** — label, button, check, radio, slider, progress — with
   box, grid and stack, and the light and dark themes.
5. **The entry**, once `text/` has caret and hit-testing and `input/` delivers
   composed text.
6. **Scroll areas, models with list, tree and table, menus and popups, dialogs.**
7. **The accessibility bridges**, AT-SPI first.

## Not now, and deliberately

- **Wrapping native controls.** Decided above.
- **Markup** — GtkBuilder XML, Qt's `.ui` files, QML. A tree built in code is the
  API; a format can be layered on it later without changing it.
- **CSS.** Decided above.
- **Docking** and multi-document layouts.
- **Rich text editing.** The entry is plain text with selection.
