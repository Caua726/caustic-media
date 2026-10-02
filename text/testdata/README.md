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
| `bomb.ttf` | made by the tool | composites that loop, fan out and nest past the limit; 40000 points |
| `bomb.otf` | made by the tool | subroutines that fan out past the operator limit |
| `bomb-type1.otf` | made by the tool | Type 1 charstrings, which are refused |

`*.sfnt.ref` is what fontTools reads from each: tables, names, every cmap
subtable's mappings, metrics, kerning — the format is described at the top of
`../tools/sfnt_reference.py`. `*.outline.ref` is every glyph's outline as
fontTools draws it, described at the top of `../tools/outline_reference.py`; the
bombs have none, being for refusing.

Both are made by development tools that need fontTools and the source fonts; the
output is versioned so the tests need neither, and is the same byte for byte each
time the tools run:

```
python3 text/tools/make_testdata.py
python3 text/tools/sfnt_reference.py
python3 text/tools/outline_reference.py
```
