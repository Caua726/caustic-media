# text/testdata

Fonts the text layer's tests read, and what fontTools reads from them.

The fonts here are subsets of fonts installed on the machine that made them,
renamed "Caustic Test …" and kept with their licences in `licenses/` — all under
the SIL Open Font License 1.1, which calls a subset a Modified Version and keeps
Reserved Font Names off it — except those "made by the tool", which are made from
nothing, to reach what real fonts rarely do.

| File | From | Why it is here |
|---|---|---|
| `glyf.ttf` | Noto Sans | static TrueType outlines, composites, GSUB/GPOS |
| `cff.otf` | Source Sans 3 | PostScript (CFF) outlines with subroutines |
| `cff2.otf` | Cantarell | a variable CFF2 font: `fvar`, `avar`, `HVAR`, `MVAR` |
| `vf.ttf` | Inter (variable) | a variable TrueType font: `gvar` |
| `kern.ttf` | Liberation Sans | the legacy `kern` table |
| `cid.otf` | Source Han Sans JP | CID-keyed CFF (FDSelect), cmap formats 12 and 14 |
| `pair.ttc` | Inter, two faces | a collection |
| `cmaps.ttf` | made by the tool | cmap formats 0, 4, 6, 12, 13 and 14; long `loca`; italic, monospaced, `USE_TYPO_METRICS` |
| `composite.ttf` | made by the tool | composites every way: offsets, scales, 2×2 transforms, matched points, nesting, both offset conventions; a contour of off-curve points alone |
| `ops.otf` | made by the tool | every Type 2 drawing operator, the flex family, hints and masks, local and global subroutines, `seac` |
| `big.otf` | made by the tool | charstrings past 64 KB (three-byte offsets), 1300 local subroutines (bias 1131), fixed-point numbers, a format 0 charset, an empty Strings INDEX before global subroutines |
| `var.ttf` | made by the tool | a variable TrueType font for what real ones seldom show: deltas to every point and to a few, interpolated (IUP) in each of its cases; intermediate regions; more than 255 point numbers; deltas of every width; composites varied, scaled every way, matched by points past 255, lending their metrics from a component that is empty; phantom points with no `HVAR`; a hidden axis; `avar` 2; `MVAR` |
| `var.otf` | made by the tool | a variable CFF2 font with two variation data, the Private DICT's `vsindex` 1, and `HVAR` with no index map |
| `bomb.ttf` | made by the tool | composites that loop, fan out and nest past the limit, and of exactly 4096 and 4097 components; 40000 points |
| `bomb.otf` | made by the tool | subroutines that fan out past the operator limit |
| `bomb-type1.otf` | made by the tool | Type 1 charstrings, which are refused |
| `shape.ttf` | made by the tool | shaping without the font's help: no `GDEF` or `GSUB`, an empty `GPOS`; bases, marks above, below and through as rectangles; a spacing mark; variation sequences |
| `fonts/` | made by the tool | a pretend system's fonts for [`../fonts`](../fonts/fonts.md): 32 files in nested directories, one not named as a font — a family in weights from 100 to 900, two widths, italic and oblique, a CFF face, legacy families split from typographic ones; variable faces over weight, width, slant, italic and optical size, some past CSS's ranges; Greek and Cyrillic companions; a collection of two CJK faces; a colour face standing for emoji; monospace; old weight classes, no `OS/2`, an `OS/2` too short, no names; weights between 400 and 500; a broken file and an empty one. Glyphs are squares: only what matching reads matters |

`*.sfnt.ref` is what fontTools reads from each: tables, names, every cmap
subtable's mappings, metrics, kerning — the format is described at the top of
`../tools/sfnt_reference.py`. `*.outline.ref` is every glyph's outline as
fontTools draws it, described at the top of `../tools/outline_reference.py`; the
bombs have none, being for refusing. `*.var.ref`, for the four variable fonts,
is each at chosen instances: HarfBuzz's coordinates for them, every glyph's
outline and advance, every `MVAR` delta — described at the top of
`../tools/var_reference.py`. `raster.ref` is paths and glyphs of these fonts
drawn as coverage, described at the top of `../tools/raster_reference.py`.
`*.shape.ref` is text shaped by HarfBuzz in each font — glyphs, clusters, glyph
flags and positions — described at the top of `../tools/shape_reference.py`.

All are made by development tools that need fontTools and the source fonts; the
output is versioned so the tests need none of them, and is the same byte for byte
each time the tools run. The outline and variation references also hold each
glyph, advance and coordinate to HarfBuzz, through uharfbuzz, and the raster
reference each glyph to FreeType, through freetype-py; they run from a virtual
environment of their own:

```
python3 -m venv --system-site-packages ~/.cache/caustic-media/venv
~/.cache/caustic-media/venv/bin/pip install uharfbuzz freetype-py

python3 text/tools/make_testdata.py
python3 text/tools/sfnt_reference.py
~/.cache/caustic-media/venv/bin/python text/tools/outline_reference.py
~/.cache/caustic-media/venv/bin/python text/tools/var_reference.py
~/.cache/caustic-media/venv/bin/python text/tools/raster_reference.py
~/.cache/caustic-media/venv/bin/python text/tools/shape_reference.py
```
