# elf-tools

Two small, dependency-free tools for shrinking ELF symbol names to save bytes.

* **`gen-symbol-map.py`** — read the symbols from a set of ELF files and emit
  a map from each original name to a fresh **short, unique, valid** name.
* **`rename-symbols.py`** — apply that map to a set of ELF files in place.

They share `elfutil.py`, a hand-rolled ELF parser (no pyelftools needed).

## Why it works on any ELF

The tools only ever read architecture-neutral structures — the section
headers and the symbol/string tables — so they are correct for:

* ELF32 and ELF64
* little endian and big endian
* any machine: x86, MIPS, RISC-V, MMIX, TileGX, SPARC, PPC64, s390x, ...
* gcc or clang, old or new binutils
* both `.symtab`/`.strtab` and `.dynsym`/`.dynstr`

The renamer **never moves a byte**: it overwrites each renamed symbol's name
string in place with the new (never-longer) name plus a NUL. Because strings
only shrink, every offset in the file — `st_name`, section sizes, section
header offsets, program headers, segment addresses, versioned-symbol name
references — stays valid untouched. This is what makes it robust everywhere.

## Short names, for real

New names are generated shortest-first (`a`, `b`, ... then two characters,
etc.) from the full set of characters legal in a symbol identifier. This is
deliberately **not** a `__renamed_<original>` prefix scheme — the goal is
fewer bytes, and a typical run cuts symbol-string size by an order of
magnitude.

The generator guarantees `len(new) <= len(original)` for every symbol (so the
in-place renamer always fits). The rare symbol too short to receive a unique
fitting name is left unrenamed rather than risk a conflict.

## What gets renamed (avoiding broken links)

By default only symbols **defined** in at least one input file are renamed. A
symbol undefined everywhere is an external reference (e.g. libc's `printf`);
renaming it would break linking against the outside world.

A defined **global** symbol can still be part of your public ABI, or be
referenced by code outside the input set (`main` is referenced by the C
runtime). The tools can't know your ABI boundary, so protect such names:

```
--keep 'main' --keep 'cis_*'      # globs, repeatable
--keep-from public-api.txt        # one glob per line
```

Use `--all` to also rename undefined externals — only when your inputs are a
closed world. Section names, the empty name and (unless `--files`) `STT_FILE`
names are never touched.

## Usage

```sh
# 1. build the map from the complete set of objects, keeping the public API
python3 gen-symbol-map.py --keep main --keep 'cis_*' obj/*.o -o symbol-map.json

# 2. apply it (in place) to the same files
python3 rename-symbols.py -m symbol-map.json obj/*.o

# preview without writing
python3 rename-symbols.py -n -m symbol-map.json obj/*.o
```

The map is JSON (`{"original": "new", ...}`); names are stored as latin-1 so
arbitrary symbol bytes round-trip losslessly.
```
