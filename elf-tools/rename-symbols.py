#!/usr/bin/env python3
"""Tool 2: rename symbols in ELF files according to a map.

Given a set of ELF files and a map produced by `gen-symbol-map.py`, rewrites
each file's symbol names in place.

How it works (and why it is robust across every ELF variant):

The renamer never moves a single byte.  For each renamed symbol it overwrites
the symbol's name string, in place, inside its string table, with the new
(no-longer) name followed by a NUL.  Because the string only shrinks:

  * st_name offsets stay valid -- they still point at the same place;
  * every other reference into the string table (versioned-symbol names in
    .gnu.version_d/.gnu.version_r, etc.) is untouched;
  * section sizes, section header offsets, program headers, segment file
    offsets and virtual addresses all stay exactly where they were.

That means we only ever read generic, architecture-neutral structures (the
section headers and the symbol/string tables), so the tool is automatically
correct for ELF32/ELF64, little/big endian and any machine type (x86, MIPS,
RISC-V, MMIX, TileGX, ...), whether produced by gcc or clang, old or new
binutils.  Both .symtab/.strtab and .dynsym/.dynstr are handled.

The same name maps consistently everywhere it occurs, so a definition in one
file and references in another stay linked.
"""

import argparse
import json
import sys

import elfutil


def load_map(path):
    with open(path) as f:
        raw = json.load(f)
    # Keys and values were stored as latin-1 text to round-trip arbitrary
    # bytes losslessly.
    return {k.encode("latin-1"): v.encode("latin-1") for k, v in raw.items()}


def rename_file(path, mapping, dry_run):
    elf = elfutil.load(path)
    changed = 0
    saved = 0
    # Avoid rewriting a shared string twice: a string table may have several
    # symbols pointing at the same offset (identical names share storage).
    done = set()  # (strtab_off, name_off)
    for tab in elf.symtabs:
        for sym in tab.symbols:
            if not sym.name or sym.name not in mapping:
                continue
            key = (sym.strtab_off, sym.name_off)
            if key in done:
                changed += 1
                continue
            new = mapping[sym.name]
            try:
                shrank = elf.overwrite_string(sym.strtab_off, sym.name_off, new)
            except elfutil.ElfError as e:
                print("  ! %s: %s (symbol %r in %s)"
                      % (path, e, sym.name, tab.name.decode("latin-1", "replace")),
                      file=sys.stderr)
                continue
            done.add(key)
            changed += 1
            saved += shrank
    if changed and not dry_run:
        elf.write(path)
    return changed, saved


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-m", "--map", required=True, help="symbol map (JSON)")
    ap.add_argument("elf", nargs="+", help="ELF files to modify in place")
    ap.add_argument("-n", "--dry-run", action="store_true",
                    help="report what would change without writing")
    args = ap.parse_args(argv)

    mapping = load_map(args.map)
    total = 0
    total_saved = 0
    for p in args.elf:
        try:
            changed, saved = rename_file(p, mapping, args.dry_run)
        except elfutil.ElfError as e:
            print("%s: %s" % (p, e), file=sys.stderr)
            continue
        total += changed
        total_saved += saved
        verb = "would rename" if args.dry_run else "renamed"
        print("%s: %s %d symbol references (%d strtab bytes freed)"
              % (p, verb, changed, saved), file=sys.stderr)
    print("total: %d references, %d bytes freed%s"
          % (total, total_saved, " (dry run)" if args.dry_run else ""),
          file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
