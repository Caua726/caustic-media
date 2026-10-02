# The UI layer

Interfaces for applications: what GTK is for C and Qt is for C++. Widgets that
exist as objects, laid out by the toolkit, and drawn by this library itself on
every platform — so a program written against it looks and behaves the same on
X11, Wayland and Windows, and links no toolkit of anybody else's.

```
ui/
  ui.cst          hub
  tree.cst        widgets as nodes in a window's tree, and the handles to them
  widget.cst      the table of functions every kind of widget fills in
  layout.cst      measure and arrange: box, grid, stack, size policies
  event.cst       routing: hit-testing, capture, bubbling, focus, shortcuts
  signal.cst      connecting a callback to what a widget emits
  model.cst       row and cell models behind lists, trees and tables
  paint.cst       damage tracking, drawing through render/'s 2D family
  style.cst       the theme: colours, metrics, fonts, per-state variants
  a11y.cst        role, name and state per widget; the platform bridges
  widgets/        label, button, check, radio, entry, slider, progress,
                  scroll, list, tree, table, menu, tabs, splitter, dialog

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
**grid** (forms), **stack** (one visible child: tabs, wizards), **scroll area**
and **splitter**. That set covers what GTK's and Qt's layouts are used for in
practice.

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
- **keyboard**: to the focused widget, then up the tree. Tab and Shift+Tab walk
  the focus chain; shortcuts and mnemonics are resolved at the window before the
  focused widget sees the key.
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

---

## Text

All of it from [`text/`](../text/text.md): shaping and layout for labels,
wrapping to the arranged width, caret positions and hit-testing for entries,
selection rectangles across bidirectional runs.

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

Two themes ship, light and dark, chosen from the platform's preference and
switchable at run time.

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

---

## What it needs from the layers below

| from | what | state |
|---|---|---|
| `window/` | windows, popups, several windows, clipboard, drag-and-drop, cursors, per-monitor scale | X11 done; Wayland and Win32 designed |
| `input/` | events; text input and IME composition | design note |
| `render/` | `draw2d`, `shapes2d`, scissor, layers | the 3D path is started; the 2D family is not |
| `text/` | shaping, layout, caret, hit-testing | design notes only |

Three of the four foundations do not exist yet, which is why the toolkit has no
code.

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
