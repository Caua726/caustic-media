# The UI layer

Interfaces for applications: what GTK is for C and Qt is for C++. Widgets that
exist as objects, laid out by the toolkit, and drawn by this library itself on
every platform — so a program written against it looks and behaves the same on
X11, Wayland and Windows, and links no toolkit of anybody else's.

```
ui/
  ui.cst          hub: every module through one use, words without lengths
  app.cst         the application: its windows, each with its own tree
  host.cst        the application on the screen, the loop; in parts — host_state,
                  _windows, _settings, _platform, _paint
  tree.cst        widgets as nodes in a window's tree, and the handles to them;
                  its types in tree_state.cst, what frames repaint in tree_draw.cst,
                  what it is lent in tree_lent.cst
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
                  focus, shortcuts, mnemonics, the modal scope; in parts —
                  router_state, _hit, _pointer, _focus, _keys, _drag
  timer.cst       things that happen later, on an explicit clock
  signal.cst      connecting a callback to what a widget emits
  model.cst       row and cell models behind lists, trees and tables
  damage.cst      what a frame has to repaint, found from the tree
  paint.cst       drawing the tree through render/'s 2D family
  painter.cst     what a widget's paint entry is handed
  style.cst       the theme: colours, metrics, fonts, per-state variants
  system.cst      the desktop's settings, as the theme and the timings
  text.cst        one text service: runs laid out and drawn
  textbuf.cst     the text an entry or a text view edits; undo in steps
  access.cst      role, name, states, value and actions per widget
  widgets/        label, button, check, radio, entry, textview, menu, slider, progress,
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

- **It does not look native, and does not try to.** The toolkit has one look
  of its own on every platform — its theme and its window frames — and
  follows the platform only in behaviour wherever that is observable: dialog
  button order, shortcut modifiers, scroll direction, double-click time, the
  system font and scale, and the light or dark preference — the XDG portal's
  `color-scheme` on Linux, `AppsUseLightTheme` on Windows.
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
use "caustic-media/ui/ui.cst" as ui;

// t is the window's tree: ui.app.tree(&a, win).
let is i64 as form = ui.grid.create(t, ui.tree.root(t));
let is i64 as name = ui.entry.create(t, form);
ui.label_for(t, form, "_Name", name);          // Alt+N goes to the entry
let is i64 as save = ui.button(t, form, "_Save");
ui.on(t, save, ui.signal.CLICKED, fn_ptr(on_save));
```

**One use.** `ui.cst` is the hub: every module of the toolkit is reached
through it by its own name — `ui.tree.Tree`, `ui.box.create`,
`ui.entry.text` — so a program has one `use` instead of one per module, and
the binary is no larger for it: what the program does not call is not in it
(the settings form is the same size either way, to within a kilobyte).
Beside the modules it has the forms a program writes with words in them —
`ui.label`, `ui.button`, `ui.check`, `ui.radio`, `ui.group`, `ui.menu_item`,
`ui.tab`, `ui.message`, `ui.set_text` and the rest — taking a zero-terminated
string where the module's form takes the bytes and their length. The
module's form stays the one for text that is not a literal: a slice of a
buffer, what an entry holds. Tested by `ui_test`.

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

**Handles, not pointers.** A program holds a widget as an `i64` — an index and a
generation into its window's node pool — so a destroyed widget's handle fails a
check instead of pointing into reused memory. The pool is bounded and sized when
the window is created; growing it is explicit, the same rule the draw queue and
the event queue follow.

The handle is one integer for every kind, not a struct per kind: Caustic
accepts one struct where another of the same size is asked for, so a
`Button` and a `Slider` handle would not be told apart at compile time, and
they would cost a cast at every call into the tree. The check is made at run
time instead. Every kind's functions start from `tree.data_of`, which answers
null for a stale handle and, for a live widget of another kind, null with the
tree's last error `WRONG_KIND` — so `slider.value` asked of a check box says
what went wrong rather than answering 0 in silence.

---

## Layout

Two passes, run only over subtrees marked dirty:

1. **measure**, bottom-up: each widget reports minimum, preferred and maximum
   size for the constraint it is offered;
2. **arrange**, top-down: each container hands its children their rectangles.

The containers are **box** (a row or column, with spacing and stretch factors),
**grid** (forms), **stack** (one visible child: tabs, wizards), **overlay**
(children layered, each aligned on its own), **scroll area** and **splitter**.
That set covers what an application's layouts need in practice. The last
two are interactive — scrollbars, a draggable handle — and take their input on
the same protocol as any widget; what they look like comes with painting and
the theme.

**Height for width.** Wrapped text is taller the narrower it is, so a widget's
height is measured again at the width it is actually given, and a row or grid
that knows its width shares it out before asking its children how tall they
need to be. Without that, a paragraph in a row is measured on one line. Nor
is a widget asked about more room than its maximum size lets it take: what
holds a paragraph capped narrower than the space would otherwise count too few
of its lines.

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
  entry keeps Ctrl+A while the window has a shortcut for it; the question
  bubbles, so what holds the focused widget may keep a key for it, as a spin
  box keeps Enter for its entry while something is typed. Then the context
  menu keys, then the key to the focused widget and up the tree, and only if
  nobody took it do Tab and Shift+Tab walk the focus chain.
- **text**: composed text and the IME's pre-edit string from `input/`'s
  `text.cst`, to the focused entry.
- **modifiers**: held as the pointer's events say them, which are right at
  their moment (`router.set_modifiers`); a key's event says the state before
  it, so the router counts a modifier key's own press and release in. Ctrl
  held over a drag makes it a copy, in whichever of the program's windows it
  is let go.

**Out.** A widget emits signals — clicked, changed, activated — and the program
connects a callback: a function pointer and a user-data pointer, called
synchronously — `signal.on` for the common case of no user pointer. No signals named by string, no generated code, no delivery queued
across threads; a callback that wants to act later posts to the window's queue
itself.

---

## Models for large data

A list of ten thousand rows does not create ten thousand widgets. Lists, trees
and tables take a **model** — functions answering how many rows there are, what
is in a cell, and that something changed — and create widgets only for the rows
on screen, recycling them as it scrolls: it is the difference between a file
manager and one that freezes opening `/usr/lib`.

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
clipboard, caret movement through bidi text — and it asks all of it of `text/`
— the caret, hit-testing, graphemes and words, selection rectangles — rather
than faking it with a monospace bitmap font.

---

## Style

A **theme**: colours, metrics (padding, spacing, border widths, corner radii),
fonts, and a variant per state — normal, hover, pressed, focused, disabled,
selected. One struct the window carries, and a widget may override fields of its
own.

**Not CSS.** A stylesheet brings the cascade, selector matching and the
invalidation CSS engines are built around. Most of what an application changes
is a colour or a spacing, and a struct does that.

One theme of the toolkit's own, caustic — cool slate greys, a teal accent, small
radii, the same on every platform — ships light, dark and high contrast, chosen
from the platform's preferences and switchable at run time. It imitates no
platform's look. Every theme holds its text to WCAG contrast — 4.5 to 1, 7 in high
contrast — whatever accent the user picked: text, the ring and the selection
are moved towards black or white until they read.

---

## Windows, popups and dialogs

Each top-level window is a `window/` window with its own tree. Menus, the list
of a combo box, popovers and tooltips are **popups**: separate windows
positioned against their parent — override-redirect on X11, `xdg_popup` on
Wayland, a `WS_POPUP` tool window on Windows — because a popup has to be able
to extend past its parent's edge. As GTK 4 does it, a popup stays a widget of
its window's tree, on a layer, with the tree's handles, router and signals:
only its pixels and its input go through the platform's popup (a
**surface**, `surface.cst`), placed by the platform from the same positioner
the tree speaks everywhere (`xdg_positioner`'s model), its damage a region of
its own and its input reaching the window's router in the window's
coordinates. Where there is no host, or the platform refuses one, it is drawn
inside the window on its layer instead, placed by that positioner within the
window. `window/` keeps several windows on one connection
(`window/x11/multi_test.cst`), which is what this needs.

Dialogs are windows with a modal flag and a result, belonging to the window
they are for or, for the whole application, to none; modal, what they belong
to waits — every other window, for the application's own — and a press on a
waiting window brings the dialog forward with the bell, as GTK and Qt do on
Linux and Windows does itself for the windows it disabled. The file dialog is our own
first; the XDG desktop portal on Linux (`window/linux/filechooser.cst`) and the
system dialog on Windows give users their own.

Clipboard and drag-and-drop come from `window/` (`transfer.cst`), on X11,
Wayland and Windows alike.

**The application on the screen** (`host.cst`). `app.cst` keeps the program's
windows and is fed by calls; the host is what feeds it. For every window of
the application that is shown it opens one of the platform's — a dialog's
belonging to its owner's, modal when it is — and puts it away when the
application's is hidden or closed, titling it as it is titled. What the
platform says becomes what the application and the window's router are told:
the pointer, buttons and wheel, keys and the text they typed, the keyboard
focus, the size, the place on the screen, the window manager's state, a drag
from another program, the close button — which goes to the window's widgets
first (`router.close_request`: a dialog answers it as Escape), then to the
program (`set_on_close`), and only then closes the window. A window waiting
on a modal dialog (`app.blocked`) is told nothing of the pointer or the keys;
a press, a key or the wheel there brings the dialog forward (`app.blocker`)
and rings the bell, and what was open over the window — a menu, a list, a
popover, a tooltip — closes as the dialog comes.
A drag begun in one window and carried over another of the program's is run
by that window's router, from what the first carries, and the first is told
what was done there (`router.drag_payload`, `drag_elsewhere`, `drag_finish`).
Each event is given inside `app.begin_dispatch`/`end_dispatch`, as the
timers are inside `app.tick`: a window closed by what it sets off — a
shortcut that quits, a timer's callback — goes on until that is over, its
tree and router still in use until then, and closes after. `present` brings
a window forward, as a later start of the program asks (`instance.cst`),
with the token that start came with (`instance.token`): an xdg-activation
token on Wayland, a startup ID on X11 (its feedback ended, its click's time
given to the window manager).
The clipboard and the primary selection are the platform's through the
trees' `set_clipboard` and `set_paste`: copied text goes out as an offer,
a paste asks for text and is answered when it comes, however large; a drop
from another program is answered with what the widget under it takes and
how. The pointer's shape is the one the widget under it asks for; the scale
is the screen's DPI, or the desktop's when its settings say one.

Each window is painted into a texture of its own on the program's device —
what changed, as the damage says — and shown from it by the parts that
changed: the painted damage and a scroll's moved pixels (where they were and
where they went), in physical pixels, are kept until a frame is shown —
through a buffer the compositor still holds, a resize just read, an expose's
lost area — and the platform's buffer gets what its age says it lacks
(`window/damage.cst`), all of the texture when that is not known; X11 then
puts, Wayland damages and Windows blits only those rectangles. Between rounds
the loop sleeps
(`window/wait.cst`) until a window has an event, a descriptor the program
watches is readable (`watch`: the accessibility bridge's — `atspi/hosted.cst`
— a portal's), another thread posts something to run (`post`), or a timer is
due; an idle program takes no time. An animation steps at the frame clock
(`app.next_due`), not each time the platform wakes the loop — a frame shown
is itself an event on X11. `step` is one round, `run` rounds until no window
is left on the screen — or, while the program holds the loop (`hold`, as one
living in the tray does with its window closed), until it lets go.

**A program started** (`program.cst`): what every program does before its
first window, in one call — the software device, the system's fonts for its
text (`system_fonts.cst`: a chain over the system's font index for each
family, weight and style the theme and its widgets ask, opened once and
kept), the application, the host, the desktop's settings followed and a
session connection for what it says to the desktop. The program opens its
windows on `program.app` and runs.

## The examples

Three programs a user would recognise, each built only from the toolkit's
pieces and each with a `--check` that drives it as a user would, under Xvfb
(`test-x11`): its keys, clicks and wheel sent through the X server to its own
window (`examples/user_input.cst`: XSendEvent from a connection of its own on X, SendInput on Windows),
so they are read the whole way — X's keycodes and modifier bits,
`window/device.cst`, the host, the router — as a user's are; its dialogs
answered directly. Events handed to the host already in the toolkit's terms
skip the platform's half: that is how Control came through X as Caps Lock,
unseen until a user pressed Ctrl+A.

- **A settings form** (`examples/settings_form.cst`): labelled fields on a
  grid with mnemonics — entries, radio buttons, a switch, a slider, check
  boxes, a spin box, a combo box — kept in a file between runs; Apply and
  Revert enabled only when something changed; the look and the language of
  the toolkit's own words applied as they are chosen; a message box for an
  email that does not hold, a question before closing over changes, a
  notification when saved.
- **A text editor** (`examples/text_editor.cst`): one instance (started
  again with a file, it hands it over), a menu bar with accelerators shown
  and shortcuts bound, Edit's items enabled and done through the text view
  (`textview.can`, `act` — an entry has the same), the text in a scroll
  area, the file chooser to open and save as, the title saying which file
  and whether it changed, the caret's place in the status bar, a question
  before closing over changes, the about dialog, the window's size kept.
- **A file browser** (`examples/file_browser.cst`): folders as a tree model
  of its own, branches read when opened; the folder chosen as a table over
  `dirmodel`; a toolbar and a path that can be typed; menus with a check
  item and the window's shortcuts; a message box for a folder that cannot
  be read; files opened by the desktop's program for them.

## The application and the desktop

What a program says to the desktop besides its windows goes, on Linux, over
the session bus: one connection of the program's, dispatched by its loop
whenever it is readable (`session.cst`), shared by the pieces below. Each is
apart from the host, so a program takes only what it uses, and each is
answered by every Linux desktop through a freedesktop interface. Windows'
counterparts — the registry's settings, `Shell_NotifyIconW`, toasts,
`ShellExecuteW` — come with its backend.

- **Icons.** The toolkit's own (`icons.cst`): symbolic, one colour — that of
  what they are drawn in, as text is — each a path on a 16 by 16 grid in a
  small language of SVG's absolute commands plus whole circles, filled even-odd
  by the text service's rasterizer, so crisp at any scale: message boxes'
  signs, arrows, places, a check, a search; `image` shows
  one. Turned into pixels (`rasterize`), one is a window's icon
  (`app.set_icon`, `_NET_WM_ICON` on X11). The desktop's icon theme
  (`icon_theme.cst`) finds a named icon as the freedesktop Icon Theme
  Specification looks it up — the theme the settings say
  (`settings.icon_theme`), its sizes and scales, what it inherits, hicolor —
  PNG only.
- **The desktop's settings** (`desktop.cst`): the settings portal, which
  Wayland has no other way to say, read at first and again when it says one
  changed, laid over what the window system says (`host.set_settings`).
- **One instance** (`instance.cst`): the first to own the application's id
  on the bus is the application; a later start hands it what it was asked —
  come forward, open these files, do this action — through
  `org.freedesktop.Application`, and leaves, its `XDG_ACTIVATION_TOKEN` and
  `DESKTOP_STARTUP_ID` handed over in `platform_data` (`activation-token`,
  `desktop-startup-id`) and taken from its environment; the instance keeps
  what each call came with, from such a start or a desktop's launcher
  (`instance_test` plays one with gdbus).
- **Notifications** (`notify.cst`): `org.freedesktop.Notifications` — a
  summary, a body, an icon of the theme, actions; the program told which
  action was chosen and when one closed.
- **The tray** (`tray.cst`): a StatusNotifierItem — its own bus name, the
  item's id, title, status, an icon of the theme or its own pixels, a tooltip
  — registered with the watcher whenever one is there, never started for it.
  Clicks and the wheel are the program's, its menu too: the item exports no
  menu over D-Bus.
- **Links** (`open_uri.cst`): opened by the desktop portal — `OpenURI`, or
  `OpenFile` handed the file itself for one of this machine — without waiting
  on it; by `xdg-open` where there is no portal, the link one argument that no
  shell reads as words.

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

## Words and languages

A toolkit says things of its own — a dialog's Cancel, a text's context menu,
the places of a file chooser, the names a screen reader reads out — and says
them in the user's language. They are one table (`strings.cst`): every word in
every language it has (English and Brazilian Portuguese), each holding what
the English holds — the same mnemonic underscore, the same places to fill in,
`%1` and `%2`, in the order the language puts them ("Light %1", "%1 claro").
A language it does not have reads English. There is no catalogue loaded at
run time: the words are code, checked by `strings_test` for being all there.

The language is the tree's (`tree.set_language`), so a widget asks for its
words with nothing but the tree it is in; the application sets it on every
window's tree from the user's locale (`app.set_locale`, or the desktop's
settings through `app.set_system`) — before the widgets are made, since words
already shown stay as they were. The user's locale is a BCP 47 tag
(`window/locale.cst`), read on Linux as POSIX has it: `LANGUAGE`'s first that
is a name, unless the locale is C; then `LC_ALL`, `LC_MESSAGES`, `LANG`. A
language written right to left (Arabic, Hebrew, Persian, Urdu…) turns the
windows' layout direction to right to left, which mirrors every container.

Numbers take the language's decimal mark ("1,5 kB", a spin box's "2,5"); a
spin box reads either. Text is UTF-8 throughout, and what is cut to fit a
buffer is cut where a character starts (`strings.fits`). The folders a file
chooser offers are where `user-dirs.dirs` puts them — "Área de trabalho" on a
desktop in Portuguese (`window/linux/user_dirs.cst`). A font's own style names
are shown as the font gives them; the ones the toolkit makes up for a variable
font's weights are in the language.

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
| `window/` | windows, popups, several windows, clipboard, drag-and-drop, cursors, per-monitor scale | X11 implemented; native Wayland tested with Weston, desktop limits documented; Win32 under isolated Wine: a program starts, draws, takes input and closes (`program_win32_test`), popups (`popup_win32_test`); DnD and desktop services not yet |
| `input/` | events; text input and IME composition | design note |
| `render/` | `draw2d`, `shapes2d`, scissor, layers | done |
| `text/` | shaping, layout, caret, hit-testing, colour glyphs | done; complex scripts later |

`input/` is still a design note: the router takes keys and text from
`window/`'s events as they are.

The toolkit takes `window/` through its portable modules only — `device`,
`display`, `cursor`, `folders`, `open`, `instance`, `input_extras` and the
rest — and imports nothing under `window/x11/`, `window/wayland/` or
`window/win32/`; `window/linux/`'s portals and folders it uses as they are
(see [`window.md`](../window/window.md), *The tree*).

## Current state

In short; each part in full, with its tests, is in [`state.md`](state.md),
and every module's functions in [`api.md`](api.md) (written from the
sources by `ui/tools/api.sh`, checked against them by the test script).

| part | modules | state |
|---|---|---|
| the tree and layout | `tree` `widget` `geom` `layout` `box` `grid` `stack` `scroll` `splitter` | built, tested: handles with an era, height for width, edges on physical pixels |
| events | `event` `router` `timer` `dnd` | built, tested: capture, bubbling, focus, shortcuts, mnemonics, the modal scope, drags |
| signals and models | `signal` `model` `flatten` `sortmodel` `dirmodel` | built, tested |
| drawing | `damage` `paint` `painter` `anim` `style` `system` | built, tested pixel for pixel on the software device; light and dark themes |
| text | `text` `textbuf` | built, tested: one service per application, undo in steps |
| widgets | `widgets/` — label, image, button, check, radio, switch, slider, progress, spin, separator, group, expander, tooltip, entry, text view, menu, menu bar, menu button, tabs, toolbar, status bar, title bar, frame, assistant, list, tree, table, combo, dialog, file, colour and font choosers, about | built, each with its test |
| accessibility | `access` `atspi/` | AT-SPI built, tested on a private bus; UI Automation not yet |
| words | `strings` | English and Brazilian Portuguese; right-to-left mirrored |
| the application | `app` `host` `program` `session` `desktop` `instance` `notify` `tray` `open_uri` `winstate` | X11 and Linux's desktops done; the Wayland host checked on Weston; Windows under Wine — see below |
| one `use` | `ui` | the hub, and the forms that take words without their length |
| examples | `examples/settings_form` `text_editor` `file_browser` | each `--check`ed on X11, on Weston and under Wine |
| immediate mode | `immediate/` | prototype, tested |

By platform: X11 is the reference — everything above runs and is tested
under Xvfb. Wayland: the host on a seated Weston with text-input-v3; external
clipboard, drag and drop and an installed input method unverified. Windows,
under isolated Wine: a program starts, draws, takes the system's input and
closes; its file dialog, known folders, one instance and links are native,
and its menus, lists, popovers and tooltips are popups of the system's; OLE
drag and drop, UI Automation and notifications not yet — and Wine is not
proof of real Windows.

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
   alone, before anything is drawn. *Done.*
2. **Event routing**: capture, bubbling, the focus chain — testable with
   synthetic events. *Done.*
3. **Painting and damage**, once `render/draw2d` exists. *Done.*
4. **The first widgets** — label, button, check, radio, slider, progress — with
   box, grid and stack, and the light and dark themes. *Done, with the text
   service, the switch, the spin box, the image, links, separators, group
   boxes and tooltips, each told to assistive technology.*
5. **The entry**, once `text/` has caret and hit-testing and `input/` delivers
   composed text. *Done, with the text view.*
6. **Scroll areas, models with list, tree and table, menus and popups, dialogs.**
   *Scroll areas, tabs, splitters, stacks and assistants, expanders, toolbars,
   status bars, title bars, menus and menu bars, lists, trees, tables, combo
   boxes, popovers and menu buttons done; dialogs, message boxes, our own
   file chooser and the desktop's through the portal, colour and font
   choosers, about dialogs and windows' state kept done.*
7. **The accessibility bridges**, AT-SPI first. *AT-SPI done
   ([`atspi/atspi.md`](atspi/atspi.md)); UI Automation comes with the Win32
   backend.*
8. **Words in the user's language**. *Done: `strings.cst` in English and
   Brazilian Portuguese, the locale from the environment, right-to-left
   languages mirrored. Windows locale is read by the native backend; its
   toolkit integration remains blocked by the compiler.*
9. **The application level**: the loop that puts the application's windows
   on the screen, the desktop's settings followed, icons, one instance,
   notifications, the tray, links. *Done on X11 and Linux's desktops. The
   Wayland host has a Weston surface/timer smoke and text-input-v3 routing,
   its popups `xdg_popup`s, and the three examples are checked on a nested
   Weston.
   The Windows toolkit integration is blocked; see `window/win32/win32.md`.*
10. **Examples**: a settings form, a text editor, a file browser. *Done,
   each `--check`ed on X11, on a nested Weston and under Wine.*
11. **A frame of the program's own** moved, resized, maximized and told
   where the window really is through the loop. *Done on X11; on Wayland —
   `xdg-decoration` asked first, the frame drawn where the compositor leaves
   it to the client — and on Windows (`WM_NCCALCSIZE`, `WM_NCHITTEST`) with
   their backends.* Then the Wayland and Win32 backends.

## Not now, and deliberately

- **Wrapping native controls.** Decided above.
- **Markup** — GtkBuilder XML, Qt's `.ui` files, QML. A tree built in code is the
  API; a format can be layered on it later without changing it.
- **CSS.** Decided above.
- **Docking** and multi-document layouts.
- **Rich text editing.** The entry is plain text with selection.
