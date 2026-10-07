# ui/atspi — the toolkit told to AT-SPI

A screen reader on Linux (Orca), a switch control, an accessibility
inspector, all read programs through AT-SPI: a D-Bus protocol on a bus of
its own, where every program puts its accessible objects and a registry
lists the programs. This directory is the bridge from the toolkit's own
accessibility model (`ui/access.cst`: roles, names, states, actions, values,
and the text widgets' text) to that protocol. Nothing in it is needed to
draw a window; a program that never opens it pays nothing, and one that
opens it on a desktop where nothing is listening pays one D-Bus question.

## The conversation

1. **Is anyone listening?** `org.a11y.Bus` on the session bus, object
   `/org/a11y/bus`, interface `org.a11y.Status`, properties `IsEnabled` and
   `ScreenReaderEnabled`. Both false: the bridge stays off, and watches
   `PropertiesChanged` there to come on later.
2. **Where is the accessibility bus?** `org.a11y.Bus.GetAddress` on the
   same object: a D-Bus address, connected to like any other.
3. **Here we are.** Our objects exported on that bus, then
   `org.a11y.atspi.Socket.Embed((so))` called on the registry
   (`org.a11y.atspi.Registry`, `/org/a11y/atspi/accessible/root`) with our
   root's reference, `(our unique name, /org/a11y/atspi/accessible/root)`;
   the answer is the desktop's reference, our root's parent.
4. From then on the registry and assistive technologies call our objects
   and listen to the signals we emit.

Every step is asynchronous: sent, and its reply handled when it comes, in
`pump`, which the program's loop calls when one of the bridge's descriptors
(`fds`) is readable and after every round of its own events, so what changed
is told. Nothing blocks the window. In the toolkit's loop (`../host.cst`),
`hosted.cst` does both: attached, the loop watches whichever connections the
bridge has — the session bus, then the accessibility bus too — and pumps it
every round.

## Objects

- `/org/a11y/atspi/accessible/root`: the application. Role `application`,
  its children the program's windows, the `Application` interface
  (`ToolkitName` "caustic", versions, `Id`).
- `/org/a11y/atspi/accessible/<window>_<widget>`: a widget of a window, both
  named by their handles (`ui/app.cst`'s and the tree's). A window's own
  object is its tree's root (`frame`, or `dialog` for a dialog window).
  Handles carry an era, so a path of a widget destroyed never names its
  slot's next tenant: it is simply gone.

These are many and change all the time, so they are one D-Bus subtree
(`dbus.conn.export_subtree`) answered from one set of tables, the path
telling which widget is asked.

A widget is exposed with the interfaces that apply to it, answered from what
`access.describe` says and from the widget's text where it has some:

| interface    | for                                   | from                                    |
|--------------|---------------------------------------|-----------------------------------------|
| Accessible   | every one                             | describe: role, name, states, children  |
| Component    | every one                             | its rectangle in the window, hit-testing, focus |
| Action       | one with actions                      | describe's action bits; access.act      |
| Value        | one with a value in a range           | describe's value, min, max, step        |
| Text         | one with text (label, entry, text view) | the kind's text entry                 |
| EditableText | an editable one                       | the kind's text entry                   |
| Selection    | one whose children can be selected    | its children's SELECTED states and SELECT action |
| Table        | a table                               | its rows and cells as laid out          |

Containers with nothing to say (`ROLE_NONE`) are not exposed: their
children are told as their nearest exposed ancestor's. What a widget says
counts, not only its kind (`access.role`): a list's own scroll area says it
is nothing, so the list's rows are its children, as an assistive technology
expects of a list.

Offsets in Text are characters, as AT-SPI counts them; the toolkit's are
UTF-8 bytes, converted at the edge. A text is never read whole to answer a
question: the widget is asked what it is (`TEXT_INFO`: lengths, caret,
selection), for pieces of it (`TEXT_SPAN`), for offsets one from the other
(`TEXT_CHAR_OF`, `TEXT_BYTE_OF`) and for the paragraph of an offset
(`TEXT_PARAGRAPH`) — a text view answers from its buffer's index, which
counts the characters of every paragraph, so a document of megabytes costs
what its paragraph does; a widget that answers only `TEXT_GET` (a label, an
entry: short) is answered from its whole text by `access.cst`. Words and
sentences are UAX #29's (a word a segment of letters, marks, numbers or
connectors), found in the paragraph they are in, decoded on its own; lines
are where the widget laid them out, found by asking it where its characters
are (`TEXT_EXTENTS`, `TEXT_OFFSET_AT`); paragraphs end at line breaks. Text
attributes are not told: every run is the whole text, with none.

Names meant for people are in the application's language (`app.set_locale`,
`strings.cst`): `GetLocalizedRoleName`, an action's `GetLocalizedName` and
`GetDescription`; `GetRoleName` and `GetName` stay AT-SPI's own, for programs.
`Locale` and `Application.GetLocale` are the application's locale as POSIX
names it ("pt_BR"), "C" when there is none.

Selection and Table are answered from the children told. A list lays out
only the rows in view, so its children — and the rows a table can give a
cell of — are those; each row says its place among all of them and how many
there are (`tree.Access` `row`, `rows`), and a table says its columns and
which widget's children head them (`columns`, `headers`). Rows are chosen
by their `SELECT` action and let go of by `DESELECT`; columns are not
chosen.

## Events

What changes is told as AT-SPI signals from the object it happened to
(`org.a11y.atspi.Event.*`, signature `siiva{sv}`):

- the focus moving: `Focus:Focus` and `Object:StateChanged` "focused";
- a state, name, description or value changing: `Object:StateChanged`,
  `Object:PropertyChange` "accessible-name" / "-description" / "-value";
- text inserted or deleted, the caret moving, the selection of text
  changing: `Object:TextChanged`, `TextCaretMoved`, `TextSelectionChanged`;
- children added or removed: `Object:ChildrenChanged`;
- a window shown or closed, becoming the active one or no longer:
  `Window:Create`, `Destroy`, `Activate`, `Deactivate`, and
  `ChildrenChanged` from the application.

The toolkit tells the bridge through one hook per tree (`tree.set_watch`),
called with the widget and what kind of change — in the middle of the
tree's own work, so the change is only queued (repeats of the same one
dropped) and told in `pump`. What was last told of every widget is kept
(`bridge.Seen`, up to `MAX_SEEN`, the oldest given up when full): a change
is the widget described again and compared, and only what differs is sent —
a repaint sends nothing. A text's change is what was deleted and what was
inserted: told by the widget as it happens (`tree.notify_text`, from its
buffer — a text view), where in characters found right then; or, for a
widget that does not tell, found by comparing the text kept with the text
now (such a text over a mebibyte is not kept, and its changes are not
told). Windows are compared
in every pump: shown or not, active or not, their titles. A widget shown,
hidden, enabled or disabled compares what is under it too, since those
states are inherited.

## Checked

`atspi_test`, `text_test`, `select_test` and `events_test` run the bridge
on a private bus where the desktop is played by the test. `tools/
atspi_check.sh` runs it against the desktop's own stack — at-spi-bus-
launcher, at-spi2-registryd — on a private session bus, read by libatspi
(Python's `gi.repository.Atspi`), the library Orca reads programs through.

## Not here

UI Automation, Windows' counterpart, comes with the Win32 backend, after
the final step of F6. The `Collection`, `Document`, `Hypertext`, `Image`,
`TableCell` and `Cache` interfaces are not exposed: AT-SPI clients fall
back without them (libatspi warns once that the cache is missing). The bus
is found through `org.a11y.Bus` only — not `AT_SPI_BUS_ADDRESS` nor the X
root window's property — and every event is sent, whether or not anyone
registered for it.
