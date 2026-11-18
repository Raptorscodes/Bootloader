#!/usr/bin/env python3
"""
Mini 7 — wrap app.bin with a header so the bootloader can verify it.

Layout of app_crc.bin (written to flash at 0x08002000):
  offset 0  magic  u32 LE   0xB00710AD
  offset 4  size   u32 LE   length of payload in bytes
  offset 8  crc32  u32 LE   zlib CRC-32 of payload (same poly verify.c must use)
  offset 12 payload         raw contents of app.bin
"""

from __future__ import annotations

import struct
import sys
import zlib
from pathlib import Path

IMAGE_MAGIC = 0xB00710AD


def make_image(app_bin: Path, out_bin: Path) -> None:
    payload = app_bin.read_bytes()
    size = len(payload)
    if size == 0:
        raise SystemExit(f"error: {app_bin} is empty")

    # zlib.crc32 is the common CRC-32 (poly 0xEDB88320). Mask to 32 bits.
    crc = zlib.crc32(payload) & 0xFFFFFFFF

    header = struct.pack("<III", IMAGE_MAGIC, size, crc)
    out_bin.write_bytes(header + payload)

    print(f"input:  {app_bin} ({size} bytes)")
    print(f"magic:  0x{IMAGE_MAGIC:08X}")
    print(f"size:   {size}")
    print(f"crc32:  0x{crc:08X}")
    print(f"output: {out_bin} ({len(header) + size} bytes)")


def main() -> None:
    script_dir = Path(__file__).resolve().parent
    default_in = script_dir.parent / "src" / "app.bin"
    default_out = script_dir.parent / "src" / "app_crc.bin"

    app_bin = Path(sys.argv[1]) if len(sys.argv) > 1 else default_in
    out_bin = Path(sys.argv[2]) if len(sys.argv) > 2 else default_out

    if not app_bin.is_file():
        raise SystemExit(f"error: missing {app_bin} — run `make` in AppCrc/src first")

    make_image(app_bin, out_bin)


if __name__ == "__main__":
    main()
