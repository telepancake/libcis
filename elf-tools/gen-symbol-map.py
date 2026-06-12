#!/usr/bin/env python3
"""Tool 1: build a symbol-renaming map from a set of ELF files.

Reads the symbol tables of every input ELF file and produces a mapping from
each original symbol name to a fresh, short, unique, valid name.  The map is
written out (default: JSON) for `rename-symbols.py` to apply.

Design choices, and why:

* SHORT names, for real.  New names are generated from the shortest valid
  identifiers upward (1 char, then 2, ...), in a base that uses every
  character legal in a symbol name.  This is the opposite of a lazy
  `__renamed_<original>` prefix scheme -- the whole point is to save bytes.

* The renamer overwrites strings in place and therefore can never make a
  name longer than the original.  So the generator guarantees, per symbol,
  len(new) <= len(original).  Symbols too short to be given a unique fitting
  name (rare) are simply left unrenamed rather than risk a conflict.

* By default we only rename symbols that are DEFINED in at least one of the
  input files.  A symbol that is undefined everywhere is an external
  reference (e.g. libc's `printf`); renaming it would break linking against
  the outside world -- exactly the "causes errors" outcome to avoid.  Pass
  --all to rename those too (use only if the whole world is in your input
  set).  Section names, the empty name and (by default) STT_FILE names are
  never renamed.

* A defined GLOBAL symbol may still be referenced from outside the input set
  (a public API, or `main`, which the C runtime references).  The tool cannot
  know your ABI boundary, so protect those names with --keep (a glob pattern,
  repeatable) or --keep-from FILE (one pattern per line).  Kept names are
  never renamed and never reused as a generated name.

* Uniqueness is global across all inputs, so a symbol defined in one file
  and referenced in another stays consistent.
"""

import argparse
import fnmatch
import json
import sys

import elfutil


# Characters legal in an ELF symbol name.  ELF itself only forbids NUL, but
# to keep names valid as assembler/linker identifiers across toolchains we
# restrict to the conservative C-identifier-ish set that gcc/clang/binutils
# all accept, plus '.' and '$' which are widely tolerated.
_FIRST = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
_REST = _FIRST + "0123456789$."


def name_generator(reserved):
    """Yield short valid names, shortest first, skipping reserved ones.

    Produces every length-1 name, then every length-2, and so on, so the
    names handed out are always the shortest still available.
    """
    length = 1
    while True:
        # Enumerate all names of the current length in odometer order.
        idx = [0] * length
        while True:
            name = _FIRST[idx[0]] + "".join(_REST[i] for i in idx[1:])
            if name not in reserved:
                yield name
            # increment the odometer (position 0 is base len(_FIRST),
            # the rest are base len(_REST))
            pos = length - 1
            while pos >= 0:
                base = _FIRST if pos == 0 else _REST
                idx[pos] += 1
                if idx[pos] < len(base):
                    break
                idx[pos] = 0
                pos -= 1
            if pos < 0:
                break
        length += 1


def collect(paths, rename_all, rename_files):
    """Return (defined_names, all_names, file_names) as sets of bytes."""
    defined = set()
    referenced = set()
    file_names = set()
    for p in paths:
        elf = elfutil.load(p)
        for tab in elf.symtabs:
            for sym in tab.symbols:
                if not sym.name:
                    continue
                if sym.type == elfutil.STT_SECTION:
                    continue
                if sym.type == elfutil.STT_FILE:
                    file_names.add(sym.name)
                    continue
                referenced.add(sym.name)
                if sym.defined:
                    defined.add(sym.name)
    return defined, referenced, file_names


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("elf", nargs="+", help="input ELF files")
    ap.add_argument("-o", "--output", default="symbol-map.json",
                    help="output map file (default: symbol-map.json)")
    ap.add_argument("--all", action="store_true",
                    help="also rename symbols that are undefined in every "
                         "input (external references) -- only safe if the "
                         "inputs are a closed world")
    ap.add_argument("--files", action="store_true",
                    help="also rename STT_FILE symbol names")
    ap.add_argument("--keep", action="append", default=[], metavar="GLOB",
                    help="never rename names matching this glob (repeatable)")
    ap.add_argument("--keep-from", metavar="FILE",
                    help="file of keep globs, one per line (# comments ok)")
    args = ap.parse_args(argv)

    keep_globs = list(args.keep)
    if args.keep_from:
        with open(args.keep_from) as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith("#"):
                    keep_globs.append(line)

    def is_kept(name):
        s = name.decode("latin-1")
        return any(fnmatch.fnmatchcase(s, g) for g in keep_globs)

    defined, referenced, file_names = collect(args.elf, args.all, args.files)

    if args.all:
        candidates = set(referenced)
    else:
        candidates = set(defined)
    if args.files:
        candidates |= file_names

    # Honour the keep-list: those names are removed from the rename set.
    if keep_globs:
        candidates = set(n for n in candidates if not is_kept(n))

    # Names we must NOT collide with: every name that exists in the inputs
    # but that we are choosing to leave alone.
    keep = (referenced | file_names) - candidates
    reserved = set(n.decode("latin-1") for n in keep)

    gen = name_generator(reserved)
    # Hand the shortest names to the symbols that have the least room,
    # processing shortest originals first.  This maximises how many symbols
    # we can rename within the len(new) <= len(original) budget.
    ordered = sorted(candidates, key=lambda n: (len(n), n))

    mapping = {}
    pending = None  # a generated name we produced but could not yet place
    skipped = 0
    for orig in ordered:
        budget = len(orig)
        if pending is None:
            pending = next(gen)
        if len(pending) <= budget:
            mapping[orig.decode("latin-1")] = pending
            reserved.add(pending)
            pending = None
        else:
            # Shortest available name is still too long for this original;
            # since later originals are no shorter, keep `pending` for them
            # and leave this symbol unrenamed.
            skipped += 1

    with open(args.output, "w") as f:
        json.dump(mapping, f, indent=0, sort_keys=True)
        f.write("\n")

    orig_bytes = sum(len(k) for k in mapping)
    new_bytes = sum(len(v) for v in mapping.values())
    print("renamed %d symbols (%d candidates, %d left unrenamed for size)"
          % (len(mapping), len(candidates), skipped), file=sys.stderr)
    print("string bytes: %d -> %d  (saved %d)"
          % (orig_bytes, new_bytes, orig_bytes - new_bytes), file=sys.stderr)
    print("wrote %s" % args.output, file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
