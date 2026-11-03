# The UI layer: reference

Written from the sources by `ui/tools/api.sh`; do not edit by hand. Every
module, and every public function in it with the first sentence of what its
comment says. The design is [`ui.md`](ui.md); what is built and tested,
[`state.md`](state.md).

A program reaches all of it through the hub, `use "ui/ui.cst" as ui;`:
`ui.<module>.<function>`, and the hub's own forms as `ui.<function>`.

## `ui/ui.cst`

hub for the toolkit.

- `len(s as *u8) as i64` — The length in bytes of a zero-terminated string; 0 for null.
- `on(t as *tree.Tree, w as i64, sig as i32, cb as tree.SignalFn) as i64` — connect with no user pointer.
- `label(t as *tree.Tree, parent as i64, s as *u8) as i64` — A label of plain text: an underscore in it is shown.
- `label_for(t as *tree.Tree, parent as i64, s as *u8, target as i64) as i64` — A label naming another widget, its mnemonic going to it: "_Name" before an entry.
- `button(t as *tree.Tree, parent as i64, s as *u8) as i64`
- `link(t as *tree.Tree, parent as i64, s as *u8, uri as *u8) as i64`
- `check(t as *tree.Tree, parent as i64, s as *u8) as i64`
- `radio(t as *tree.Tree, parent as i64, s as *u8, group as i64) as i64` — A radio button: in the group of the radio button group, or the first of a new one when group is 0.
- `group(t as *tree.Tree, parent as i64, s as *u8) as i64`
- `expander(t as *tree.Tree, parent as i64, s as *u8) as i64`
- `menubutton(t as *tree.Tree, parent as i64, s as *u8) as i64`
- `tab(t as *tree.Tree, w as i64, s as *u8) as i64` — A page of tabs w, titled s: the widget to put the page's content in.
- `menu_item(t as *tree.Tree, m as i64, s as *u8, id as i64) as i64`
- `menu_check(t as *tree.Tree, m as i64, s as *u8, id as i64, on as i32) as i64`
- `menu_radio(t as *tree.Tree, m as i64, s as *u8, id as i64, group as i64, on as i32) as i64`
- `submenu(t as *tree.Tree, m as i64, s as *u8) as i64`
- `menu_title(t as *tree.Tree, bar as i64, s as *u8, m as i64) as i64` — Menu m on the menu bar, titled s.
- `tool_button(t as *tree.Tree, bar as i64, s as *u8) as i64`
- `tool_toggle(t as *tree.Tree, bar as i64, s as *u8) as i64`
- `column(t as *tree.Tree, table as i64, s as *u8, width as f64) as i64`
- `dialog(t as *tree.Tree, title as *u8) as i64`
- `dialog_button(t as *tree.Tree, d as i64, s as *u8, response as i64) as i64`
- `message(t as *tree.Tree, kind as i32, title as *u8, say as *u8, buttons as i32) as i64` — A message box: kind dialog.INFO, QUESTION, WARNING or ERROR; buttons dialog.OK_ONLY and the rest.
- `file_chooser(t as *tree.Tree, mode as i32, title as *u8) as i64`
- `file_filter(t as *tree.Tree, fc as i64, s as *u8, patterns as *u8) as i64` — A filter of the file chooser named s, its patterns parted by semicolons — "*.txt;*.md" — where the module's add_filter takes them parted by zeros.
- `placeholder(t as *tree.Tree, w as i64, s as *u8) as i32`
- `tip(tt as *tooltip.Tooltips, w as i64, s as *u8) as i32`
- `cell_text(s as *u8) as model.Value` — A cell's text, from a model's cell function.
- `set_text(t as *tree.Tree, w as i64, s as *u8) as i32` — What w shows: a label's text, plain; an entry's or a text view's, as if set by the program; a button's, a check box's or a radio button's words, a mnemonic's underscore marked.

## `ui/access.cst`

what assistive technology is told about a widget, and what it may ask one to do.

- `role_name(r as i64) as *u8` — A role's name, as a bridge or a test prints it; "unknown" for a number that is none.
- `set_name(q as *tree.Access, text as *u8, n as i64) as void` — text, n bytes of UTF-8 the kind keeps: its name.
- `set_description(q as *tree.Access, text as *u8, n as i64) as void`
- `set_range(q as *tree.Access, value as f64, min as f64, max as f64, step as f64) as void` — A value in a range, moved by step.
- `role(t as *tree.Tree, w as i64) as i64` — The role w is told as: its kind's, or the one its entry says instead; a window for a tree's root with none; none for a widget gone.
- `describe(t as *tree.Tree, r as *router.Router, w as i64, out as *tree.Access) as i32` — What w is, into *out: 1, or 0 — and nothing, every field zero — for a widget that is gone.
- `act(t as *tree.Tree, w as i64, action as i64, arg as f64) as i32` — Asks w to do action — one of its kind's — with arg for SET_VALUE: 1 when it did.
- `text_of(t as *tree.Tree, w as i64, q as *tree.TextQuery) as i32` — w's text, caret, selection and whether it can be edited, into q: 1, or 0 for a widget with none.
- `text_do(t as *tree.Tree, w as i64, q as *tree.TextQuery) as i32` — q.op done to w's text, or asked of it: 1 when it was.
- `text_info(t as *tree.Tree, w as i64, q as *tree.TextQuery) as i32` — What w's text is but the text: text_len, chars, caret, selection, whether it can be edited, whether it tells its changes — 1, or 0 for a widget with none.
- `text_span(t as *tree.Tree, w as i64, a as i64, b as i64, q as *tree.TextQuery) as i32` — Bytes a to b of w's text, held to it, in one piece: into q.text and q.text_len, until it next changes.
- `text_char_of(t as *tree.Tree, w as i64, at as i64) as i64` — The characters before byte at of w's text; all of them past its end.
- `text_byte_of(t as *tree.Tree, w as i64, k as i64) as i64` — Where character k of w's text starts, in bytes; its end past the last.
- `text_paragraph(t as *tree.Tree, w as i64, at as i64, a as *i64, b as *i64) as i32` — The paragraph byte at of w's text is in, from *a to *b, its line feed in it: 1, or 0 for a widget with none.
- `text_around(t as *tree.Tree, w as i64, max as i64, q as *tree.TextQuery) as i32` — w's text around its caret, for an input method to compose against: at most max bytes of whole characters, within the caret's paragraph (its line feed left out), the selection in it as far as it fits — into q.text an...

## `ui/anim.cst`

things that change over time: a widget fading in or out, sliding aside, a colour or a number moving to another.

- `of(t as *tree.Tree) as *Animator` — For the widgets of t, at 60 frames a second until set_interval says otherwise.
- `lend(t as *tree.Tree, a as *Animator) as void`
- `init(t as *tree.Tree) as Animator`
- `fade(an as *Animator, w as i64, a as f64, ms as i64, ease as i64) as i64` — w's opacity to a, over ms milliseconds along ease (math.curve.EASE_*, or THEME_EASE).
- `move(an as *Animator, w as i64, x as f64, y as f64, ms as i64, ease as i64) as i64` — w's translation to x, y.
- `color(an as *Animator, w as i64, target as *math.color.Color, c as math.color.Color,` — The program's colour at target to c, w repainted as it changes.
- `number(an as *Animator, w as i64, target as *f64, v as f64, ms as i64, ease as i64) as i64` — The program's number at target to v, w repainted as it changes.
- `set_quiet(an as *Animator, id as i64) as void` — Its widget not repainted, all of it, at each step: what the tween moves is drawn by its own step (on_step) — a scroll area's offset, which moves its content's pixels, or its bars' fading, which repaints only the bars.
- `on_step(an as *Animator, id as i64, cb as AnimFn, user as *u8) as void` — cb(user) after every step, the last included — for what a repaint does not cover, such as laying out again.
- `on_done(an as *Animator, id as i64, cb as AnimFn, user as *u8) as void` — cb(user) once, when the tween reaches its end — by time or by finish().
- `set_repeat(an as *Animator, id as i64, runs as i64, alternate as i32) as void` — Runs it runs times in all, 0 or less for ever; alternate sends every other run back the way it came, its curve backwards too.
- `cancel(an as *Animator, id as i64) as void` — Stops it where it is.
- `finish(an as *Animator, id as i64) as void` — Puts it at its end now, and it is done.
- `running(an as *Animator, id as i64) as i32`
- `active(an as *Animator) as i64`
- `last_error(an as *Animator) as i32`
- `set_interval(an as *Animator, ms as i64) as void` — The frame clock: ms between ticks while anything runs — the display's refresh, where the platform says it.
- `next_due(an as *Animator) as i64` — When the next tick is wanted: -1 with nothing running, at once — the last tick's time — for a tween waiting to start, else a frame after the last.
- `tick(an as *Animator, now as i64) as i64` — Steps every tween to now, and returns how many it stepped.

## `ui/app.cst`

the application: its windows, each with its own tree, and what they share.

- `limits_default() as Limits`
- `open(l as *Limits) as App`
- `close(a as *App) as void` — Closes every window still open — their widgets' destroy entries run — and releases the table.
- `is_open(a as *App) as i32`
- `last_error(a as *App) as i32`
- `window_count(a as *App) as i64`
- `window_open(a as *App, width as f64, height as f64) as i64` — A new window of width by height logical pixels, inactive, at scale 1, with an empty tree and the application's settings.
- `begin_dispatch(a as *App) as void` — Something done for the windows — an event given to one, the timers and animations stepped — begun and over.
- `end_dispatch(a as *App) as void`
- `window_close(a as *App, h as i64) as void` — Closes the window: every widget in it is destroyed, and every window that belongs to it closed first — once what is being done for the windows is over (begin_dispatch).
- `window_valid(a as *App, h as i64) as i32`
- `next_window(a as *App, h as i64) as i64` — The open window after h, in table order; 0 starts the walk and ends it.
- `tree(a as *App, h as i64) as *tree.Tree` — The window's tree and router; null for a stale handle.
- `router(a as *App, h as i64) as *router.Router`
- `pointer_shape(a as *App, h as i64) as i32` — The pointer's shape in window h (router.pointer_shape), for the loop to give the platform when it changes; the default for no window.
- `animator(a as *App, h as i64) as *anim.Animator` — The window's animator, for tweens on its widgets; null for a stale handle.
- `set_user(a as *App, h as i64, user as *u8) as void`
- `user(a as *App, h as i64) as *u8`
- `dialog_open(a as *App, owner as i64, width as f64, height as f64) as i64` — A window belonging to owner, for a dialog: hidden until shown, kept above its owner and closed with it, a dialog to the platform.
- `owner(a as *App, h as i64) as i64` — The window h belongs to, 0 for none.
- `is_dialog(a as *App, h as i64) as i32`
- `set_modal(a as *App, h as i64, on as i32) as void` — Modal: while it is shown, the windows it belongs to — its owner, and theirs — wait.
- `is_modal(a as *App, h as i64) as i32`
- `show(a as *App, h as i64) as void` — Shown or hidden: what the platform's loop opens or puts away.
- `hide(a as *App, h as i64) as void`
- `is_shown(a as *App, h as i64) as i32`
- `blocked(a as *App, h as i64) as i32` — Whether h waits on a modal window shown above it — one belonging to it, or to one that does, or the application's own.
- `blocker(a as *App, h as i64) as i64` — The modal window h waits on that waits on nothing itself — the one a person pressing on h is to be shown — or 0 when h waits on none.
- `set_title(a as *App, h as i64, s as *u8, n as i64) as void` — The window's title, s n bytes, copied: what the platform shows for it.
- `title(a as *App, h as i64) as *u8` — Its title, zero-terminated; title_length bytes.
- `title_length(a as *App, h as i64) as i64`
- `window_of(a as *App, t as *tree.Tree) as i64` — The window whose tree t is, 0 for none.
- `clipboard_set(a as *App, which as i32, c as *clip.Content) as i32` — c on which, tree.CLIPBOARD or PRIMARY, copied: 1, or 0 with no window on the platform to put it there.
- `clipboard_get(a as *App, which as i32, formats as i32, name as *u8, n as i64, cb as ClipboardAnswerFn, user as *u8) as i32` — What which holds, in one of formats (clip bits; NAMED: the type named name, n bytes), asked for: cb hears the answer later, with format 0 when there was none of them.
- `clipboard_answer(a as *App, format as i32, name as *u8, n as i64, data as *u8, len as i64) as void` — A clipboard_get's answer, from the platform's loop.
- `on_clipboard_changed(a as *App, cb as ClipboardChangedFn, user as *u8) as void` — cb hears each change of the clipboard and the primary selection, by another program or this one.
- `clipboard_offered(a as *App, formats as i32) as void` — What the clipboard offers, from the platform's loop: every window's tree.clipboard_has answers by it.
- `clipboard_changed(a as *App, which as i32, formats as i32) as void` — A change, from the platform's loop: which, 0 the clipboard and 1 the primary selection, offers formats now.
- `clipboard_has(a as *App, format as i32) as i32` — Whether the clipboard offers format (clip bits: any of them); 1 while the platform's loop has not said — a paste then finds out.
- `resize(a as *App, h as i64, width as f64, height as f64) as void` — The window's size in logical pixels — the platform's physical size over the scale.
- `set_icon(a as *App, px as *u32, size as i64) as void` — Every window's icon from now on: size by size pixels, 0xAARRGGBB each (icons.rasterize makes one of the toolkit's), kept by the program while it is the icon.
- `icon(a as *App, size as *i64) as *u32`
- `icon_generation(a as *App) as i64` — How many times the icon was set: the platform's loop sets it again when this changes.
- `width(a as *App, h as i64) as f64` — Its size, in logical pixels; 0 for no such window.
- `height(a as *App, h as i64) as f64`
- `set_scale(a as *App, h as i64, s as f64) as void` — Physical pixels per logical one, from the monitor the window is on.
- `scale(a as *App, h as i64) as f64`
- `set_active(a as *App, h as i64, on as i32) as void` — Whether the window has the keyboard focus.
- `is_active(a as *App, h as i64) as i32`
- `set_position(a as *App, h as i64, x as f64, y as f64) as void` — Whether the platform has the window maximized, or full screen: kept for window_state, nothing redrawn.
- `position(a as *App, h as i64, x as *f64, y as *f64) as i32`
- `set_decoration(a as *App, h as i64, w as i64) as void` — The window's frame drawn by the toolkit — w, a decoration widget (widgets/decoration.cst) in its tree — in place of the automatic one the host gives every window: the platform's window is opened without a frame of...
- `decoration(a as *App, h as i64) as i64`
- `set_automatic_decoration(a as *App, h as i64, w as i64) as void` — The frame the host made for the window, the program having given none.
- `set_see_through(a as *App, h as i64, on as i32) as void` — Whether what the window leaves unpainted shows what is behind it — its frame's shadow drawn over that — rather than the window's colour.
- `decoration_is_automatic(a as *App, h as i64) as i32`
- `set_maximized(a as *App, h as i64, on as i32) as void`
- `is_maximized(a as *App, h as i64) as i32`
- `set_fullscreen(a as *App, h as i64, on as i32) as void`
- `is_fullscreen(a as *App, h as i64) as i32`
- `window_state(a as *App, h as i64) as winstate.State` — What there is to keep of the window between runs (winstate.save); all 0 for a closed one.
- `needs_frame(a as *App, h as i64) as i32` — Whether the window has anything to lay out or repaint.
- `frame(a as *App, h as i64) as i64` — Lays the window out at its size and collects what has to be repainted.
- `set_cache(a as *App, h as i64, cc as *paint.Cache) as void` — The textures the window paints cached and translucent widgets through (paint.Cache), on the device its target is on; the program opens it and closes it after the window.
- `cache(a as *App, h as i64) as *paint.Cache`
- `paint(a as *App, h as i64, c as *draw2d.Canvas, theme as *style.Theme, time as i64,` — Paints what the last frame collected on c — begun by the caller on the window's target — with the window's own router for hover, press and focus, its cache and its active state; in theme, or the application's when...
- `paint_surface(a as *App, h as i64, s as i64, c as *draw2d.Canvas, theme as *style.Theme, time as i64,` — The same for surface s of window h — a widget of it shown in a window of its own (tree.surface_open) — on c, begun at the surface's size.
- `theme(a as *App) as *style.Theme` — The theme every window paints in.
- `set_theme(a as *App, th as *style.Theme) as void` — A new theme for every window, laid out and repainted in it; th is copied.
- `set_prefs(a as *App, p as *style.Prefs) as void` — The theme the system's preferences call for — dark, high contrast, the accent, the interface font, text scale, reduced motion — in every window.
- `set_system(a as *App, s as *wsettings.Settings) as void` — What the desktop says — window.read_settings, the portal — in every window: the theme its preferences call for, and its timings in every router.
- `settings(a as *App) as *router.Settings`
- `set_locale(a as *App, tag as *u8, n as i64) as void` — The user's language, a BCP 47 tag of n bytes: the toolkit's words in it — English where strings.cst has none of it — and every window laid out in its direction, right to left for Arabic, Hebrew and the others.
- `language(a as *App) as i32`
- `locale(a as *App, out as *u8, cap as i64) as i64` — The tag the language came from into out, cap bytes with the NUL; its length, 0 for none.
- `set_text_service(a as *App, s as *u8) as void` — The text service (ui/text.cst) every window, open and to come, lays its text out with; the program keeps it open while the windows are.
- `set_settings(a as *App, s as router.Settings) as void` — New settings from the system: every window's router takes them, and every window opened later.
- `timers(a as *App) as *timer.Timers`
- `next_due(a as *App) as i64` — When the loop has something to do next with no event to wake it — a timer, a long press being held, a window's next frame while something in it animates — or -1 for nothing.
- `tick(a as *App, now as i64) as i64` — Runs the timers due by now, every window's due long press, and steps every window's animations to now.

## `ui/box.cst`

a row or a column of widgets.

- `create(t as *tree.Tree, parent as i64, orientation as i32) as i64` — A new box as the last child of parent.
- `set_spacing(t as *tree.Tree, w as i64, spacing as f64) as void`
- `set_padding(t as *tree.Tree, w as i64, left as f64, top as f64, right as f64, bottom as f64) as void`
- `set_homogeneous(t as *tree.Tree, w as i64, on as i32) as void` — Every visible child gets the same share of the main axis, whatever it prefers — a row of buttons that should match.

## `ui/clip.cst`

what a program puts on the clipboard: content of several formats at once, each its bytes or made only when another program asks.

- `set_encoder(cb as EncodeFn) as void`
- `init(c as *Content) as void`
- `free(c as *Content) as void`
- `clear(c as *Content) as void` — Emptied, ready to be filled again.
- `formats(c as *Content) as i32` — Which formats it has, bits.
- `has(c as *Content, format as i32) as i32`
- `add_text(c as *Content, s as *u8, n as i64) as i32` — n bytes of UTF-8 at s: 1, or 0 when they could not be kept.
- `add_html(c as *Content, s as *u8, n as i64) as i32` — An HTML fragment, n bytes of UTF-8 — text added beside it is what a program that reads no HTML pastes.
- `add_png(c as *Content, p as *u8, n as i64) as i32` — A PNG file's n bytes, in place of an image added before.
- `add_image(c as *Content, img as *buffer.Image) as i32` — An image's pixels, copied: the host encodes them as a PNG when another program asks for one.
- `add_files(c as *Content, paths as *u8, n as i64) as i32` — n absolute paths, back to back at paths, each zero-terminated.
- `add_named(c as *Content, name as *u8, n as i64, p as *u8, len as i64) as i32` — The program's own type named name (n bytes, a MIME type), len bytes at p.
- `set_provider(c as *Content, cb as ProvideFn, user as *u8) as void` — What makes what is added on demand, and its user.
- `ask(cb as ProvideFn, user as *u8, format as i32, name as *u8, n as i64, out as *Bytes) as i32` — The provider cb asked for a format as the platform asks: text/html by name heard as HTML.
- `add_lazy(c as *Content, f as i32) as i32` — Formats — TEXT, PNG, HTML bits — added to be made on demand by the provider.
- `add_lazy_named(c as *Content, name as *u8, n as i64) as i32` — The program's own type named name (n bytes) made on demand.
- `put(out as *Bytes, p as *u8, n as i64) as i32` — n bytes at p into out, a provider's answer: 1, or 0 for no room.
- `offer(c as *Content) as *transfer.Offer` — What the platform's clipboard is handed: the content's offer, its formats with their bytes and those made on demand.
- `copy(dst as *Content, src as *Content) as i32` — dst made a copy of src: 1, or 0 (dst then empty).
- `text_of(c as *Content) as *Bytes`
- `png_of(c as *Content) as *Bytes` — The PNG — an image's pixels encoded now, once, where there is what does (set_encoder).
- `named_of(c as *Content, name as *u8, n as i64) as *Bytes` — The bytes of the type named name (n bytes) — made now when on demand — null when it has none by that name.
- `html_of(c as *Content) as *Bytes`
- `html_text(s as *u8, n as i64, out as *Bytes) as i32` — The text of an HTML fragment, n bytes of UTF-8, into out (emptied first): its tags out, the content of style, script, head, title and comments with them, the usual entities said, runs of white space one space but in p...

## `ui/damage.cst`

what has to be repainted, found from the tree.

- `collect(t as *tree.Tree) as i64` — Brings the tree's damage regions up to date — the window's, and each surface's (tree.surface_damage_count) — run it after layout, and returns how many rectangles the window's holds.
- `bounds(t as *tree.Tree) as math.rect.Rect` — The smallest rectangle around the whole region; empty when there is none.

## `ui/desktop.cst`

the desktop's settings, followed in the application's loop.

- `follow(d as *Desktop, hs as *host.Host) as i32` — The portal's settings read and followed by hs from now on — d where the program keeps it while followed: 1, or 0 with no session bus.
- `stop(d as *Desktop) as void` — No longer followed.

## `ui/dirmodel.cst`

a folder's entries as a flat model.

- `open(p as *u8, n as i64) as *Dir` — The folder at path, n bytes, read; null when it cannot be, or memory is refused.
- `close(d as *Dir) as void`
- `model_of(d as *Dir) as *model.Model`
- `path(d as *Dir) as *u8`
- `path_length(d as *Dir) as i64`
- `count(d as *Dir) as i64`
- `is_dir(d as *Dir, index as i64) as i32`
- `name(d as *Dir, index as i64) as *u8`
- `name_length(d as *Dir, index as i64) as i64`
- `index_of_name(d as *Dir, s as *u8, n as i64) as i64` — Where the entry named s, n bytes, is shown; -1 when it is not.
- `full_path(d as *Dir, index as i64, out as *u8, cap as i64) as i64` — The folder's path, "/", and the entry's name into out, cap bytes; how many, 0 when it does not fit or there is no such entry.
- `set_path(d as *Dir, p as *u8, n as i64) as i32` — The folder at p, n bytes, read instead: 1, said as a reset; 0 when it cannot be, d as it was.
- `reload(d as *Dir) as i32` — The same folder read again.
- `up(d as *Dir) as i32` — The folder it is in read instead; 0 at the top.
- `set_hidden(d as *Dir, on as i32) as void` — Names starting with a dot shown (1) or not (0, at first); read again.
- `set_folders_only(d as *Dir, on as i32) as void` — Folders only (1), as when a folder is to be chosen, or files too (0, at first); read again.
- `set_icons(d as *Dir, folder as i64, file as i64) as void` — A folder's name shown with icon folder, a file's with file — integers, as a list or table takes them (listview.show_icon); 0 for none.
- `set_language(d as *Dir, lang as i32) as void` — Sizes said in language lang (strings.cst) from now on.
- `set_filter(d as *Dir, s as *u8, n as i64) as void` — The files shown from now on, s, n bytes: patterns split at ';' or spaces; empty for all.
- `size_text(v as i64, lang as i32, out as *u8) as i64` — A size as a person reads it in language lang, into out (32 bytes); how many bytes: bytes under 1000, then one decimal of kB, MB or GB.
- `date_text(secs as i64, out as *u8) as i64` — Seconds since 1970 as "YYYY-MM-DD HH:MM", in UTC, into out (16 bytes): the days turned into a date as the proleptic Gregorian calendar has them.

## `ui/dnd.cst`

what a drag carries.

- `text(s as *u8, n as i64, actions as i32) as Payload` — Text, n bytes of s, to be done with as actions says.
- `files(s as *u8, n as i64) as Payload` — n paths, back to back at s, each zero-terminated: files from outside, which are only ever copied.
- `item(t as *tree.Tree, w as i64, i as i64, actions as i32) as Payload` — Item i of widget w in t — a row of a list — to be done with as actions says.
- `file_at(p as *Payload, i as i64) as *u8` — The i-th path, zero-terminated; null for none.
- `add_content(p as *Payload, c as *clip.Content) as void` — c's formats offered too, c the maker's until DRAG_END.
- `content(c as *clip.Content, actions as i32) as Payload` — Content of several formats, c the maker's until DRAG_END, to be done with as actions says.
- `set_image(p as *Payload, argb as *u8, w as i64, h as i64, hx as i64, hy as i64) as void` — The image under the pointer: argb, w by h pixels of 0xAARRGGBB, the maker's until the drag begins, the pointer at hx, hy of it.
- `set_no_image(p as *Payload) as void` — No image under the pointer.
- `set_image_rect(p as *Payload, x as f64, y as f64, w as f64, h as f64) as void` — The part of the widget its image shows, in the widget's own coordinates: a list's row.
- `fill(p as *Payload) as void` — Text made on demand made now, as it is let go.
- `bytes_of(p as *Payload, format as i32, name as *u8, n as i64, len as *i64) as *u8` — The bytes of format — TEXT, PNG, HTML, NAMED by name, n bytes — of what was let go, *len of them, wherever they came from; null for none of it.
- `has_named(p as *Payload, name as *u8, n as i64) as i32` — Whether it offers the type named name, n bytes.
- `take_named(p as *Payload, name as *u8, n as i64) as void` — The named type a target takes of it, as it answers DRAG_OVER with NAMED.
- `named_taken(p as *Payload, n as *i64) as *u8` — The named type taken — take_named's, else the first it offers but text/html — zero-terminated, *n its length; null for none.

## `ui/event.cst`

what the toolkit delivers to a widget.

- `make(kind as i32, time as i64) as Event`

## `ui/flatten.cst`

a tree's rows as a list: those shown, one under another.

- `open(src as *model.Model) as *Flat` — The rows of src that show — its top level, all closed — as a flat model (model_of).
- `close(f as *Flat) as void`
- `model_of(f as *Flat) as *model.Model` — What a list view reads: the rows shown, under parent 0.
- `source(f as *Flat) as *model.Model`
- `count(f as *Flat) as i64`
- `depth(f as *Flat, i as i64) as i32` — Row i's depth, 0 at the top; -1 past the rows.
- `is_expanded(f as *Flat, i as i64) as i32`
- `expandable(f as *Flat, i as i64) as i32` — Whether row i has children to show, asked of the tree.
- `parent_index(f as *Flat, i as i64) as i64` — The row row i is under, -1 at the top.
- `index_of(f as *Flat, id as i64) as i64` — Where row id shows, -1 when it does not.
- `expand(f as *Flat, i as i64) as i32` — Row i opened: its children read and shown under it, closed.
- `collapse(f as *Flat, i as i64) as i32` — Row i closed: every row under it gone.
- `toggle(f as *Flat, i as i64) as i32`

## `ui/geom.cst`

the sizes layout trades in, and what each widget carries for it.

- `size_zero(s as *Size) as void`
- `constraint(w as f64, h as f64) as Constraint`
- `unbounded() as Constraint`
- `item_default() as Item`

## `ui/grid.cst`

rows and columns at once.

- `create(t as *tree.Tree, parent as i64) as i64`
- `set_spacing(t as *tree.Tree, w as i64, columns as f64, rows as f64) as void`
- `set_padding(t as *tree.Tree, w as i64, left as f64, top as f64, right as f64, bottom as f64) as void`

## `ui/host.cst`

the application on the screen.

- `open(a as *app.App, dv as *gpu.device.Device, svc as *u8) as *Host` — The application a, shown with dv — the device its text service svc is on — the windows it has open and those to come.
- `close(hs as *Host) as void`
- `native(hs as *Host, h as i64) as *device.Window` — The platform's window for the application's h, null when it has none.
- `parent_handle(hs as *Host, h as i64, out as *u8, cap as i64) as i64` — The window h as another program — a portal's dialog, the application a link opens in — names it to put what it shows above it (device.parent_handle): its length into out (cap bytes), 0 for none.
- `present(hs as *Host, h as i64, token as *u8, n as i64) as i32` — The platform's window of h brought forward and given the keyboard — as a later start of the program asks (instance.cst) — with the token, n bytes, that start came with (instance.token; n 0 for none): the composito...
- `shown(hs as *Host) as i64` — How many windows are on the screen.
- `watch(hs as *Host, fd as i64, cb as WatchFn, user as *u8) as i32` — fd watched: cb(user, fd) when it is readable.
- `watched(hs as *Host) as i64` — How many descriptors are watched.
- `unwatch(hs as *Host, fd as i64) as void`
- `every_round(hs as *Host, cb as RoundFn, user as *u8) as i32` — cb(user) once every round, after the events: the accessibility bridge's pump.
- `set_on_close(hs as *Host, cb as CloseFn, user as *u8) as void` — What is asked when the platform asks to close a window nothing in it kept open; none set: it closes.
- `post(hs as *Host, cb as RoundFn, user as *u8) as i32` — cb(user) run by the loop, in its thread, as soon as it can: from any thread.
- `hold(hs as *Host) as void` — The loop going on with no window on the screen — a program living in the tray, its window hidden — until release is called as often.
- `release(hs as *Host) as void`
- `quit(hs as *Host) as void` — The loop to end after this round.
- `set_settings(hs as *Host, s as *wsettings.Settings) as void` — The desktop's settings, from what the program reads beside the platform — the settings portal, read whole each time — laid over what the platform says, now and after the platform's are read again.
- `settings(hs as *Host) as *wsettings.Settings` — The desktop's settings as the windows follow them.
- `feed(hs as *Host, h as i64, e as *wevent.Event, now as i64) as void` — One event the platform said of the application's window h, given to it as if it came from there now.
- `feed_input(hs as *Host, h as i64, e as *ie.Event, now as i64) as void` — And one thing a person did there, as the platform's translator says it.
- `pointer_shape(hs as *Host, h as i64) as i32` — The pointer's shape over h, as last set; 0 before any.
- `popup_pointer_shape(hs as *Host, h as i64, s as i64) as i32` — The pointer's shape over the popup of h's surface s, as last set; 0 before any, or when s has no popup.
- `busy(hs as *Host) as i32` — Whether a window has something to paint or show.
- `step(hs as *Host, timeout_ms as i64) as i32` — One round: the windows put up and away as the application's are, every event given to it, what was posted run, the timers, the round's callbacks, what changed painted and shown — then asleep until something is to be...
- `run(hs as *Host) as void` — Rounds until no window is left on the screen and nothing holds the loop, or quit is asked.

## `ui/host_clip.cst`

the windows' clipboard: text and content of several formats copied, a paste in the formats a widget or the program takes, the clipboard followed.


## `ui/host_drag.cst`

a drag between the program and the platform.


## `ui/host_paint.cst`

a window of the application's painted and shown: only what changed, as the damage says, into its texture and onto the screen.


## `ui/host_platform.cst`

what the platform says of a window, told the application and its router: the pointer, keys, text, focus, size, state, drags and requests to close.


## `ui/host_popups.cst`

the surfaces of a window's tree on the screen: a platform popup (window/device.cst's Config.popup) for each widget the tree shows in a window of its own (tree.surface_open) — a menu, a list dropping down, a tooltip.


## `ui/host_settings.cst`

the desktop's settings, followed: the platform's, the portal's, the program's over them, handed to the application.


## `ui/host_state.cst`

what the parts of the application's loop share: the host, each window it put on the screen, and their limits.


## `ui/host_windows.cst`

the application's windows put on the screen and away: a platform window for each one shown, its frame, its floor.


## `ui/icon_theme.cst`

icons by name from the desktop's icon theme.

- `find(bases as *u8, theme as *u8, name as *u8, size as i64, scale as i64, out as *u8, cap as i64) as i64` — Icon name at size by size logical pixels, scale physical pixels to one, from theme — the colon-separated base directories bases looked in, in order (default_bases) — its file's path into out, cap bytes with the NU...
- `default_bases(home as *u8, hn as i64, data_dirs as *u8, dn as i64, out as *u8, cap as i64) as i64` — The base directories the desktop keeps icons in, colon-separated, into out: home's .icons and .local/share/icons, each of data_dirs (XDG_DATA_DIRS, dn bytes: "" for the usual /usr/local/share:/usr/share) with /icons, ...
- `load(path as *u8) as image.buffer.Image` — The icon at path, a PNG, decoded: RGBA, its width 0 when it could not be.

## `ui/icons.cst`

the toolkit's own icons, drawn from paths.

- `source(id as i32) as *u8` — Icon id's path, in the language above; "" for no such icon.
- `shape(id as i32, p as *path.Path) as i32` — Icon id into p, cleared first, on the 16 grid: 1, or 0 for no such icon.
- `parse(s as *u8, p as *path.Path) as i32` — A path in the language above into p, cleared first: 1, or 0 for nothing.
- `draw(svc as *text.Service, c as *draw2d.Canvas, id as i32, x as f64, y as f64, size as f64, col as math.color.Color) as i32` — Icon id drawn on c at x, y (logical), size by size, in col, through the text service svc — the one the window's text is drawn with.
- `coverage(id as i32, size as i64, out as *u8) as i32` — Icon id as pixels, size by size: how much of each pixel it covers, 0 to 255, a row of size bytes after another, into out (size * size bytes).
- `rasterize(id as i32, size as i64, col as math.color.Color, out as *u32) as i32` — Icon id as a window's icon, size by size, in col: 0xAARRGGBB a pixel, as window.device.set_icon takes it, into out (size * size of them).

## `ui/instance.cst`

one instance of the application.

- `claim(inst as *Instance, c as *dbus.conn.Conn, id as *u8, n as i64, on_activate as ActivateFn, on_open as OpenFn,` — The application id, n bytes, claimed on c: 1 when this is the instance, what it is asked handed to the callbacks (any null) as c is dispatched; 0 when another is — hand it what this start was asked (*_remote) and le...
- `is_primary(inst as *Instance) as i32`
- `path(inst as *Instance) as *u8`
- `path_length(inst as *Instance) as i64`
- `release(inst as *Instance) as void` — No longer the instance: the name and the object let go.
- `token(inst as *Instance) as *u8` — The token the current call came with (activation-token, else desktop-startup-id), zero-terminated; token_length 0 for none.
- `token_length(inst as *Instance) as i64`
- `activate_remote(c as *dbus.conn.Conn, id as *u8, n as i64) as i32` — The instance of id, n bytes, asked to come forward: 1 when it answered.
- `open_remote(c as *dbus.conn.Conn, id as *u8, n as i64, s as *u8, sn as i64, count as i64) as i32` — The instance of id asked to open count uris, back to back at s, each zero-terminated, n bytes in all: 1 when it answered.
- `action_remote(c as *dbus.conn.Conn, id as *u8, n as i64, action as *u8, an as i64) as i32` — The instance of id asked to do the action name (an), with no parameter: 1 when it answered.

## `ui/layout.cst`

measure, place, and the pass that runs them.

- `measure(t as *tree.Tree, w as i64, c as *geom.Constraint, out as *geom.Size) as void` — What w would like; nothing when it is hidden itself.
- `measure_hidden(t as *tree.Tree, w as i64, c as *geom.Constraint, out as *geom.Size) as void` — The same, for a widget that is hidden but still has to be accounted for — a stack sizes itself to its largest page, shown or not.
- `overlay_measure(t as *tree.Tree, w as i64, c as *geom.Constraint, out as *geom.Size) as void` — Every visible child gets the whole area and aligns within it on its own; later children sit above earlier ones.
- `overlay_arrange(t as *tree.Tree, w as i64, r as *math.rect.Rect) as void`
- `place_exact(t as *tree.Tree, w as i64, r as *math.rect.Rect) as void` — w given exactly r — in its parent's coordinates, its edges on physical pixels — whatever its margins, alignment and limits say: a surface where the platform put it.
- `place(t as *tree.Tree, w as i64, area as *math.rect.Rect) as void`
- `run(t as *tree.Tree, w as f64, h as f64) as void` — Lays out the whole tree for a window of w by h logical pixels.
- `min_size(t as *tree.Tree, out as *geom.Size) as void` — The smallest window the content fits in: the root's minimum.
- `distribute(n as i64, size as *f64, mins as *f64, maxs as *f64, stretch as *f64, room as f64) as void` — With room to spare it goes by stretch, and an entry that would pass its maximum takes only what fits and drops out, the rest shared again among the others.
- `rect(t as *tree.Tree, w as i64) as math.rect.Rect`
- `visual_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — The rectangle in window coordinates: the parents' offsets added up.
- `window_rect(t as *tree.Tree, w as i64) as math.rect.Rect`
- `scale(t as *tree.Tree) as f64`
- `set_scale(t as *tree.Tree, s as f64) as void` — How many physical pixels a logical one is — from the window's monitor, and again whenever the window moves to one with another scale.
- `direction(t as *tree.Tree, w as i64) as i32`
- `invalidate(t as *tree.Tree, w as i64) as void`
- `set_margin(t as *tree.Tree, w as i64, left as f64, top as f64, right as f64, bottom as f64) as void`
- `set_align(t as *tree.Tree, w as i64, horizontal as i32, vertical as i32) as void`
- `set_min_size(t as *tree.Tree, w as i64, mw as f64, mh as f64) as void`
- `set_max_size(t as *tree.Tree, w as i64, mw as f64, mh as f64) as void`
- `set_stretch(t as *tree.Tree, w as i64, horizontal as f64, vertical as f64) as void` — The share of spare room a container gives w, per axis: a row reads the horizontal one, a column the vertical one, a grid both.
- `set_cell(t as *tree.Tree, w as i64, row as i32, col as i32, row_span as i32, col_span as i32) as void` — Where w sits in a grid: its row and column, and how many of each it spans.
- `set_direction(t as *tree.Tree, w as i64, dir as i32) as void` — The direction changes how a whole subtree arranges, so the whole subtree is invalidated, not just the path to the root.

## `ui/model.cst`

how lists, trees and tables read the program's data.

- `none() as Value`
- `text(p as *u8, len as i64) as Value`
- `integer(i as i64) as Value`
- `float(f as f64) as Value`
- `boolean(b as i32) as Value`
- `handle(h as i64) as Value`
- `make(self as *u8, count as CountFn, row_id as RowIdFn, cell as CellFn) as Model`
- `count(m as *Model, parent as i64) as i64`
- `row_id(m as *Model, parent as i64, index as i64) as i64`
- `columns(m as *Model) as i64`
- `parent_of(m as *Model, id as i64) as i64`
- `index_of(m as *Model, id as i64) as i64` — The row's position under its parent, or -1 when the model does not know the id.
- `has_children(m as *Model, id as i64) as i32`
- `cell(m as *Model, id as i64, column as i64, role as i32, out as *Value) as i32` — Fills out with the cell's value for role and returns 1, or leaves it V_NONE and returns 0.
- `set_cell(m as *Model, id as i64, column as i64, role as i32, v as *Value) as i32` — 1 if the model took the value, 0 if it refused or is read-only.
- `observe(m as *Model, cb as ObserverFn, user as *u8) as i64` — cb(user, change *Change) is called for every change.
- `unobserve(m as *Model, id as i64) as void`
- `inserted(m as *Model, parent as i64, first as i64, n as i64) as void`
- `removed(m as *Model, parent as i64, first as i64, n as i64) as void`
- `changed(m as *Model, parent as i64, first as i64, n as i64) as void`
- `reset(m as *Model) as void`

## `ui/notify.cst`

the desktop's notifications.

- `open(nt as *Notifier, c as *dbus.conn.Conn, app as *u8, n as i64, on_action as ActionFn, on_closed as ClosedFn,` — Notifications from the application named app (n bytes) over c — told of what is chosen and closed through the callbacks, either null.
- `stop(nt as *Notifier) as void` — No longer told.
- `replace(nt as *Notifier, id as i64, summary as *u8, sn as i64, body as *u8, bn as i64, icon as *u8, inn as i64,` — Shown in place of notification id (0: a new one): summary, body, an icon name of the desktop's theme ("" for none), count actions — a key and what it says, back to back at actions, each zero-terminated, an bytes in ...
- `send(nt as *Notifier, summary as *u8, sn as i64, body as *u8, bn as i64, icon as *u8, inn as i64, actions as *u8,` — A new notification: see replace.
- `close(nt as *Notifier, id as i64) as void` — Notification id taken away.

## `ui/open_uri.cst`

a link opened in the desktop's application for it.

- `local_path(s as *u8, n as i64, out as *u8) as i64` — The path of a file URI of this machine — "file:///p", "file://localhost/p", "file:/p" — its escapes undone, into out (MAX_URI bytes, zero-terminated): its length, or -1 when it names another machine or is not one.
- `open(rq as *Request, c as *dbus.conn.Conn, uri as *u8, n as i64, parent as *u8, pn as i64) as i32` — Uri (n bytes) opened in the desktop's application for it, asked from the window parent (pn bytes, "" for none; see host.parent_handle), over c — the program's session connection, or one not open.
- `how(rq as *Request) as i32` — Who opened it: PENDING, PORTAL, XDG_OPEN or FAILED.

## `ui/paint.cst`

drawing the tree, region by region, through render/'s 2D family.

- `cache_open(d as *dev.Device, l as *draw2d.Limits) as Cache`
- `cache_close(cc as *Cache) as void`
- `cache_used(cc as *Cache, t as *tree.Tree) as i64` — How many entries hold a texture for a widget still alive.
- `options(theme as *style.Theme) as Options`
- `subtree(t as *tree.Tree, w as i64, x as f64, y as f64, rw as f64, rh as f64, c as *draw2d.Canvas, o as *Options) as void` — w and what it holds painted on c, which the caller has begun at rw by rh: the rectangle at x, y of w, in its own coordinates, at c's corner — what a drag shows of the widget it began from.
- `frame(t as *tree.Tree, c as *draw2d.Canvas, o as *Options, st as *Stats) as i64` — Paints every rectangle of the tree's damage region on c, which the caller has begun.
- `frame_surface(t as *tree.Tree, sf as i64, c as *draw2d.Canvas, o as *Options, st as *Stats) as i64` — Paints every rectangle of surface s's damage region — a widget shown in a window of its own (tree.surface_open) — on c, which the caller has begun at the surface's size: what is at the surface's corner in the wind...

## `ui/painter.cst`

what a widget's paint entry is handed.

- `has(p as *Painter, bit as i64) as i32`
- `visible(p as *Painter, r as math.rect.Rect) as i32` — Whether r (window coordinates) reaches the part being repainted — for a widget drawing many things, most of which may lie outside it.

## `ui/popup.cst`

something shown over the window until it is dismissed.

- `create(t as *tree.Tree) as i64` — A new popup, closed, as the last child of the tree's root.
- `set_kind(t as *tree.Tree, p as i64, kind as i32) as void` — What its window is to the platform: surface.MENU, as a popup starts, or surface.LIST for a list dropping down from a control.
- `content(t as *tree.Tree, p as i64) as i64` — What it holds: its first child.
- `is_open(t as *tree.Tree, p as i64) as i32`
- `set_arrow(t as *tree.Tree, p as i64, on as i32) as void` — An arrow pointing at what it is opened against (1), or none (0, at first).
- `arrow_side(t as *tree.Tree, p as i64) as i32` — Which side its arrow is on, positioner.NONE for none.
- `arrow_rect(t as *tree.Tree, p as i64) as math.rect.Rect` — Its arrow's box, in the window: empty for none.
- `placed(t as *tree.Tree, p as i64) as math.rect.Rect` — Where it was put, in the window.
- `open_at(t as *tree.Tree, r as *router.Router, p as i64, rect as math.rect.Rect, anchor as i32, gravity as i32,` — Opened against rect — the window's coordinates — from its anchor point, growing as gravity says, kept inside the room there is; the focus as focus says (FOCUS_SELF, FOCUS_FIRST or KEEP_FOCUS).
- `open_from(t as *tree.Tree, r as *router.Router, p as i64, from as i64, rect as math.rect.Rect, anchor as i32,` — The same, rect what of widget from it is opened against — and placed again when from moves.
- `close(t as *tree.Tree, p as i64) as void` — Closed: the window's input and the focus given back as they were; CLOSED.

## `ui/program.cst`

a program of the toolkit's, started.

- `open(p as *Program, fonts_dir as *u8) as i32` — Started, with the system's fonts — or only those under fonts_dir, when it is not null: a program shipping its own.
- `app(p as *Program) as *app.App`
- `host(p as *Program) as *host.Host`
- `fonts(p as *Program) as *system_fonts.Fonts`
- `conn(p as *Program) as *dbus.conn.Conn` — The session bus connection the loop dispatches; null with no session bus.
- `run(p as *Program) as void` — Rounds until no window is left on the screen and nothing holds the loop, or host.quit is asked.
- `close(p as *Program) as void` — Everything closed, its windows put away; nothing more a second time.

## `ui/router.cst`

where each event goes.

- `settings_default() as Settings`
- `of(t as *tree.Tree) as *Router` — The router lent to t (lend): its window's, for a widget that opens a popup of its own.
- `lend(t as *tree.Tree, r as *Router) as void`
- `init(s as Settings) as Router`
- `inputs(r as *Router) as i64` — How many presses and key presses have come so far: what changes when the user does something, for a tooltip to go away on.
- `hovered(t as *tree.Tree, r as *Router, w as i64) as i32` — Whether w is under the pointer — itself or through a descendant.
- `capture(r as *Router) as i64` — The widget holding the pointer, 0 for none.
- `refresh_hover(t as *tree.Tree, r as *Router, time as i64) as void` — For when widgets moved under a pointer that did not: after a layout pass, a scroll, a widget shown or hidden.
- `pointer_move(t as *tree.Tree, r as *Router, x as f64, y as f64, time as i64) as void`
- `pointer_shape(t as *tree.Tree, r as *Router) as i32` — The pointer's shape where it is, for the window to show: asked of the widget holding a press, wherever the pointer went, or else of the deepest one under it, then up the tree until one has a shape of its own.
- `dismiss(t as *tree.Tree, r as *Router, time as i64) as i32` — The window system took a popup down — a press outside the program's windows, another program taking the pointer, the compositor's popup_done — with no press here to say so: the modal scope hears PRESS_OUTSIDE, as ...
- `pointer_leave(t as *tree.Tree, r as *Router, time as i64) as void` — The pointer left the window.
- `pointer_button(t as *tree.Tree, r as *Router, button as i32, pressed as i32,` — button is 1 left, 2 middle, 3 right; pressed is 1 down, 0 up.
- `pointer_cancel(t as *tree.Tree, r as *Router, button as i32, time as i64) as void` — The press of button is over and clicks nothing: the platform took it — a move or resize it follows from there — or let everything go, and no release will come.
- `scroll(t as *tree.Tree, r as *Router, x as f64, y as f64, dx as f64, dy as f64,` — dx, dy: how far, in logical pixels, or in notches when discrete is 1.
- `scroll_end(t as *tree.Tree, r as *Router, x as f64, y as f64, time as i64) as void` — The fingers that were scrolling lifted — Wayland's axis_stop.
- `gesture(t as *tree.Tree, r as *Router, x as f64, y as f64, kind as i32, phase as i32, fingers as i32,` — A touchpad's gesture (input/event.cst's SWIPE, PINCH, HOLD) at x, y: phase BEGIN, UPDATE or END (event.GESTURE_*), fingers of them, moved dx, dy, its scale since it began and its rotation this update (degrees), an end...
- `repeat_move(t as *tree.Tree, r as *Router, time as i64) as void` — What a held pointer is over moved, the pointer did not — a scroll area following a drag past its edge: the widget holding it hears a move where the pointer is, as if it had moved there.
- `set_source(r as *Router, source as i32) as void`
- `next_due(r as *Router) as i64` — When a pending long press is due, or -1 — for the loop to wake by, since nothing arrives while a finger is held still.
- `tick(t as *tree.Tree, r as *Router, now as i64) as i32` — Fires a long press due by now: the widget that captured the press hears POINTER_CANCEL and loses the pointer, since what it was pressed for is not happening, and CONTEXT_MENU goes where the press was, bubbling, with b...
- `caret_off(r as *Router, now as i64) as i32` — Whether a caret is off at time now.
- `set_scope(t as *tree.Tree, r as *Router, w as i64) as void`
- `set_scope_also(t as *tree.Tree, r as *Router, w as i64, also as i64) as void` — A scope set, with also's subtree let through.
- `restore(t as *tree.Tree, r as *Router, scope as i64, w as i64, ring as i32) as void` — A popup ending: the scope as it was before it — scope, 0 for none — and the focus given back to w, as give_back gives it, in one step, so that nothing else is focused on the way — as set_scope would, finding the...
- `scope(r as *Router) as i64`
- `focus_visible(r as *Router) as i32` — Whether to draw the focus ring: focus last moved by keyboard.
- `give_back(t as *tree.Tree, r as *Router, w as i64, ring as i32) as i32` — The focus given back to w — a popup closing, to what had it before — its ring shown as ring says, as it was then; no move by keyboard, so the widget hears FOCUS_IN as from the pointer, and an entry does not select...
- `set_modifiers(r as *Router, mods as i32) as void` — The modifiers held, as the platform says them with the pointer's events — the state at the moment, which a pointer event has right — from now on.
- `modifiers(r as *Router) as i32`
- `key(t as *tree.Tree, r as *Router, k as i32, mods as i32, pressed as i32,` — A key press goes, in order: 1.
- `text(t as *tree.Tree, r as *Router, s as *u8, len as i64, time as i64) as i32` — Text the user typed, UTF-8 and already composed — dead keys and input methods have done their work below.
- `close_request(t as *tree.Tree, r as *Router, time as i64) as i32` — The platform asked the window to close: CLOSE_REQUEST to the focused widget, bubbling, then to each widget of the window until one takes it — a dialog, which answers as Escape.
- `paste(t as *tree.Tree, r as *Router, w as i64, which as i32, s as *u8, len as i64, time as i64) as i32` — Text asked of the clipboard (tree.request_paste) for widget w, given to it as PASTE when it is still there.
- `paste_as(t as *tree.Tree, r as *Router, w as i64, which as i32, format as i32, name as *u8, n as i64,` — What was asked of the clipboard in one of several formats (tree.request_paste_as) for widget w, given to it as PASTE when it is still there: format the one that came (ui/clip.cst's bits; 0 for none), for NAMED its nam...
- `preedit(t as *tree.Tree, r as *Router, s as *u8, len as i64, cursor as i64, time as i64) as i32` — The input method's composition — s, len bytes, its caret cursor bytes in; empty when it is over — to the focused widget as PREEDIT.
- `preedit_segment(t as *tree.Tree, r as *Router, s as *u8, len as i64, cursor as i64,` — The same, with the part the method is converting — its target segment, seg_start to seg_end bytes in (equal for none) — which the widget shows as selected within the composition.
- `delete_surrounding(t as *tree.Tree, r as *Router, before as i64, after as i64, time as i64) as i32` — An input method can revise text immediately around its caret (for example, autocorrect replacing a just-committed word).
- `add_shortcut(r as *Router, k as i32, mods as i32, cb as ShortcutFn, user as *u8) as i64` — Returns an id for remove_shortcut, or 0 when the table is full.
- `add_shortcut_for(r as *Router, w as i64, k as i32, mods as i32, cb as ShortcutFn, user as *u8) as i64` — One widget w owns: see above.
- `remove_shortcut(r as *Router, id as i64) as void`
- `add_mnemonic(t as *tree.Tree, r as *Router, w as i64, letter as i32) as i32` — Returns 1, or 0 when w is stale or the table is full.
- `remove_mnemonics(r as *Router, w as i64) as void`
- `mnemonics_visible(r as *Router) as i32` — Whether to draw the mnemonic underlines: while Alt is held.
- `begin_drag(t as *tree.Tree, r as *Router, w as i64, p as *dnd.Payload) as i32` — Begins a drag of p from w, which holds the pointer — a widget does on DRAG_START.
- `drag_enter(t as *tree.Tree, r as *Router, p as *dnd.Payload, x as f64, y as f64, time as i64) as i32` — A drag from outside came over the window at x, y: what is taken there, dnd bits, 0 for nothing — the platform's answer.
- `drag_move(t as *tree.Tree, r as *Router, x as f64, y as f64, time as i64) as i32` — It moved to x, y: what is taken there.
- `drag_leave(t as *tree.Tree, r as *Router, time as i64) as void` — It left, or was let go somewhere it was not taken.
- `drag_drop(t as *tree.Tree, r as *Router, x as f64, y as f64, time as i64) as i32` — It was let go at x, y, what it offers now there to be had: what was done with it, dnd.COPY or MOVE, 0 for nothing.
- `drag_update(r as *Router, p as *dnd.Payload) as i32` — A drag from outside has what it offers now — the platform has it only once it is let go: p's data in place of what it said at drag_enter, its offers and actions as they were.
- `drag_payload(r as *Router) as *dnd.Payload` — What a drag begun here carries; null for none.
- `drag_elsewhere(t as *tree.Tree, r as *Router, time as i64) as void` — The drag begun here went over another window: what took it here left.
- `drag_finish(t as *tree.Tree, r as *Router, done as i32, time as i64) as i32` — It was let go over another window, which did done with it (dnd bits, 0 for nothing): over here as if let go here, its source told.
- `drag_gone_out(t as *tree.Tree, r as *Router, time as i64) as void` — Gone: what took it here left, and the pointer no longer runs it.
- `drag_stayed(r as *Router) as void` — The platform would not have it: the pointer's again.
- `drag_is_out(r as *Router) as i32`
- `drag_back_over(t as *tree.Tree, r as *Router, x as f64, y as f64, time as i64) as i32` — Back over this window, at x, y: what is taken there.
- `drag_back_drop(t as *tree.Tree, r as *Router, x as f64, y as f64, time as i64) as i32` — Let go over this window, at x, y: what was done with it.
- `drag_out_done(t as *tree.Tree, r as *Router, done as i32, time as i64) as i32` — The platform says it ended, done (dnd bits, 0 for nothing): its source told.
- `drag_x(r as *Router) as f64` — Where the drag is over this window.
- `drag_y(r as *Router) as f64`
- `drag_active(r as *Router) as i32`
- `drag_forget_content(r as *Router, c as *clip.Content) as void` — What made c going away: a drag still carrying it goes on without it, offering what it has left of its own.
- `drag_carried(r as *Router) as *dnd.Payload` — What a drag over the window carries, whoever began it.
- `drag_source(r as *Router) as i64` — The widget a drag over the window began from, 0 for one from outside.
- `drag_target(r as *Router) as i64` — What takes it where it is, 0 for nothing; and what of it.
- `drag_takes(r as *Router) as i32`
- `add_drop_target(r as *Router, w as i64, offers as i32) as i32` — w taken as a drop target of the program's, of what offers says: a drag offering some of it is taken there, and a drop on it said with signal.DROPPED.
- `remove_drop_target(r as *Router, w as i64) as void`
- `hit(t as *tree.Tree, x as f64, y as f64) as i64` — The deepest visible widget containing the point, searching children last to first because later children are drawn above earlier ones.
- `hover(r as *Router) as i64` — The deepest widget under the pointer, 0 for none.
- `focused(t as *tree.Tree, r as *Router) as i64` — The focused widget, or 0 — also when the one that had focus has since been destroyed, hidden or disabled, or is outside the scope.
- `focus(t as *tree.Tree, r as *Router, w as i64, by_keyboard as i32) as i32` — Gives w the keyboard focus; 0 clears it.
- `reveal_rect(t as *tree.Tree, w as i64, r as *math.rect.Rect) as void` — Asks every ancestor of w to bring the rectangle r (window coordinates) into view — a scroll area moves its content until it shows.
- `reveal(t as *tree.Tree, w as i64) as void`
- `focus_next(t as *tree.Tree, r as *Router, dir as i32) as i32` — Moves focus to the next (dir 1) or previous (dir -1) widget that can take it, in tree order within the scope, wrapping.

## `ui/router_drag.cst`

drags begun in the window, over it and over the program's other windows.


## `ui/router_focus.cst`

the focus: the modal scope, the chain Tab walks, and what is brought into view as it moves.

- `focused(t as *tree.Tree, r as *Router) as i64`
- `focus(t as *tree.Tree, r as *Router, w as i64, by_keyboard as i32) as i32`
- `reveal_rect(t as *tree.Tree, w as i64, r as *math.rect.Rect) as void`
- `reveal(t as *tree.Tree, w as i64) as void`
- `focus_next(t as *tree.Tree, r as *Router, dir as i32) as i32`

## `ui/router_hit.cst`

the widget under a point: hit-testing, delivery to it and up, hover.

- `hit(t as *tree.Tree, x as f64, y as f64) as i64` — The widget under window point x, y; 0 for none.
- `hover(r as *Router) as i64`

## `ui/router_keys.cst`

keys and the text they type: to the focused widget and up, the window's shortcuts, the mnemonics.


## `ui/router_pointer.cst`

the pointer as the platform says it: moves, presses and their counts, the wheel, touches, long presses, the caret's blink.


## `ui/router_state.cst`

what the router's parts share: the router, its settings and limits, and the small walks of the tree they all take.


## `ui/scroll.cst`

a viewport onto content larger than it.

- `create(t as *tree.Tree, parent as i64) as i64` — A new scroll area as the last child of parent; its first child is the content.
- `content(t as *tree.Tree, w as i64) as i64` — The content: the first child, 0 for none.
- `set_policy(t as *tree.Tree, w as i64, horizontal as i32, vertical as i32) as void`
- `set_told(t as *tree.Tree, w as i64, on as i32) as void` — Told to assistive technology as a scroll pane (1, at first), or as nothing (0): a widget's own area, a list's, whose content is told as the widget's.
- `set_bars(t as *tree.Tree, w as i64, style as i32) as void`
- `set_bar_size(t as *tree.Tree, w as i64, thickness as f64) as void`
- `set_min_thumb(t as *tree.Tree, w as i64, length as f64) as void`
- `set_line(t as *tree.Tree, w as i64, step as f64) as void` — How far a notch of the wheel and an arrow key go — the window sets it from the system's lines per notch.
- `line(t as *tree.Tree, w as i64) as f64`
- `min_thumb(t as *tree.Tree, w as i64) as f64`
- `page(t as *tree.Tree, w as i64, axis as i32) as f64` — A page: most of the view, so a line of context stays in sight.
- `offset_x(t as *tree.Tree, w as i64) as f64`
- `offset_y(t as *tree.Tree, w as i64) as f64`
- `set_offset(t as *tree.Tree, w as i64, x as f64, y as f64) as i32` — Scrolls to x, y — clamped to the content, on whole physical pixels — ending a glide or a fling where it is.
- `scroll_by(t as *tree.Tree, w as i64, dx as f64, dy as f64) as i32`
- `bar_shown(t as *tree.Tree, w as i64, axis as i32) as i32`
- `view_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — The view: all of the area, less what classic bars take.
- `bar_rect(t as *tree.Tree, w as i64, axis as i32) as math.rect.Rect` — A bar runs along the far edge — the left one for a vertical bar in right to left — and stops short of the corner when both are shown.
- `thumb_rect(t as *tree.Tree, w as i64, axis as i32) as math.rect.Rect`
- `follow(t as *tree.Tree, w as i64, x as f64, y as f64, time as i64) as void` — w, holding the pointer, has it at window x, y: each scroll area holding w follows it past its edge, or stops.
- `scroll_to(t as *tree.Tree, w as i64, item as i64, ax as i32, ay as i32) as i32` — Scrolls w so the widget item inside it is where ax and ay say — START, CENTER, END or NEAREST, START the right in right to left.

## `ui/session.cst`

a session bus connection run by the application's loop.

- `open(s as *Session, hs as *host.Host) as i32` — A session connection, dispatched by hs from now on — s where the program keeps it: 1, or 0 with no session bus.
- `conn(s as *Session) as *dbus.conn.Conn`
- `close(s as *Session) as void`

## `ui/signal.cst`

what widgets tell the program: clicked, changed, toggled.

- `connect(t as *tree.Tree, w as i64, sig as i32, cb as tree.SignalFn, user as *u8) as i64` — Connects cb to signal sig of widget w.
- `on(t as *tree.Tree, w as i64, sig as i32, cb as tree.SignalFn) as i64` — connect with no user pointer: what a program with its state in globals connects most of the time.
- `disconnect(t as *tree.Tree, id as i64) as void` — Disconnects by id.
- `emit(t as *tree.Tree, w as i64, sig as i32, arg as *u8) as i64` — Calls every callback connected to signal sig of w, in connection order.

## `ui/sortmodel.cst`

a flat model's rows in the order of one of its columns.

- `open(src as *model.Model) as *Sort` — src's rows, unsorted, as a flat model (model_of).
- `close(s as *Sort) as void`
- `model_of(s as *Sort) as *model.Model`
- `column(s as *Sort) as i64`
- `order(s as *Sort) as i32`
- `source_index(s as *Sort, i as i64) as i64` — Where the model has the row shown at i, -1 past the rows.
- `sort(s as *Sort, column as i64, order as i32) as void` — Sorted by column, up or down — or unsorted, NONE — and said as a reset.
- `hook(user as *u8, column as i64, order as i32) as void` — sort() by the name a table's header calls: user is the Sort.

## `ui/splitter.cst`

panes divided by handles the user drags.

- `create(t as *tree.Tree, parent as i64, orientation as i32) as i64` — A new splitter as the last child of parent.
- `set_handle_size(t as *tree.Tree, w as i64, thickness as f64) as void`
- `set_grab(t as *tree.Tree, w as i64, reach as f64) as void`
- `set_step(t as *tree.Tree, w as i64, step as f64) as void`
- `step(t as *tree.Tree, w as i64) as f64`
- `current_handle(t as *tree.Tree, w as i64) as i64`
- `handle_count(t as *tree.Tree, w as i64) as i64`
- `handle_rect(t as *tree.Tree, w as i64, i as i64) as math.rect.Rect` — Handle i: the gap between the i-th and the next visible pane, in the splitter's coordinates.
- `position(t as *tree.Tree, w as i64, i as i64) as f64` — Where handle i starts, measured from the start edge.
- `set_position(t as *tree.Tree, w as i64, i as i64, pos as f64) as i32` — Moves handle i to pos, as far as its two neighbours allow: neither below its minimum nor past its maximum.

## `ui/stack.cst`

one page at a time, and children layered on each other.

- `create(t as *tree.Tree, parent as i64) as i64`
- `create_overlay(t as *tree.Tree, parent as i64) as i64`
- `set_current(t as *tree.Tree, s as i64, page as i64) as void` — Shows page and hides every other page.
- `current(t as *tree.Tree, s as i64) as i64` — The current page, or 0 if none was set or it has been destroyed.

## `ui/strings.cst`

the toolkit's own words, in the languages it has them in.

- `language_of(tag as *u8, n as i64) as i32` — The language a BCP 47 tag of n bytes reads in: Portuguese, of any country, Brazil's; anything else English.
- `decimal(lang as i32) as u8` — The mark between a number's whole and its fraction.
- `text(lang as i32, id as i64, n as *i64) as *u8` — Word id in lang, *n bytes of UTF-8 kept for good: "" for no such word.
- `of(t as *tree.Tree, id as i64, n as *i64) as *u8` — Word id in the tree's language.
- `fits(s as *u8, n as i64, room as i64) as i64` — The bytes of s, n of UTF-8, that fit in room, cut where a character starts: never part of one.
- `fill(lang as i32, id as i64, a as *u8, an as i64, b as *u8, bn as i64, out as *u8, cap as i64) as i64` — Word id in lang with %1 and %2 filled by a (an bytes) and b (bn), into out, cap bytes with the NUL; how many bytes, what does not fit left out whole characters at a time.

## `ui/style.cst`

the theme: what every widget's look is made of.

- `hex(v as i64) as math.color.Color` — 0xRRGGBB as written, opaque.
- `hexa(v as i64, alpha as i64) as math.color.Color` — The same with an alpha, 0 to 255.
- `with_alpha(c as math.color.Color, a as f64) as math.color.Color`
- `mix(a as math.color.Color, b as math.color.Color, t as f64) as math.color.Color` — a to b by t, channel by channel, alpha too.
- `over(top as math.color.Color, bottom as math.color.Color) as math.color.Color` — top laid over bottom, as the blending puts it on the screen.
- `luminance(c as math.color.Color) as f64` — Relative luminance, as WCAG defines it: linear light, weighted for the eye.
- `contrast(a as math.color.Color, b as math.color.Color) as f64` — The WCAG contrast ratio, 1 to 21, the same whichever way round.
- `text_on(bg as math.color.Color) as math.color.Color` — Black or white, whichever reads better on bg.
- `pick(v as *Variants, state as i64, checked as i32) as Look` — The look for a painter's state; checked for widgets that are on.
- `finish(th as *Theme) as void` — Every theme ends here: text made readable, the accent's dependants, and the variants derived from the palette.
- `derive(th as *Theme) as void` — The variants from the palette.
- `light() as Theme`
- `dark() as Theme`
- `high_contrast(dark as i32) as Theme` — For low vision: black and white, borders on everything, text at 7 to 1.
- `with_accent(th as *Theme, c as math.color.Color) as void`
- `with_font(th as *Theme, family as *u8, size as f64) as void` — The interface font, and its size for body text: every other size keeps its proportion to it.
- `with_text_scale(th as *Theme, k as f64) as void` — Larger or smaller text everywhere — the platform's text scaling.
- `with_reduced_motion(th as *Theme, on as i32) as void`
- `prefs_default() as Prefs`
- `for_prefs(p as *Prefs) as Theme`
- `overrides() as Overrides`
- `override_color(o as *Overrides, role as i32, c as math.color.Color) as i32` — Sets role to c, replacing what it was set to.
- `override_radius(o as *Overrides, r as f64) as void`
- `override_text_scale(o as *Overrides, k as f64) as void`
- `apply(th as *Theme, o as *Overrides) as void` — th with o laid on it: the roles set, then what depends on them derived again — the accent's dependants when the accent changed, the variants always.

## `ui/surface.cst`

a widget on a layer put against a rectangle of the window: a menu, a list dropping down, a popover, a tooltip.

- `open(t as *tree.Tree, w as i64, kind as i32, grab as i32, from as i64, rect as math.rect.Rect,` — w — a child of the root on a layer — shown against rect, window coordinates, of the widget from (0 for the window itself, rect then in its coordinates as they are): from its anchor point, growing as gravity says, ...
- `open_offset(t as *tree.Tree, w as i64, kind as i32, grab as i32, from as i64, rect as math.rect.Rect,` — The same, moved off_x, off_y from where it would go — the way the positioner moves it, mirrored when it is flipped: a shadow round it reaching past what it is opened against.
- `close(t as *tree.Tree, w as i64) as void` — Put away: its window, when it has one, and what it was anchored to forgotten.
- `is_native(t as *tree.Tree, w as i64) as i32` — Whether it is in a window of its own.
- `rect(t as *tree.Tree, w as i64) as math.rect.Rect` — Where it is, window coordinates: where the platform put it, where it was put inside the window, or where layout has it.
- `anchor_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — What it was last placed against, window coordinates.
- `follow(t as *tree.Tree) as i64` — Every widget placed in the tree and shown, placed again where what it is anchored to has moved or its own size has changed — run after layout, and layout run again when it says so.
- `clear(t as *tree.Tree) as i32` — Whether, in a window of its own, it can draw round itself what shows what is behind it — a shadow, rounded corners — as the host says before it opens.
- `set_input(t as *tree.Tree, w as i64, r as math.rect.Rect) as void` — Where it takes the pointer, window coordinates — its frame, not the shadow round it — told its window when it has one.

## `ui/system.cst`

the desktop's settings, as the toolkit uses them.

- `prefs(s as *wsettings.Settings) as style.Prefs` — The theme's preferences from s.
- `input(s as *wsettings.Settings, base as router.Settings) as router.Settings` — base with the timings s has: the rest of base — long press, which button opens a context menu — is the platform's, not the user's, and kept.

## `ui/system_fonts.cst`

the system's fonts, for the text service.

- `open(sf as *Fonts) as i32` — The system's fonts: 1, or 0 when it has none, and nothing is left open.
- `open_dir(sf as *Fonts, path as *u8) as i32` — The fonts under path, and only those: 1, or 0 when there are none.
- `set_prefs(sf as *Fonts, p as *chain.Prefs) as void` — The families the generic names stand for — sans-serif, monospace, system-ui, emoji, CJK — for chains opened from now on.
- `index_of(sf as *Fonts) as *index.Index`
- `count(sf as *Fonts) as i64` — How many chains are kept.
- `resolve(user as *u8, family as *u8, weight as i64, italic as i32) as *font.Font` — The text service's resolver (text.ResolveFn), user the Fonts: the font for a family — or a list of them, as CSS's font-family; null for the interface's — a weight, 100 to 900, and italic or not.
- `close(sf as *Fonts) as void` — Every chain closed, their files let go, and the index; nothing after an open that found no fonts, or a close.

## `ui/text.cst`

the toolkit's text: one service per application that lays text out and draws it, and runs — what a widget keeps of its text.

- `limits_default() as Limits`
- `style_of(ts as style.TextStyle) as Style` — The theme's text of a role: its family, size and weight.
- `run() as Run`
- `run_close(r as *Run) as void`
- `set_text(r as *Run, text as *u8, n as i64) as i32` — The run's text from here on: n bytes of UTF-8 copied in — from anywhere, the run's own text included — and nothing laid out.
- `text_of(r as *Run) as *u8` — The text, which the caller may change in place before the run is laid out again; set_length shortens it.
- `text_length(r as *Run) as i64`
- `set_length(r as *Run, n as i64) as void`
- `glyph_count(r as *Run) as i64`
- `glyph(r as *Run, i as i64) as *Glyph`
- `face_count(r as *Run) as i64`
- `face(r as *Run, i as i64) as *Face`
- `line_count(r as *Run) as i64`
- `line(r as *Run, i as i64) as *Line`
- `deco_count(r as *Run) as i64`
- `deco(r as *Run, i as i64) as *Deco`
- `color_of(r as *Run, k as i64) as math.color.Color` — Style k's colour, alpha 0 when it has none of its own.
- `underline_at(r as *Run, at as i64, out as *Deco) as i32` — The rectangle under the character at byte offset at — its glyphs', where an underline of it alone would be drawn — into *out: 1, or 0 when no glyph starts there.
- `baseline(r as *Run) as f64` — The first line's baseline, from the run's top; 0 for a run of no lines.
- `of(t as *tree.Tree) as *Service` — A service on device d — null to measure alone — holding what l says, resolving fonts with resolve(user, ...).
- `lend(t as *tree.Tree, s as *Service) as void`
- `open(d as *dev.Device, l as *Limits, resolve as ResolveFn, user as *u8, family as *u8) as *Service`
- `close(s as *Service) as void`
- `set_family(s as *Service, family as *u8) as void` — The interface font a style of no family is set in, from here on.
- `fill_path(s as *Service, c as *draw2d.Canvas, p as *path.Path, t as *raster.flatten.Transform, rule as i32,` — Path p — its points mapped by t into logical units — filled in col on c under rule, through the service's coverage pages as its glyphs are: an icon (icons.cst).
- `next_frame(s as *Service) as void` — A frame begins: what the glyph caches hold for it is kept until the next.
- `font_for(s as *Service, family as *u8, weight as i64, italic as i32) as *font.Font` — The font for a family, weight and style: resolved once and remembered.
- `lay_layout(s as *Service, l as *layout.Layout, lstyles as *model.Style, text as *u8, nbytes as i64,` — The run's text set in styles, each span of it in its style and the rest in style 0, in the box params describes: 1, or 0 when it could not be — a style with no font, more than MAX_STYLES of them, memory refused, or ...
- `lay_text(s as *Service, r as *Run, styles as *Style, nstyles as i64, spans as *model.Span, nspans as i64,`
- `lay(s as *Service, r as *Run, text as *u8, nbytes as i64, styles as *Style, nstyles as i64,` — The text — nbytes of UTF-8, copied into the run — laid out as lay_text does.
- `lay_plain(s as *Service, r as *Run, text as *u8, nbytes as i64, st as *Style, params as *model.Params) as i32` — One style over the whole of the text: the common case.
- `upload(s as *Service) as i32` — Every changed part of every page sent to its texture: 1, or 0 when the device refused one.
- `draw(s as *Service, c as *draw2d.Canvas, r as *Run, x as f64, y as f64, fg as math.color.Color) as i32` — Run r drawn on c with its top left at (x, y), logical pixels, in fg where a style gives no colour of its own.

## `ui/textbuf.cst`

the text an entry or a text view edits.

- `open() as Buf`
- `close(b as *Buf) as void`
- `length(b as *Buf) as i64`
- `chars(b as *Buf) as i64` — How many characters it holds.
- `set_listener(b as *Buf, cb as EditFn, user as *u8) as void` — Who is told every edit, with user; null for nobody.
- `added(b as *Buf) as i64`
- `take_change(b as *Buf, lo as *i64, hi as *i64, delta as *i64) as i32` — What changed since the last time it was asked, for a view of the text to lay out again only that: from *lo — nothing before it moved — to *hi, as the text is now, which is *delta bytes longer than it was.
- `byte_at(b as *Buf, i as i64) as i64` — The byte at offset i, -1 past the text.
- `read(b as *Buf, at as i64, n as i64, out as *u8) as void` — n bytes from at into out.
- `span(b as *Buf, at as i64, n as i64) as *u8` — n bytes from at, in one piece, until the next edit; the gap moved away only when it is inside them.
- `paragraphs(b as *Buf) as i64`
- `para_start(b as *Buf, i as i64) as i64`
- `para_end(b as *Buf, i as i64) as i64` — Where paragraph i ends, its line feed left out.
- `para_of(b as *Buf, at as i64) as i64` — The paragraph offset at is in; a line feed is its paragraph's.
- `char_of(b as *Buf, at as i64) as i64` — The characters before offset at: its paragraph's, then those from its start.
- `byte_of(b as *Buf, k as i64) as i64` — Where character k starts: found in its paragraph; the end past the last.
- `insert(b as *Buf, at as i64, s as *u8, n as i64) as i32` — The program's edits: not remembered, and forgetting what was.
- `delete(b as *Buf, at as i64, n as i64) as i32`
- `replace(b as *Buf, at as i64, n as i64, s as *u8, m as i64) as i32` — n bytes at at made s, m bytes.
- `forget(b as *Buf) as void` — Nothing to undo or redo.
- `can_undo(b as *Buf) as i32`
- `can_redo(b as *Buf) as i32`
- `caret(b as *Buf) as i64` — Where the caret goes, after the last undo or redo.
- `break_group(b as *Buf) as void` — The next edit a step of its own: the caret moved, the selection changed.
- `type_in(b as *Buf, at as i64, s as *u8, n as i64) as i32` — Typed: s, n bytes, in at at — joining the typing before it when it goes on from where that ended and does not start a word.
- `erase(b as *Buf, at as i64, n as i64) as i32` — Erased: n bytes from at — by Backspace when the caret was after them, by Delete when it was at at — joining a run of the same: backspaces each just before the last, deletes each where the last was.
- `erase_ahead(b as *Buf, at as i64, n as i64) as i32` — Erased by Delete, known to be: n bytes from at, the caret at at — joining only deletes forward at the same place, so a backspace before it stays a step of its own and undoing it puts the caret back at at.
- `move(b as *Buf, at as i64, n as i64, to as i64) as i32` — n bytes at at moved to before offset to of the text as it was — a selection dragged — one step: undone, the caret at the bytes' old end; redone, at their new one.
- `change(b as *Buf, at as i64, n as i64, s as *u8, m as i64) as i32` — A change of its own: n bytes at at made s, m bytes — a paste, a replacement, what is typed over a selection.
- `paste_in(b as *Buf, at as i64, s as *u8, n as i64) as i32` — A paste: s, n bytes, in at at, a step of its own.
- `undo(b as *Buf) as i32` — The last step undone: 1, or 0 for none.
- `redo(b as *Buf) as i32` — The last step undone, done again: 1, or 0 for none.

## `ui/timer.cst`

things that happen later: the caret blinking, a tooltip appearing after a pause, a list scrolling while a drag holds at its edge.

- `of(t as *tree.Tree) as *Timers` — The timers lent to t (lend): the application's.
- `lend(t as *tree.Tree, ts as *Timers) as void`
- `init() as Timers`
- `add(ts as *Timers, now as i64, delay as i64, interval as i64, cb as TimerFn, user as *u8) as i64` — Runs cb(user) delay milliseconds after now, then every interval if interval is above 0.
- `cancel(ts as *Timers, id as i64) as void`
- `next_due(ts as *Timers) as i64` — When the earliest pending timer is due, or -1 when none is pending.
- `fire(ts as *Timers, now as i64) as i64` — Runs every timer due at or before now, once each, and returns how many ran.

## `ui/tray.cst`

the application's icon in the desktop's tray.

- `g_category(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_id(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_title(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_status(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_window(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_none(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_icon(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_no_pixels(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `g_pixels(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void` — Its pixels: one image, width, height and ARGB32 in network order — alpha first.
- `g_tooltip(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void` — Its tooltip: the item's icon name, no pixels of its own, a title, what it says.
- `g_not_menu(c as *dbus.conn.Conn, w as *dbus.wire.Writer, user as *u8) as void`
- `m_activate(c as *dbus.conn.Conn, k as *dbus.conn.Call, user as *u8) as void`
- `m_menu(c as *dbus.conn.Conn, k as *dbus.conn.Call, user as *u8) as void`
- `m_secondary(c as *dbus.conn.Conn, k as *dbus.conn.Call, user as *u8) as void`
- `m_scroll(c as *dbus.conn.Conn, k as *dbus.conn.Call, user as *u8) as void`
- `open(tr as *Tray, c as *dbus.conn.Conn, id as *u8, n as i64, title as *u8, tn as i64) as i32` — The item of the application id (n bytes) titled title (tn), on c, kept at tr while it is open — shown once a watcher takes it (shown).
- `set_handlers(tr as *Tray, on_activate as ClickFn, on_menu as ClickFn, on_secondary as ClickFn, on_scroll as ScrollFn,` — Told of clicks and the wheel: any null.
- `shown(tr as *Tray) as i32` — Whether a watcher has it: shown in a panel.
- `service(tr as *Tray) as *u8` — Its bus name.
- `service_length(tr as *Tray) as i64`
- `set_title(tr as *Tray, s as *u8, n as i64) as void`
- `set_status(tr as *Tray, st as i32) as void`
- `set_icon_name(tr as *Tray, s as *u8, n as i64) as void` — An icon of the desktop's theme by name (n bytes; "" for none).
- `set_icon(tr as *Tray, px as *u32, size as i64) as i32` — Its own pixels, size by size ARGB (as app.set_icon takes them; 0 for none), for a panel that has no icon of that name.
- `set_tooltip(tr as *Tray, title as *u8, tn as i64, body as *u8, bn as i64) as void` — What it says when the pointer rests on it: a title and a line or more.
- `close(tr as *Tray) as void` — Taken away: its object and name let go.

## `ui/tree.cst`

widgets as nodes in a window's tree, and the handles to them.

- `limits_default() as Limits`
- `open(l as *Limits) as Tree`
- `close(t as *Tree) as void` — Destroys every widget still alive, children first, then releases the mapping.
- `is_open(t as *Tree) as i32`
- `connection_at(t as *Tree, i as i64) as *Connection`
- `scratch_mark(t as *Tree) as i64` — Where the scratch stack stands now, to hand back to scratch_release.
- `scratch_alloc(t as *Tree, bytes as i64) as *u8` — bytes of scratch, 8-aligned, or null with LAYOUT_TOO_LARGE when there is not enough left.
- `scratch_release(t as *Tree, mark as i64) as void` — Gives back everything taken since mark.
- `root(t as *Tree) as i64`
- `live_count(t as *Tree) as i64`
- `capacity(t as *Tree) as i64`
- `last_error(t as *Tree) as i32`
- `clear_error(t as *Tree) as void`
- `create(t as *Tree, k as *Kind, parent as i64) as i64` — Adds a widget of kind k as the last child of parent.
- `create_before(t as *Tree, k as *Kind, parent as i64, before as i64) as i64` — The same, just before `before`, one of parent's children; 0 is the end.
- `destroy(t as *Tree, w as i64) as void` — Destroys w and everything under it.
- `reparent(t as *Tree, w as i64, new_parent as i64) as i32` — Moves w, with its subtree, to the end of new_parent's children.
- `insert(t as *Tree, w as i64, parent as i64, before as i64) as i32` — Moves w, with its subtree, under parent just before `before` — one of parent's children, or 0 for the end.
- `move_before(t as *Tree, w as i64, before as i64) as i32` — Moves w among its siblings, just before `before`; 0 is the end.
- `parent(t as *Tree, w as i64) as i64`
- `first_child(t as *Tree, w as i64) as i64`
- `last_child(t as *Tree, w as i64) as i64`
- `next_sibling(t as *Tree, w as i64) as i64`
- `prev_sibling(t as *Tree, w as i64) as i64`
- `index_of(t as *Tree, w as i64) as i64` — Where w is among its siblings, from 0; -1 for the root or a stale handle.
- `child_at(t as *Tree, w as i64, i as i64) as i64` — The child of w at index i, 0 when there is none.
- `child_count(t as *Tree, w as i64) as i64`
- `kind(t as *Tree, w as i64) as *Kind`
- `data(t as *Tree, w as i64) as *u8` — The widget's data, zeroed at creation; null for a stale handle or a kind that asked for none.
- `data_of(t as *Tree, w as i64, k as *Kind) as *u8` — The data of w when it is of kind k — what a kind's own functions start from.
- `data_of2(t as *Tree, w as i64, k as *Kind, k2 as *Kind) as *u8` — The same for a module that makes two kinds sharing one data struct — a button and a link, a check box and a radio button.
- `set_visible(t as *Tree, w as i64, on as i32) as void`
- `is_visible(t as *Tree, w as i64) as i32`
- `set_enabled(t as *Tree, w as i64, on as i32) as void`
- `is_enabled(t as *Tree, w as i64) as i32`
- `set_focusable(t as *Tree, w as i64, on as i32) as void` — Whether w can take keyboard focus at all.
- `set_flag(t as *Tree, w as i64, bit as i64, on as i32) as void` — The flags a program sets itself — F_FOCUS_WITHIN_LOOK, F_CACHE and F_NO_INPUT — and F_CACHE_DIRTY, which damage.cst and paint.cst keep.
- `is_textured(t as *Tree, w as i64) as i32`
- `stale_textures(t as *Tree, w as i64) as void` — w's texture, if it has one, and every one around it, are to be painted again — damage.cst's, when something inside changed.
- `has_flag(t as *Tree, w as i64, bit as i64) as i32`
- `is_focusable(t as *Tree, w as i64) as i32`
- `item(t as *Tree, w as i64) as *geom.Item` — The Item layout keeps for w, or null for a stale handle.
- `invalidate_subtree(t as *Tree, w as i64) as void` — Marks w, every descendant and every ancestor.
- `invalidate_layout(t as *Tree, w as i64) as void`
- `needs_layout(t as *Tree, w as i64) as i32`
- `clear_layout(t as *Tree, w as i64) as void`
- `invalidate_paint(t as *Tree, w as i64) as void`
- `invalidate_paint_subtree(t as *Tree, w as i64) as void` — w and everything under it — for a change every descendant shows, such as being disabled.
- `needs_paint(t as *Tree, w as i64) as i32`
- `clear_paint(t as *Tree, w as i64) as void`
- `is_enabled_here(t as *Tree, w as i64) as i32` — Its own disabled flag, ancestors aside; painting walks down and carries theirs.
- `is_hidden_here(t as *Tree, w as i64) as i32` — Its own hidden flag, ancestors aside; damage.cst walks down and carries theirs.
- `painted(t as *Tree, w as i64) as math.rect.Rect`
- `placed(t as *Tree, w as i64) as math.rect.Rect`
- `set_painted(t as *Tree, w as i64, at as math.rect.Rect, shown as math.rect.Rect) as void` — Where w was laid out, and the part of that on screen.
- `set_layer(t as *Tree, w as i64, layer as i64) as void`
- `set_text_service(t as *Tree, s as *u8) as void` — The text service (ui/text.cst) every widget that shows text lays it out with: one per application, so a form of fifty labels holds one shaper.
- `set_theme(t as *Tree, th as *style.Theme) as void` — th must outlive the tree, or be replaced first; null goes back to the default.
- `set_overrides(t as *Tree, w as i64, o as *style.Overrides) as void` — Lays o over the theme for w and everything under it; null removes it.
- `set_opacity(t as *Tree, w as i64, a as f64) as void` — From 0, not drawn, to 1, opaque; outside that, the nearer end.
- `is_valid(t as *Tree, w as i64) as i32` — --- Walking the tree ---
- `damage_add(t as *Tree, r as *math.rect.Rect) as void` — Adds r to what has to be repainted.
- `damage_clip(t as *Tree, r as math.rect.Rect) as void` — Keeps only what lies inside r — the window, at collection.
- `damage_count(t as *Tree) as i64`
- `damage_at(t as *Tree, i as i64) as math.rect.Rect`
- `paint_wanted(t as *Tree) as i32` — Whether anything may need repainting: damage added, a widget marked, a layout invalidated, a widget destroyed.
- `want_paint(t as *Tree) as void` — For a change the tree cannot see: layout moved something, a scale changed.
- `damage_clear(t as *Tree) as void` — After the frame is presented: nothing is owed any more.
- `blink(t as *Tree, w as i64) as void` — The caret of w — the widget text is typed into — blinked: drawn again where it is when the frame is collected, as w's kind says (Kind.caret), or all of w when it says nothing.
- `blinked(t as *Tree) as i64` — The widget whose caret blinked since the last frame; 0 for none.
- `set_moves(t as *Tree, on as i32) as void` — Whether the window the tree is drawn in moves pixels as shift says before painting the damage over them — as ui/host.cst does.
- `moves(t as *Tree) as i32`
- `shift_request(t as *Tree, w as i64, dx as f64, dy as f64, keep as *math.rect.Rect) as void` — Widget w's content moved dx, dy, logical pixels: the pixels of keep — in w's own coordinates, what of it shows the content, not its bars — may be moved rather than painted again.
- `shift_asked(t as *Tree, dx as *f64, dy as *f64, keep as *math.rect.Rect) as i64` — The widget whose ask damage.collect is to weigh, with how far and what may move; 0 when none asked, or more than one did.
- `set_shifted(t as *Tree, ok as i32, at as math.rect.Rect) as void` — damage.collect's answer: the window's rectangle at to be moved, or none.
- `shift(t as *Tree, r as *math.rect.Rect, dx as *f64, dy as *f64) as i32` — After collect: 1 when the pixels of *r — window coordinates, logical pixels — are to be moved dx, dy before the damage is painted over them.
- `set_shift_surface(t as *Tree, s as i64) as void` — Which window's pixels shift moves: the surface the scroll area is in, -1 for the window itself.
- `shift_surface(t as *Tree) as i64` — Which window's pixels the shift asked moves, as set_shift_surface said.
- `has_layered(t as *Tree) as i32` — Whether any widget is on a layer above 0, shown or not.
- `layer(t as *Tree, w as i64) as i64`
- `layered_count(t as *Tree) as i64` — How many widgets are on a layer above 0; with none, painting and hit-testing skip looking for them.
- `set_surfaces(t as *Tree, cb as SurfaceFn, user as *u8) as void` — The host's function the tree asks to open, move and close its surfaces' windows (cb(user, what, s)); a null one keeps every widget in the window.
- `has_surfaces(t as *Tree) as i32` — Whether a host lends the tree its windows.
- `set_surfaces_clear(t as *Tree, on as i32) as void` — Whether the host's windows for surfaces show what is behind where they are not painted — a compositor runs — so a widget can draw a shadow and rounded corners round itself: known before it opens one, as the host s...
- `surfaces_clear(t as *Tree) as i32` — What set_surfaces_clear said, where a host lends windows at all.
- `surface_open(t as *Tree, w as i64, kind as i32, grab as i32, from as i64, place as *positioner.Positioner) as i64` — w shown in a window of its own — see tree_surface.cst's open — laid out and painted there from now on: its surface, or -1 when it stays in the window.
- `surface_close(t as *Tree, w as i64) as void` — Its window put away; shown again, it is inside the window.
- `surface_index(t as *Tree, w as i64) as i64` — The surface w is the widget of, -1 for none.
- `surface_of(t as *Tree, w as i64) as i64` — The surface w is drawn in, -1 for the window.
- `surface_widget(t as *Tree, s as i64) as i64` — Surface s's widget; 0 for a free surface.
- `surface_kind(t as *Tree, s as i64) as i32` — What its window is to the platform: SURFACE_MENU, _LIST or _TOOLTIP.
- `surface_grab(t as *Tree, s as i64) as i32` — Whether its window takes the pointer and keyboard for itself while open.
- `surface_set_grab(t as *Tree, s as i64, on as i32) as void` — The host says its window holds no grab after all: the platform refused it.
- `surface_owner(t as *Tree, s as i64) as i64` — The surface it opened against, -1 for the window.
- `surface_place(t as *Tree, s as i64) as positioner.Positioner` — Against what of the window — its anchor rectangle in window coordinates, logical pixels — and how, its size the widget's own.
- `surface_rect(t as *Tree, s as i64) as math.rect.Rect` — Where the platform put it, window coordinates: outside the window, larger than it, as the platform allows.
- `surface_set_rect(t as *Tree, s as i64, r as math.rect.Rect) as void` — The host says where it went; laid out and painted there.
- `surface_native(t as *Tree, s as i64) as i64` — The host's own handle for its window, as set_native gave it; 0 before.
- `surface_set_input(t as *Tree, s as i64, r as math.rect.Rect) as void` — Where surface s's widget takes the pointer — its frame, not its shadow — in window coordinates: its window's input region.
- `surface_input(t as *Tree, s as i64, r as *math.rect.Rect) as i32` — See surface_set_input.
- `surface_set_native(t as *Tree, s as i64, n as i64) as void` — The host's own handle for its window, kept for it: its popup's slot.
- `surface_see_through(t as *Tree, s as i64) as i32` — Whether what it leaves unpainted shows what is behind it — rounded corners, a shadow — as the host says its window allows.
- `surface_set_see_through(t as *Tree, s as i64, on as i32) as void` — What the host says of its window: whether it shows what is behind it.
- `placed_widget(t as *Tree, s as i64) as i64` — Every widget placed against a rectangle, shown by the host or inside the window — what ui/surface.cst places again as what it is anchored to moves: entries 0 ..
- `placed_native(t as *Tree, s as i64) as i32` — Whether entry s is shown by the host, not inside the window.
- `placed_place(t as *Tree, s as i64) as positioner.Positioner` — How entry s was placed: its positioner, its anchor rectangle where it was.
- `placed_anchor(t as *Tree, s as i64) as math.rect.Rect` — Its anchor rectangle where it is now — what it is anchored to may have moved — in window coordinates.
- `placed_from(t as *Tree, s as i64) as i64` — The widget entry s is anchored to; 0 for the window itself.
- `placed_kind(t as *Tree, s as i64) as i32` — What entry s is: SURFACE_MENU, _LIST or _TOOLTIP.
- `placed_grab(t as *Tree, s as i64) as i32` — Whether entry s takes the pointer and keyboard for itself while open.
- `placed_rect(t as *Tree, s as i64) as math.rect.Rect` — Where entry s is, window coordinates, as last placed.
- `placed_set_rect(t as *Tree, s as i64, r as math.rect.Rect) as void` — Where entry s was placed inside the window.
- `placed_entry(t as *Tree, w as i64) as i64` — The entry w has, shown by the host or not; -1 for none.
- `surface_damage_count(t as *Tree, s as i64) as i64` — Surface s's own damage region, as the window's is: window coordinates.
- `surface_damage_at(t as *Tree, s as i64, i as i64) as math.rect.Rect` — Its i-th rectangle.
- `surface_damage_clip(t as *Tree, s as i64, r as math.rect.Rect) as void` — Its region cut to r: what lies outside its window is not drawn.
- `damage_add_to(t as *Tree, s as i64, r as *math.rect.Rect) as void` — r to repaint in surface s — the window for -1.
- `damage_add_for(t as *Tree, w as i64, r as *math.rect.Rect) as void` — r to repaint wherever w is drawn.
- `text_service(t as *Tree) as *u8`
- `set_animator(t as *Tree, a as *u8) as void` — The window's animator (anim.cst), for widgets that move on their own — a switch's knob sliding, a progress bar's pulse.
- `animator(t as *Tree) as *u8`
- `set_timers(t as *Tree, ts as *u8) as void` — The application's timers (timer.cst), for widgets that act after a pause — a menu opening a submenu the pointer rests on.
- `timers(t as *Tree) as *u8`
- `set_router(t as *Tree, r as *u8) as void` — The window's router (router.cst), for a widget that opens a popup of its own — an entry its context menu — which takes the window's input while it is open.
- `router(t as *Tree) as *u8`
- `request_focus(t as *Tree, w as i64) as void` — A widget's event entry gets the tree, not the router, yet some must move the focus: arrows within a group of radio buttons.
- `focus_request(t as *Tree) as i64`
- `take_focus_request(t as *Tree) as i64` — The focus asked for, and no longer.
- `set_clipboard(t as *Tree, cb as ClipboardFn, user as *u8) as void`
- `copy(t as *Tree, which as i32, text as *u8, n as i64) as void` — n bytes of UTF-8 at text to which, CLIPBOARD or PRIMARY; the program copies them before it returns.
- `set_paste(t as *Tree, cb as PasteFn, user as *u8) as void` — Text a widget wants pasted is asked of the program, which fetches it — a clipboard answers when it answers — and hands it to the router (router.paste), which gives it to the widget as PASTE.
- `request_paste(t as *Tree, which as i32, w as i64) as void` — w wants the text of which, CLIPBOARD or PRIMARY.
- `set_clipboard_content(t as *Tree, cb as ContentFn, user as *u8) as void` — Content of several formats — ui/clip.cst's: text, HTML, an image, files, a program's own types, each its bytes or made on demand — is handed to the program, which puts it on the platform's clipboard, copied before...
- `copy_content(t as *Tree, which as i32, c as *clip.Content) as void` — c to which, CLIPBOARD or PRIMARY.
- `set_paste_as(t as *Tree, cb as PasteAsFn, user as *u8) as void` — A paste in one of several formats is asked of the program, which asks the clipboard which it has, fetches the first it has of those — the program's own type, HTML, an image, text — and hands it to the router (rout...
- `request_paste_as(t as *Tree, which as i32, w as i64, formats as i32, name as *u8, n as i64) as void` — w wants which in one of formats, clip bits (NAMED: the type named name, n bytes).
- `request_paste_plain(t as *Tree, which as i32, w as i64) as void` — w, a text field, wants which as text: the text it holds, else the text of the HTML it holds (clip.PLAIN), heard as TEXT.
- `clipboard_lent(t as *Tree) as i32` — Whether the platform's clipboard is there behind it: functions set.
- `set_clipboard_offered(t as *Tree, formats as i32) as void` — What the clipboard offers, said by the program as it changes (clip bits).
- `clipboard_has(t as *Tree, format as i32) as i32` — Whether the clipboard offers format (clip bits: any of them); 1 while the program has not said — a paste then finds out.
- `set_text_area(t as *Tree, w as i64, r as math.rect.Rect) as void` — The widget text is being typed into says where its caret is, in the window: the input method puts its candidates there (an X input method's spot, text-input-v3's cursor rectangle, IMM32's composition window), and know...
- `text_area(t as *Tree) as i64` — The widget text is typed into, 0 for none — also once it is gone.
- `text_area_rect(t as *Tree) as math.rect.Rect`
- `set_ime_reset(t as *Tree, cb as ImeResetFn, user as *u8) as void` — A widget whose caret left the input method's composition — a click, a key, its text set — gives the composition up, and the input method has to as well: what it committed next would be what was given up.
- `reset_ime(t as *Tree, w as i64) as void` — w gave its composition up.
- `set_window_ops(t as *Tree, cb as WindowOpFn, user as *u8) as void` — What the window does when a widget asks — moved, maximized, its menu — the program passing it on to the platform (_NET_WM_MOVERESIZE, xdg_toplevel.move, WM_NCLBUTTONDOWN...).
- `window_op(t as *Tree, op as i32, x as f64, y as f64) as i32` — A widget asks the window for op, at window x, y.
- `set_watch(t as *Tree, cb as WatchFn, user as *u8) as void` — Who keeps the window this tree is in — the application — and how it names the window, for what a widget asks of it: a window of its own for a dialog, shown or hidden (HOST_*).
- `notify(t as *Tree, w as i64, what as i32) as void` — Whoever watches told what happened to w.
- `notify_text(t as *Tree, w as i64, what as i64, at as i64, s as *u8, n as i64) as void` — Whoever watches told w's text changed: n bytes of s in at at, or out — TEXT_INSERTED or TEXT_DELETED.
- `set_language(t as *Tree, lang as i32) as void` — The language its widgets speak their own words in: a language of ui/strings.cst.
- `language(t as *Tree) as i32`
- `text_change(t as *Tree) as *TextChange` — The change being told, while WATCH_TEXT is; null otherwise.
- `set_host(t as *Tree, cb as HostFn, user as *u8, window as i64) as void`
- `host_id(t as *Tree) as i64` — How the application names this tree's window, 0 for none: a window closed and its slot taken again is named anew, so a widget holding on to another tree can tell it is still the one it knew.
- `ask_host(t as *Tree, op as i32, arg as i64) as i64` — A widget asks op of the application, with arg: its answer, or 0 with no one there to ask.
- `set_labelled_by(t as *Tree, w as i64, label as i64) as void` — label names w from now on; 0 for none.
- `set_tab_group(t as *Tree, w as i64, g as i64) as void` — w one of the widgets of Tab group g — any number but 0, the same for all of them; 0 for none.
- `tab_group(t as *Tree, w as i64) as i64`
- `labelled_by(t as *Tree, w as i64) as i64` — The widget that names w, or 0 — also once that widget is gone.
- `theme(t as *Tree) as *style.Theme`
- `overrides(t as *Tree, w as i64) as *style.Overrides`
- `set_translation(t as *Tree, w as i64, x as f64, y as f64) as void` — Translation moves a widget and everything under it from where layout put it: painted there, hit there, its rectangle in the window there — and laid out exactly as before, which is what lets an animation slide someth...
- `translation_x(t as *Tree, w as i64) as f64`
- `translation_y(t as *Tree, w as i64) as f64`
- `opacity(t as *Tree, w as i64) as f64`

## `ui/tree_draw.cst`

what a frame repaints and where things are drawn: the damage region, what moved, the layers above the rest, each widget's translation and opacity.

- `damage_add(t as *Tree, r as *math.rect.Rect) as void` — The window's region.
- `damage_clip(t as *Tree, r as math.rect.Rect) as void`
- `damage_add_to(t as *Tree, s as i64, r as *math.rect.Rect) as void` — Surface s's region; the window's for -1, or for a surface not open.
- `surface_damage_clip(t as *Tree, s as i64, r as math.rect.Rect) as void` — Surface s's region cut to r.
- `surface_damage_count(t as *Tree, s as i64) as i64` — How many rectangles surface s's region holds.
- `surface_damage_at(t as *Tree, s as i64, i as i64) as math.rect.Rect` — Its i-th rectangle, window coordinates.
- `damage_count(t as *Tree) as i64`
- `damage_at(t as *Tree, i as i64) as math.rect.Rect`
- `paint_wanted(t as *Tree) as i32`
- `want_paint(t as *Tree) as void`
- `damage_clear(t as *Tree) as void`
- `blink(t as *Tree, w as i64) as void`
- `blinked(t as *Tree) as i64`
- `set_moves(t as *Tree, on as i32) as void`
- `moves(t as *Tree) as i32`
- `shift_request(t as *Tree, w as i64, dx as f64, dy as f64, keep as *math.rect.Rect) as void`
- `shift_asked(t as *Tree, dx as *f64, dy as *f64, keep as *math.rect.Rect) as i64`
- `set_shifted(t as *Tree, ok as i32, at as math.rect.Rect) as void`
- `set_shift_surface(t as *Tree, s as i64) as void` — Which window's pixels the shift asked moves: a surface, -1 the window.
- `shift_surface(t as *Tree) as i64` — What set_shift_surface said.
- `shift(t as *Tree, r as *math.rect.Rect, dx as *f64, dy as *f64) as i32`
- `has_layered(t as *Tree) as i32`
- `layer(t as *Tree, w as i64) as i64`
- `layered_count(t as *Tree) as i64`
- `set_translation(t as *Tree, w as i64, x as f64, y as f64) as void`
- `translation_x(t as *Tree, w as i64) as f64`
- `translation_y(t as *Tree, w as i64) as f64`
- `opacity(t as *Tree, w as i64) as f64`

## `ui/tree_lent.cst`

what the tree is lent and keeps for its widgets: the theme, the application's services, the focus asked for, where text is typed and the host's text, which widget names which.

- `text_service(t as *Tree) as *u8`
- `set_animator(t as *Tree, a as *u8) as void`
- `animator(t as *Tree) as *u8`
- `set_timers(t as *Tree, ts as *u8) as void`
- `timers(t as *Tree) as *u8`
- `set_router(t as *Tree, r as *u8) as void`
- `router(t as *Tree) as *u8`
- `request_focus(t as *Tree, w as i64) as void`
- `focus_request(t as *Tree) as i64`
- `take_focus_request(t as *Tree) as i64`
- `set_clipboard(t as *Tree, cb as ClipboardFn, user as *u8) as void`
- `copy(t as *Tree, which as i32, text as *u8, n as i64) as void`
- `set_paste(t as *Tree, cb as PasteFn, user as *u8) as void`
- `request_paste(t as *Tree, which as i32, w as i64) as void`
- `set_clipboard_content(t as *Tree, cb as ContentFn, user as *u8) as void`
- `copy_content(t as *Tree, which as i32, c as *clip.Content) as void`
- `set_paste_as(t as *Tree, cb as PasteAsFn, user as *u8) as void`
- `request_paste_as(t as *Tree, which as i32, w as i64, formats as i32, name as *u8, n as i64) as void`
- `set_clipboard_offered(t as *Tree, formats as i32) as void`
- `clipboard_has(t as *Tree, format as i32) as i32`
- `set_text_area(t as *Tree, w as i64, r as math.rect.Rect) as void`
- `text_area(t as *Tree) as i64`
- `text_area_rect(t as *Tree) as math.rect.Rect`
- `set_ime_reset(t as *Tree, cb as ImeResetFn, user as *u8) as void`
- `reset_ime(t as *Tree, w as i64) as void`
- `set_window_ops(t as *Tree, cb as WindowOpFn, user as *u8) as void`
- `window_op(t as *Tree, op as i32, x as f64, y as f64) as i32`
- `set_watch(t as *Tree, cb as WatchFn, user as *u8) as void`
- `notify(t as *Tree, w as i64, what as i32) as void`
- `notify_text(t as *Tree, w as i64, what as i64, at as i64, s as *u8, n as i64) as void`
- `set_language(t as *Tree, lang as i32) as void`
- `language(t as *Tree) as i32`
- `text_change(t as *Tree) as *TextChange`
- `set_host(t as *Tree, cb as HostFn, user as *u8, window as i64) as void`
- `host_id(t as *Tree) as i64`
- `ask_host(t as *Tree, op as i32, arg as i64) as i64`
- `set_labelled_by(t as *Tree, w as i64, label as i64) as void`
- `set_tab_group(t as *Tree, w as i64, g as i64) as void`
- `tab_group(t as *Tree, w as i64) as i64`
- `labelled_by(t as *Tree, w as i64) as i64`
- `theme(t as *Tree) as *style.Theme`
- `overrides(t as *Tree, w as i64) as *style.Overrides`

## `ui/tree_state.cst`

what the tree's parts share: its types — the tree, its nodes, the kinds of widget and what they are asked — its limits and constants, and the node table's own accessors.

- `is_valid(t as *Tree, w as i64) as i32`

## `ui/tree_surface.cst`

the widgets shown in platform windows of their own: the table of surfaces, what the host is asked, and which surface a widget is drawn in.

- `set_surfaces(t as *Tree, cb as SurfaceFn, user as *u8) as void` — The host's function, asked to open, move and close windows: see tree.cst.
- `has_surfaces(t as *Tree) as i32` — Whether a host lends the tree its windows.
- `entry(t as *Tree, w as i64) as i64` — The entry w has, shown by the host or not; -1 for none.
- `index(t as *Tree, w as i64) as i64` — The surface w is the widget of — shown by the host — -1 for none.
- `surface_of(t as *Tree, w as i64) as i64` — The surface w is drawn in — its own, or its nearest ancestor's: a submenu in a window of its own is drawn there, not in its menu's — or -1 for the window.
- `surface_of_slot(t as *Tree, i as i64) as i64` — The same for a node's slot: what tree.cst has in hand when it damages.
- `open(t as *Tree, w as i64, kind as i32, grab as i32, from as i64, place as *positioner.Positioner) as i64` — w — a widget on a layer — shown in a window of its own, of kind and grabbing as said, against place's anchor rectangle (window coordinates, logical pixels) of the widget from (the window for 0), place's size the w...
- `close(t as *Tree, w as i64) as i32` — w's entry freed, its window put away when it had one: 2 when it had, 1 for an entry inside the window, 0 for none.
- `widget(t as *Tree, s as i64) as i64` — Surface s's widget, shown by the host; 0 for none.
- `kind(t as *Tree, s as i64) as i32` — What its window is to the platform: tst.SURFACE_MENU, _LIST or _TOOLTIP.
- `grab(t as *Tree, s as i64) as i32` — Whether its window takes the pointer and keyboard while open.
- `owner(t as *Tree, s as i64) as i64` — The surface it opened against, -1 for the window.
- `place(t as *Tree, s as i64) as positioner.Positioner` — Its positioner as it was last placed: the anchor rectangle in window coordinates.
- `rect(t as *Tree, s as i64) as math.rect.Rect` — Where the platform put it, window coordinates.
- `set_rect(t as *Tree, s as i64, r as math.rect.Rect) as i32` — Where the platform put it, in window coordinates: 1 when that is new.
- `native(t as *Tree, s as i64) as i64` — The host's handle for its window; 0 before it has one.
- `set_native(t as *Tree, s as i64, n as i64) as void` — The host's handle for its window, kept for it.
- `see_through(t as *Tree, s as i64) as i32` — Whether its window shows what is behind where it is not painted.
- `set_see_through(t as *Tree, s as i64, on as i32) as void` — What the host says of its window's transparency.
- `placed_widget(t as *Tree, s as i64) as i64` — Entry s's widget, shown by the host or inside the window; 0 for a free one.
- `placed_native(t as *Tree, s as i64) as i32` — Whether entry s is shown by the host.
- `placed_place(t as *Tree, s as i64) as positioner.Positioner` — The positioner as it was last placed, its anchor rectangle in window coordinates.
- `placed_anchor(t as *Tree, s as i64) as math.rect.Rect` — The anchor rectangle where it is now, in window coordinates: what it is anchored to moved with it.
- `placed_from(t as *Tree, s as i64) as i64` — The widget entry s is anchored to; 0 for the window.
- `placed_kind(t as *Tree, s as i64) as i32` — What entry s is: tst.SURFACE_MENU, _LIST or _TOOLTIP.
- `placed_grab(t as *Tree, s as i64) as i32` — Whether entry s takes the pointer and keyboard while open.
- `placed_rect(t as *Tree, s as i64) as math.rect.Rect` — Where an entry inside the window was put, window coordinates; a surface's is the host's (rect).
- `placed_set_rect(t as *Tree, s as i64, r as math.rect.Rect) as void` — Where entry s was placed inside the window.
- `set_input(t as *Tree, s as i64, r as math.rect.Rect) as i32` — Where the widget of surface s takes the pointer, window coordinates — its frame, not the shadow round it: what the host gives its window as its input region.
- `input(t as *Tree, s as i64, r as *math.rect.Rect) as i32` — 1 and *r when the widget said where it takes the pointer; 0 for all of it.
- `set_grab(t as *Tree, s as i64, on as i32) as void` — Whether it holds a grab, as its window turned out: the host opened it without one where the platform refused it.

## `ui/widget.cst`

the table of functions every kind of widget fills in.

- `changed(t as *tree.Tree, w as i64, what as i32) as void`
- `kind(name as *u8, data_bytes as i64) as tree.Kind` — A kind with no behaviour yet; the caller sets the entries it needs.
- `theme(t as *tree.Tree, w as i64, out as *style.Theme) as void` — The theme that applies to w: its tree's, with every override from the root down to w laid on in that order, the nearest last.

## `ui/winstate.cst`

a window's size and state kept between runs.

- `state(width as f64, height as f64) as State`
- `path_in(home as *u8, xdg as *u8, app as *u8, an as i64, name as *u8, nn as i64, out as *u8, cap as i64) as i64` — Where window name — nn bytes — of program app — an bytes — is kept, given HOME and XDG_STATE_HOME as home and xdg: into out, cap bytes; how many, 0 when nowhere, a name is not one a file can have, or it does n...
- `write(p as *u8, n as i64, st as *State) as i32` — st written to the file at p, n bytes, its folders made: 1, or 0 when it could not be.
- `read(p as *u8, n as i64, st as *State) as i32` — The file at p, n bytes, read into st: 1; 0 when there is none or it is not as written — st then as it was.
- `save(app as *u8, an as i64, name as *u8, nn as i64, st as *State) as i32` — Window name of program app kept: 1, or 0 when it could not be.
- `load(app as *u8, an as i64, name as *u8, nn as i64, st as *State) as i32` — Window name of program app as it was kept, into st: 1, or 0 with st as it was.

## `ui/widgets/about.cst`

what a program is: its name and version, what it does, where it lives, who made it, and its licence.

- `create(t as *tree.Tree, s as *u8, n as i64) as i64` — A new about dialog for the program named s, n bytes, titled "About" and its name in the tree's language; hidden.
- `set_version(t as *tree.Tree, a as i64, s as *u8, n as i64) as void`
- `set_comments(t as *tree.Tree, a as i64, s as *u8, n as i64) as void`
- `set_copyright(t as *tree.Tree, a as i64, s as *u8, n as i64) as void`
- `set_website(t as *tree.Tree, a as i64, uri as *u8, s as *u8, n as i64) as void` — Its website: uri — the program's, kept alive — shown as s, n bytes.
- `set_logo(t as *tree.Tree, a as i64, tex as i64, w as f64, h as f64) as void` — Its image: texture tex, shown w by h.
- `set_license(t as *tree.Tree, a as i64, s as *u8, n as i64) as void` — The licence's text, s, n bytes: the License button then.
- `add_credits(t as *tree.Tree, a as i64, s as *u8, sn as i64, names as *u8, nn as i64) as void` — Who made it: a section titled s, sn bytes — "Created by", "Documented by", or the program's own — of names, nn bytes, one a line.
- `add_authors(t as *tree.Tree, a as i64, names as *u8, nn as i64) as void` — A section of its authors — "Created by" in the tree's language — names, nn bytes, one a line.
- `add_documenters(t as *tree.Tree, a as i64, names as *u8, nn as i64) as void` — A section of who documented it: "Documented by".
- `page(t as *tree.Tree, a as i64) as i32`
- `name_label(t as *tree.Tree, a as i64) as i64`
- `version_label(t as *tree.Tree, a as i64) as i64`
- `comments_label(t as *tree.Tree, a as i64) as i64`
- `website_link(t as *tree.Tree, a as i64) as i64`
- `copyright_label(t as *tree.Tree, a as i64) as i64`
- `logo_of(t as *tree.Tree, a as i64) as i64`
- `credits_label(t as *tree.Tree, a as i64) as i64`
- `license_label(t as *tree.Tree, a as i64) as i64`

## `ui/widgets/assistant.cst`

a task in steps, one at a time.

- `create(t as *tree.Tree, parent as i64) as i64` — A new assistant, with no steps yet, as the last child of parent.
- `count(t as *tree.Tree, w as i64) as i64`
- `current(t as *tree.Tree, w as i64) as i64`
- `page(t as *tree.Tree, w as i64, i as i64) as i64` — Step i, where its content goes.
- `title(t as *tree.Tree, w as i64) as i64` — The label of the current step's title.
- `back_button(t as *tree.Tree, w as i64) as i64`
- `next_button(t as *tree.Tree, w as i64) as i64`
- `cancel_button(t as *tree.Tree, w as i64) as i64`
- `complete(t as *tree.Tree, w as i64, i as i64) as i32` — Whether step i is complete: Next goes on from it only then.
- `set_current(t as *tree.Tree, w as i64, i as i64) as i32` — Step i shown under its title.
- `add(t as *tree.Tree, w as i64, s as *u8, n as i64) as i64` — A new step, last, titled s — n bytes.
- `set_complete(t as *tree.Tree, w as i64, i as i64, on as i32) as void` — Step i complete (1) or not (0): only a complete step lets Next go on.
- `bind(t as *tree.Tree, r as *router.Router, w as i64) as void` — Enter for Next or Finish, Escape for Cancel, and the buttons' mnemonics.

## `ui/widgets/button.cst`

something to press.

- `kind() as *tree.Kind`
- `create(t as *tree.Tree, parent as i64, s as *u8, n as i64) as i64` — A new button around a label of text — n bytes of UTF-8, an underscore marking its mnemonic's letter — as the last child of parent.
- `create_icon(t as *tree.Tree, parent as i64, tex as i64, w as f64, h as f64) as i64` — A new button around an image of texture tex, w by h; its image's description names it.
- `create_holding(t as *tree.Tree, parent as i64) as i64` — A new button holding nothing yet: what the program makes under it is laid out in it, set_content saying which it is named by.
- `set_content(t as *tree.Tree, b as i64, w as i64) as i32` — What it holds from now on, and is named by: w, a widget under it.
- `link(t as *tree.Tree, parent as i64, s as *u8, n as i64, uri as *u8) as i64` — A new link: text, underlined in the link colour, that says ACTIVATED with uri — the program's, kept alive — when clicked.
- `content(t as *tree.Tree, b as i64) as i64` — What it holds; 0 for a widget that is no button.
- `set_variant(t as *tree.Tree, b as i64, v as i32) as i32` — NORMAL, SUGGESTED, DESTRUCTIVE or FLAT.
- `set_toggle(t as *tree.Tree, b as i64, on as i32) as i32` — A toggle button, which stays down until clicked again; not one, it is up.
- `is_toggle(t as *tree.Tree, b as i64) as i32` — 1 for a toggle button; 0 for one that is not, or for no button.
- `set_active(t as *tree.Tree, b as i64, on as i32) as i32` — A toggle down or up, saying TOGGLED when that changes it.
- `is_active(t as *tree.Tree, b as i64) as i32`
- `is_down(t as *tree.Tree, b as i64) as i32` — Whether it is held down now: by a press the pointer is still over, or by Space.
- `set_default(t as *tree.Tree, r as *router.Router, b as i64) as i32` — The window's default button: it answers Enter wherever the focus is, unless a focused button takes Enter itself, and is drawn in the accent.
- `set_cancel(t as *tree.Tree, r as *router.Router, b as i64) as i32` — The window's cancel button: it answers Escape wherever the focus is.
- `bind_mnemonic(t as *tree.Tree, r as *router.Router, b as i64) as i32` — Alt and its label's mnemonic letter, from now on, focus and click it.

## `ui/widgets/check.cst`

check boxes and radio buttons.

- `create(t as *tree.Tree, parent as i64, s as *u8, n as i64) as i64` — A new check box, off, with a label of text — an underscore marking its mnemonic's letter — as the last child of parent.
- `radio(t as *tree.Tree, parent as i64, s as *u8, n as i64, group as i64) as i64` — A new radio button in group's group — group is any radio of it — or, for 0, in a group of its own, chosen.
- `content(t as *tree.Tree, w as i64) as i64` — Its label; 0 for a widget that is neither.
- `state(t as *tree.Tree, w as i64) as i32`
- `is_down(t as *tree.Tree, w as i64) as i32` — Whether it is held down now: by a press the pointer is still over, or by Space.
- `join(t as *tree.Tree, w as i64, group as i64) as i32` — Radio w into group's group, off: 1, or 0 when either is no radio.
- `set_state(t as *tree.Tree, w as i64, st as i32) as i32` — OFF, ON, or MIXED for a check box; a radio set on is chosen, set off leaves its group with none.
- `set_three_state(t as *tree.Tree, w as i64, on as i32) as i32` — A check box whose clicks go off, on, mixed and round; not, mixed goes on.
- `bind_mnemonic(t as *tree.Tree, r as *router.Router, w as i64) as i32` — Alt and its label's mnemonic letter, from now on, focus and click it.
- `indicator(t as *tree.Tree, w as i64) as math.rect.Rect` — Its indicator, in the window; nothing for a widget that is neither.

## `ui/widgets/colorpicker.cst`

a colour chosen: from a palette, on a plane, or typed as its hex code; in a dialog, or behind a button.

- `swatch_count() as i64`
- `swatch_color(i as i64) as math.color.Color` — Swatch i's colour; transparent past the last.
- `create(t as *tree.Tree, parent as i64) as i64` — A new chooser, white, its palette shown and its alpha not, as the last child of parent.
- `swatch(t as *tree.Tree, w as i64, i as i64) as i64`
- `plane_of(t as *tree.Tree, w as i64) as i64`
- `hue_of(t as *tree.Tree, w as i64) as i64`
- `alpha_of(t as *tree.Tree, w as i64) as i64`
- `entry_of(t as *tree.Tree, w as i64) as i64`
- `preview(t as *tree.Tree, w as i64) as math.rect.Rect` — Where the colour is shown, in the window.
- `selected(t as *tree.Tree, w as i64) as i64` — The swatch of the colour, -1 for none.
- `color(t as *tree.Tree, w as i64) as math.color.Color`
- `hsv(t as *tree.Tree, w as i64, h as *f64, s as *f64, v as *f64) as void`
- `hex(t as *tree.Tree, w as i64, out as *u8) as i64` — Its hex code into out (12 bytes); how many bytes.
- `set_color(t as *tree.Tree, w as i64, c as math.color.Color) as i32` — The colour from now on — opaque without alpha shown — said when it changes it.
- `set_hsv(t as *tree.Tree, w as i64, h as f64, s as f64, v as f64) as i32`
- `set_alpha_shown(t as *tree.Tree, w as i64, on as i32) as void` — The strip of alpha shown, or not: opaque again then.
- `set_palette_shown(t as *tree.Tree, w as i64, on as i32) as void`
- `dialog(t as *tree.Tree, s as *u8, n as i64) as i64` — A new dialog titled s, n bytes, holding a chooser, with Cancel and Select; hidden.
- `chooser_of(t as *tree.Tree, dlg as i64) as i64` — The chooser in dialog dlg; 0 for none.
- `button(t as *tree.Tree, parent as i64, c as math.color.Color) as i64` — A new button showing colour c, as the last child of parent.
- `button_color(t as *tree.Tree, b as i64) as math.color.Color`
- `set_button_color(t as *tree.Tree, b as i64, c as math.color.Color) as void` — Its colour from now on, not said.
- `button_dialog(t as *tree.Tree, b as i64) as i64` — Its dialog, made the first time it is pressed; 0 before.
- `button_dialog_tree(t as *tree.Tree, b as i64) as *tree.Tree`

## `ui/widgets/combo.cst`

one of a list's rows chosen, from a list that drops down.

- `create(t as *tree.Tree, parent as i64, m as *model.Model) as i64` — A new combo box of m's rows, none chosen, as the last child of parent.
- `create_entry(t as *tree.Tree, parent as i64, m as *model.Model) as i64` — A new combo box to type in, of m's rows, empty, as the last child of parent: an entry, a button at its end.
- `entry_of(t as *tree.Tree, w as i64) as i64` — Its entry and its button, for one typed in.
- `button_of(t as *tree.Tree, w as i64) as i64`
- `suggestions(t as *tree.Tree, w as i64) as i64` — How many rows are suggested for what is typed.
- `selected(t as *tree.Tree, w as i64) as i64`
- `selected_id(t as *tree.Tree, w as i64) as i64` — The chosen row's id, 0 for none.
- `text_label(t as *tree.Tree, w as i64) as i64` — The label it shows the chosen row in.
- `set_selected(t as *tree.Tree, w as i64, index as i64) as void` — Row index chosen — -1 for none — shown and said.
- `is_open(t as *tree.Tree, w as i64) as i32`
- `popup_of(t as *tree.Tree, w as i64) as i64`
- `list_of(t as *tree.Tree, w as i64) as i64`
- `open(t as *tree.Tree, w as i64) as void` — Its list opened under it, the chosen row chosen there, the keyboard on it.
- `arrow_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — Its arrow's place, in the window.

## `ui/widgets/decoration.cst`

a window's frame, drawn by the toolkit.

- `create(t as *tree.Tree, parent as i64) as i64` — A new frame filling parent — the tree's root, usually — a title bar across its top and a column under it for what the window holds.
- `header_of(t as *tree.Tree, w as i64) as i64`
- `content_of(t as *tree.Tree, w as i64) as i64`
- `grip(t as *tree.Tree, w as i64, which as i32) as i64` — Edge or corner which's handle; 0 for none.
- `set_maximized(t as *tree.Tree, w as i64, on as i32) as void`
- `is_maximized(t as *tree.Tree, w as i64) as i32`
- `set_fullscreen(t as *tree.Tree, w as i64, on as i32) as void` — Full screen: only what it holds, no title bar.
- `is_fullscreen(t as *tree.Tree, w as i64) as i32`
- `set_flat(t as *tree.Tree, w as i64, on as i32) as void` — Flat (1) where the platform has no shadow of the toolkit's drawn round the window — X11, Windows, or nothing behind showing through: no shadow, no rounded corners of its own, still resized from every edge.
- `is_flat(t as *tree.Tree, w as i64) as i32`
- `set_tiled(t as *tree.Tree, w as i64, sides as i32) as void` — The sides it is tiled to (TILED_*), 0 for none.
- `tiled(t as *tree.Tree, w as i64) as i32`
- `extents(t as *tree.Tree, w as i64) as Extents`
- `geometry(t as *tree.Tree, w as i64) as math.rect.Rect` — The window as the desktop counts it, in the window's coordinates.
- `input(t as *tree.Tree, w as i64) as math.rect.Rect` — Where it takes the pointer: the window, and its edges' reach beyond.

## `ui/widgets/dialog.cst`

a question put to the user, with buttons to answer.

- `set_order(order as i32) as void` — The buttons' order for every dialog from now on.
- `window_tree(t as *tree.Tree) as *tree.Tree` — The tree a new dialog for t's window is made in: a window of its own, belonging to t's, when the application can open one; t itself otherwise, the dialog then drawn inside the window.
- `app_window_tree(t as *tree.Tree) as *tree.Tree` — The same for a dialog of the whole application's: a window of its own belonging to no window — shown modal, every window of the application waits on it, not only t's.
- `ref(t as *tree.Tree, w as i64) as Ref`
- `ref_valid(r as *Ref) as i32` — Whether it is still that dialog, in that window.
- `dispose(r as *Ref) as void` — Gone: its window closed when it has one of its own, destroyed otherwise.
- `create(t as *tree.Tree, s as *u8, n as i64) as i64` — A new dialog titled s, n bytes, hidden, as the last child of the tree's root — filling it in a dialog's window (window_tree), its title then the window's.
- `content(t as *tree.Tree, w as i64) as i64` — Where the program puts what it asks: a column.
- `title_label(t as *tree.Tree, w as i64) as i64`
- `text_label(t as *tree.Tree, w as i64) as i64` — A message box's text.
- `is_shown(t as *tree.Tree, w as i64) as i32`
- `add_button(t as *tree.Tree, w as i64, s as *u8, n as i64, response as i64) as i64` — A button titled s — n bytes, an underscore before its mnemonic — standing for response.
- `button_for(t as *tree.Tree, w as i64, response as i64) as i64` — The button standing for response, 0 for none.
- `set_default_response(t as *tree.Tree, w as i64, response as i64) as void` — The answer Enter gives, its button drawn in the accent.
- `set_secondary(t as *tree.Tree, w as i64, response as i64, on as i32) as i32` — The button of response at the start, after help — as an about dialog's Credits and License are — or back among the rest (on 0).
- `is_secondary(t as *tree.Tree, w as i64, response as i64) as i32`
- `show(t as *tree.Tree, r as *router.Router, w as i64, modal as i32) as void` — Shown in the middle of the window — modal (1), over it dimmed, its input the dialog's — the focus on the first of what it holds.
- `hide(t as *tree.Tree, w as i64) as void` — Hidden, the window's input and the focus given back as they were — or its own window put away; nothing said.
- `set_check(t as *tree.Tree, w as i64, cb as CheckFn, user as *u8) as void` — What looks at each answer before it is given.
- `respond(t as *tree.Tree, w as i64, response as i64) as void` — Answered with response: hidden, then said — unless its check holds the answer back.
- `message(t as *tree.Tree, kind as i32, s as *u8, n as i64, text as *u8, tn as i64, buttons as i32) as i64` — A message box of kind (INFO, WARNING, ERROR, QUESTION) titled s, saying text, with buttons (OK_ONLY, OK_CANCEL, YES_NO, YES_NO_CANCEL, CLOSE_ONLY), the affirmative the default.
- `icon_of(t as *tree.Tree, w as i64) as i64` — A message box's icon widget, and the glyph's label on it; 0 for none.
- `backdrop_of(t as *tree.Tree, w as i64) as i64` — What dims the window while it is shown modal; 0 until it first is.
- `icon_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — A message box's icon, in the window; empty for none.
- `icon_shape(t as *tree.Tree, w as i64) as i32` — The icon a message box shows (icons.cst); 0 for no message box.
- `backdrop_colour(th as *style.Theme) as math.color.Color` — What the window behind a modal dialog is dimmed by.

## `ui/widgets/entry.cst`

one line of text to type in.

- `create(t as *tree.Tree, parent as i64) as i64` — A new, empty entry as the last child of parent.
- `text(t as *tree.Tree, w as i64) as *u8` — Its text, UTF-8, length(t, w) bytes, until it next changes; null for a widget that is no entry.
- `length(t as *tree.Tree, w as i64) as i64`
- `shown(t as *tree.Tree, w as i64) as *u8` — Its text as shown — a password's bullets, a composition — and its length.
- `shown_length(t as *tree.Tree, w as i64) as i64`
- `run(t as *tree.Tree, w as i64) as *txt.Run` — The run it draws its text from.
- `selection(t as *tree.Tree, w as i64, a as *i64, b as *i64) as i32` — The selection: where it began and where the caret is, either way round, byte offsets in its text; 1 when it is not empty.
- `composition(t as *tree.Tree, w as i64, a as *i64, b as *i64) as i64` — The input method's composition shown in it: its length, and the part being converted into *a, *b — bytes into it, equal for none.
- `caret(t as *tree.Tree, w as i64) as i64` — Where the caret is, a byte offset in its text.
- `select(t as *tree.Tree, w as i64, a as i64, b as i64) as void` — Selects a ..
- `x_of(t as *tree.Tree, w as i64, at as i64) as f64` — The window x of the caret before byte at of its text.
- `x_of_shown(t as *tree.Tree, w as i64, at as i64) as f64` — The same for byte at of its text as shown.
- `caret_x(t as *tree.Tree, w as i64) as f64` — The window x of its caret — inside a composition, of the composition's.
- `text_top(t as *tree.Tree, w as i64) as f64` — The window y of the top of its line of text.
- `scroll(t as *tree.Tree, w as i64) as f64` — How far its text is moved left to keep the caret in view.
- `drop_at(t as *tree.Tree, w as i64) as i64` — Where text being dragged would be dropped, a byte offset; -1 when nothing is dragged.
- `set_text(t as *tree.Tree, w as i64, s as *u8, n as i64) as i32` — Its text from now on — a program's, so not filtered and nothing to undo — cut to its limit, the caret at its end.
- `set_placeholder(t as *tree.Tree, w as i64, s as *u8, n as i64) as i32` — What it shows while empty, in the secondary colour.
- `placeholder(t as *tree.Tree, w as i64) as *u8` — Its placeholder, placeholder_length bytes; null for no entry.
- `placeholder_length(t as *tree.Tree, w as i64) as i64`
- `set_password(t as *tree.Tree, w as i64, on as i32) as i32` — A password or not: shown as bullets, never copied.
- `set_max_length(t as *tree.Tree, w as i64, n as i64) as i32` — At most n characters; 0 for no limit.
- `set_filter(t as *tree.Tree, w as i64, f as FilterFn, user as *u8) as i32` — Only the characters f says yes to go in from now on — typed, pasted or dropped; a null f lets everything in.
- `set_editable(t as *tree.Tree, w as i64, on as i32) as i32` — Editable or read-only.
- `set_width_chars(t as *tree.Tree, w as i64, n as i64) as i32` — Room for n characters — n of its digits wide, and no narrower — rather than its own width (n below 0, as it starts: eight of its size by preference, two at least).
- `set_frame(t as *tree.Tree, w as i64, on as i32) as i32` — Drawn with a frame (1, as it starts) or without (0) — its text alone, for a widget that holds it inside a field of its own, as a spin box does.
- `set_activates_default(t as *tree.Tree, w as i64, on as i32) as i32` — Enter the window's default button's (1) or its own (0, as it starts).
- `context_menu(t as *tree.Tree, w as i64) as i64` — Its context menu: 0 until one was asked for, or for no entry.
- `can(t as *tree.Tree, w as i64, id as i64) as i32` — Whether the context menu's action id (textedit.UNDO … SELECT_ALL) can be done now: for a program's own Edit menu, its items enabled as these say — a password never cut or copied.
- `act(t as *tree.Tree, w as i64, id as i64) as i32` — The action id done, as if chosen from its context menu: 1, or 0 when it cannot be done now (can).

## `ui/widgets/expander.cst`

a title that shows or hides what is under it.

- `create(t as *tree.Tree, parent as i64, s as *u8, n as i64) as i64` — A new, closed expander titled s — n bytes, an underscore before its mnemonic — as the last child of parent.
- `content(t as *tree.Tree, w as i64) as i64` — Where what it holds goes: an overlay, each child aligned on its own.
- `title(t as *tree.Tree, w as i64) as i64` — Its title: a label.
- `expanded(t as *tree.Tree, w as i64) as i32`
- `set_expanded(t as *tree.Tree, w as i64, on as i32) as i32` — Open (1) or closed (0).
- `bind_mnemonic(t as *tree.Tree, r as *router.Router, w as i64) as i32` — Alt and its title's mnemonic turn it, from now on.
- `chevron_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — The chevron's square, in the window.

## `ui/widgets/filechooser.cst`

a file, a name to save to, or a folder, chosen.

- `set_home(s as *u8, n as i64) as void` — Home from now on: s, n bytes.
- `set_config_home(s as *u8, n as i64) as void` — Where the user's settings are from now on — the folder of user-dirs.dirs, naming home's folders — s, n bytes; none (n 0): XDG_CONFIG_HOME, or home's .config.
- `create(t as *tree.Tree, mode as i32, s as *u8, n as i64) as i64` — A new chooser of mode — OPEN, SAVE or FOLDER — titled s, n bytes: a dialog, hidden, showing home.
- `set_folder(t as *tree.Tree, fc as i64, p as *u8, n as i64) as i32` — The folder at p, n bytes, shown: 1, or 0 when it cannot be read.
- `folder(t as *tree.Tree, fc as i64) as *u8`
- `folder_length(t as *tree.Tree, fc as i64) as i64`
- `chosen(t as *tree.Tree, fc as i64, out as *u8, cap as i64) as i64` — The path chosen, into out, cap bytes; how many — 0 before an answer.
- `set_name(t as *tree.Tree, fc as i64, s as *u8, n as i64) as void` — The name to save to, s, n bytes.
- `confirm_dialog(t as *tree.Tree, fc as i64) as i64` — The question before replacing a file, made the first time; 0 before.
- `confirm_dialog_tree(t as *tree.Tree, fc as i64) as *tree.Tree`
- `add_filter(t as *tree.Tree, fc as i64, s as *u8, n as i64, patterns as *u8, pn as i64) as i64` — A filter named s showing the files patterns match — parted by zeros, "*.txt\0*.md"; "" for all — at the end of its combo box; the first added is the one used.
- `set_filter(t as *tree.Tree, fc as i64, i as i64) as void` — Filter i the one used.
- `table(t as *tree.Tree, fc as i64) as i64`
- `places(t as *tree.Tree, fc as i64) as i64`
- `up_button(t as *tree.Tree, fc as i64) as i64`
- `location(t as *tree.Tree, fc as i64) as i64` — The entry saying where it is, to type another place in.
- `name_entry(t as *tree.Tree, fc as i64) as i64`
- `filter_combo(t as *tree.Tree, fc as i64) as i64`

## `ui/widgets/fontpicker.cst`

a font chosen: its family, its style, its size.

- `create(t as *tree.Tree, parent as i64, ix as *index.Index) as i64` — A new chooser over ix — the program's, alive as long as it is — as the last child of parent.
- `set_font(t as *tree.Tree, w as i64, s as *u8, n as i64, weight as i64, italic as i32, size as f64) as i32` — The font from now on: the family named s, n bytes, its style nearest weight and italic, size held to 4 to 400; said.
- `family(t as *tree.Tree, w as i64) as *u8`
- `family_length(t as *tree.Tree, w as i64) as i64`
- `weight(t as *tree.Tree, w as i64) as i64`
- `italic(t as *tree.Tree, w as i64) as i32`
- `width(t as *tree.Tree, w as i64) as f64`
- `size(t as *tree.Tree, w as i64) as f64`
- `font(t as *tree.Tree, w as i64, out as *Font) as i32` — The font as a whole into out: 1, or 0 for no chooser.
- `style_name(t as *tree.Tree, w as i64, n as *i64) as *u8` — The chosen style's name, n bytes into *n; null for none.
- `search_of(t as *tree.Tree, w as i64) as i64`
- `families_of(t as *tree.Tree, w as i64) as i64`
- `styles_of(t as *tree.Tree, w as i64) as i64`
- `size_of(t as *tree.Tree, w as i64) as i64`
- `preview_of(t as *tree.Tree, w as i64) as i64`
- `dialog(t as *tree.Tree, s as *u8, n as i64, ix as *index.Index) as i64` — A new dialog titled s, n bytes, holding a chooser over ix, with Cancel and Select; hidden.
- `chooser_of(t as *tree.Tree, dlg as i64) as i64`
- `button(t as *tree.Tree, parent as i64, ix as *index.Index) as i64` — A new button saying a font over ix — the theme's text until set — as the last child of parent.
- `button_label(t as *tree.Tree, b as i64) as i64`
- `button_dialog(t as *tree.Tree, b as i64) as i64`
- `button_dialog_tree(t as *tree.Tree, b as i64) as *tree.Tree` — The tree its dialog is in: a window of its own where the application can open one (dialog.window_tree); null before it is pressed.
- `button_font(t as *tree.Tree, b as i64, out as *Font) as i32`
- `set_button_font(t as *tree.Tree, b as i64, s as *u8, n as i64, weight as i64, italic as i32, size as f64) as i32` — Its font from now on: the family named s, n bytes, the style nearest weight and italic, at size; not said.

## `ui/widgets/group.cst`

a titled frame around related controls.

- `create(t as *tree.Tree, parent as i64, s as *u8, n as i64) as i64` — A new group box titled text — an underscore marking its mnemonic's letter — as the last child of parent; its content is the next child made in it.
- `title(t as *tree.Tree, g as i64) as i64` — Its title; 0 for a widget that is no group box.
- `content(t as *tree.Tree, g as i64) as i64` — What it holds, its child after the title; 0 for none, or for a widget that is no group box.
- `bind_mnemonic(t as *tree.Tree, r as *router.Router, g as i64) as i32` — Alt and its title's mnemonic letter, from now on, focus the first thing in it that can take the focus.
- `frame(t as *tree.Tree, g as i64) as math.rect.Rect` — Its frame, in the window; nothing for a widget that is no group box.

## `ui/widgets/image.cst`

a picture: a texture of the program's.

- `kind() as *tree.Kind`
- `create(t as *tree.Tree, parent as i64, tex as i64, w as f64, h as f64) as i64` — A new image of texture tex, w by h logical pixels, as the last child of parent.
- `create_icon(t as *tree.Tree, parent as i64, id as i32, size as f64) as i64` — A new image of the toolkit's own icon id (icons.cst), size by size logical pixels: symbolic, in the colour it is given.
- `set_icon(t as *tree.Tree, im as i64, id as i32, size as f64) as i32` — The toolkit's own icon id in place of a texture, size by size; 0 for none.
- `icon(t as *tree.Tree, im as i64) as i32`
- `texture(t as *tree.Tree, im as i64) as i64` — Its texture, the program's; 0 for none.
- `set_image(t as *tree.Tree, im as i64, tex as i64, w as f64, h as f64) as i32` — Another texture, of w by h; 0 for none.
- `set_fit(t as *tree.Tree, im as i64, on as i32) as i32` — Scaled to fit what it is given, keeping its proportions; else at its size.
- `set_symbolic(t as *tree.Tree, im as i64, on as i32) as i32`
- `set_smooth(t as *tree.Tree, im as i64, on as i32) as i32` — Filtered between texels when scaled (the default), or the nearest taken — pixel art.
- `set_premultiplied(t as *tree.Tree, im as i64, on as i32) as i32` — Its texture's colours already carry their alpha.
- `set_description(t as *tree.Tree, im as i64, s as *u8, n as i64) as i32` — What it shows, said to assistive technology: n bytes of UTF-8 the program keeps alive.

## `ui/widgets/label.cst`

text that says what something is.

- `kind() as *tree.Kind`
- `create(t as *tree.Tree, parent as i64, s as *u8, n as i64) as i64` — A new label of text — n bytes of UTF-8, copied — as the last child of parent, in the body style.
- `set_text(t as *tree.Tree, w as i64, s as *u8, n as i64) as i32` — Its text from now on, copied: 1, or 0 when memory is refused and it keeps the old.
- `set_text_mnemonic(t as *tree.Tree, w as i64, s as *u8, n as i64) as i32` — Its text with a mnemonic: an underscore before a letter marks it — the first so marked is the mnemonic, underlined while Alt is held — and is not shown; two underscores show one.
- `set_text_marked(t as *tree.Tree, w as i64, s as *u8, n as i64) as i32` — The same text and underline, its letter left for the widget that made the label to bind — a menu's items take their letters without Alt, a tab's with bind_mnemonics.
- `mnemonic(t as *tree.Tree, w as i64) as i32` — Its mnemonic's letter, as set_text_mnemonic found it; 0 for none.
- `bind_mnemonic(t as *tree.Tree, r as *router.Router, w as i64) as i32` — Alt and its mnemonic's letter, from now on, focus and activate the widget it labels — itself when it labels none.
- `text(t as *tree.Tree, w as i64) as *u8` — Its text, until it is next set; null and 0 for a widget that is no label.
- `text_length(t as *tree.Tree, w as i64) as i64`
- `run(t as *tree.Tree, w as i64) as *txt.Run` — How it was last laid out; null for a widget that is no label.
- `set_style(t as *tree.Tree, w as i64, role as i64) as void` — Which of the theme's text styles it is set in: BODY and the rest.
- `set_tone(t as *tree.Tree, w as i64, tone as i64) as void` — Which of the theme's colours it is drawn in: NORMAL and the rest.
- `set_wrap(t as *tree.Tree, w as i64, on as i32) as void` — Broken into lines to fit the width it is given.
- `set_lines(t as *tree.Tree, w as i64, lines as i64, ellipsis as i32) as void` — At most lines lines, 0 for any number; what does not fit ends in an ellipsis when ellipsis is 1.
- `set_align(t as *tree.Tree, w as i64, align as i64) as void` — Where its lines go across: model.ALIGN_START (the default), END, LEFT, RIGHT, CENTER, JUSTIFY.
- `set_spans(t as *tree.Tree, w as i64, styles as *txt.Style, nstyles as i64, spans as *model.Span,` — Spans of its text in other styles: styles[0] for the text outside every span, then what spans name.
- `set_target(t as *tree.Tree, w as i64, target as i64) as void` — The widget it names — for assistive technology, and for its mnemonic — 0 for none.
- `target(t as *tree.Tree, w as i64) as i64`
- `text_left(t as *tree.Tree, w as i64) as f64` — Where its text is drawn in its rectangle, from its left and its top: for what is laid over it, a caret or an input method's window.
- `text_top(t as *tree.Tree, w as i64) as f64`
- `set_selectable(t as *tree.Tree, w as i64, on as i32) as i32` — Selectable or not: 1, or 0 when memory is refused or w is no label.
- `selection(t as *tree.Tree, w as i64, a as *i64, b as *i64) as i32` — The selection: where it began and where the caret is, either way round; 1 when it is not empty.
- `select(t as *tree.Tree, w as i64, a as i64, b as i64) as void` — Selects a ..

## `ui/widgets/listview.cst`

rows read from a model, only those seen made.

- `create(t as *tree.Tree, parent as i64, m as *model.Model) as i64` — A new list view of m's rows — m stays where it is while the list has it — as the last child of parent, choosing one row at a time.
- `set_model(t as *tree.Tree, w as i64, m as *model.Model) as void` — Its rows from now on m's — or none for null — the selection and the cursor gone with the old ones.
- `model_of(t as *tree.Tree, w as i64) as *model.Model`
- `count(t as *tree.Tree, w as i64) as i64`
- `id_at(t as *tree.Tree, w as i64, index as i64) as i64` — The id of the row at index, 0 for none.
- `set_selection_mode(t as *tree.Tree, w as i64, mode as i32) as void`
- `selection_mode(t as *tree.Tree, w as i64) as i32`
- `is_selected(t as *tree.Tree, w as i64, index as i64) as i32`
- `selected_count(t as *tree.Tree, w as i64) as i64`
- `next_selected(t as *tree.Tree, w as i64, from as i64) as i64` — The first row chosen at or after from, -1 for none: from 0, then from one past each, every row chosen in order.
- `select_range(t as *tree.Tree, w as i64, a as i64, b as i64) as void` — Rows a to b, b not among them, chosen — in SINGLE only a, the rest not.
- `select(t as *tree.Tree, w as i64, index as i64) as void`
- `unselect_range(t as *tree.Tree, w as i64, a as i64, b as i64) as void`
- `unselect(t as *tree.Tree, w as i64, index as i64) as void`
- `select_all(t as *tree.Tree, w as i64) as void`
- `unselect_all(t as *tree.Tree, w as i64) as void`
- `cursor(t as *tree.Tree, w as i64) as i64`
- `set_cursor(t as *tree.Tree, w as i64, index as i64) as void` — The keyboard on row index, brought into view; the anchor there too.
- `scroll_to_row(t as *tree.Tree, w as i64, index as i64) as void` — Row index at the top of the view, or as near as it goes.
- `row_height(t as *tree.Tree, w as i64) as f64`
- `row_at(t as *tree.Tree, w as i64, x as f64, y as f64) as i64` — The row at window x, y, -1 for none.
- `set_drag_content(t as *tree.Tree, w as i64, cb as DragContentFn, actions as i32, user as *u8) as i32` — A row pressed and dragged is cb's content from now on (see DragContentFn), to be done with as actions says: dropped on what takes it here, and on other programs once it leaves the program's windows.
- `set_reorderable(t as *tree.Tree, w as i64, on as i32) as void`
- `reorderable(t as *tree.Tree, w as i64) as i32`
- `drop_at(t as *tree.Tree, w as i64) as i64` — Where a row dragged over it would go, before which row (count() for the end); -1 when none is.
- `row_widget(t as *tree.Tree, w as i64, index as i64) as i64` — The widget showing row index, 0 when it is not in view.
- `index_of_row(t as *tree.Tree, rw as i64) as i64` — The row widget rw's row: its index, -1 for none.
- `set_factory(t as *tree.Tree, w as i64, setup as SetupFn, bind as BindFn, user as *u8) as void` — setup(user, t, row) fills each new row widget, bind(user, t, row, id, index) shows a row in one; the rows made so far made again.
- `area(t as *tree.Tree, w as i64) as i64` — The scroll area its rows are in.
- `set_horizontal(t as *tree.Tree, w as i64, on as i32) as void` — Rows wider than the view scrolled across (1), or cut to it (0, at first).
- `set_roles(t as *tree.Tree, w as i64, role as i64, row_role as i64) as void` — Told as role, its rows as row_role — a tree, its tree items — instead of a list of list items; 0 for those.
- `set_table(t as *tree.Tree, w as i64, columns as i64, headers as i64) as void` — Told as a table of columns columns, headed by the children of headers (0 for none).
- `set_row_access(t as *tree.Tree, w as i64, cb as RowAccessFn, user as *u8) as void`
- `set_single_click(t as *tree.Tree, w as i64, on as i32) as void` — A plain click activating its row, as in a list to pick one thing from, or (0, at first) only a double click.
- `go_to(t as *tree.Tree, w as i64, index as i64) as void` — The keyboard to row index as an arrow key would move it there: alone chosen, the anchor there, brought into view.
- `show_text(t as *tree.Tree, l as i64, m as *model.Model, id as i64, column as i64) as void` — Label l showing row id's text in column of m — a number in its digits — or nothing.
- `show_icon(t as *tree.Tree, im as i64, m as *model.Model, id as i64, column as i64) as void` — Image im showing row id's ICON in column of m — one of the toolkit's own icons (an integer) or a texture of the program's (a handle), ICON_SIZE square — or hidden when the row has none.
- `row_icon(t as *tree.Tree, rw as i64) as i64` — The icon of row widget rw, one of the list's own making; 0 for none.
- `digits(v as i64, out as *u8) as i64` — The decimal digits of v into out, 24 bytes; how many.

## `ui/widgets/menu.cst`

a popup menu: a column of items over the window.

- `create(t as *tree.Tree) as i64` — A new menu, closed, as the last child of the tree's root.
- `add_item(t as *tree.Tree, m as i64, s as *u8, n as i64, id as i64) as i64` — An item that is chosen, id its id; its handle, or 0 for no room or m no menu.
- `add_check(t as *tree.Tree, m as i64, s as *u8, n as i64, id as i64, on as i32) as i64` — An item turned over as it is chosen, checked as on says.
- `add_radio(t as *tree.Tree, m as i64, s as *u8, n as i64, id as i64, group as i64, on as i32) as i64` — One of the items of group in m of which one is checked: chosen, it is.
- `add_separator(t as *tree.Tree, m as i64) as i64`
- `add_submenu(t as *tree.Tree, m as i64, s as *u8, n as i64) as i64` — An item opening a submenu; the submenu, to put items in — its item is item_of — or 0.
- `set_accel(t as *tree.Tree, item as i64, k as i32, mods as i32) as i32` — The accelerator item shows at its end — the program's shortcut for it, which it does not bind itself; key 0 shows none.
- `accel_label(t as *tree.Tree, item as i64) as i64`
- `item_label(t as *tree.Tree, item as i64) as i64`
- `id_of(t as *tree.Tree, item as i64) as i64`
- `submenu_of(t as *tree.Tree, item as i64) as i64`
- `item_of(t as *tree.Tree, m as i64) as i64` — The item a submenu is opened from; 0 for a menu the program pops up.
- `checked(t as *tree.Tree, item as i64) as i32`
- `set_checked(t as *tree.Tree, item as i64, on as i32) as i32` — A check or radio item checked or not; a radio item checked unchecks the rest of its group.
- `set_sideways(t as *tree.Tree, m as i64, cb as SideFn, user as *u8) as void` — cb(user, dir) when Left or Right finds nothing to open or close in m or its submenus: a menu bar going on to the next of its menus, or back.
- `set_outside(t as *tree.Tree, m as i64, cb as OutsideFn, user as *u8) as void` — cb(user, x, y) when the pointer moves outside m while it is open: a menu bar opening the menu of the title it reached.
- `is_open(t as *tree.Tree, m as i64) as i32`
- `current(t as *tree.Tree, m as i64) as i64` — The item chosen in m: the one with the focus, or the one whose submenu is open; 0 for none.
- `scroll(t as *tree.Tree, m as i64) as f64` — How far its items are moved up, when it is cut to the window.
- `popup_at(t as *tree.Tree, r as *router.Router, m as i64, rect as math.rect.Rect, anchor as i32, gravity as i32,` — Menu m popped up against rect — the window's coordinates — growing as gravity says from its anchor point, flipped, slid and cut to stay inside the window; its first item chosen when select_first is 1, as when the ...
- `popup_from(t as *tree.Tree, r as *router.Router, m as i64, from as i64, rect as math.rect.Rect, anchor as i32,` — The same, rect what of widget from it is popped up against — a menu bar's title, a menu button — and placed again when from moves.
- `popup_at_pointer(t as *tree.Tree, r as *router.Router, m as i64) as i32` — At the pointer: its corner there, to the right and below — to the left right to left — flipped and slid as popup_at.
- `close(t as *tree.Tree, m as i64) as void` — The whole menu m is in closed; scope and focus given back; CLOSED.

## `ui/widgets/menubar.cst`

menus behind titles in a row.

- `create(t as *tree.Tree, parent as i64) as i64` — A new, empty menu bar as the last child of parent.
- `count(t as *tree.Tree, w as i64) as i64`
- `title(t as *tree.Tree, w as i64, i as i64) as i64` — Title i, 0 past the last.
- `open_index(t as *tree.Tree, w as i64) as i64` — The title whose menu is open, -1 for none.
- `add(t as *tree.Tree, w as i64, s as *u8, n as i64, m as i64) as i64` — A title — s, n bytes, an underscore before its mnemonic — opening m, at the end.
- `bind(t as *tree.Tree, r as *router.Router, w as i64) as void` — F10 and the titles' mnemonics, from now on.

## `ui/widgets/menubutton.cst`

a button that opens a menu, or a popover.

- `create(t as *tree.Tree, parent as i64, s as *u8, n as i64) as i64` — A new menu button titled s — n bytes, an underscore before its mnemonic — as the last child of parent, opening nothing yet.
- `label_of(t as *tree.Tree, w as i64) as i64`
- `set_menu(t as *tree.Tree, w as i64, m as i64) as void` — What it opens from now on: menu m, instead of a popover.
- `set_popover(t as *tree.Tree, w as i64, p as i64) as void` — Popover p, instead of a menu.
- `menu_of(t as *tree.Tree, w as i64) as i64`
- `popover_of(t as *tree.Tree, w as i64) as i64`
- `is_open(t as *tree.Tree, w as i64) as i32`
- `bind_mnemonic(t as *tree.Tree, r as *router.Router, w as i64) as void` — Its mnemonic, from now on, opens it.
- `open(t as *tree.Tree, w as i64) as void` — What it opens, opened as by the keys.
- `arrow_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — Its arrow's place, in the window.

## `ui/widgets/progress.cst`

how far something has gone.

- `create(t as *tree.Tree, parent as i64) as i64` — A new progress bar, empty, as the last child of parent.
- `set_fraction(t as *tree.Tree, w as i64, f as f64) as i32` — Filled to f, held to 0 ..
- `fraction(t as *tree.Tree, w as i64) as f64`
- `pulse(t as *tree.Tree, w as i64) as i32` — No fraction to show: the pulse, until a fraction is set.
- `pulsing(t as *tree.Tree, w as i64) as i32`
- `pulse_at(t as *tree.Tree, w as i64) as f64` — Where the pulse is: 0 at the start, 1 at the end.
- `bar(t as *tree.Tree, w as i64) as math.rect.Rect` — Its bar, in the window; nothing for a widget that is no progress bar.

## `ui/widgets/separator.cst`

a line between things.

- `create(t as *tree.Tree, parent as i64, vertical as i32) as i64` — A new separator, across — up and down for vertical 1 — as the last child of parent.
- `set_vertical(t as *tree.Tree, w as i64, on as i32) as i32` — Up and down rather than across: 1, or 0 for a widget that is no separator.

## `ui/widgets/slider.cst`

a value chosen along a track.

- `create(t as *tree.Tree, parent as i64, min as f64, max as f64, step as f64) as i64` — A new slider across, from min to max by step, at min, as the last child of parent.
- `set_value(t as *tree.Tree, w as i64, v as f64) as i32` — Its value from now on, held to its range and step, saying VALUE_CHANGED when that changes it.
- `value(t as *tree.Tree, w as i64) as f64`
- `set_range(t as *tree.Tree, w as i64, min as f64, max as f64, step as f64) as i32` — Another range and step, the value held to them; 0 for a range the wrong way round.
- `set_page(t as *tree.Tree, w as i64, page as f64) as i32` — How far Page Up and Down move it; 0 for ten steps.
- `set_vertical(t as *tree.Tree, w as i64, on as i32) as i32` — Up and down rather than across.
- `set_marks(t as *tree.Tree, w as i64, values as *f64, n as i64) as i32` — Marks at n values the program keeps alive; null and 0 for none.
- `track(t as *tree.Tree, w as i64) as math.rect.Rect` — Its track and its knob, in the window; nothing for a widget that is no slider.
- `knob(t as *tree.Tree, w as i64) as math.rect.Rect`

## `ui/widgets/spin.cst`

a number in a field, with buttons to step it.

- `create(t as *tree.Tree, parent as i64, min as f64, max as f64, step as f64) as i64` — A new spin box from min to max by step, at min, as the last child of parent.
- `set_value(t as *tree.Tree, w as i64, v as f64) as i32` — Its value from now on, held to range and step, saying VALUE_CHANGED when that changes it; what was typed is dropped.
- `value(t as *tree.Tree, w as i64) as f64`
- `set_range(t as *tree.Tree, w as i64, min as f64, max as f64, step as f64) as i32` — Another range and step, the value held to them, its decimals the step's unless set; 0 for a range the wrong way round.
- `set_digits(t as *tree.Tree, w as i64, digits as i32) as i32` — How many decimals it shows; below 0, as many as its step has.
- `text_entry(t as *tree.Tree, w as i64) as i64` — Its parts: the entry showing its text, and its buttons; 0 for a widget that is no spin box.
- `minus(t as *tree.Tree, w as i64) as i64`
- `plus(t as *tree.Tree, w as i64) as i64`
- `editing(t as *tree.Tree, w as i64) as i32` — Whether what is typed shows, not yet its value.

## `ui/widgets/statusbar.cst`

what is going on, along a window's foot.

- `create(t as *tree.Tree, parent as i64) as i64` — A new status bar, saying nothing, as the last child of parent.
- `permanent(t as *tree.Tree, w as i64) as i64` — The row at its end, for what the program keeps there.
- `label_of(t as *tree.Tree, w as i64) as i64` — The label its message is shown in.
- `message(t as *tree.Tree, w as i64) as *u8` — What it says now; null and 0 when nothing.
- `message_length(t as *tree.Tree, w as i64) as i64`
- `push(t as *tree.Tree, w as i64, ctx as i64, s as *u8, n as i64) as i64` — s, n bytes — the first MAX_TEXT kept, whole characters — said under context ctx, over what was said.
- `pop(t as *tree.Tree, w as i64, ctx as i64) as i32` — The newest message under ctx gone.
- `remove(t as *tree.Tree, w as i64, id as i64) as i32` — Message id gone, whatever its context.
- `flash(t as *tree.Tree, w as i64, s as *u8, n as i64, ms as i64, now as i64) as i64` — s said for ms milliseconds from now, then taken away — over another one flashed, which goes at once.

## `ui/widgets/switch.cst`

on or off, as a light switch is.

- `create(t as *tree.Tree, parent as i64) as i64` — A new switch, off, as the last child of parent.
- `set_active(t as *tree.Tree, w as i64, on as i32) as i32` — On or off, saying TOGGLED when that changes it, the knob sliding there.
- `is_active(t as *tree.Tree, w as i64) as i32`
- `knob(t as *tree.Tree, w as i64) as f64` — Where its knob is now: 0 at the start, 1 at the end.
- `is_down(t as *tree.Tree, w as i64) as i32` — Whether it is held down now: by a press the pointer is still over, or by Space.
- `track(t as *tree.Tree, w as i64) as math.rect.Rect` — Its track, in the window; nothing for a widget that is no switch.

## `ui/widgets/tableview.cst`

a model's rows in columns, under a header.

- `create(t as *tree.Tree, parent as i64, m as *model.Model) as i64` — A new table view of m's rows, with no columns yet, as the last child of parent.
- `list(t as *tree.Tree, w as i64) as i64` — The list view its rows are in: its selection, its cursor, its rows.
- `set_drag_content(t as *tree.Tree, w as i64, cb as listview.DragContentFn, actions as i32, user as *u8) as i32` — A row pressed and dragged is cb's content from now on, as a list's rows are (listview.set_drag_content), the row of the list under its headers.
- `add_column(t as *tree.Tree, w as i64, s as *u8, n as i64, width as f64) as i64` — A column titled s, n bytes, width wide, at the end; its index, or -1.
- `column_count(t as *tree.Tree, w as i64) as i64`
- `column_width(t as *tree.Tree, w as i64, col as i64) as f64`
- `set_column_width(t as *tree.Tree, w as i64, col as i64, width as f64) as void` — Column col width wide, or MIN_WIDTH when that is less.
- `set_resizable(t as *tree.Tree, w as i64, col as i64, on as i32) as void`
- `set_sortable(t as *tree.Tree, w as i64, col as i64, on as i32) as void`
- `header_cell(t as *tree.Tree, w as i64, col as i64) as i64` — Column col's header cell, and its title's label.
- `header_label(t as *tree.Tree, w as i64, col as i64) as i64`
- `cell_label(t as *tree.Tree, row as i64, col as i64) as i64` — The label showing column col in row widget row; 0 for none.
- `cell_icon(t as *tree.Tree, row as i64, col as i64) as i64` — The icon of column col in row widget row, hidden when the row has none there; 0 for no such column.
- `set_stretch(t as *tree.Tree, w as i64, col as i64) as void` — Column col taking the room the columns leave, or the last for none such.
- `stretch(t as *tree.Tree, w as i64) as i64` — The column taking the room the columns leave.
- `sort_column(t as *tree.Tree, w as i64) as i64`
- `sort_order(t as *tree.Tree, w as i64) as i32`
- `set_sort_hook(t as *tree.Tree, w as i64, cb as SortFn, user as *u8) as void`
- `set_sort(t as *tree.Tree, w as i64, col as i64, order as i32) as void` — Sorted by col, order: shown in the header, asked of the hook.
- `arrow_rect(t as *tree.Tree, w as i64, col as i64) as math.rect.Rect` — The arrow's place in column col's header cell, in the window; empty when the rows are not sorted by it.

## `ui/widgets/tabs.cst`

pages one at a time, chosen from a strip of tabs.

- `create(t as *tree.Tree, parent as i64) as i64` — New tabs, with no pages yet, as the last child of parent.
- `count(t as *tree.Tree, w as i64) as i64`
- `current(t as *tree.Tree, w as i64) as i64`
- `page(t as *tree.Tree, w as i64, i as i64) as i64` — Page i, where its content goes — an overlay: each child aligned on its own.
- `tab(t as *tree.Tree, w as i64, i as i64) as i64`
- `title(t as *tree.Tree, w as i64, i as i64) as i64` — Tab i's title: a label.
- `strip(t as *tree.Tree, w as i64) as i64`
- `add(t as *tree.Tree, w as i64, s as *u8, n as i64) as i64` — A new page, last, under a tab titled s — n bytes, an underscore before its mnemonic.
- `set_current(t as *tree.Tree, w as i64, i as i64) as i32` — Page i current, its tab the one that takes the focus, brought into view.
- `remove(t as *tree.Tree, w as i64, i as i64) as i32` — Tab i and its page gone.
- `set_side(t as *tree.Tree, w as i64, side as i32) as void` — Along the TOP or the BOTTOM.
- `set_closable(t as *tree.Tree, w as i64, i as i64, on as i32) as void` — A close button on tab i, or none.
- `close_rect(t as *tree.Tree, w as i64, i as i64) as math.rect.Rect` — Tab i's close button, in the window; empty for none.
- `overflowing(t as *tree.Tree, w as i64) as i32` — More tabs than fit: 1.
- `arrow_rect(t as *tree.Tree, w as i64, which as i64) as math.rect.Rect` — The arrow back (0) or on (1), in the window; empty when all fit.
- `bind_mnemonics(t as *tree.Tree, r as *router.Router, w as i64) as void` — Alt and each title's mnemonic choose its tab from now on.

## `ui/widgets/textedit.cst`

what the entry and the text view share.

- `seq(b as i64) as i64` — The length of the sequence a byte begins, as its bits say.
- `decode(p as *u8, at as i64, n as i64) as i64` — The codepoint of the whole sequence at at.
- `whole(s as *u8, i as i64, n as i64, k as i64) as i32` — Whether the k bytes from i of s, n long, are one well-formed sequence, as Unicode's table 3-7 has them.
- `accept(s as *u8, n as i64, room as i64, f as FilterFn, user as *u8, lines as i32, out as *u8) as i64` — What goes in of s, n bytes, into out — room for n bytes there: what is no UTF-8 left out, each character f says no to left out (f null lets all in), at most room characters (below 0: any number), and each line end a...
- `context_menu(t as *tree.Tree, items as *i64) as i64` — A new context menu of an editable text, its items into items by id — room for SELECT_ALL + 1.
- `can(id as i64, editable as i32, seen as i32, selected as i32, undo as i32, redo as i32, any as i32, paste as i32) as i32` — Whether action id can be done: editable, the text seen — no password — something selected, something to undo or redo, any text, something on the clipboard to paste.
- `offer(t as *tree.Tree, items as *i64, editable as i32, seen as i32, selected as i32, undo as i32, redo as i32,` — Its items enabled as what they do can be done (can).

## `ui/widgets/textview.cst`

text of many lines.

- `create(t as *tree.Tree, parent as i64) as i64` — A new, empty text view as the last child of parent — a scroll area, as a rule.
- `text(t as *tree.Tree, w as i64) as *u8`
- `length(t as *tree.Tree, w as i64) as i64`
- `paragraphs(t as *tree.Tree, w as i64) as i64`
- `paragraph_of(t as *tree.Tree, w as i64, at as i64) as i64` — The paragraph offset at is in.
- `caret(t as *tree.Tree, w as i64) as i64`
- `selection(t as *tree.Tree, w as i64, a as *i64, b as *i64) as i32`
- `laid(t as *tree.Tree, w as i64) as i64` — How many paragraphs are kept laid out now.
- `layouts(t as *tree.Tree, w as i64) as i64` — How many times a paragraph has been laid out — what a kept one saves.
- `rect_of(t as *tree.Tree, w as i64, at as i64) as math.rect.Rect` — The caret at offset at, as it would be drawn, in the window.
- `offset_at(t as *tree.Tree, w as i64, x as f64, y as f64) as i64` — The offset nearest the window's point x, y: above the text, in its first line; below it, its end.
- `caret_rect(t as *tree.Tree, w as i64) as math.rect.Rect` — Its caret, in the window: inside a composition, the composition's.
- `drop_at(t as *tree.Tree, w as i64) as i64` — Where text being dragged would be dropped; -1 when nothing is dragged.
- `context_menu(t as *tree.Tree, w as i64) as i64`
- `select(t as *tree.Tree, w as i64, a as i64, b as i64) as void`
- `set_text(t as *tree.Tree, w as i64, s as *u8, n as i64) as i32` — Its text from now on — not filtered, nothing to undo — the caret at its start; CR LF and a lone CR made line feeds.
- `set_editable(t as *tree.Tree, w as i64, on as i32) as i32`
- `set_wrap(t as *tree.Tree, w as i64, mode as i32) as i32` — Wrapped at its width by word (WRAP_WORD, as it starts), or not (WRAP_NONE): one line a paragraph, as wide as the widest.
- `set_font(t as *tree.Tree, w as i64, family as *u8, size as f64) as i32` — Its text in family (null for the theme's), size logical pixels (0 for the theme's): a font of its own for what is edited in it, as an editor's preferences choose.
- `set_accepts_tab(t as *tree.Tree, w as i64, on as i32) as i32` — Tab typed (1, as it starts), or left to move the focus (0).
- `can(t as *tree.Tree, w as i64, id as i64) as i32` — Whether the context menu's action id (textedit.UNDO … SELECT_ALL) can be done now: for a program's own Edit menu, its items enabled as these say.
- `act(t as *tree.Tree, w as i64, id as i64) as i32` — The action id done, as if chosen from its context menu: 1, or 0 when it cannot be done now (can).

## `ui/widgets/titlebar.cst`

a window's title bar, drawn by the toolkit.

- `create(t as *tree.Tree, parent as i64) as i64` — A new title bar, untitled, a close button at its end, as the last child of parent.
- `start(t as *tree.Tree, w as i64) as i64` — Where the program's widgets go at its start, and at its end.
- `end(t as *tree.Tree, w as i64) as i64`
- `title(t as *tree.Tree, w as i64) as i64` — Its title's label.
- `set_title(t as *tree.Tree, w as i64, s as *u8, n as i64) as void`
- `set_subtitle(t as *tree.Tree, w as i64, s as *u8, n as i64) as void` — A line under the title; none for n 0.
- `set_maximized(t as *tree.Tree, w as i64, on as i32) as void` — The window is maximized, or not: its button restores, or maximizes.
- `is_maximized(t as *tree.Tree, w as i64) as i32`
- `set_rounding(t as *tree.Tree, w as i64, r as f64) as void` — Its top corners rounded by r, as the window's are when the toolkit draws its frame (decoration.cst); 0 for square.
- `button(t as *tree.Tree, w as i64, which as i32) as i64` — The window's button which (MINIMIZE, MAXIMIZE, CLOSE), 0 when it has none.
- `buttons(t as *tree.Tree, w as i64) as i32` — Which of the window's buttons it shows: BTN_* bits, always minimize, then maximize, then close.
- `set_buttons(t as *tree.Tree, w as i64, flags as i32) as void` — The window's buttons it shows: flags, BTN_* bits.

## `ui/widgets/toolbar.cst`

tools in a row.

- `create(t as *tree.Tree, parent as i64) as i64` — A new, empty toolbar as the last child of parent.
- `count(t as *tree.Tree, w as i64) as i64`
- `item(t as *tree.Tree, w as i64, i as i64) as i64` — Tool i, 0 past the last.
- `add_button(t as *tree.Tree, w as i64, s as *u8, n as i64) as i64` — A flat button titled s — n bytes, an underscore before its mnemonic — at the end.
- `add_toggle(t as *tree.Tree, w as i64, s as *u8, n as i64) as i64` — The same, staying down until pressed again.
- `add_separator(t as *tree.Tree, w as i64) as i64`
- `shown(t as *tree.Tree, w as i64) as i64` — How many of its tools are in the row: the rest are in its menu.
- `overflowing(t as *tree.Tree, w as i64) as i32`
- `more(t as *tree.Tree, w as i64) as i64` — The button at its end that offers what does not fit.
- `overflow_menu(t as *tree.Tree, w as i64) as i64`

## `ui/widgets/tooltip.cst`

a short text shown over what the pointer rests on.

- `open(t as *tree.Tree, r as *router.Router) as Tooltips` — The tooltips of a window's tree t and router r, none yet.
- `close(tt as *Tooltips) as void` — The bubble taken down; the tips forgotten.
- `set(tt as *Tooltips, w as i64, s as *u8, n as i64) as i32` — w's tip from now on: n bytes of UTF-8 the program keeps alive, or none for null.
- `shown(tt as *Tooltips) as i64` — The bubble showing now, 0 for none; the widget it is for; its label.
- `showing_for(tt as *Tooltips) as i64`
- `label_of(tt as *Tooltips) as i64`
- `tick(tt as *Tooltips, now as i64) as void` — The time is now: show, keep or hide the bubble.
- `next_due(tt as *Tooltips) as i64` — When it next wants to be told the time, -1 for no time of its own: the delay's end while the pointer rests on a tip not yet shown.

## `ui/widgets/treeview.cst`

a tree model's rows, branches opened and closed.

- `create(t as *tree.Tree, parent as i64, src as *model.Model) as i64` — A new tree view of src's rows — src stays where it is while the tree view has it — as the last child of parent, its top level shown.
- `list(t as *tree.Tree, w as i64) as i64` — The list view it shows its rows in: its selection, its cursor, its rows.
- `flat(t as *tree.Tree, w as i64) as *flatten.Flat` — The rows shown, as flatten.cst keeps them.
- `is_expanded(t as *tree.Tree, w as i64, index as i64) as i32`
- `expand(t as *tree.Tree, w as i64, index as i64) as i32`
- `collapse(t as *tree.Tree, w as i64, index as i64) as i32`
- `indent(t as *tree.Tree, w as i64) as f64` — How much further in each level is.
- `set_indent(t as *tree.Tree, w as i64, px as f64) as void`
- `row_label(t as *tree.Tree, row as i64) as i64` — A row widget's name label and its expander.
- `row_expander(t as *tree.Tree, row as i64) as i64`
