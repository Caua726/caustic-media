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
| A2.2 | `min_size` traversal when nothing was invalidated | NOT_APPLICABLE | `1003faf`; layout.measured() counts: nothing changed → 0 measures (layout_test); the walk itself is the kept-size check; a two-slot measure cache tried: 71–73 µs both ways, +72 B a node — reverted |
| A3.1 | icon coverage cache (id, size, scale) | FIXED | `3347ba6`; text.draw_mark/fill_mark keep an icon's coverage keyed by id, transform and fill rule; 96–106 → 24–32 ms (200 frames × 60 icons + text); icons_test counts fills |
| A3.2 | parsed icon path cache | FIXED | `3347ba6`; a cached icon is not parsed again (draw_mark hit skips the parse) |
| A3.3 | icons evict glyph pages (shared atlas) | FIXED | `3347ba6`; icons on their own coverage pages (limits.mark_pages); icons_test: no glyph page used |
| A3.4 | per-icon upload | FIXED | `3347ba6`; a hit uploads nothing; misses upload with the glyph pages |
| A4.1 | entry hint run relaid every `_relay` | FIXED | `2eeb826`; placeholder laid out only when it or the style changes; entry 5 → 2 layouts a keystroke (300 → 120); entry_test placeholder_kept |
| A4.2 | entry "Ag"/"0" metrics relaid every `_relay` | FIXED | `2eeb826`; text.sizes() keeps line height and digit width per font and size; text_test sized() |
| A4.3 | label ellipsis width relaid on every text change | FIXED | `2eeb826`; ellipsis width from text.sizes(); 3 → 2 layouts per changed cut label (3600 → 2400) |
| A5.1 | label selection draws the run once per rectangle | FIXED | `994900e`, `da06bba`; each clipped draw (one a selection rectangle, kept for identical pixels) finds its first glyph by halving and stops past its last, in any run (yslack: a raised glyph widens the room by its rise): 1.29–1.38 → 1.05–1.16 ms; O(rectangles·log n + glyphs on their lines) instead of O(rectangles·glyphs); text_test strips, two mutants caught |
| A5.2 | textview selection draws the run once per rectangle | FIXED | same draw as A5.1 (`994900e`, `da06bba`); textview_test passes |
| A6.1 | `widget.theme` copies the Theme per call | FIXED | `8bd7990`; widget.theme_at returns the tree's theme by pointer when nothing overrides it; 106 call sites; 61–64 → 50–51 µs |
| B1 | glyph cache `_look` twice per hit | FIXED | `60104e6`; get uses the first lookup (_hit); 166–185 → 123–131 ns a cached glyph drawn |
| B2.1 | `% cap` per probe | FIXED | `60104e6`; table rounded to a power of two, probed by mask; atlas_test 100 → 128 |
| B2.2 | `raster.subpixel` division per axis | FIXED | `60104e6`; power-of-two steps by arithmetic shift; atlas_test compares with raster.subpixel incl. negative pens |
| B3 | `color.kind` scans table directory per glyph | FIXED | `d8586af`; color.any cached per open face; plain faces skip kind; color_test any() |
| B4 | shaping plan compiled per shaped item | FIXED | `53297f5`; last plan kept by face/script/lang/dir/coords for default and Thai shapers with no text features; HarfBuzz 80,714 cases pass; a4 bench 1,241.8 → 1,216.3 M instructions (−2.0%), time within ms resolution |
| B5.1 | per-glyph divisions in `_draw` | FIXED | `d8586af`; divisions by power-of-two scale/page sizes as exact multiplications, others still divided |
| B5.2 | `_open` scan before the clip test | FIXED | `d8586af`; clip test before _open |
| B5.3 | `upload` at the end of every run | NOT_APPLICABLE | shown not to cost: upload sends only dirty regions (clean pages upload nothing); skipping even the clean-page loop measured 242–274 vs 218–247 ns a 3-glyph run — no difference; reverted |
| B5.4 | `_open` scan per glyph without last-face cache | FIXED | `d8586af`; each run face opened once a draw; with B3/B5: 131–145 → 81–82 ns a glyph |
| C1.1 | present: copy then separate swizzle/premultiply | FIXED | `cb9f39a`; texture.read_region_as converts as it copies; device.native_pixels/submit_native; 1080p 1.52–2.11 → 1.31–1.66 ms, same bytes |
| C1.2 | buffer-age damage recopied and reconverted | FIXED | the reconversion is gone: the window's texture is in the buffer's order (`7e9f7e3`), so the copy buffer age requires is a plain copy (depth-24 X11) or a premultiply alone, 0.94 → 0.46 ms a full opaque 1080p frame; the copy of the last frame's damage itself is required (not redundant: overlaps merged) |
| C1.3 | offscreen texture in platform (BGRA) order | FIXED | `7e9f7e3`; texture.BGRA8 drawn into by trading red and blue as colours are made (pass order, coverage tables, pictures turned, texels turned back, clears); host uses it where native_pixels says an order; opaque present 0.94 → 0.46 ms/1080p; bands_test: whole scene BGRA8 = RGBA8 traded, one thread and bands, three mutants caught; RGBA frame same hash, +0.29% instructions |
| C2 | Wayland `_to_argb` byte-wise | FIXED | `fca4705`; gpu/bgra (via window/pixels) two pixels a word, opaque pairs no arithmetic; 1080p 1.86–1.96 → 0.94 ms; pixels_test byte-exact vs old code |
| C3.1 | ortho/screen-space vertex fast path | FIXED | `e91bea1`; mvp read in place, w = 1 when last row is 0 0 0 1, no divide by w = 1; exact |
| C3.2 | `_six` shades 6 vertices of a quad | FIXED | `e91bea1`; repeated vertices (bytes equal) reuse the shaded corner; rect_test mutation (forced reuse) fails |
| C3.3 | tint multiply when tint is white | FIXED | `e91bea1`; with C3.1/C3.2: 12.2–13.0 → 9.6–10.0 M cycles, 38.6 → 35.0 M instructions a frame, same hash |
| C3.4 | 72-byte f64 vertices | PARTIAL | `53de283`: nine-f64 decode in place (35.0 → 30.1 M instructions a frame); `dbd0c6e`: 64-byte vertices, no z (draw2d's z is always 0): same hash, 29.470 → 29.359 M instructions, vertex buffer 4.7 → 4.2 MB; f32/u8 positions and colours BLOCKED by the pixel-exact rule (they round; prerequisite: the owner accepting that) |
| C3.5 | vertices stored twice / copied per flush (= E2.1) | FIXED | `c3c1ebf`; caps.maps_in_place; draw2d writes into the vertex buffer; 30.08 → 29.33 M instructions, ~1.9 MB copy a frame fewer |
| C4 | `rect_shaded`/`_same_shaded` by-value structs | FIXED | `7d9ed09`; confirmed rep movsb 6×80 B in the callee; now pointers; same hash, time within noise |
| C5 | band threads spawned per pass | FIXED | `4e8fba0`; keep_threads (Linux, opt-in, host.run); 4 threads best 1.26–1.34 → 1.14–1.18 ms a frame; bands_test both modes |
| C6 | `_image_fills` written from band workers | FIXED | `7d9ed09`; raster.count_fills off around band passes; bands_test |
| D1.1 | X11 ConfigureNotify not coalesced | FIXED | `ca02456`; size settled when next asked; x11_test 5 sizes → 1 rebuild (mutant: 5) |
| D1.2 | SHM backing rebuilt on every size | FIXED | `dc0edd5`; segments kept with 25% room when the size fits; drag step 2.53–2.66 → 0.46–0.57 ms |
| D2.1 | SETTINGS rethemes/relayouts when nothing changed | FIXED | `6958eec`; equal theme/locale skipped; 515–605 → 37 µs; app_test |
| D2.2 | `set_direction` invalidates when unchanged | FIXED | `6958eec`; unchanged direction does nothing; app_test |
| D3 | `pointer_shape` hit test every round | FIXED | `64bdac0`; tree stamp memo; 4807 → 3 ns; pointer_shape_test |
| E1.1 | tree mapping zeroed at open | FIXED | `1003faf`; fresh anonymous mappings not cleared; tree open 1.24–1.64 → 0.63–0.79 ms |
| E1.2 | app/device blocks zeroed at open | FIXED | `1003faf`; same, app.open and device init |
| E2.2 | canvas virtual reservation per canvas | FIXED | `995562d`: in-place canvases reserve no second vertex room (5,046,272 → 327,680 B a default canvas besides its buffer); `2261c02`: nested cache canvases start at 8,192 vertices and grow (12.4 MB less for three); the host's and first cache canvas keep the program's limits |
| E2.3 | `invalidate_paint_subtree` on texture recreation | FIXED | `c590076`; root alone marked; cached subtrees not re-rendered on a new window texture (cache_test: renders 0, pixels as uncached; 1 render with the old marking) |

## Architecture (tracked apart)

| ID | Option | State |
|---|---|---|
| F1 | shape coverage cache | BLOCKED — evaluated: 60 rounded controls (fill + border, 120×32, r 6, scale 1.5) 38,520 vertices, 4.53 ms vs 0.36 ms as two quads each (f1 bench); largest remaining gain; a mask composites once where the tessellation blends overlapping fringe triangles in turn, so not pixel-exact: prerequisite the owner accepting that, and an atlas budget. Exact work on the same path is in the rasteriser (_row_span 31%, _fill_unlit 15%, _triangle 11% of it) |
| F2 | retained per-run text geometry | FIXED — `28eb3f9` glyph-cache hints; `53dbcc9` one reserve a quad; `0d7d9fd` a run drawn again as it was copies its kept quads in, checked against every face and glyph-cache entry they came from. A run drawn again 65–77 → 15 ns a glyph; r_bench 29.359 → 26.818 M instructions a frame, same hash; vertices, batches and pixels byte-identical (text_test, draw2d_test), each invalidation guard's removal caught |
| F3 | direct-to-buffer / swapchain presentation (C1 option 3) | NOT_APPLICABLE (software) / BLOCKED (GPU) — with C1.3 presenting is a copy (0.15–0.2 ms) or premultiply (0.46 ms) a full opaque 1080p frame; painting into the buffer instead means repainting what its age lacks (the last frames' damage), costlier than copying it, and breaks scroll pixel moves within a kept texture; swapchain presentation needs a GPU backend, none exists |
| F4 | skip background clear under an opaque root | FIXED — `a208bed`: the window's first widget is told it is painted over the background just laid (painter.S_ON_WINDOW_BG); a square opaque decoration leaves its fill out: one full-window fill less per full repaint (0.13 ms at 1080p), same pixels (decoration_test, mutant caught) |

## Cross-project (tracked apart)

| ID | Item | State |
|---|---|---|
| X1 | CausticOS #15: toolkit damage collapsed into one bounding box | FIXED in this repo (`e1874ce`): rectangles kept apart, a COMMIT each; damage_test (Linux): two corners copy 840 px, not 467,040; built for caustic-x86_64, not run on CausticOS (needs its image) |
| X2 | CausticOS G29 / browser F7: background/incremental image decoding API | PARTIAL (`6b86c39`, `90222aa`): load.Job background decode with sync fallback, one lock (caustic-image JPEG/WebP globals); cut-short, noisy and cancelled decodes tested; incremental (partial-image) decoding BLOCKED: caustic-image decodes whole files only, and siblings are not edited here |

## Gates

- Baseline `caustic-mk run test` on `c893bd7`: failed at
  `sh ui/tools/api.sh | diff ui/api.md -` (api.md 7 entries stale on main);
  with api.md regenerated (`193ba9b`): rc=0.
- Final, at `0d7d9fd`: test, test-x11, test-wayland, test-win32 and
  test-win32-native all rc=0.

## New issues found

- Caustic std math: sin/cos send an argument near a multiple of pi/2 —
  where every corner arc starts — through the big-argument reduction
  (_rem_pio2_big, 1.25% of the rounded-controls bench). Sibling, not edited.
- Found and fixed here: 4e8fba0 called os.linux.futex outside a Linux test,
  so Windows builds of the toolkit stopped in codegen (`02a5fd5`).
- Tooltip colour checks on Wayland and Win32 read a pixel on the tooltip's
  text (failing on c893bd7 too); read in its padding now (`ed3203b`, `bbd5bb6`).

- caustic-image: JPEG and WebP decoders keep their state in module
  variables — two decodes at once crash (load_test without the lock, 3 of 3).
  Sibling, not edited; load.cst serialises decodes.

- Caustic compiler: a struct used as an array element type in another struct before its own declaration is sized 0 with no diagnostic (seen with `Sizes` in `ui/text.cst`; fixed by declaring it first). Sibling repo, not edited.

- `ui/api.md` stale on `main` (fixed in `193ba9b`).

- Caustic runtime: `_start` ends with `exit` (syscall 60), not `exit_group`,
  when main returns. Threads still alive outlive main and the process's
  status becomes the last thread's (a program returning 3 reported 0 with a
  parked worker). Sibling repo, not edited; the band pool is opt-in and
  stopped explicitly because of it.

- Icons at fractional scale (1.5): the uncached path's own output differs
  by up to 229 per channel between atlas slots (sampling of coverage placed
  at different sub-texel offsets); the mark cache is identical to its own
  first draw within 1. Pre-existing.

- `host_popup_wayland_test` "painted in the tooltip's colour" fails on
  `c893bd7` (before this work) and after; not a perf regression.
