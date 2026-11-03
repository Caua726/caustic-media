# The UI layer: what is built

What each part of the toolkit does today and what tests it — the design is
[`ui.md`](ui.md), the functions [`api.md`](api.md). Kept beside the design
rather than in it, so that the one says what the toolkit is meant to be and
the other what it already is, each in its own file.

**The tree** (`tree.cst`, `widget.cst`, tested by `tree_test.cst`): handles
with an era, the tree sized at open and never grown, children kept in order,
recursive destruction children-first with each kind's `destroy` entry — which
may destroy other widgets in turn, a text view its menu, even one the walk would
have reached next — reparent
refusing cycles, inserting before a given sibling and moving among siblings
(`create_before`, `insert`, `move_before`, with `index_of` and `child_at`), hidden and disabled inherited from ancestors, and layout
invalidation that climbs to the root. It carries what the application lends
its widgets: the text service (`set_text_service`, a bare pointer so that the
tree does not bring the text stack into programs that show none), the
program's function for text copied to the clipboard or the primary selection
(`set_clipboard`, `copy`), which widget names which (`set_labelled_by`,
forgotten when the label goes), the window's animator (`set_animator`) and the
application's timers (`set_timers`), for a widget that acts after a pause, and
the window's router (`set_router`), for a widget that opens a popup of its own.
The four are bare pointers in the tree, which must not import the modules
that import it; each module hands its own out typed — `router.of(t)`,
`timer.of(t)`, `anim.of(t)`, `text.of(t)` — and lends it with `lend`, so the
cast is made in one place rather than at every call.
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
animator; `app.pointer_shape` says what the pointer looks like in a window,
for the loop to hand the platform when it changes. A window can belong to
another — a dialog's (`dialog_open`) — and be modal: shown, the windows it
belongs to, up its owners, wait (`blocked`); one belonging to no window is the
application's own, and shown modal every other window waits, but for those
that belong to it and another of the application's own; `blocker` says which
modal a press on a waiting window is to bring forward — the one that waits on
nothing itself. It is fed by calls, as the router is: the platform loop that
opens real windows and presents frames sits above it.

**Layout** (`geom.cst`, `layout.cst`, `box.cst`, `grid.cst`, `stack.cst`, each
with its test): margins, alignment, minimum and maximum, right to left,
height for width, a pass that skips what did not change, and box, grid, stack
— as large as its largest page, what is inside a page it does not show counted
too: only a widget's own hidden flag makes it measure as nothing — and overlay
— whose size leaves out what is on a layer, so an open popup does
not make the window fit it. No container has a fixed ceiling on children: they work in a
scratch area that is part of the tree's mapping, sized from its capacity, and a
grid too sparse to fit says so with `LAYOUT_TOO_LARGE` instead of dropping
cells. The smallest window the content fits in (`layout.min_size`, the
root's minimum) is the window's floor: the host tells the platform, again
whenever it changes (`host_x11_test`). Edges land on physical pixels: `layout.set_scale` gives the window's
scale, and every rectangle's edges — not its size — are rounded to that grid
relative to a parent already on it, so 100 split three ways at scale 1 is 33,
34 and 33 edge to edge, and at 1.5 is 50 physical pixels each (`snap_test`). The tests were checked against deliberate breakages — each rule
removed in turn — and catch every one.

**Popups' places** (`../window/positioner.cst`, tested by `positioner_test`): Wayland's
xdg_positioner, the most constrained of the three platforms' models and so
the one the toolkit speaks everywhere — a popup is put against an anchor
rectangle, never at a point of the screen: a point of the rectangle, the way
the popup grows from it, an offset, and on an axis where that leaves the
bounds the adjustments allowed, in the spec's order — flipped when the other
side fits, slid back inside, cut to fit unless that leaves nothing. A popup
just touching an edge fits.

**Surfaces** (`tree_surface.cst`, `surface.cst`, tested by
`tree_surface_test`, `surface_test`, `surface_damage_test`,
`surface_paint_test` and `surface_router_test`): a widget on a layer put
against a rectangle — a menu, a list dropping down, a popover, a tooltip —
shown in a platform window of its own where the host lends the tree its
windows (`tree.set_surfaces`), at most `tree.MAX_SURFACES` at once, and inside
the window on its layer where there is no host or it could not open one, as
before. Either way it stays a widget of the tree, its handles, router and
signals its own. Layout puts it where the platform put it
(`tree.surface_rect`, relative to what it was opened against: the window, or
the menu a submenu belongs to); what it is anchored to — a widget, and a
rectangle in that widget's coordinates — is remembered, and `surface.follow`
places it again when that moves or its own size changes (a list under an
entry that grows, a popover over a button a resize moved). Its damage is a
region of its own, in window coordinates and clipped to it, and none of it
the window's even where it lies over the window; a scroll area inside it
moves its pixels (`tree.shift_surface`), and one in the window under it the
window's, which a popup drawn inside the window would have refused; it holds
`tree.MAX_DAMAGE` rectangles at most, as the window's does.
`paint.frame_surface` paints it on its own canvas from its origin, pixel for
pixel as it is painted inside the window, at any scale — a cached widget
and a faded one inside it too; the window is painted without it; what it leaves unpainted is nothing where its window is seen
through (`tree.surface_see_through`) and the window's colour where not. The
router takes its input in window coordinates — a point past the window's
edges is the popup's when it lies in one — moving between it and the window
leaves the one and enters the other, the window system taking it down is a
press outside at no point (`router.dismiss`), and a widget that takes no
input (`tree.F_NO_INPUT`, a tooltip) lets the pointer through to what lies
beneath, as its window's input region does on the screen
(`tree.surface_set_input`).

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

Drawn and in time (tested by `scrolling_test`): its bars over the content,
through a kind's `paint_over` entry — drawn after the children, in the same
clip. Classic bars always, a track and a thumb, darker under the pointer and
darker still held; overlay bars only while something happens — the content
scrolled, the pointer moving over the area — a thin line along the far edge,
the whole bar with its track under the pointer, faded `HIDE_MS` after the last
thing unless the pointer rests on one. A notch of the wheel glides there along
an ease-out, notches in a row heading on from where the last was going, and an
area at its end still hands the rest on; a touchpad's scroll moves at once and,
when the fingers lift (`router.scroll_end`, from Wayland's `axis_stop`), flies
on with the speed of its last few scrolls, slowing as e^(-t/325 ms) until two
hundredths of a pixel a millisecond, stopped by another scroll, a press or the
end. Less motion asked for, it does neither. A widget holding a drag calls
`follow` as the pointer moves: each area holding it, the pointer past its view,
steps towards it every `FOLLOW_MS` — half the distance past, 2 to 40 pixels —
and the widget hears the move again where the pointer now is
(`router.repeat_move`), so a selection or a drop place goes on with it; the
text view does. `scroll_to` puts a widget inside at the view's start, middle or
end, or just into view. What it needs in time — the glide, the fling, the
following — sits apart from its widget data, made the first time.

**Splitters** (`splitter.cst`, tested by `splitter_test`): any number of panes
in a row or a column with a handle between each two. Each pane's size is kept
(in its layout record's `slot`), so a pane dragged wider stays wider; when the
splitter changes size the difference is shared from the kept sizes by stretch,
equally when nothing stretches. A handle moves only its two neighbours, never
past a minimum or maximum, and a press within `grab` of it takes it — the
splitter claims those points from the panes. It takes focus: the arrows along
its axis move the current handle, Home and End as far as it goes, F8 and
Shift+F8 go round the handles. Right to left puts the first pane on the right
and counts positions from there. Drawn as a line down each handle's middle,
the one held in the accent colour, and with the focus shown by keyboard a ring
round the current handle, not the whole splitter; told as a split pane across
or up and down, its value the current handle's place, which assistive
technology sets or steps.

**Events** (`event.cst`, `router.cst`, `timer.cst`, tested by `router_test`,
`focus_test`, `shortcut_test` and `timer_test` with synthetic events): hit-testing
that respects clipping, bubbling, capture by whoever handles the press,
per-widget enter and leave kept as a chain so a destroyed widget does not
confuse it, click counting and the drag threshold from settings, scrolling that
bubbles — past a disabled widget, where a press stops —
context menus by right click, Menu key or Shift+F10, focus with Tab order,
focus ring only after keyboard use, what holds the focus told as it comes in
and leaves (`FOCUS_ENTER`, `FOCUS_LEAVE`, for a composite that commits when
its part loses the focus), a modal scope, window shortcuts with the override
that bubbles, mnemonics — what one chooses takes the focus when it can, as by
Tab, then hears `ACTIVATE`; Alt going down or up repaints the window — and
timers with ids that stay dead once cancelled.
A long press — a finger or pen held still for `long_press_ms`, or a mouse when
the settings ask — opens the context menu where the press was: the widget that
captured the press hears `POINTER_CANCEL`, and the release that follows is no
click. The router says when one is due and the application's loop ticks it,
since nothing arrives while a finger is still (`longpress_test`); the touch and
pen events themselves come from `input/` through the host, which says which
the pointer is with `router.set_source`: the first finger drives it (button 1
from its down to its up; lifted, nothing hovered; a `TOUCH_CANCEL` clicks
nothing), a pen too (its tip button 1, barrel buttons 3 and 2), and a
touchpad's gestures go to the widget under the pointer as `GESTURE`
(`router.gesture`) — a scroll area stops a fling when fingers rest on the
touchpad. The router is fed by plain calls — `pointer_move`,
`pointer_button`, `key` and `text` — which `input/` calls, `paste`, giving a
widget the text it asked the clipboard for (`tree.request_paste`, answered by
the program), `preedit`, the input method's composition with its caret, and
`delete_surrounding`, its byte counts around the caret, to the focused widget.
The host feeds the platform input method's `TEXT`, `PREEDIT` and
`DELETE_SURROUNDING` events into these routes on all three platforms, places
its candidates at `tree.text_area_rect`, and tells it what the field with the
keyboard holds around the caret and what it is for (`access.text_around`,
the field's role: a password's text never). The widget says where its caret is
(`tree.set_text_area`) and when it gives a composition up (`tree.reset_ime`),
so the program can tell its input method that what it commits next is not what
was given up. FOCUS_IN says whether the focus came by keyboard. A press outside
a modal scope goes nowhere, but the scope hears of it (`PRESS_OUTSIDE`), so a
popup closes; the press is spent on that, and opens no context menu where it
landed; the pointer moving outside it, over nothing it may reach, is told to it
too (`MOVE_OUTSIDE`), for a menu bar's open menu to find the pointer on another
title. One subtree outside a scope may be let through it (`set_scope_also`):
an entry keeps the focus, and its presses, while what it suggests is shown.
When the popup ends, `restore` puts the scope back and gives the focus back in
one step (`give_back`: the ring as it was, but no move by keyboard, so an entry
does not select all of itself), nothing else focused on the way; a scope
destroyed while set is none, so the window is never left shut.

The router keeps a text caret's blink (`blink_test`): on for two thirds of the
desktop's cycle and off for the rest, started again by each
key, text, composition, paste, press and change of focus, and steady again a
while after the last (`caret_blink_timeout_ms`, ten seconds), since a caret
blinking in a window nobody types into would keep the machine awake. It says
when the caret next changes (`next_due`), asks then for the widget typed into
to be drawn again, and painting asks it whether the caret is off at the
frame's time (`caret_off`), which widgets hear as `S_CARET_OFF`.

The pointer's shape (`pointer_shape_test`) is CSS's cursors, numbered as
Wayland's cursor-shape-v1 has them (`tree.SHAPE_*`), so the window layer maps
each to an X cursor, a Wayland shape (or the theme's images the client
draws) or a Windows IDC_; a shape the platform has not got is shown as the
default pointer, chosen by the host, and the desktop's cursor theme and size
are followed as they change. A kind may say, for a
point of a widget, which one the pointer takes there or that its parent
decides (`Kind.shape`); `router.pointer_shape` asks the widget holding a
press, wherever the pointer went, or else the deepest one under it, and up —
skipping what is disabled, since a shape says what can be done there. The
text beam over an entry and a selectable label, a hand over a link, the
resizing arrows over a splitter's handle.

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
overlapping and bordering ones merge when their box is no larger than the two
of them — a band along a hole's top and one down its side stay two — and past
the limit a rectangle goes into the one it enlarges least, so it can cover
more than changed but never less. `tree.paint_wanted` says whether a frame
has anything to collect at all.

Scrolled, a scroll area does not have its content painted again: it asks to
move the pixels that stay in view (`tree.shift_request`), and `collect` takes
the ask when the window moves pixels (`tree.set_moves`, as the host's does)
and nothing but what the area holds is drawn over them — no shown widget on a
layer above, no ancestor drawn through a texture or over its children,
nothing drawn after the area reaching them. Then what moved with the content
is not damaged; the area damages what comes into view, its bars, and, where
a focus ring round it or round what holds it is shown or changes, a band at
its edges as deep as the ring reaches. The window moves the pixels
(`tree.shift`, `command.move_region`) before painting the damage over them;
`shift_test` checks every frame of it — scrolled either way, both ways at
once, past the view, a row changed meanwhile, focused, the focus leaving, a
popup over it, two scrolled at once, a faded area, the wheel's glide and the
bars fading — against the same frame painted whole. The glide and the bars'
fade step quietly (`anim.set_quiet`): their steps draw what they change, the
offset or the bars, rather than the whole area. A caret that blinks is drawn
again alone where it is when the frame is collected (`tree.blink`,
`Kind.caret`, given by the text view and the entry), not the text around it.

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
it and is hit-tested first: a popup inside the window. A kind's `paint_over`
entry draws after its children, over them, in the same clip — a scroll area's
bars. The focus ring is drawn after the subtree, inside the edge, unless the
widget drew its own. A clip a
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
cubic Béziers — or the theme's (`style.Motion.ease`: ease-out exponential).
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
platform is `window/`'s (`window/settings.cst`: XSETTINGS, the portal over
D-Bus, the registry on Windows); `system.cst` turns what it reads into `style.Prefs` and the
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
so a label given its own width on snapped edges
does not wrap its last word; wrapping, no narrower than its
widest word; at most a number of lines with an ellipsis, and then, not
wrapping, cut to its width and no narrower than the ellipsis. Drawn in the
theme's colour for its tone — text, secondary, accent, error, warning,
success, link — faded when disabled, quieter in the backdrop (in its normal
tone, in the colour what holds it gives instead), aligned across
by its alignment in the widget's direction and centred down. Its text is its
name to assistive technology, and the name of the widget it labels. A
mnemonic (`_Save`; `__` for an underscore) underlines its letter while Alt is
held, and in a tree that has its window's router — every window's does — the
letter is bound there as the text is set: Alt and it focus and activate what
the label names, followed when the text or the target changes, let go when
the text has none. A menu's items, a menu bar's and a tab's titles set theirs
with `set_text_marked`, which underlines without binding: their owners bind
them (`tabs.bind_mnemonics`) or take them without Alt.
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
`check_test`): one widget for both — an indicator beside a
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
toggled by Space released or Enter, and by assistive technology;
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
shows a bubble just below the pointer: in a platform tooltip of its own where
the host lends the tree its windows, reaching past the window's edges and kept
on the screen, taking no grab; elsewhere on a layer above everything, kept
inside the window. In the theme's tooltip colours — square where its window
shows nothing behind it — its text in the caption style wrapped to
twenty-five of the body's ems; it takes no input, the pointer going through
it to what is beneath (`tree.F_NO_INPUT`, and an empty input region on the
screen); a tooltip to assistive technology, named by its text. Leaving the
widget, a press or a key (the router counts them) hides it, and it does not
show again until the pointer leaves and comes back.

**Spin boxes** (`widgets/spin.cst`, tested by `spin_test`): a number in an
entry held to its range and step, with as many decimals as the step has
(six at most) or as set, as wide as the widest value the range holds; two
flat buttons at its end — mirrored right to left — each disabled at its end
of the range, giving the focus to the entry. One field is drawn around the
frameless entry and the buttons, in its focused look while the entry has
the focus. The arrows step it as they go down, Page Up and Down move ten
steps, Control and Page Up or Down go to the ends, the wheel a
step a notch — over a disabled button too. Typed into, the entry edits as
any does, taking only what a number can hold (a comma for the point too),
thirty-two at most; Enter, the focus leaving, a button or a step commits it
held to range and step, or puts the value back when it is no number; Escape
puts it back. While something is typed, Enter and Escape are its own, ahead
of the window's default and cancel buttons; otherwise Enter is the default
button's.

**The text buffer** (`textbuf.cst`, tested by `textbuf_test`): what an entry or
a text view edits — a gap buffer, any range had in one piece for laying out
(the gap moved only when inside it); paragraphs indexed through every edit, so
the paragraph of an offset is a search away; line ends made line feeds on the
way in. Undo and redo go by the steps a person would undo by: a run of typing
broken at a word's start or by the caret moving, a run of backspaces or of
deletes forward (`erase_ahead` when it is known to be Delete, never joined to
a backspace), a paste, a replacement or text moved by a drag (`move`) alone,
each putting the caret back where it belongs; the program's own edits forget
what could be undone, whose places they move. Two megabytes in twenty
thousand paragraphs stay quick. It says what changed since it was last asked
(`take_change`) — one span from the first byte touched to the last, as the
text is now, and how much longer it got — undo and redo included, so a view
of it lays out again only those paragraphs.

**Entries** (`widgets/entry.cst`, tested by `entry_test`): one line of text
in a field, kept in a text buffer and undone in its steps; a line feed typed
or pasted is a space, and what is no UTF-8 is left out. The caret moves by
grapheme and by word (Control), left and right as the text is drawn, Home and
End to the ends, Shift selecting; the pointer places it, drags a selection,
selects a word with a double click and all with a triple, and Shift and a
click extend. Pressed inside the selection and dragged, the selection goes
where it is dropped — where it would go drawn as a caret — a copy with
Control; each drop a step to undo. What is selected goes to the primary
selection, the middle button pastes it where it is pressed; Control and C, X
and V, Control and Insert, Shift and Insert and Shift and Delete copy, cut
and paste through the program's clipboard. Its editing keys are its own
ahead of the window's shortcuts, and Enter too unless it activates the
window's default (`set_activates_default`); Enter emits ACTIVATED, every change
to the text CHANGED. An input method's composition shows at the caret,
underlined, with its own caret, until committed as text; composing over a
selection takes its place, and a key, a click, the text set, read-only or the
focus leaving give it up and tell the input method. Past its width the text
moves along only as far as the caret needs, never ending short of the
field's end; right to left, a text that fits is at the far side, its
placeholder too. A password shows a bullet per character, is never copied or
cut, and takes no composition; a placeholder shows in the secondary colour
while it is empty; a limit in characters cuts what goes in to what fits, and
a shorter limit cuts the text; a filter lets in only the characters it says
yes to, typed, pasted or dropped. Read-only it is still selected and copied.
Focused by the keyboard, all of it is selected. It is laid out again when the
theme's text changes; eight of the body's ems wide by preference, a line
tall and padded — or room for so many characters (`set_width_chars`), none
leaving its width to what holds it — and drawn without its frame inside a
field of its holder's (`set_frame`). Its caret blinks, and is not drawn while its window is not
the active one. Told as an entry, or as password text, editable or
read-only. Its context menu — a right click, a long press, the Menu key
or Shift+F10 — offers Undo, Redo, Cut, Copy, Paste, Delete and Select All,
their keys shown, each only when it can be done: at the pointer, a click
outside the selection moving the caret there first, or below the caret from
the keyboard with its first item chosen; made the first time, gone with the
entry.

**Text views** (`widgets/textview.cst`, tested by `textview_test`): text of
many lines, its paragraphs broken at line feeds, meant to be a scroll area's
content, which it asks to keep the caret in view. Typing, Enter, Tab (unless
set not to take it, when Tab moves the focus as ever), pasting with line ends
kept as line feeds, undo in steps, the clipboard and the primary selection, a
composition, its context menu, a selection dragged — as the entry has them,
through `widgets/textedit.cst`, which both share. Wrapped by word at its width,
or not at all, one line a paragraph as wide as the widest. The caret moves by
grapheme and word across paragraph ends, up and down by lines into the
paragraphs above and below keeping where across it was, Home and End to the
line as drawn, Control to the text's ends, Page Up and Down by the height in
view; the pointer as the entry's, a triple click the paragraph. A large
document is laid out only where it is seen: each paragraph's height estimated
from its length until it is laid out, a Fenwick tree over the heights finding
a paragraph's top and the paragraph at a height without walking them all, a few
dozen paragraphs kept laid out, the least recently used given up first; a text
of sixteen kilobytes or less laid out whole. An edit lays out again only the
paragraphs the buffer says changed, the rest moved along with what they had —
checked against a view made with the same text after every kind of edit. Twenty
thousand paragraphs open, scroll to their end and are typed into with a few
dozen laid out. Asked how tall it would be at a width it is not at — as a row
asks before it shares out its room — a large text guesses from the paragraphs'
lengths and keeps the heights it found at its own. Told as text of many lines,
editable or read-only.

**Menus** (`widgets/menu.cst`, tested by `menu_test`): a popup menu the
program builds once and pops up against a rectangle or at the pointer —
items with mnemonics and the accelerators they show (Ctrl, Alt, Shift and
Super before the key, a letter as its capital, named keys by name),
separators, check items, radio items in groups, submenus. A child of the
root, on a layer above everything; where the host lends the tree its windows
it is shown in a platform popup of its own (`surface.cst`), reaching past the
window's edges, cut to the screen — flat, square where that window shows
nothing behind it — and elsewhere inside the window, cut to it; either way
placed by the positioner — flipped from an edge, slid inside, cut to the room
there is and then scrolled by the wheel and by the keys moving through it; a
submenu beside its item, the other side right to left, its first item level
with it, in a popup of its own belonging to its menu's. While it is open the window's input is
its own: the router's scope, every key claimed ahead of the window's
shortcuts, a press outside closing it. The arrows move through what can be
chosen, wrapping, Home and End to the ends, the arrow towards the line's end
opens a submenu and the other closes one; Enter and Space choose; a letter,
with or without Alt, chooses the one item it marks or goes on to the next of
several; Escape closes a submenu, or the menu. The pointer chooses what it
rests on and what it clicks; a press that opened the menu, dragged to an
item and let go there, chooses it, while one let go at once leaves it open.
Resting on a submenu's item opens it, and on another item closes it, after a
pause when the window's timers are lent, at once when not; passing over
another item on the way into the open submenu keeps it open. Choosing closes
the whole menu, the scope and the focus given back as they were, then emits
ACTIVATED with the item's id — a check item turned over, a radio item
checked, its group's others not, first; closing emits CLOSED. Told as a
menu of items, check items and radio items, checkable, checked, expandable
and expanded as they are, chosen by assistive technology's press.

**Tabs** (`widgets/tabs.cst`, tested by `tabs_test`): pages one at a time
under a strip of tabs — along the top or the bottom, from the right in right
to left — a stack showing the current page, as large as the largest, so
choosing another moves nothing around it. A tab's index is its place in the
strip and its page's in the stack; the first page made is current, and a page
taken away hands on to the next, or the last. Chosen by a press on its tab;
by the arrows while a tab has the focus — only the current tab takes it, so
the strip is one stop for Tab — Home and End to the ends; from anywhere inside
by Ctrl and Page Down or Tab for the next and Ctrl and Page Up or Shift and Tab
for the one before, round from the ends, the focus moved to the tab chosen when
it was in the page going away; by Alt and a title's mnemonic; by assistive
technology's select. Each change says VALUE_CHANGED. A tab may carry a close
button, which says CLOSED with the tab's index and leaves the closing to the
program. More tabs than fit: arrows at the strip's ends and the wheel move them
along, and the one chosen is brought into view. Drawn with the current tab
underlined in the accent on the side facing its page and titled in the text's
colour, the others dimmer, the one under the pointer lit; told as a list of
tabs, each named by its title and selected when current.

**Expanders** (`widgets/expander.cst`, tested by `expander_test`): a title
with a small triangle at its start — at the right in right to left — and under
it, while expanded, what it holds, in an overlay the program fills; closed, only
as tall as its title, what it holds hidden through the tree. Turned by a press
on its title, Space or Enter with the focus, Alt and its title's mnemonic, and
assistive technology, saying TOGGLED each time; the triangle points along the
line when closed and down when open, and the focus ring goes round the title's
row only. Told as a button that expands, named by its title.

**Toolbars** (`widgets/toolbar.cst`, tested by `toolbar_test`): flat buttons,
toggles, separators — or any widget made with the toolbar as its parent — in
a row from the start edge, as tall as a control. Its buttons are one stop for
Tab, the one last focused, and the arrows along the row move between them past
separators and what is disabled, Home and End to the ends. Those that do not
fit leave the row, and a button at its end offers them from a menu — a toggle
as a check item as it stands, a disabled tool disabled — where choosing one
presses it as a click would; that button is the last stop for the arrows, and
Space, Enter or Down there opens the menu with its first item chosen. The stop
stays in the row: one that leaves it, or is destroyed, hands it to the last
button still there, or to the menu button when none fits. Told as a tool bar,
across.

**Status bars** (`widgets/statusbar.cst`, tested by `statusbar_test`): a
message, the newest pushed, in the dimmer text colour, and a row at the end
for what the program keeps there, under a line. Messages are pushed under a
context the program picks for each part of itself that speaks, popped by it —
its newest — or taken away by the id push gave; one flashed goes by itself
after a while through the window's timers, and a newer flash takes its place.
Told as a status bar named by its message.

**Menu bars** (`widgets/menubar.cst`, tested by `menubar_test`): titles in a
row, each with a menu the program built, opened under it flush with its start.
A press on a title opens its menu and another on it closes it; while one is
open, the pointer reaching another title opens that one instead (the menu
hears the pointer outside it, `menu.set_outside`), and Left and Right with
nothing to open or close in the menu go to the menu before or after, round
from the ends (`menu.set_sideways`). F10 opens the first and Alt with a
title's mnemonic its own, each with its first item chosen; Escape closes, the
focus going back where it was. The title whose menu is open is lit. Told as a
menu bar of items that open, expanded while open.

**Assistants** (`widgets/assistant.cst`, tested by `assistant_test`): a task
in steps, one at a time — the step's title on top, the step under it, and at
the foot Cancel at the start, Back and Next at the end, mirrored in right to
left. Next reads Finish on the last step and is held back while the step is not
complete (`set_complete`); Back is held back on the first. The steps are kept
in a stack, as large as the largest, so nothing moves from one to the next.
Moving says VALUE_CHANGED, Finish ACTIVATED and Cancel CLOSED — closing is the
program's to do. Bound to a window's router, Enter is Next or Finish and
Escape Cancel. Told as a panel named by its step's title.

**Title bars** (`widgets/titlebar.cst`, tested by `titlebar_test`): a
window's title bar drawn by the toolkit, its own on every platform. The
program's widgets at its start; the title, and a subtitle under it, after
them from the start, cut with an ellipsis where there is no more room; the
program's widgets at its end; the window's buttons last — minimize, maximize,
close, whichever `set_buttons` asks for, always in that order: a dialog has
only close. As tall as the theme's `titlebar_height`, taller only for what it
holds; the buttons squares of `caption_button` with the theme's corners, lit
under the pointer, close in the destructive colour, their glyphs stroked — a
line, a square, two squares once maximized, a cross. What the window does is
asked of it through the tree: the program lends `tree.set_window_ops` a
callback, and widgets call `tree.window_op` with WIN_MOVE, WIN_MINIMIZE,
WIN_MAXIMIZE, WIN_CLOSE or WIN_MENU at a window point, for the program to pass
on to the platform. The buttons minimize, maximize — restore once
`set_maximized` says so — and close; a drag on the bar's empty places moves the
window, a double click maximizes it, and the right button opens the window's
own menu where it was pressed. Drawn in the title bar's colour, no line under
it, all mirrored in right to left; told as a title bar named by its title, its
buttons named for what they do.

**Window frames** (`widgets/decoration.cst`, tested by `decoration_test`):
the frame itself, the same on every platform. A title bar across the window's
top, rounded with it, a column under it for what the window holds, a hairline
round it in the border colour, and the window's colour inside rounded corners
within a shadow where the platform shows what is behind the window; the
buffer is then larger than the window as the desktop counts it, and
`geometry()`, `extents()` and `input()` say by how much and where the pointer
is taken — the window and the edges' reach just beyond it, never the rest of
the shadow. Eight handles lie over the edges and corners, a corner taking 16
pixels of each side by it: the pointer is the resize cursor there, and a press
asks the window, through `tree.window_op`, to resize from that edge
(WIN_RESIZE_N … WIN_RESIZE_NW). Maximized it has no shadow, rounding, edges
or hairline, and its maximize button restores; full screen it is only what it
holds; tiled to a side (TILED_*) it has no shadow or rounding at all and no
edge on that side; in an inactive window its shadow is lighter; flat
(`set_flat`) — on X11, which has no word for a shadow outside the window; on
Windows, whose system draws the shadow and rounds the corners itself; where
nothing behind the window would show through it — it has no shadow or
rounding of its own, all of it inside the window, still resized from every
edge. Painted with `paint.Options.see_through`, where every damaged region
starts from nothing instead of the window's colour (`draw2d.replace_rect`,
written, not blended), so the shadow lies over whatever is behind the window.

Every top-level window and dialog the host shows has one. A program gives a
window a frame it made with `app.set_decoration`; any other gets the host's
automatic frame when it is first shown, its root's children moved into its
body, keeping their overlay layout; popups remain above it. The automatic
title bar follows the window's title, its buttons all three or, a dialog's,
close alone. Dialogs are measured with the frame before opening, so their
content and title bar fit the platform's window. A program's own frame given
later takes the automatic one's place, what it held moved back under the root.
The window's menu is the toolkit's own (`widgets/menu.cst`): restore or
maximize, minimize and close — a dialog's, close — in the tree's language.

**List views** (`widgets/listview.cst`, tested by `listview_test`): the rows
of a flat model in a scroll area of their own, each as tall as the first,
with widgets only for the rows in view and one past them; scrolled, each is
given to the row that comes into view in its place — row i always in the
same one while it is seen, so a scroll by a row binds one row again. A row
shows its model's TEXT of column 0 after its ICON when it has one — one of
the toolkit's icons (an integer) or a texture of the program's (a handle),
`ICON_SIZE` square, at the row's start — or what the program's factory makes
of it (`set_factory`: setup a new row widget, bind a row to one). Selection is
of positions, none, one or many, kept as ranges — a million rows chosen is
two numbers — that move with the rows inserted and removed. The keyboard's
row moves with the arrows, Page Up and Down, Home and End, choosing it
alone, from the anchor with Shift, nothing with Control; Space chooses,
Control+Space turns over, Control+A all and Shift+Control+A none; a click
chooses, with Control turns over, with Shift from the anchor, a double click
or Enter activates; the right button chooses the row it is on unless chosen
already, and goes on for whoever shows a menu. Typing finds the next row
starting with what was typed in the last second, the same letter again going
on. Rows in the theme's row look, the keyboard's row ringed instead of the
list. Told as a list, many selectable when so, of list items; those built on
it say other roles and add to what a row says (`set_roles`,
`set_row_access`).

**Trees** (`flatten.cst`, `widgets/treeview.cst`, tested by `flatten_test`
and `treeview_test`): a tree model's rows that show — the top level and,
under each open row, its children — are a flat model of their own, each row
with its depth; a branch's children are read only
when it opens, and the tree's changes become the changes of the rows shown.
A tree view is a list view over it: each row an expander and a name, one
indent further in per level; the expander, Right and Left (the other way
right to left) open and close a branch, Right going on to an open row's first
child and Left to a closed row's parent. Told as a tree of tree items,
expandable, expanded while open.

**Tables** (`sortmodel.cst`, `widgets/tableview.cst`, tested by
`sortmodel_test` and `tableview_test`): a sort model shows another flat
model's rows in the order of a column — its SORT value, else its TEXT;
numbers as numbers, text with ASCII letters' case aside; equal rows as they
came — passing the model's changes on where the rows now stand. A table view
is a list view of one cell per column under a header of the columns' titles,
each cell its column's TEXT after its ICON as a list's rows show them:
columns side by side, one — the last, or the one `set_stretch` says — taking
the room they leave, wider ones scrolled across with the header following. A column's edge in its header is dragged
to resize it, never below `MIN_WIDTH`; a sortable column's header clicked
asks the sort hook for the rows sorted by it, up and then down, and shows the
arrow. Told as a table of table rows under column headers.

**Popups** (`popup.cst`, tested by `popup_test`): what is made with one as
parent, framed in the popover colour on a layer above the window, opened
against a rectangle — or against a widget (`open_from`), followed as it moves
— and placed by the positioner: flipped, slid, cut to the room there is. In a
platform popup of its own where the host lends the tree its windows, reaching
past the window's edges; there a shadow lies round its frame and its corners
are rounded where its window is seen through, the shadow taking no input, and
it is flat and square where it is not. Open, it is the router's modal scope: Tab goes round what it
holds, a press outside or Escape closes it, and the focus goes to the first
of what it holds, to the popup, or stays where it is (`KEEP_FOCUS`, what has
it let through the scope), coming back when it closes. CLOSED says it
closed. With an arrow (`set_arrow`) it is a popover: an `ARROW`-long point on
the side facing what it was opened against, at that rectangle's middle as
near as its corners let it, moving to the other side when flipped.

**Combo boxes** (`widgets/combo.cst`, tested by `combo_test`): the chosen row
of a flat model in a control with an arrow; pressed, or Space, Enter,
Alt+Down or F4, a list of the rows drops down under it — as wide or wider, at
most `MAX_ROWS` tall, the chosen row chosen there — where a click or Enter
chooses and Escape or a press outside leaves it as it was; closed, Up and
Down choose at once. It follows the model, the chosen row staying chosen as
rows come and go before it. One to type in is an entry with a button at its
end: the button, Alt+Down or F4 show all the rows; typing shows the rows its
text starts, the focus staying in the entry, Up and Down moving through them
and Enter or a click taking one's text. VALUE_CHANGED says another row is
chosen. Told as a combo box, editable when typed in, expanded while open.

**Menu buttons** (`widgets/menubutton.cst`, tested by `menubutton_test`): a
control titled with a mnemonic, an arrow at its end, opening a menu under it
flush with its start, or a popover under it, centred, the focus on the first
of what it holds — on a press, Space, Enter, Down or its mnemonic, a menu
opened by the keys with its first item chosen. Drawn pressed while open;
told as a button that expands, expanded while open.

**Dialogs** (`widgets/dialog.cst`, tested by `dialog_test`): a titled frame
holding what the program puts in it above a row of buttons, one per answer.
Where the application can open windows it has one of its own
(`window_tree`): belonging to the window, or to none (`app_window_tree`), a
dialog of the whole application's, which shown modal keeps every window of it
waiting. Where it cannot it is drawn in the middle of the window, on a layer
above it: modal, the window behind is dimmed and the dialog is the router's
modal scope — presses outside go nowhere, the window's shortcuts wait; not
modal, the window goes on. The buttons stand in
the platform's order (`set_order`): the affirmative answers last at the end,
as Linux desktops and macOS have them, or first, as Windows does, Help at the start,
all mirrored right to left. The default answer's button is in the accent,
has the focus when nothing in the dialog takes it, and answers Enter; Escape
answers CANCEL. A check (`set_check`) looks at an answer first and can keep
the dialog shown — how a file chooser refuses Open with nothing chosen. An
answer hides the dialog, then ACTIVATED says it. `message()` makes a message
box: an icon for information, a warning, an error or a question beside its
text, and a set of buttons. Told as a dialog, a message box as an alert.

**Folders as models** (`dirmodel.cst`, tested by `dirmodel_test`): one
folder's entries, read with getdents64 and stat, as a flat model of three
columns — the name; the size as a person reads it, sorted by its bytes; when
it changed, sorted by its seconds; the name with an icon for a folder or a
file once the program says which (`set_icons`). Folders first, then names
with letters' case aside; hidden names, files no pattern of a filter matches ("*.txt;*.md",
`*` and `?`), or all files when only folders are wanted, left out. Another
path, up, or the same again says a reset.

**File choosers** (`widgets/filechooser.cst`, tested by `filechooser_test`):
our own, a dialog holding the places a person starts from (home, its
desktop, documents and downloads when there, the computer's top), each with
its icon, beside the current folder — a button up and its path in an entry,
its entries in a table with a folder's icon or a file's, the names taking
the room, the filters in a combo box once added. To open a file, to save to
a name typed (asking first before replacing one), or to choose a folder; a
folder activated opens, Backspace and Alt+Up go up, Control+H shows the
hidden ones. A path typed where it is ("~" home, a name in the folder shown,
"." and ".." taken) opens a folder, or is taken as a file chosen — a name in
a folder there when saving; Control+L goes there. Open, Save or
Select does nothing until the answer is right; then `chosen()` is the whole
path.

**Colour choosers** (`widgets/colorpicker.cst`, tested by `colorpicker_test`):
its own palette — a column a hue, five shades down it, then the cool greys — over
a plane of saturation across and value up beside a strip of hue, a strip of
alpha when asked for, and the colour over a checkerboard beside its hex
code to type ("#rrggbb", "#rrggbbaa", "#rgb", "#rgba"). Colours are the
toolkit's, sRGB-encoded, and hue, saturation and value are of those values;
a grey keeps its hue. The palette is one Tab stop, the arrows going round
it; the plane and the strips move by keys as sliders do. `dialog()` holds one
with Cancel and Select; `button()` shows a colour and opens that dialog.

**Font choosers** (`widgets/fontpicker.cst`, tested by `fontpicker_test`):
over the program's font index — the families searched by any part of their
name, the chosen family's styles (a variable face's weights at each hundred
of its range, by name), a size in logical pixels, and a line of text in the
font chosen. Another family keeps the style nearest the one chosen.
`dialog()` and `button()` as for colours, the button saying "Family Style
Size".

**About dialogs** (`widgets/about.cst`, tested by `about_test`): the
program's image, name, version, what it does, its website and copyright down
the middle, each only once given; Credits and License, toggles at the
foot's start (a dialog's secondary buttons, `dialog.set_secondary`), show
who made it and the licence in their place.

**The application on the screen** (`host.cst`, tested by `host_x11_test`
under Xvfb with a second client playing the user and the window manager):
real windows for the application's, titled, painted and shown; X's clicks,
keys, resizes and close buttons reaching the widgets; the clipboard through
X; a modal dialog in a window of its own above its owner, the owner told
nothing meanwhile — a press, a key or the wheel on it bringing the dialog
forward (`_NET_ACTIVE_WINDOW`, xdg-activation under the press's own serial)
and ringing the bell, once for a burst, and Windows doing both itself for the
owner it disabled (`host_modal_win32_test`, the bell heard on Wine's X
server); the menus, lists, popovers and tooltip open over a window closed as
a modal comes over it (`host_x11_test`, `host_popup_wayland_test`); a dialog
of the whole application's, transient for the root with every window of the
program in one group on X11, every other window disabled on Windows; a text dragged from one window to another; the loop asleep
until a timer, a descriptor, another thread or an event wakes it. The
accessibility bridge runs in it (`atspi/hosted.cst`, `atspi_hosted_test` on a
private bus). Every window opens with a frame of the toolkit's own — the
program's (`app.set_decoration`) or the automatic one — and none of the window
manager's, drawn flat on X11; its title bar dragged, its edges pressed and its
buttons are asked of the window manager (`_NET_WM_MOVERESIZE`…), the press let
go of once the window manager has the pointer, and its menu is the toolkit's;
where it takes the pointer is told the window manager as the frame is laid
out, in physical pixels; the frame follows what the window manager says —
maximized, full screen. Wayland text-input-v3 publishes
the caret rectangle when the compositor provides the protocol. Candidate
placement with an installed IME remains unverified; X11 candidate placement
at the caret is not implemented.

Its trees' surfaces are platform popups (`host_popups.cst`, tested by
`host_popup_x11_test` under Xvfb, `host_popup_wayland_test` on a Weston with
its desktop shell nested in Xvfb, and `host_popup_win32_test` on Wine at 96
and 144 dpi, each with the platform's own input playing the user; the
platform's popups themselves by `window/`'s `popup_*_test`, Wayland's at
scales 1 and 2): an override-redirect window with the pointer
and keyboard grabbed on X11 — the press's implicit XI2 grab let go of first
—, an `xdg_popup` grabbing under the seat's latest press or key on Wayland, a
`WS_POPUP` tool window never activated, owned and capturing the mouse, on
Windows. A menu taller than its window reaches past its edge and an item
there is chosen by a click; a press on the desktop or another program takes
it down, the press spent; a submenu is a popup of its menu's; the pointer
reaching another title of a menu bar opens that one; a combo box's list drops
past the window's bottom and a row there is chosen; a tooltip reaches past
the window's edge and a press on it goes to what is beneath; a popover points
at its button, its shadow and corners seen through where they can be. Their
input reaches the window's router in its coordinates, and the pointer's
shape over one is what is under it there (`host.popup_pointer_shape`); the
window stays the active one while a popup has the keyboard; a button held
when its popup closed is let go of even where its release comes nowhere; a
press on the desktop takes a submenu down before its menu; the title bar's
right button opens the window's own menu in a popup; the window moved, it
moves them; no longer active, what holds no grab — a tooltip — goes, the
platform taking grabbing ones down itself; minimized, hidden or closed,
nothing of it stays up. Weston refuses a grab under a key's
serial, so a popup opened by a key is opened there again without one, closed
when the window stops being active. A combo box typed into keeps the keys and
the input method with its entry while its list is shown, in a popup taking
no grab (`ime_x11_test` with Fcitx5, `ime_wayland_test` on KWin, whose
text-input-v3 Weston 15 does not have, with and without Fcitx5).

**A program started** (`program.cst`, tested by `program_x11_test` under
Xvfb on a bare private bus, and with none; its fonts by `system_fonts_test`
over the test fonts): a window of text in the fonts found, run until it is
closed, everything let go after.

**The examples** (`examples/settings_form.cst`, `text_editor.cst`,
`file_browser.cst`, each checked by its own `--check` under Xvfb, on a Weston
nested in Xvfb and on Wine, and its links by `check_needed.sh`): the three
programs above, driven by the system's own input (`examples/user_input.cst`:
XSendEvent on X, the X server's XTest through `xdotool` for the nested
Weston, SendInput on Windows), each opening a field's menu in a window made
small — no smaller than its floor — past the window's bottom. What they asked of
the toolkit is in it now: a text view's and an entry's edit actions for a
program's own Edit menu (`can`, `act`), a window closed from inside its own
event closing after it, `host.present`, a file link's path
(`open_uri.local_path`).

**The application and the desktop**, each on a private bus that starts none
of the desktop's services (`dbus/tools/run_dbus_bare.sh`: stand-ins that
leave a mark when something asks for them to be started): the session
connection in the loop, the loop held with no window (`session_test`); the
settings portal followed (`desktop_test`, under Xvfb); one instance
(`instance_test`); notifications (`notify_test`); the tray, never starting a
watcher (`tray_test`); links through the portal and through a stand-in
`xdg-open` (`open_uri_test`). The toolkit's icons (`icons_test`), drawn in
message boxes and images; the desktop's icon theme
(`icon_theme_test`, over themes made for it in `testdata/icons`), its name
from XSETTINGS and the portal. Not yet: SVG icons, a menu
exported with the tray's item (`com.canonical.dbusmenu`).

**Words** (`strings.cst`, tested by `strings_test`, and each widget's own
test in Portuguese): the table of the toolkit's words in English and
Brazilian Portuguese, filled in (`fill`) and cut whole characters at a time
(`fits`); the tree's language, set by the application from the locale
(`window/locale.cst`, tested by `window_locale_test`, through the desktop's
settings, `settings.K_LOCALE`), the windows' direction right to left for a
language written so; message boxes, assistants, the text's context menu,
accelerators ("Ctrl+Espaço"), about dialogs, file choosers (places where
`user-dirs.dirs` says, tested by `window_user_dirs_test`; sizes with the
decimal mark), colour and font choosers, title bars, combo boxes, spin
boxes and the AT-SPI bridge (its `Locale`, roles' and actions' names for
people) all in it.

**A window's state** (`winstate.cst`, tested by `winstate_test`): its size
and whether it was maximized or full screen, kept between runs in a file of
its own under `$XDG_STATE_HOME` (else `~/.local/state`), written whole under
another name and moved over the old, refused when it is not what was
written; never where it was, which Wayland does not say. The application
keeps what the platform says of each window (`app.set_maximized`,
`set_fullscreen`) for `app.window_state`.
