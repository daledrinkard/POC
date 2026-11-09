"""
Convert a PMBus register CSV (e.g. a Fusion Power Designer export) to an SQD
command file.

Usage: python csv_to_sqd.py <file.csv> <i2c_addr>

<i2c_addr> is a 2-digit hex I2C address (e.g. "3C"), used to build the SWxx
line prefix below.

Each CSV line is: <command type>,<cmd code>,<value>[,<name>]
Only "Byte Write", "Word Write" and "Block Write" lines are converted; all
other lines are ignored. Output lines are:

    SWxx <cmd:2 hex> <data> P <spaces> <name> <CRLF>

where <xx> is the 2-digit hex I2C address, <data> is 2 hex digits for Byte
Write, 4 for Word Write, and the raw hex string (0x stripped) for Block
Write, and <name> is CSV field 4 (omitted with its spaces if empty). Output
is written to <file>.SQD.
"""

import csv
import os
import sys

# data field width in hex digits; None = pass through unchanged (Block Write)
WRITE_WIDTHS = {
    "Byte Write": 2,
    "Word Write": 4,
    "Block Write": None,
}

# placed between the P terminator and the command name (CSV field 4)
NAME_SEPARATOR = "    "


def strip_hex(text):
    text = text.strip()
    if text[:2].lower() == "0x":
        text = text[2:]
    return text.upper()


def fixed_hex(text, digits, lineno):
    value = int(text, 16)
    if value >= 1 << (4 * digits):
        raise ValueError(f"line {lineno}: {text} does not fit in {digits} hex digits")
    return f"{value:0{digits}X}"


def convert(in_path, i2c_addr):
    out_path = os.path.splitext(in_path)[0] + ".SQD"
    count = 0

    with open(in_path, newline="") as fin, open(out_path, "w", newline="") as fout:
        for lineno, row in enumerate(csv.reader(fin), start=1):
            if len(row) < 3:
                continue
            kind = row[0].strip()
            if kind not in WRITE_WIDTHS:
                continue

            cmd = fixed_hex(row[1], 2, lineno)
            width = WRITE_WIDTHS[kind]
            if width is None:
                data = strip_hex(row[2])
            else:
                data = fixed_hex(row[2], width, lineno)

            name = row[3].strip() if len(row) > 3 else ""
            line = f"SW{i2c_addr}{cmd}{data}P"
            if name:
                line += NAME_SEPARATOR + name
            fout.write(line + "\r\n")
            count += 1

    return out_path, count


def main():
    if len(sys.argv) != 3:
        print(f"usage: {os.path.basename(sys.argv[0])} <file.csv> <i2c_addr>")
        return 1

    i2c_addr = sys.argv[2].strip()
    try:
        if len(i2c_addr) != 2:
            raise ValueError
        int(i2c_addr, 16)
    except ValueError:
        print(f"error: i2c_addr must be a 2-digit hex number, got {i2c_addr!r}")
        return 1
    i2c_addr = i2c_addr.upper()

    out_path, count = convert(sys.argv[1], i2c_addr)
    print(f"wrote {count} lines to {out_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
