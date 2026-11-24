# PERFORMANCE_STATUS — caustic-media

Branch `agent/media-perf`, worktree `../.agent-media-perf`, from `main`
`c893bd7` (the tree the original audit read; `main` has no commit since).
Source of the items: "Performance audit — caustic-media", the final answer of
the 2026-11-21 session (copy: `/tmp/cm-audit/audit.md` while this runs).

States: FIXED (code committed, test + A/B evidence), PARTIAL, UNRESOLVED,
BLOCKED (with the prerequisite), NOT_APPLICABLE (measured or shown not to
exist). Low gain is not NOT_APPLICABLE.

## Core subitems (deduplicated) — denominator 45

| ID | Subitem | State | Evidence / commit |
|---|---|---|---|
| A1.1 | textview: one layout for Run and Layout | FIXED | `4556a45`; text.lay_text_in lays once into the paragraph Layout; textview_test "a keystroke: each paragraph laid out once" fails on parent |
| A1.2 | entry: one layout for Run and Layout | FIXED | `4556a45`; entry 2 → 1 layout a keystroke (120 → 60 for 60 keys); entry_test counts it |
| A1.3 | selectable label: one layout for Run and Layout | FIXED | `4556a45`; label_test "selectable: laid out no more than a label that is not" fails on parent |
| A2.1 | label `_measure` at unbounded width clobbers the drawn run | FIXED | `404911b`; 5 → 3 layouts per changed cut label per host frame (6000 → 3600, 137 → 89 ms, 40 labels × 30 frames); `label_test` least_kept fails on base, passes on fix |
| A2.2 | `min_size` traversal when nothing was invalidated | UNRESOLVED | |
| A3.1 | icon coverage cache (id, size, scale) | UNRESOLVED | |
| A3.2 | parsed icon path cache | UNRESOLVED | |
| A3.3 | icons evict glyph pages (shared atlas) | UNRESOLVED | |
| A3.4 | per-icon upload | UNRESOLVED | |
| A4.1 | entry hint run relaid every `_relay` | FIXED | `2eeb826`; placeholder laid out only when it or the style changes; entry 5 → 2 layouts a keystroke (300 → 120); entry_test placeholder_kept |
| A4.2 | entry "Ag"/"0" metrics relaid every `_relay` | FIXED | `2eeb826`; text.sizes() keeps line height and digit width per font and size; text_test sized() |
| A4.3 | label ellipsis width relaid on every text change | FIXED | `2eeb826`; ellipsis width from text.sizes(); 3 → 2 layouts per changed cut label (3600 → 2400) |
| A5.1 | label selection draws the run once per rectangle | UNRESOLVED | |
| A5.2 | textview selection draws the run once per rectangle | UNRESOLVED | |
| A6.1 | `widget.theme` copies the Theme per call | UNRESOLVED | |
| B1 | glyph cache `_look` twice per hit | FIXED | `60104e6`; get uses the first lookup (_hit); 166–185 → 123–131 ns a cached glyph drawn |
| B2.1 | `% cap` per probe | FIXED | `60104e6`; table rounded to a power of two, probed by mask; atlas_test 100 → 128 |
| B2.2 | `raster.subpixel` division per axis | FIXED | `60104e6`; power-of-two steps by arithmetic shift; atlas_test compares with raster.subpixel incl. negative pens |
| B3 | `color.kind` scans table directory per glyph | FIXED | `d8586af`; color.any cached per open face; plain faces skip kind; color_test any() |
| B4 | shaping plan compiled per shaped item | UNRESOLVED | |
| B5.1 | per-glyph divisions in `_draw` | FIXED | `d8586af`; divisions by power-of-two scale/page sizes as exact multiplications, others still divided |
| B5.2 | `_open` scan before the clip test | FIXED | `d8586af`; clip test before _open |
| B5.3 | `upload` at the end of every run | UNRESOLVED | |
| B5.4 | `_open` scan per glyph without last-face cache | FIXED | `d8586af`; each run face opened once a draw; with B3/B5: 131–145 → 81–82 ns a glyph |
| C1.1 | present: copy then separate swizzle/premultiply | UNRESOLVED | |
| C1.2 | buffer-age damage recopied and reconverted | UNRESOLVED | |
| C1.3 | offscreen texture in platform (BGRA) order | UNRESOLVED | |
| C2 | Wayland `_to_argb` byte-wise | UNRESOLVED | |
| C3.1 | ortho/screen-space vertex fast path | UNRESOLVED | |
| C3.2 | `_six` shades 6 vertices of a quad | UNRESOLVED | |
| C3.3 | tint multiply when tint is white | UNRESOLVED | |
| C3.4 | 72-byte f64 vertices | UNRESOLVED | |
| C3.5 | vertices stored twice / copied per flush (= E2.1) | UNRESOLVED | |
| C4 | `rect_shaded`/`_same_shaded` by-value structs | UNRESOLVED | |
| C5 | band threads spawned per pass | UNRESOLVED | |
| C6 | `_image_fills` written from band workers | UNRESOLVED | |
| D1.1 | X11 ConfigureNotify not coalesced | UNRESOLVED | |
| D1.2 | SHM backing rebuilt on every size | UNRESOLVED | |
| D2.1 | SETTINGS rethemes/relayouts when nothing changed | UNRESOLVED | |
| D2.2 | `set_direction` invalidates when unchanged | UNRESOLVED | |
| D3 | `pointer_shape` hit test every round | UNRESOLVED | |
| E1.1 | tree mapping zeroed at open | UNRESOLVED | |
| E1.2 | app/device blocks zeroed at open | UNRESOLVED | |
| E2.2 | canvas virtual reservation per canvas | UNRESOLVED | |
| E2.3 | `invalidate_paint_subtree` on texture recreation | UNRESOLVED | |

## Architecture (tracked apart)

| ID | Option | State |
|---|---|---|
| F1 | shape coverage cache | UNRESOLVED |
| F2 | retained per-run text geometry | UNRESOLVED |
| F3 | direct-to-buffer / swapchain presentation (C1 option 3) | UNRESOLVED |
| F4 | skip background clear under an opaque root | UNRESOLVED |

## Cross-project (tracked apart)

| ID | Item | State |
|---|---|---|
| X1 | CausticOS #15: toolkit damage collapsed into one bounding box | UNRESOLVED |
| X2 | CausticOS G29 / browser F7: background/incremental image decoding API | UNRESOLVED |

## Gates

- Baseline `caustic-mk run test` on `c893bd7`: failed at
  `sh ui/tools/api.sh | diff ui/api.md -` (api.md 7 entries stale on main);
  with api.md regenerated (`193ba9b`): rc=0.

## New issues found

- Caustic compiler: a struct used as an array element type in another struct before its own declaration is sized 0 with no diagnostic (seen with `Sizes` in `ui/text.cst`; fixed by declaring it first). Sibling repo, not edited.

- `ui/api.md` stale on `main` (fixed in `193ba9b`).
