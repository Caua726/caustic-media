# PERFORMANCE_FINAL_REPORT — caustic-media

Done on branch `agent/media-perf` from `main` `c893bd7`, merged into
`main` at `5fea8a2` on 2026-11-24 (with caustic-image and caustic-compact
`main` at v0.2.0, local tags). Nothing pushed. Source: the
"Performance audit — caustic-media" of 2026-11-21. Per-item detail, one row
each, is in `PERFORMANCE_STATUS.md`; this report sums it up.

## Counts — the audit's 45 core subitems

| State | Count | Items |
|---|---|---|
| FIXED | 43 | A1.1–A1.3, A2.1, A3.1–A3.4, A4.1–A4.3, A5.1, A5.2, A6.1, B1, B2.1, B2.2, B3, B4, B5.1, B5.2, B5.4, C1.1, C1.2, C1.3, C2, C3.1, C3.2, C3.3, C3.4, C3.5, C4, C5, C6, D1.1, D1.2, D2.1, D2.2, D3, E1.1, E1.2, E2.2, E2.3 |
| PARTIAL | 0 | — |
| UNRESOLVED | 0 | — |
| NOT_APPLICABLE | 2 | A2.2, B5.3 (measured: the cost does not exist) |
| BLOCKED | 0 | — |

Architecture, tracked apart:

| Item | State | Why |
|---|---|---|
| F4 | FIXED (`a208bed`) | |
| F1 | FIXED (`4b171a0`) | rounding accepted by the owner on 2026-11-24 |
| F2 | FIXED (`28eb3f9`, `53dbcc9`, `0d7d9fd`) | |
| F3 (software) | every software step done (C1.1–C1.3, C2); direct-to-buffer NOT_APPLICABLE | with C1.3, copying is cheaper than repainting what the buffer's age lacks |
| F3 (GPU swapchain) | BLOCKED | investigated: Vulkan bindings exist, no backend; see below |

Cross-project, tracked apart:

| Item | State |
|---|---|
| X1 | FIXED, and verified on CausticOS in an isolated checkout |
| X2 | FIXED: background (`load.Job`) and incremental (`load.Stream`) decoding, over caustic-image and caustic-compact v0.2.0 |

## Measured A/B (best of repeated runs, same machine, before → after)

| Item | Measure | Before → after |
|---|---|---|
| A1 | layouts a keystroke (entry) | 2 → 1 |
| A2.1 | layouts per changed cut label per frame; 40 labels × 30 frames | 5 → 3; 137 → 89 ms |
| A3 | 200 frames × 60 icons + text | 96–106 → 24–32 ms |
| A4 | entry layouts a keystroke | 5 → 2 |
| A5 | clipped selection draw | 1.29–1.38 → 1.05–1.16 ms; any run, raised glyphs included |
| A6 | theme lookups | 61–64 → 50–51 µs |
| B1/B3/B5 | a cached glyph drawn | 166–185 → 81–82 ns |
| B4 | a4 bench, whole program | 1,241.8 → 1,216.3 M instructions |
| C1.1 | 1080p present | 1.52–2.11 → 1.31–1.66 ms |
| C1 premultiply | 1080p, translucent / opaque | 7.24 → 4.31–4.45 ms / 1.05 → 0.93 ms |
| C1.3 | 1080p present into a premultiplied buffer, opaque (BGRA8 window texture) | 0.94 → 0.46 ms |
| F4 | full repaint of a maximized window | one 1080p fill fewer (0.13 ms) |
| F2 (hints, one reserve a quad, kept quads) | a run drawn again, a glyph | 65–77 → 53 → 15 ns; r_bench 29.359 → 26.818 M instructions a frame (−8.7%), same hash |
| C3.4 (no z) | r_bench frame, same hash | 29.470 → 29.359 M instructions; vertex buffer 4.7 → 4.2 MB |
| C3.4 (f32, `00be77f`) | r_bench run, same hash; kept-quad replay | 839.1 → 834.3 M instructions; 18–19 → 15 ns a glyph; vertex buffer 4.2 → 2.1 MB |
| F1 (`4b171a0`) | 60 rounded controls (fill + border, 120×32, r 6, scale 1.5) | 38,520 → 6,840 vertices; 4.53 → 0.59 ms (two-quads floor 0.37 ms) |
| C2 | Wayland conversion, 1080p | 1.86–1.96 → 0.94 ms |
| C3.1–3 | per frame | 12.2–13.0 → 9.6–10.0 M cycles |
| C3.4/C3.5 | per frame | 35.0 → 30.1 → 29.33 M instructions |
| C5 | 4 band threads, a frame | 1.26–1.34 → 1.14–1.18 ms |
| D1.1 | 5 sizes in a burst | 5 → 1 rebuild |
| D1.2 | X11 drag step | 2.53–2.66 → 0.46–0.57 ms |
| D2 | settings said again | 515–605 → 37 µs |
| D3 | pointer_shape, nothing changed | 4807 → 3 ns |
| E1 | tree open | 1.24–1.64 → 0.63–0.79 ms |
| E2.2 | default canvas block; three nested cache canvases | 5,046,272 → 327,680 B; −12.4 MB reserved |
| E2.3 | cached subtree renders on a new window texture | 1 → 0 |
| X1 | CausticOS, two small corner changes in 800×600 | 467,040 → 840 px copied |
| X1 on CausticOS | same QEMU session, COMMIT area | 7,052,720 px / 93 COMMITs → 4,878,192 px / 133 COMMITs (−31%) |
| X2 (caustic-image) | 512×512 PNG, 396,523 B, fed 16 KB at a time | feeds 37 ms + finish 7 ms → feeds 8 ms + finish 0 ms (one-shot decode 6 ms); first rows after 16 KB |
| X2 (caustic-media) | 1024×768 PNG, 1,188,400 B, `load.Stream` in 16 KB pieces | slowest `stream_feed` 0.23–0.38 ms; whole 24–27 ms vs `load.bytes` 19–20 ms; same image |
| caustic-compact | inflate, 40 × (window + big) | 23.8–24.2 → 23.3–23.7 ms after the cut-stream fix (unchanged) |

Not kept, because nothing measured better:
- B5.3: skipping clean uploads gave 242–274 ns a run after, against
  218–247 ns before.
- A2.2: a two-slot measure cache gave 71–73 µs both ways.

## Pixel evidence

Raster, vertex and presentation changes were checked against references that
already existed. The new tests follow the same rule:
- `rect_test` compares the full 2D scene with and without the fast paths,
  every pixel.
- `gpu_test` compares the device path with the bare rasteriser.
- `bands_test` covers both pool modes.
- `draw2d_test` covers in-place and uploaded vertices, with growth.
- The X11 tests compare `XGetImage` byte for byte, with and without
  MIT-SHM, including the transparent window.
- `pixels_test` checks every channel at every alpha against the old
  division, with the pixel both first and second in its pair, at every
  count and start.
- `cache_test` compares every frame against the same tree painted with no
  cache, now including a new target filled with garbage.
- The shaper passes HarfBuzz's 80,714 cases, plus the same cases cut in two.

F1 and C3.4 are no longer pixel-exact (owner's decision, 2026-11-24); their
evidence is quality, not equality:
- F1: every corner pixel of the cached path is within 25.5/255 of a 16×16
  supersampled exact area (draw2d_test). Against the old tessellation
  (`f1_cmp`): 1.7–2.1% of pixels differ, mean 0.12–0.18/255, largest 73–81,
  all at corners where the old path was the inaccurate one (1-px border,
  R 18, pixel (12,1): true coverage 0.904, mask 0.89, old ≈0.3). Translucent
  interiors are unchanged; a frame with 40 radii keeps the first shape's
  corner (no in-frame eviction). Text is untouched by F1.
- C3.4: r_bench, the full scene with text, icons and translucency, kept its
  hash (5118441270678321031) across the f32 change; rect_test, gpu_test,
  bands_test and draw2d_test pass unchanged.

Mutants were run for the new checks: C3.2 forced reuse, D1.1, the A5 reach,
the B4 memo ignoring direction, the X2 decode lock, the premultiply rounding,
and the 4-pixel group. Each was caught.

## Commits (c893bd7..HEAD)

These are the hashes on branch `agent/media-perf`. On `main`,
caustic-push-daily re-dates commits as it sends them to GitHub, which gives
each one, and every one after it, a new hash (same tree, message and
author); the subjects below identify them. The merge was `5fea8a2` when
the final gates ran on it; the first daily send made it `485dcb5`.

```
5fea8a2 Merge agent/media-perf: the performance audit's fixes, progressive image loading
6baa23a image/load: a Stream decodes an image as its bytes arrive
00be77f draw2d: 32-byte f32 vertices
4b171a0 shapes2d: rounded rectangles from kept corner masks
0d7d9fd ui/text: a run drawn again as it was copies its quads in
53dbcc9 draw2d: a quad reserves its six vertices at once
dbd0c6e draw2d: 64-byte vertices, no z
28eb3f9 text/atlas, ui/text: a run's glyph found where it was last time
a208bed ui: a square opaque frame over the window's background is not filled again
7e9f7e3 gpu: a BGRA8 render target; the host draws in its window buffer's order
bbd5bb6 ui/host_popup_win32_test: the tooltip's colour read in its padding
90222aa tests: decodes cut short or cancelled; CausticOS damage clipped and capped
02a5fd5 gpu/software: the band pool's futex calls inside the test for Linux
2261c02 ui/paint: a nested cache canvas starts small and grows
da06bba ui/text: a clipped draw finds its glyphs by halving in any run
ed3203b ui/host_popup_wayland_test: the tooltip's colour read in its padding
bf5f84b gpu/bgra: four pixels a step while they are opaque
9ffd9e9 gpu/bgra: premultiplied by shifts, red and blue in one product
53297f5 text/shape: a plan kept for the next text that would compile the same
c590076 ui/host_paint: a window's new texture keeps its subtrees' textures
6b86c39 image/load: decoding in the background, as a Job
e1874ce window/causticos: damage kept as rectangles, not one box (CausticOS #15)
995562d draw2d: a canvas drawing in place reserves no second room for vertices
1003faf ui, gpu: fresh mappings are not cleared again
8bd7990 ui: a widget reads the theme where it is, not a copy
994900e ui/text: a clipped draw walks only the lines that can reach the clip
64bdac0 ui/router: the pointer's shape remembered while nothing changes
6958eec ui: the desktop's settings said again lay out and draw nothing again
dc0edd5 window/x11: a new size that fits keeps the SHM segments
ca02456 window/x11: a burst of sizes rebuilds the buffers once
4e8fba0 gpu/software: band workers kept between passes while a loop runs
cb9f39a present: the frame converted as it is copied into the window's buffer
fca4705 window: one conversion to a platform's pixels, two at a time
c3c1ebf draw2d: on a device that draws from its buffers in place, no vertex copy
53de283 gpu/software: draw2d's vertices read as the nine f64s they are
e91bea1 gpu/software: a quad shades four corners; flat projections skip w
7d9ed09 gpu/software: a quad's corners by pointer; band workers do not race on a count
3347ba6 ui: icons kept as coverage on pages of their own
d8586af ui/text: a glyph away from the clip opens nothing; plain faces skip colour
60104e6 text/atlas: a glyph found is looked up once, by masks and shifts
4556a45 ui: a widget that keeps a Layout lays its text out once
2eeb826 ui: a style's ellipsis, line height and digit width laid out once
404911b ui/label: the least size leaves a cut or wrapped run as it is drawn
193ba9b ui/text: count the layouts a service makes; api.md current
```

Each fix commit gives its claim, its test and its A/B in the message. Three
commits update status documents only (`3af5475`, `cd678d4` and the final
one), so they are not listed above.

## API guidance (new or changed public calls)

- **Rounded shapes (`4b171a0`):** `shapes2d.fill_rounded*` and
  `stroke_rounded` use cached corner masks by themselves when the shape has
  one radius, sits on whole pixels and the canvas has no transform or
  polygon clip. `shapes2d.kept_corners(0)` turns it off (tests, references).
  A canvas keeps up to 32 masks and frees them at `close`;
  `draw2d.mask_count` reports them.
- **f32 vertices (`00be77f`):** `draw2d.Vertex` is eight f32s (32 bytes),
  `pipeline.layout_pos2_color_uv_f32()`. A custom pipeline fed draw2d's
  buffer must use that layout.
- **Images as their bytes arrive (`6baa23a`):** `image.load.Stream`:
  - `stream_open`, then `stream_feed(&s, data, n)` per piece (copies,
    returns at once), `stream_end`; `stream_size` once the header is read;
    `stream_rows` rows of `stream_shown(&s)` final (non-interlaced PNG);
    `stream_done`, `stream_take` (the image `load.bytes` gives),
    `stream_error`; `stream_close` at any time, also the cancel.
  - Ownership: the Stream stays put from open to close; `stream_shown` is
    the Stream's until close; the taken image is the caller's.
  - Threads: the decoding is on the Stream's own thread; a shown row is
    written before `stream_rows` counts it and never changes after, so it
    is read without a lock. Final JPEG/WebP decodes take the decode lock,
    as `bytes()` does. Close every Stream before `main` returns.
  - Where no thread can start, `stream_feed`/`stream_end` do the work.
  - Underneath: caustic-image `progressive.cst` (`feed`, `finish`, `take`,
    `close`, `kept`) and caustic-compact `inflate_stream.cst`
    (`inflate_stream`, `inflate_stream_run`), both v0.2.0.

- `gpu.texture.BGRA8` is a render target in a window buffer's order. The
  host uses it whenever `device.native_pixels` names an order;
  `read_region_as` from it is then a plain copy (`BGRA`) or a premultiply
  alone (`BGRA_PREMULTIPLIED`). A device that cannot create one returns 0
  from `texture.create`, and the caller falls back to `RGBA8`, as the host
  does. `raster.set_bgra` is the software device's per-pass order: the
  backend sets it at `BEGIN_PASS` and resets it at `END_PASS`.
- **Kept quads (`0d7d9fd`):** `ui.text.draw` keeps a run's quads by
  itself. Callers change nothing. A run's quads are copied in from its
  third identical draw on, which means:
  - the same service, pen, scale and colours;
  - the same canvas origin and innermost clip;
  - no transform or polygon clip.

  The run keeps up to about 520 bytes per drawn glyph, and gives them back
  when it is laid out again, given new text, or closed with `run_close`.
  `Service.replayed` counts copied draws.

  New lower-level calls: `atlas.entry_copy`, `entry_same` and `entry_found`,
  and `draw2d.quad_state`, `vertex` and `glyph_quads`.
- `pipeline.layout_pos2_color_uv()` is a 64-byte vertex layout
  (x, y, colour, uv), which draw2d now uses. `atlas.get_hinted` is
  `atlas.get` given where the glyph was found last time (`*hint`, -1 when
  not known); `ui.text` keeps that in `Glyph.hint`.
- `painter.S_ON_WINDOW_BG` tells the window's first widget that it is
  painted straight over the background just laid down. A widget that fills
  that same opaque colour without rounding can skip the fill.
  `paint.set_bg_hint(0)` turns this off for tests.
- `gpu.texture.read_region_as(d, h, x, y, w, ht, dst, stride, convert)` reads
  a region in the order a platform wants: `bgra.AS_IS`, `BGRA` or
  `BGRA_PREMULTIPLIED`. `read_region` is the `AS_IS` case.
- `window.device`:
  - `native_pixels(w)` says which conversion a window's buffer wants.
  - `submit_native` and `submit_region_native` present pixels that are
    already converted. The host uses them whenever `native != 0`.
- `gpu.keep_threads(d, on)` keeps the software band workers between passes.
  It works on Linux only and is opt-in. Pair every `keep_threads(d, 1)`
  with `keep_threads(d, 0)` before `main` returns: the Caustic runtime
  ends with `exit`, not `exit_group`, so live workers would outlive `main`
  and the exit status would be lost. `ui.host.run` already pairs them.
- `device.caps.maps_in_place` is 1 where the device draws from a mapped
  buffer's own memory, as the software device does. In that case draw2d
  writes vertices straight into the buffer.
- `image.load.Job` decodes in the background (`image/image.md` has an
  example):
  - The calls are `start`, `done`, `take`, `cancel` and `error`.
  - `start` copies the bytes and returns at once. On a target with no
    threads it decodes before returning.
  - A Job stays put from `start` until `take` or `cancel`. Take or cancel
    every Job before `main` returns, and do not call `start` again before
    taking.
  - `bytes()` and `file()` are unchanged and synchronous. All decodes share
    one lock.
- `text.shape.plans_compiled(s)`, `layout.measured()`, `tree.stamp()` and
  `tree.bump()`, plus a few other counters, exist for tests. A widget's
  shape function that reads state changing with no paint or layout mark
  must call `tree.bump()`, or `pointer_shape` keeps the old shape.
- `widget.theme_at(t, w, &room)` returns a pointer to the tree's own theme.
  Use it instead of copying `widget.theme(...)`.

## What is left, and why

- **The v0.2.0 tags are local.** caustic-media's Causticfile asks for
  caustic-image and caustic-compact `v0.2.0`, which exist only in the local
  repositories (nothing was pushed or released). Builds here use the
  `.caustic/deps` checkouts already at those tags; a fresh clone elsewhere
  cannot fetch them until both tags (and the three `main` branches) are
  pushed.

- **C3.4, FIXED:** 64-byte vertices with no z (`dbd0c6e`), then eight f32s
  (`00be77f`) once the owner accepted rounding. u8 colours were not done:
  the colour is four f32s inside the 32 bytes, and packing it to 4 bytes
  would save 12 more a vertex at the price of an unpack per vertex in the
  software device, which r_bench did not justify.
- **F1, FIXED (`4b171a0`):** see the A/B and pixel sections. Not covered:
  gradients whose top and bottom differ, two radii, shapes off whole
  pixels, transforms and polygon clips; they keep the tessellation.
- **F2, FIXED:** done in three steps.
  - `28eb3f9`: each glyph remembers its glyph-cache entry.
  - `53dbcc9`: a quad reserves its six vertices at once.
  - `0d7d9fd`: a run drawn again the same way copies its kept quads in.
  - Only one thing is left out: colour-font runs walk their glyphs as
    before.
  - Tried and dropped: inlining floor in the glyph cache's subpixel step,
    and in the rasteriser's row span. Neither gained anything.
- **F3:**
  - **Software, done:** C1.1–C1.3 and C2 leave presenting a copy
    (0.15–0.2 ms) or a premultiply alone (0.46 ms) per full opaque 1080p
    frame. Painting straight into the window's buffer would mean repainting
    everything its age lacks, which costs more than copying it, and it would
    break scroll pixel moves within a kept texture: NOT_APPLICABLE.
  - **GPU swapchain, BLOCKED (investigated, not implemented):** `gpu/vk/bind`
    holds Vulkan bindings (loader, structs, layout checks against lavapipe),
    but there is no `vk/backend.cst`; `gl/` and `d3d/` are design notes;
    `gpu.cst` gives `NO_BACKEND` for every backend but software. The machine
    has lavapipe and radeon ICDs and glslc. Prerequisite: a Vulkan backend
    implementing `device.cst` (draw2d's pipeline as SPIR-V, textures,
    a swapchain on the window's native handles). That is a backend project,
    not a performance fix, and nothing here claims it.
- **X1, verified on CausticOS (2026-11-24):** no release was needed. An
  isolated clone in `/var/tmp/cm-x1` (disk, not the `/tmp` RAM disk; removed
  after the run) of
  causticos `19e1629`, with its caustic-media dependency overridden by this
  branch at `00be77f`, built (122 targets, 23 MB ISO, under the caps).
  `scripts/test-clip-focus.sh` passed. A QEMU/QMP session (typing with the
  pointer on the title bar, maximise twice) gave 0 differing pixels between
  the screenshots, and copied 31% less (above). The sibling checkout was
  not touched. Each frame sends up to 16 COMMITs of 32 bytes instead of one.
- **X2, FIXED and integrated (2026-11-24):**
  - caustic-compact `main` `33023cf` (local tag `v0.2.0`): `a08d265` fixes
    the inflate that never returned on a cut stream (root cause: the 9-bit
    table paths took codes from the zero bits past the input's end) and a
    one-byte gzip taken as empty; `33023cf` adds `inflate_stream`, which
    resumes at the symbol where the input ran out, Huffman tables kept, so
    a stream fed in any pieces costs one inflate.
  - caustic-image `main` `f7d9733` (local tag `v0.2.0`): the JPEG test
    build fix, the PNG scanline helpers, `progressive.cst` over
    `inflate_stream`; its private inflater (`zsteps`) is gone and PNG, TIFF
    and CSIF use caustic-compact's fixed inflate again. `finish` keeps the
    rows decoded as the PNG came when the file arrived whole and checks
    out, so nothing is decoded twice.
  - caustic-media `6baa23a`: `load.Stream` (above), dependencies at v0.2.0.
  - Overhead: 512×512 PNG fed in 16 KB pieces, 37 ms of feeds + 7 ms
    finish (about 7× a one-shot decode) → 8 ms + 0 ms against 6 ms; the
    stream inflater is 1.7× `inflate` per byte, nothing repeated. In
    caustic-media a 1024×768 PNG in 16 KB pieces: slowest `stream_feed`
    0.23–0.38 ms on the caller's thread, whole 24–27 ms vs `load.bytes`
    19–20 ms, the same image.
  - Limits: rows appear for non-interlaced PNG only; JPEG, GIF, BMP and
    QOI give their size early and their pixels at the end. A PNG whose
    first IHDR has an impossible size is refused at once, even if a second
    IHDR (malformed by the spec) would have been accepted by `decode()`.

## New issues found (not part of the audit)

- **Caustic std math:** sin/cos send an argument near a multiple of π/2 —
  where every corner arc starts — through the big-argument reduction.
  That is 1.25% of the rounded-controls bench.
- **Found and fixed here:** `4e8fba0` called `os.linux.futex` outside a
  Linux test, so Windows builds of the toolkit stopped in codegen. Fixed
  in `02a5fd5`.
- **Tooltip colour checks:** on Wayland and Win32 they read a pixel on the
  tooltip's own text, failing on `c893bd7` as well. They now read it in
  the padding (`ed3203b`, `bbd5bb6`).

- **Caustic runtime:** `_start` ends with `exit` (syscall 60), not
  `exit_group`. Threads still alive when `main` returns keep the process
  running, and its exit status is lost (3 was reported as 0).
- **Caustic compiler:** a struct used as an array element type before it is
  declared is sized 0, with no diagnostic.
- **caustic-image:** the JPEG and WebP decoders keep their state in module
  variables, so two decodes at once crash (3 of 3 runs without the lock).
- **Icons at scale 1.5:** the uncached path differs from itself by up to 229
  a channel between atlas slots. This predates the branch.
- **`ui/api.md`:** was stale on `main`; regenerated in `193ba9b`.
- **mmap failure checks:** `draw2d.open` and `sw_buffer_create` test
  `mmap_anon` for 0 only, not for -1. This predates the branch and was
  found in review.
- **caustic-image's JPEG tests did not build:** `encode_ex` declared `i`
  twice (f9ecbfa). Fixed on caustic-image `main` (`78bdef3`).
- **caustic-compact `inflate` hung on a cut stream:** cut inside a Huffman
  block, it decoded the zero bits past the end without stopping, so a
  crafted 1 KB PNG with valid CRCs hung `png_decode` — and `load.bytes`
  here. Fixed at the root in caustic-compact `a08d265` (with regression
  tests: every fixture cut as raw, zlib and gzip, the PNG's stream, 400
  garbage inputs); caustic-image keeps the PNG as `repro_png_cut`.
- **caustic-compact `gzip_decompress`** took a lone first byte as an empty
  stream; it is a member cut short now (`a08d265`). Empty input still
  gives 0.
- **Profile finding not in the audit:** `cmap` format-12 lookup is 9.45% of
  an entry keystroke.

## Review

One read-only reviewer read every fix commit's diff. It found no serious bug.
Its residual risks:
- **Face key:** the shaper memo keys the face by pointer and size, so a font
  freed and another of the same size mapped at the same address would hit a
  stale plan (about 20%).
- **Job misuse:** `start` again before `take` leaks the result; this is
  documented.
- **Pointer-shape stamp:** a shape function reading state that changes with
  no invalidation would stay stale; `tree.bump` covers it.
- **COMMIT count:** checked above; the sends are blocking whole writes.

## Gates

Final, on the integrated `main` branches (caustic-media `5fea8a2` in a
clean worktree, caustic-image `6d92042`, caustic-compact `33023cf`), one at a
time inside the 2.5 GiB memory and 100% CPU caps, `WAYLAND_DISPLAY` unset:

| Script | Result |
|---|---|
| caustic-media `caustic-mk run test` | rc=0, no FAIL (includes the `ui/api.md` gate, `causticos_damage_test`, `load_test` with the Stream checks) |
| caustic-media `caustic-mk run test-x11` | rc=0 |
| caustic-media `caustic-mk run test-wayland` | rc=0 |
| caustic-media `caustic-mk run test-win32` | rc=0 |
| caustic-media `caustic-mk run test-win32-native` | rc=0 |
| caustic-image `caustic-mk run test` | all green (40 suites, with `test_progressive` and `repro_png_cut`) |
| caustic-compact `caustic-mk run test` | all green (with `[truncated]` and `[inflate_stream]`) |

After them caustic-image took one more commit, `f7d9733`, which removes two
unused locals from `png_decode`; its suite and caustic-media's `load_test`
and `save_test` passed again against it. The gates also passed at
`00be77f` and `0d7d9fd` on the branch.

Getting there took three fixes:
- **Windows build (my regression):** the Win32 scripts first failed on a
  Linux system call in Windows codegen, which this branch had introduced
  (`02a5fd5` fixes it).
- **Tooltip colour checks:** the Wayland and Win32 checks read a pixel on
  the tooltip's text and failed on `c893bd7` as well. They now read it in
  the padding (`ed3203b`, `bbd5bb6`).
- **One timeout:** `test-win32-native` timed out once at
  `win32_dnd_named_test`. That test passes in 19.3 s both on `c893bd7` and
  on this branch, and every later run of the script passed.

The branch on CausticOS: built and run in the isolated checkout (X1 above).
