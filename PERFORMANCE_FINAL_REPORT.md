# PERFORMANCE_FINAL_REPORT — caustic-media

Branch `agent/media-perf` (worktree `../.agent-media-perf`), from `main`
`c893bd7`. Not merged, rebased or pushed; `main` untouched. Source: the
"Performance audit — caustic-media" of 2026-11-21. Per-item detail, one row
each, is in `PERFORMANCE_STATUS.md`; this report sums it up.

## Counts — the audit's 45 core subitems

| State | Count | Items |
|---|---|---|
| FIXED | 42 | A1.1–A1.3, A2.1, A3.1–A3.4, A4.1–A4.3, A5.1, A5.2, A6.1, B1, B2.1, B2.2, B3, B4, B5.1, B5.2, B5.4, C1.1, C1.2, C1.3, C2, C3.1, C3.2, C3.3, C3.5, C4, C5, C6, D1.1, D1.2, D2.1, D2.2, D3, E1.1, E1.2, E2.2, E2.3 |
| PARTIAL | 1 | C3.4 (compact vertices blocked by the pixel-exact rule) |
| UNRESOLVED | 0 | — |
| NOT_APPLICABLE | 2 | A2.2, B5.3 (measured: the cost does not exist) |
| BLOCKED | 0 | (C3.4's remainder is blocked; it is counted as PARTIAL) |

Architecture, tracked apart:

| Item | State | Why |
|---|---|---|
| F4 | FIXED (`a208bed`) | |
| F1 | BLOCKED | the pixel-exact rule |
| F2 | UNRESOLVED | evaluated; the bound is too small for the state it needs |
| F3 | NOT_APPLICABLE in software | with C1.3, copying is cheaper than repainting what the buffer's age lacks |
| F3 (GPU swapchain) | BLOCKED | there is no GPU backend |

Cross-project, tracked apart:

| Item | State |
|---|---|
| X1 | FIXED in this repository |
| X2 | PARTIAL: incremental decoding is blocked on caustic-image |

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

Mutants were run for the new checks: C3.2 forced reuse, D1.1, the A5 reach,
the B4 memo ignoring direction, the X2 decode lock, the premultiply rounding,
and the 4-pixel group. Each was caught.

## Commits (c893bd7..HEAD)

```
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

- `gpu.texture.BGRA8` is a render target in a window buffer's order. The
  host uses it whenever `device.native_pixels` names an order;
  `read_region_as` from it is then a plain copy (`BGRA`) or a premultiply
  alone (`BGRA_PREMULTIPLIED`). A device that cannot create one returns 0
  from `texture.create`, and the caller falls back to `RGBA8`, as the host
  does. `raster.set_bgra` is the software device's per-pass order: the
  backend sets it at `BEGIN_PASS` and resets it at `END_PASS`.
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

- **C3.4, rest BLOCKED by the pixel-exact rule:** compact f32/u8 vertices
  move positions and colours by rounding. Prerequisite: the owner accepts
  non-exact vertex quantisation.
- **F1, BLOCKED by the pixel-exact rule (biggest remaining gain):**
  - 60 rounded controls make 38,520 vertices and take 4.53 ms, against
    0.36 ms as two quads each.
  - A coverage mask composites once, where the tessellation blends
    overlapping fringe triangles one after another, so the pixels differ.
  - Prerequisites: the owner accepts that, and decides an atlas budget.
  - The exact work left on that path is in the rasteriser: `_row_span`
    31%, `_fill_unlit` 15%, `_triangle` 11%.
- **F2, UNRESOLVED (evaluated, not done):**
  - A kept run must still touch each glyph's atlas slot, so it is not
    evicted, and must still emit its vertices.
  - Only the lookups (`_look`, 2.6% of a text-heavy frame) and part of
    `_draw` (4.8%) would go.
  - In exchange, each run would keep per-run slot and era state that can
    go stale.
- **F3:**
  - **Software, NOT_APPLICABLE:** after C1.3 presenting is a copy
    (0.15–0.2 ms) or a premultiply alone (0.46 ms) per full opaque 1080p
    frame. Painting into the window's buffer instead would mean repainting
    everything its age lacks, which costs more than copying it, and it
    would break scroll pixel moves within a kept texture.
  - **GPU swapchain, BLOCKED:** it needs a GPU backend, and none exists.
- **X1:**
  - Built for `caustic-x86_64` and tested on Linux through `_frame_rects`,
    including clipping and the 16-rectangle cap.
  - Not run on CausticOS, which needs that repository's system image.
  - It sends up to 16 COMMIT messages a frame instead of one. Each is
    32 bytes, written whole by `_write_all`.
- **X2, incremental decoding BLOCKED:**
  - caustic-image decodes whole files only, and sibling repositories are
    not edited here.
  - Background decoding is done and tested, including cancellation and
    cut-short, noisy and malformed input.

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

All five gate scripts pass at `a208bed`, the last code commit. They ran one
at a time, inside the 2.5 GiB memory and 100% CPU caps, with
`WAYLAND_DISPLAY` unset:

| Script | Result |
|---|---|
| `caustic-mk run test` | rc=0, no FAIL (includes the `ui/api.md` gate, `causticos_damage_test`, `load_test`) |
| `caustic-mk run test-x11` | rc=0 |
| `caustic-mk run test-wayland` | rc=0 |
| `caustic-mk run test-win32` | rc=0 |
| `caustic-mk run test-win32-native` | rc=0 |

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

Not run: the branch on CausticOS itself, which needs the CausticOS
repository's system image.
