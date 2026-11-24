# PERFORMANCE_FINAL_REPORT — caustic-media

Branch `agent/media-perf` (worktree `../.agent-media-perf`), from `main`
`c893bd7`. Not merged, rebased or pushed; `main` untouched. Source: the
"Performance audit — caustic-media" of 2026-11-21. Per-item detail, one row
each, is in `PERFORMANCE_STATUS.md`; this report sums it up.

## Counts — the audit's 45 core subitems

| State | Count | Items |
|---|---|---|
| FIXED | 37 | A1.1–A1.3, A2.1, A3.1–A3.4, A4.1–A4.3, A6.1, B1, B2.1, B2.2, B3, B4, B5.1, B5.2, B5.4, C1.1, C2, C3.1, C3.2, C3.3, C3.5, C4, C5, C6, D1.1, D1.2, D2.1, D2.2, D3, E1.1, E1.2, E2.3 |
| PARTIAL | 5 | A5.1, A5.2, C1.2, C3.4, E2.2 |
| UNRESOLVED | 2 | B5.3, C1.3 |
| NOT_APPLICABLE | 1 | A2.2 |
| BLOCKED | 0 | — |

Tracked apart:
- Architecture: F1–F4 were evaluated and measured. None was rewritten, as
  the goal asked; all four are UNRESOLVED.
- Cross-project: X1 is FIXED in this repository; X2 is PARTIAL.

## Measured A/B (best of repeated runs, same machine, before → after)

| Item | Measure | Before → after |
|---|---|---|
| A1 | layouts a keystroke (entry) | 2 → 1 |
| A2.1 | layouts per changed cut label per frame; 40 labels × 30 frames | 5 → 3; 137 → 89 ms |
| A3 | 200 frames × 60 icons + text | 96–106 → 24–32 ms |
| A4 | entry layouts a keystroke | 5 → 2 |
| A5 | clipped selection draw | 1.29–1.38 → 1.05–1.16 ms |
| A6 | theme lookups | 61–64 → 50–51 µs |
| B1/B3/B5 | a cached glyph drawn | 166–185 → 81–82 ns |
| B4 | a4 bench, whole program | 1,241.8 → 1,216.3 M instructions |
| C1.1 | 1080p present | 1.52–2.11 → 1.31–1.66 ms |
| C1 premultiply | 1080p, translucent / opaque | 7.24 → 4.31–4.45 ms / 1.05 → 0.93 ms |
| C2 | Wayland conversion, 1080p | 1.86–1.96 → 0.94 ms |
| C3.1–3 | per frame | 12.2–13.0 → 9.6–10.0 M cycles |
| C3.4/C3.5 | per frame | 35.0 → 30.1 → 29.33 M instructions |
| C5 | 4 band threads, a frame | 1.26–1.34 → 1.14–1.18 ms |
| D1.1 | 5 sizes in a burst | 5 → 1 rebuild |
| D1.2 | X11 drag step | 2.53–2.66 → 0.46–0.57 ms |
| D2 | settings said again | 515–605 → 37 µs |
| D3 | pointer_shape, nothing changed | 4807 → 3 ns |
| E1 | tree open | 1.24–1.64 → 0.63–0.79 ms |
| E2.2 | default canvas block | 5,046,272 → 327,680 B |
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

## What is left, and why (real blockers only)

- **C1.3 (BGRA target):**
  - At most 0.7 ms on a full opaque 1080p frame. A plain read takes 0.23 ms
    and the fused read 0.93 ms.
  - The work would carry a target format through every raster write and
    blend path.
  - F3 removes the copy altogether, so it is the better place to spend the
    effort.
- **C1.2:** the recopy of the last frame's damage is inherent to two buffers
  fed from an offscreen texture. It is now one fused, faster pass; only F3
  removes it.
- **C3.4:** compact f32/u8 vertices change positions and colours, so they
  cannot be pixel-exact. The nine-f64 decode was kept instead.
- **A5:** one clipped call per selection rectangle remains, but each call
  walks only the lines that can reach its clip. Runs that are not sorted by
  line still walk everything.
- **E2.2:** the per-canvas vertex buffer (4.7 MB virtual) and the growth
  limit remain. Shrinking them changes how big a frame can be.
- **F1 (biggest remaining gain):**
  - 60 rounded controls make 38,520 vertices and take 4.53 ms, against
    0.36 ms as two quads each.
  - A coverage cache would not be pixel-exact against the tessellation, and
    the atlas budget must be decided first. That is a design decision for
    the owner, not a speculative rewrite.
- **F2:** about 12% of a text-heavy frame is text-side CPU. Rasterisation
  (52%) is unaffected.
- **F3:** this is the route to GPU backends. It needs buffer-age painting
  on each platform.
- **F4:** at most 0.13 ms per full 1080p repaint.
- **X1:** built for `caustic-x86_64` and tested on Linux through
  `_frame_rects`. It was not run on CausticOS, which needs that
  repository's image.
  - It now sends up to 16 COMMIT messages a frame instead of one. Each is
    32 bytes, written whole by `_write_all`, and the window manager adds
    each to its damage.
- **X2:** incremental, partial-image decoding needs changes inside
  caustic-image. That is a sibling repository and was not edited.

## New issues found (not part of the audit)

- **Caustic runtime:** `_start` ends with `exit` (syscall 60), not
  `exit_group`. Threads still alive when `main` returns keep the process
  running, and its exit status is lost (3 was reported as 0).
- **Caustic compiler:** a struct used as an array element type before it is
  declared is sized 0, with no diagnostic.
- **caustic-image:** the JPEG and WebP decoders keep their state in module
  variables, so two decodes at once crash (3 of 3 runs without the lock).
- **Icons at scale 1.5:** the uncached path differs from itself by up to 229
  a channel between atlas slots. This predates the branch.
- **`host_popup_wayland_test`:** "painted in the tooltip's colour" fails on
  `c893bd7` as well. This predates the branch.
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

All gates were run at `bf5f84b`, inside the memory and CPU caps, with
`WAYLAND_DISPLAY` unset.
- `caustic-mk run test` returned rc=0, with no FAIL lines. This includes the
  `ui/api.md` gate and the new `causticos_damage_test`.
- `caustic-mk run test-x11` returned rc=0.
- `caustic-mk run test-wayland` returned rc=1 at `host_popup_wayland_test`
  ("painted in the tooltip's colour"). That check fails the same way on
  `c893bd7`, before this branch. The script stops there, so the 42 steps
  after it were run one by one, and all passed.
- Not run: `test-win32` and `test-win32-native` (Windows parts come later in
  the project's order), and CausticOS on the system itself.
