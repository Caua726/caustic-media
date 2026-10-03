#!/usr/bin/env python3
# image/tools/make_pngs.py — the PNGs image/load_test.cst decodes, written
# byte by byte so that their pixels are known exactly: 3 by 2 pixels each, in
# every colour type a PNG has, at 8 bits and at 16.
#
#   python3 image/tools/make_pngs.py
import os
import struct
import zlib

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "testdata")


def chunk(kind, data):
    return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF)


def png(name, w, h, depth, ctype, rows, extra=b""):
    raw = b"".join(b"\x00" + r for r in rows)
    data = (b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, depth, ctype, 0, 0, 0))
            + extra
            + chunk(b"IDAT", zlib.compress(raw, 9))
            + chunk(b"IEND", b""))
    with open(os.path.join(OUT, name), "wb") as f:
        f.write(data)


def main():
    os.makedirs(OUT, exist_ok=True)
    # Gray: 0 64 128 / 192 255 7.
    png("gray.png", 3, 2, 8, 0, [bytes([0, 64, 128]), bytes([192, 255, 7])])
    # Gray and alpha.
    png("gray_alpha.png", 3, 2, 8, 4, [bytes([10, 255, 20, 128, 30, 0]), bytes([40, 1, 50, 2, 60, 3])])
    # RGB.
    png("rgb.png", 3, 2, 8, 2, [bytes([255, 0, 0, 0, 255, 0, 0, 0, 255]),
                                bytes([1, 2, 3, 4, 5, 6, 7, 8, 9])])
    # RGBA.
    png("rgba.png", 3, 2, 8, 6, [bytes([255, 0, 0, 255, 0, 255, 0, 128, 0, 0, 255, 0]),
                                 bytes([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12])])
    # RGBA at 16 bits: the high byte of each, 0x1280 and the rest.
    row0 = struct.pack(">12H", 0xFFFF, 0, 0x1280, 0xFFFF, 0, 0, 0, 0, 0x8000, 0x7FFF, 0x00FF, 0xFF00)
    row1 = struct.pack(">12H", *([0x4000] * 12))
    png("rgba16.png", 3, 2, 16, 6, [row0, row1])
    # A palette of three, the second half transparent.
    plte = chunk(b"PLTE", bytes([200, 100, 50, 0, 0, 0, 9, 8, 7]))
    trns = chunk(b"tRNS", bytes([255, 128]))
    png("palette.png", 3, 2, 8, 3, [bytes([0, 1, 2]), bytes([2, 1, 0])], plte + trns)
    # Nothing.
    open(os.path.join(OUT, "empty.png"), "wb").close()
    # Not an image.
    with open(os.path.join(OUT, "not.png"), "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\nnothing more")


if __name__ == "__main__":
    main()
