# Finding fonts

A program asks for "Noto Sans, bold, 13 pixels" and gets faces: the one that
best answers the request, and behind it the ones that draw what it cannot. This
is fontconfig's job on Linux and DirectWrite's on Windows; here it is ours,
because both are large, and because what they do is not complicated — reading
every font file's names once, remembering them, and choosing among them by
rules CSS already wrote down.

```
fonts/
  fonts.cst     the hub: the system's fonts indexed, the interface's chains
  scan.cst      the font files under a directory, with their times and sizes
  face.cst      what a face is: its names, weights, widths, styles, coverage
  index.cst     every face found, kept in a file between runs
  match.cst     the face of a family that best answers a request (CSS)
  chain.cst     the faces behind it, for what it cannot draw
  pool.cst      bytes that grow, which the others keep their records in
```

`match.cst` is imported as `matching`: `match` is a word of the language.

The system's font directories, and where the index is kept, are the
platform's knowledge and come from [`window/fonts.cst`](../../window/fonts.cst):

| | Fonts | Index |
|---|---|---|
| Linux | `/usr/share/fonts`, `/usr/local/share/fonts`, `$XDG_DATA_HOME/fonts` (`~/.local/share/fonts`), `~/.fonts` | `$XDG_CACHE_HOME/caustic-media/fonts.index` (`~/.cache/…`) |
| Windows | `%WINDIR%\Fonts`, `%LOCALAPPDATA%\Microsoft\Windows\Fonts` | `%LOCALAPPDATA%\caustic-media\fonts.index` |

---

## The index

Every face of every font file under those directories — named `.ttf`, `.otf`,
`.ttc` or `.otc`, in any case — read for what matching needs and no more
([`face.cst`](face.cst)):

- **names**: the typographic family and style (name IDs 16 and 17) or, without
  them, the legacy ones (1 and 2); the legacy family besides, which a program
  may ask for ("Noto Sans Light"); the full and PostScript names;
- **weight** and **width** as CSS has them: one each from `OS/2` for a static
  face — classes 1 to 9 of weight read as hundreds, as old fonts write them —
  or the ranges of a variable face's `wght` and `wdth` axes;
- **style**: what the face can be drawn as — normal, italic, oblique — from
  `OS/2` (or `head` without it), widened by an `ital` or `slnt` axis;
- **monospace** (`post`), **colour** (`COLR`, `CBDT` or `sbix`), **variable**;
- **coverage**: the pages of 256 characters its cmap reaches
  ([`sfnt.md`](../sfnt/sfnt.md#coverage)), as runs of pages.

Coverage is coarse on purpose. Exact coverage of the 8726 faces on the machine
this was written on — 82 pages each on average, five in seven of them partly
filled — would be 16 to 20 MB; fontconfig's cache there is 201 MB. Pages make
the index 3 MB, and the face's own cmap answers exactly once the page says it
is worth asking (see the chains, below).

The walk ([`scan.cst`](scan.cst)) follows links, into directories too, and
visits nothing twice: each directory and file is known by its device and inode,
so a link back up ends there and a file reached by two names is listed under the
first. Each directory's names are sorted, so the same tree gives the same list
whatever order the filesystem keeps.

Reading every font file at every start would take seconds — 4.7 here, the
first time — so the index is kept in a file with each font file's size and
modification time to the nanosecond. At start the directories are walked again,
which takes 60 ms, and only files new or changed since are read; files gone are
dropped, and the file is written again only when something changed. A font file
that does not open is kept as a file of no faces, so it is not read again until
it changes. The next start takes 90 ms.

The file is the index as it is in memory behind a header — magic, version, the
sizes of the two records, their counts, the text's length — and anything that
is not exactly that is refused and the index made again: cut, grown, from
another version, with a record pointing outside it, a face its file does not
list, counts so large their sizes wrap round to the file's length, ranges that
are not numbers. It is written whole to another name and moved over the old one,
so a reader never sees half of it.

Families are found by name as CSS compares them, ASCII letters without case, in
an order kept beside the records; a name no face has as its family is looked
for among the legacy families.

---

## Matching, as CSS does it

A request is a family and a weight (1–1000), a width (a percentage) and a
style. The family's faces are narrowed in CSS Fonts' order (level 4, §5.2) — the
algorithm every browser implements, so a program here chooses the face a web
page would:

1. **width**: the requested one if a face has it; otherwise, at or below 100%,
   the nearest narrower, then the nearest wider; above, the other way round.
2. **style**: italic asks for italic, then oblique, then normal; oblique for
   oblique, then italic, then normal; normal for normal, then oblique, then
   italic.
3. **weight**: the requested one; otherwise, between 400 and 500, the nearest
   heavier up to 500, then the nearest lighter, then heavier past 500; below
   400 the nearest lighter, then heavier; above 500 the nearest heavier, then
   lighter.

A variable face is a range of weights and widths and matches every value in it:
the value settled on is where it is drawn. Oblique angles are not matched by
degree as level 4 does — a face is oblique or not — and a variable face drawn
oblique is slanted CSS's 14 degrees, as far as its axis goes.

---

## Chains

No face draws everything. A chain ([`chain.cst`](chain.cst)) is the faces asked
for and the faces found behind them, and a character is drawn by the first that
has it:

1. **the faces asked for**, in order — a list as CSS's `font-family` is, names
   quoted or not, generic names (`sans-serif`, `serif`, `monospace`,
   `system-ui`, `emoji`, and the `ui-` ones) standing for the families the
   program prefers for them. When none exists, `sans-serif`'s; when none of
   those, the first family by name that reaches the first page and is not in
   colour.
2. **for an emoji** — Extended_Pictographic, and the regional indicators that
   make flags — the preferred emoji families; **for CJK** — what is East Asian
   wide or fullwidth, and the halfwidth katakana, conjoining Hangul jamo and
   Bopomofo tone marks that are not — the preferred CJK families, the text's
   language's first: Japanese, Korean and the two Chinese draw the same
   ideographs differently.
3. **any family that has it**: the first family's other faces and the families
   whose names begin with its name ("Noto Sans Devanagari" for "Noto Sans"),
   then the others, colour faces after the rest except for an emoji; in each of
   those, faces at the width and style asked for before the others, so a
   condensed family is not taken for normal text only because its name sorts
   first; then by name. Within a family, the face CSS matching gives among
   those reaching the character's page, so a bold text falls back to bold.

A character no face has is drawn by the first face, as its missing glyph — the
box that says something is missing, never nothing.

The preferences are lists a program can replace; the defaults name the families
the platforms ship — Noto Sans CJK, Source Han Sans, Microsoft YaHei and
JhengHei, Yu Gothic and Meiryo, Malgun Gothic; Noto Color Emoji, Segoe UI Emoji,
Apple Color Emoji. The CJK order follows the locale (`LC_ALL`, `LC_CTYPE`,
`LANG`): Traditional Chinese for `zh` with `Hant` or of Taiwan, Hong Kong or
Macau, Simplified for other `zh` and for every other language.

Faces are opened when first asked, their files mapped. A face found not to have
a character is not kept; the chain learns that page of it instead, 256 bits,
and does not open it for that page again — the index's coarse coverage made
exact where the text has been. Answers are remembered by character.

`design()` gives the instance a face is drawn at for a size: its matched weight
and width, its style on an `ital` or `slnt` axis, and `opsz` following the size.

---

## The interface font

The family the desktop names for interface text and for monospace text, read by
[`window/settings.cst`](../../window/settings.cst), is what `fonts.ui()` and
`fonts.mono()` open a chain on — `system-ui` and `monospace` when the desktop
names none, so sans-serif's families stand behind a family that is not
installed.

---

## Bounds

A walk goes 16 directories below its root, and leaves out a path longer than
511 bytes. 256 faces of one collection are read. A name is kept to 255 bytes. A
chain holds 64 faces; past that, what none of them has is drawn as missing.

---

## Current state

Linux and Windows: the directories, the walk, the index and its file, matching
and chains, the interface's chains. What each system is asked is in `file.cst`:
Linux's `getdents64`, `stat` and `mmap`; Windows' `FindFirstFileW`, each path's
file information (its volume serial and file index are the identity that keeps
a file reached twice, or a directory by another spelling, from being walked
again; a link or junction is followed to what it names) and a read-only view of
a file mapping. Paths are UTF-8; Windows' are given to the W calls as UTF-16.
The index file and its directories are written by std/io, whose Windows calls
are still the ANSI ones, as is std/env's `getenv` that names the places: a
profile path outside ASCII is not yet one this can keep its index under.

Tested against fonts made for it ([`../testdata/fonts`](../testdata/README.md)):
the walk's order, links, loops, depth and names; every face's description; the
index kept, refused when forged, and kept up to date; CSS's matching at each of
its turns; every rule of the chains, the pages learned, the bound on faces held;
and, end to end, this machine's own fonts, the index kept in `/tmp`.

On Windows `fonts_win32_test.cst` runs in an isolated Wine (`caustic-mk run
test-win32`): the test fonts walked as Windows lists them, each size and time
against what std/io reads by other calls, the same directory by another
spelling not walked twice, the index kept and read back and made again for the
one font that changed, chains mapping their faces, and a font installed in
`%LOCALAPPDATA%\Microsoft\Windows\Fonts` found by the system's own discovery
(a new Wine prefix's `C:\windows\Fonts` is empty: Wine draws its own fonts from
elsewhere).

Mutation-tested: every mutant killed but those that cannot be told apart — a
hash's quality, a guard whose case the code before it already took — and two
that only a fault would show: a file cut while it is read, and a count of files
wrapped round, whose loop reads past its buffer into whatever lies next.
