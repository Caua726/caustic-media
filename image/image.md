# The image layer

Pixels on the CPU. An `Image` is memory the program owns, in a known format,
that it can transform, generate, pack and eventually hand to `render/` to become
a texture.

```
image/
  image.cst     hub
  buffer.cst    the Image type: pixels, format, stride, ownership
  load.cst      decode and encode — opt-in, through caustic-image
  transform.cst resize, crop, rotate, flip, convert, premultiply, blit
  generate.cst  solid, gradient, checker, noise
  mipmap.cst    the chain a texture wants at upload
  atlas.cst     rectangle packing, and the atlas as a thing you can query
```

---

## Why this exists when caustic-image already does most of it

[caustic-image](https://github.com/Caua726/caustic-image) is a good library and
it is not a framework. It decodes eleven formats — bmp, gif, ico, jpeg, png,
pnm, qoi, tga, tiff, vp8, webp — and it already carries the pixel operations:
`op_convert`, `op_crop`, `op_flip_h`, `op_flip_v`, `op_rotate`,
`op_resize_nearest`, `op_resize_bilinear`, `op_premultiply`.

So this layer does not exist to reimplement any of that. It exists because a
framework has to present **its own type**, and that is a different job from
having the algorithm:

- An `Image` uses this framework's colour type from `math/color`, its rectangle
  from `math/rect`, its error model — a possibly-invalid handle and `last_error`
  rather than a return code.
- It converts to a `render.TextureHandle` in one call, because that is the
  reason most images exist here.
- It is the same shape as every other resource in the library, so a program that
  has learned one has learned this.

This is the same reason SDL has `SDL_Surface` with libpng available, and raylib
has `Image` with stb_image available. The decode belongs to a library; the type
belongs to the framework.

**Decoding stays opt-in.** `image.load(path)` is what pulls caustic-image in; a
program that only generates or transforms images never links a decoder. Same
rule as the GPU backends.

---

## What is here that is nowhere else

Three things neither caustic-image nor `render/` covers, and each has a consumer
already waiting:

**Atlas packing.** Rectangle bin packing, plus the atlas as something you can
query — where did this sub-image land, what are its UVs. `text/` needs it for a
glyph atlas and `render/`'s 2D batching needs it for sprites, and batching by
atlas is the single largest 2D win available. It is an algorithm rather than an
image operation, but the atlas *is* an image, and splitting the two would put
the packer somewhere it has no relationship with its own output.

**Mipmap chains.** Generated on the CPU here, uploaded as levels by
`render/texture`. A GPU can generate them itself, and a software backend cannot,
so having the CPU path means the same code works on both.

**Generation.** Solid, gradient, checker, noise. Small, and it is what makes a
program able to draw something before it has any assets — a checkerboard is the
fastest way to see that texturing works at all.

---

## Ownership

An `Image` owns its pixels or borrows them, and says which. Borrowing matters:
decoding straight into a buffer the program already has, or wrapping a region of
a larger image without copying, are both things a framework should not force a
copy for.

That mirrors `gpu/software`'s `Target`, which is a view over memory the caller
owns, and it is the same reason: *"no hidden allocations"* means the library does
not decide to allocate on a program's behalf without being asked.

---

## Decoding in the background

`load.bytes` and `load.file` decode on the caller's thread and return when
the image is whole; they stay as they are. A program that must not wait —
a file manager's thumbnails, a browser's images — uses a `load.Job`:

```
let is load.Job as j;              // stays put until take or cancel
load.start(&j, data, len);         // copies the bytes; returns at once
...                                // each frame: draw a placeholder
if (load.done(&j) == 1) {
    let is image.buffer.Image as im = load.take(&j);   // no pixels: load.error(&j)
}
// or, no longer wanted: load.cancel(&j);
```

Where no thread can be started (the target has none, or spawning fails),
`start` decodes before it returns and the Job is done at once: the same
code runs, only without the overlap. Every decode — a Job's, `bytes`'s,
`file`'s — takes one lock, because caustic-image's JPEG and WebP decoders
keep their state in module variables; Jobs overlap the program, not each
other. A Job decodes a complete file, so there is no partial image to show
while it runs, and `cancel` after the decode began waits for it to end and
drops the result. A program takes or cancels its Jobs before `main` returns
(its runtime ends with `exit`, not `exit_group`).

## Decoding as the bytes arrive

A program that gets an image in pieces — a download, a pipe — uses a
`load.Stream`, which decodes on a thread of its own as the pieces come
(caustic-image's `progressive.cst`):

```
let is load.Stream as s;           // stays put from open to close
load.stream_open(&s);
load.stream_feed(&s, data, n);     // per piece: copies, returns at once
load.stream_end(&s);               // no more pieces
...                                // each frame:
if (load.stream_size(&s, &w, &h) == 1) { /* lay out w x h */ }
let is i64 as n = load.stream_rows(&s);   // rows of stream_shown(&s) final
if (load.stream_done(&s) == 1) {
    let is image.buffer.Image as im = load.stream_take(&s);  // no pixels: stream_error
}
load.stream_close(&s);             // any time; also the cancel
```

- The size is known from the header (PNG, JPEG, GIF, BMP, QOI), before any
  pixel.
- A non-interlaced PNG shows its finished rows as they come: the first
  `stream_rows` rows of `stream_shown` hold their final pixels, in the
  format `stream_take` will give, and never change once shown, so they are
  read without a lock. Other formats give their pixels at the end.
- The image `stream_take` gives is the one `load.bytes` gives for the same
  bytes. It is the caller's; `stream_shown` stays the Stream's until close.
- Refused (a wrong CRC, a size no image may have, cut short, not an image),
  the Stream is done with no image and `stream_error` says why; rows shown
  before stay.
- `stream_feed` and the queries take the Stream's lock only to hand bytes
  over; the decoding is all on its thread. The final decode of a JPEG or
  WebP takes the same lock as `bytes()`. `stream_close` stops the thread at
  its next piece, or waits for a final decode already running.
- Where no thread can be started, `stream_feed` and `stream_end` do the work
  themselves. Close every Stream before `main` returns.

---

## Formats

The framework's own format enum — RGBA8, RGB8, R8, RGBA16F, RGBA32F and the
block-compressed ones a GPU wants — mapped onto what caustic-image decodes into
and onto what `render/texture` accepts. Conversion between them is
`transform.cst`'s job.

Block compression (BC1–BC7) is worth naming as *not now*: it is what real games
ship textures as, it is a real compressor to write, and nothing needs it until
there is a program with enough texture memory pressure to care.

---

## A note on `video/`

The same argument that justifies this layer would justify a `video/` — decoding
frames to images, presenting them in step with audio, seeking. There is prior
work to draw on:
[DecoderH264](https://github.com/Caua726/DecoderH264) exists, and caustic-image
already carries a VP8 decoder, which is most of a WebM video decoder.

Not now, and not decided. Recorded so the shape of this layer leaves room for it:
a video frame is an `Image`, and if that stays true, `video/` is a scheduler
rather than a second pixel pipeline.

---

## Current state

`buffer.cst` (the `Image`, owned or borrowed), `generate.cst`, `transform.cst`
(crop, flips, nearest resize; alpha multiplied in and out; a rectangle copied
between images, cut to both), `atlas.cst` and `load.cst`, with the hub
`image.cst` — which does not import `load.cst`.

**The atlas** packs rectangles into pages of an `Image` each by their skyline,
bottom-left; opens pages as the last fills, up to a number fixed at open; and
when none has room takes back the page least recently used — whole, its era
counted on so its slots read as gone — but never one used in the frame being
drawn, so a rectangle that does not fit is `NO_ROOM`, said, never dropped. Each
page keeps the region changed since it was last uploaded. `atlas_test` checks
where rectangles land, the pages' order of reuse, the dirty regions, and three
thousand random rectangles packed without overlap into nine tenths of four
pages.

**Loading** is through caustic-image, which the Causticfile fetches with
caustic-compact at their tags: any format it decodes, into the nearest format
here — gray R8, colour RGB8, anything transparent RGBA8, 16-bit channels by
their high byte. `load_test` decodes PNGs of every colour type written byte by
byte by `tools/make_pngs.py`, pixels known exactly, and streams PNGs, a JPEG
and a BMP in pieces: size early, rows as final, the image as `bytes()`'s;
cut short, a wrong CRC, an absurd size, cancelled at each point, four at
once on four threads. The three libraries this
repository builds on are also built for Windows and run under wine
(`caustic-mk run test-win32`).

**Writing** is through caustic-image too, whose encoder uses caustic-compact's
deflate: `save.png` makes a PNG of an RGBA8, RGB8 or R8 image, its rows borrowed,
into a caustic-compact `Bytes`. `save_test` decodes what it wrote back through
the library and, as a black box, through Pillow (`tools/png_read.py`). `dib.cst`
is Windows' other image on the clipboard: `dib_from_png` a PNG as a 32-bit DIB,
a BMP file without its fourteen-byte header; `png_from_dib` a CF_DIB's or
CF_DIBV5's bytes as a PNG — the file header put back, a 32-bit `BI_BITFIELDS` with
the usual masks being the same bytes as `BI_RGB`, which is all caustic-image's BMP
reader takes; other masks refused rather than shown wrong. It is the toolkit that
converts (`ui/host_clip.cst`): `window/` only carries the bytes.

Mutation-tested: every mutant killed but those that cannot be — the skyline's
merging of steps of one height and its handling of a step ending at a
rectangle's edge change no placement, and a copy of no width copies nothing
either way — the checks no test could tell apart removed.

## Order of work

1. **The `Image` type and ownership** — enough for `render/texture` to take one.
2. **Generation**, which needs nothing and unblocks seeing textures work.
3. **Atlas packing**, which `text/` blocks on. Done.
4. **Load** through caustic-image. Done; saving waits for something that needs
   it.
5. **Mipmaps**, when a texture large enough to alias appears.
