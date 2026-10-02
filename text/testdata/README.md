# text/testdata

Fonts the text layer's tests read, and what fontTools reads from them.

Every font here is a subset of a font installed on the machine that made them,
renamed "Caustic Test …" and kept with its licence in `licenses/` — all are under
the SIL Open Font License 1.1, which calls a subset a Modified Version and keeps
Reserved Font Names off it. `cmaps.ttf` is made from nothing.

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

`*.sfnt.ref` is what fontTools reads from each: tables, names, every cmap
subtable's mappings, metrics, kerning — the format is described at the top of
`../tools/sfnt_reference.py`.

Both are made by development tools that need fontTools and the source fonts; the
output is versioned so the tests need neither, and is the same byte for byte each
time the tools run:

```
python3 text/tools/make_testdata.py
python3 text/tools/sfnt_reference.py
```
