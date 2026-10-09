#!/usr/bin/env python3
"""Write the reference helper's own fixtures with an independent PNG writer.

Python's zlib and struct encode these, not caustic-image, so the helper's
decode path is checked against bytes it did not produce. The pixels follow
the same formulas reference_test.cst draws with tgt.put.
"""
import struct
import zlib
from pathlib import Path

OUT = Path(__file__).resolve().parents[1] / 'testdata' / 'reference'


def chunk(kind, data):
    body = kind + data
    return struct.pack('>I', len(data)) + body + struct.pack('>I', zlib.crc32(body) & 0xffffffff)


def png(path, width, height, color_type, pixel):
    channels = {0: 1, 2: 3, 6: 4}[color_type]
    rows = b''.join(b'\0' + b''.join(bytes(pixel(x, y)[:channels]) for x in range(width))
                    for y in range(height))
    data = (b'\x89PNG\r\n\x1a\n'
            + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, color_type, 0, 0, 0))
            + chunk(b'IDAT', zlib.compress(rows, 9))
            + chunk(b'IEND', b''))
    path.write_bytes(data)


def pattern(x, y):
    alpha = 128 if (x + y) % 5 == 0 else 255
    return (x * 32 + 7, y * 40 + 3, (x + y) * 16, alpha)


def opaque(x, y):
    return (200 - x * 30, 50 + y * 50, 90 + x * y, 255)


def gray(x, y):
    return (40 + 70 * x + 30 * y,)


def main():
    OUT.mkdir(parents=True, exist_ok=True)
    png(OUT / 'pattern-v1.png', 8, 6, 6, pattern)
    png(OUT / 'opaque-v1.png', 5, 4, 2, opaque)
    png(OUT / 'gray-v1.png', 3, 2, 0, gray)


if __name__ == '__main__':
    main()
