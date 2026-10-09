# Visual references

Every image here is versioned in its name (`-vN`); a changed expectation is a
new version and a reviewed commit, never an overwrite in place. All are part of
caustic-media and licensed under its MIT license (repository `LICENSE`).

| File | Producer | Content |
|---|---|---|
| `pattern-v1.png` | `tools/make_references.py` (Python zlib PNG writer) | 8×6 RGBA: R = 32x+7, G = 40y+3, B = 16(x+y), A = 128 where (x+y) mod 5 = 0, else 255 |
| `gray-v1.png` | `tools/make_references.py` | 3×2 gray without alpha: V = 40+70x+30y |
| `opaque-v1.png` | `tools/make_references.py` | 5×4 RGB without alpha: R = 200−30x, G = 50+50y, B = 90+xy |

Both exercise `reference.cst` itself (`reference_test.cst`). Rendering
references produced by an independent rasterizer are added beside them with
the same provenance rows.
