# bench/ — compiled code-size benchmark

Tracks how much machine code (`.text`) real, STL-heavy C++ projects pull in when
built against **libcis** at `-Os`. This is the size-regression dashboard for the
library: shrink the STL, watch these numbers drop.

## Projects (git submodules under `projects/`)

Header-only, no third-party dependencies, heavy STL users, and buildable on
libcis's target (`g++-10 -std=gnu++20 -fno-exceptions -fno-rtti`, freestanding):

| project | pinned | what it exercises |
|---|---|---|
| [fmtlib/fmt](https://github.com/fmtlib/fmt) | `11.2.0` | formatting: `<string>`, charconv, ranges |
| [martinus/unordered_dense](https://github.com/martinus/unordered_dense) | `v4.8.1` | dense hash map/set, hashing, `<vector>` |
| [Neargye/magic_enum](https://github.com/Neargye/magic_enum) | `v0.9.8` | enum reflection, `<string_view>`, `<array>` |
| [nlohmann/json](https://github.com/nlohmann/json) | `v3.12.0` | (de)serialization, `<map>`, `<vector>`, `<string>` |
| [marzer/tomlplusplus](https://github.com/marzer/tomlplusplus) | `v3.4.0` | parser + value tree, `<string_view>`, `<variant>` |
| [doctest/doctest](https://github.com/doctest/doctest) | `v2.5.2` | test runner, registry, reporters |

Each is built with exceptions/RTTI disabled via its own opt-out macro
(`JSON_NOEXCEPTION`, `TOML_EXCEPTIONS 0`, `FMT_EXCEPTIONS 0`,
`DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS`), set in the driver.

Pull them in with:

```sh
git submodule update --init --depth 1
```

## Running

```sh
bench/codesize.py            # table for every project
bench/codesize.py fmt        # just one
bench/codesize.py --json     # machine-readable
```

Each project's driver (`drivers/<name>.cpp`) is compiled + linked against libcis
the same way `tools/run_std.sh` links the conformance suite — freestanding,
`-nostdinc++ -Iinclude`, `support.cpp -nodefaultlibs ...` — at `-Os` **plus a
flags-only optimized baseline** (see below), then **run** (a size for a binary
that doesn't work is meaningless) and measured with `size -A`. Exit status is 0
iff every selected project compiled, linked and ran, so the script doubles as a
test.

**Flags-only optimized baseline.** No one ships a size-sensitive binary at plain
`-Os`, so the default build adds `-flto` (which alone cuts `.text` ~20–25%),
`-fmerge-all-constants`, `-ffunction-sections -fdata-sections -Wl,--gc-sections`,
`-fvisibility=hidden`, `-fno-asynchronous-unwind-tables -fno-unwind-tables`, and
(when `ld.gold` is present) `-fuse-ld=gold -Wl,--icf=safe`. This is the honest
baseline that *library* changes are measured on top of (see
`code-size-techniques.md` §E0/§6). Recover the plain-`-Os` delta with `--no-opt`;
add one-off flags with `CODESIZE_EXTRA_FLAGS="..."`.

The compiler defaults to `g++-10` (the canonical target; see `CONVENTIONS.md`,
and `tools/run_std.sh` which hardcodes it). Override with `CXX=...` (the bench
also builds cleanly on g++-13). The default is pinned rather than probed because
`.text` differs between g++ majors, so comparable numbers need a fixed toolchain.

## Reading the table

```
libcis code size @ -Os +opt(flags)   (CXX=g++-10)
project                .text     +base   .rodata   .data    .bss
baseline               59518                2248       8    4952
fmt                   104510    +44992      7816      16    4992
unordered_dense        63688     +4170      2280       8    4968
magic_enum             59788      +270      2360       8    4952
json                   97518    +38000     30528       8    4920
tomlplusplus          111399    +51881     11328       8    5048
doctest               128721    +69203     13344       8   10016
```

`.text` is the whole binary's code; `baseline` is an empty `int main(){}`, so its
`.text` is the fixed cost of `src/support.cpp` + the freestanding link. The
`+base` column is each project's **marginal** code over that baseline — the part
that actually reflects libcis's STL footprint. (Numbers above are illustrative;
they move with libcis and the pinned project versions.)

## Journal

Two journals, by audience:

- **`sizes.md`** — human baseline snapshots (commit + compiler + pins + table).
- **`sizes.jsonl`** — machine journal, one JSON object per line, appended by
  `record.py` for the "batch of agents trying stuff" workflow.

After a change, record a parsable entry (the description is mandatory — it's how
an experiment is found later; git commit + dirty flag are captured automatically):

```sh
bench/record.py "what I changed / am measuring"
bench/record.py --print          # index of all journal entries
```

The append is done under an exclusive file lock, so many agents can record
concurrently without corrupting the log. A later analysis pass can stream
`sizes.jsonl` and pick out the heads/tails (smallest/largest `.text` per project,
deltas between commits, ...).

## Techniques

`code-size-techniques.md` is a cited design memo on how a standard-library
*implementation* can shrink the `.text` of programs that use it — the catalog the
"try stuff" agents should draw experiments from. It ends with a ranked, libcis-
specific action plan (phase 0 = free build flags, then error-path consolidation,
then instantiation-volume work).

`type-erasure-and-constexpr.md` is an empirical findings writeup: measured results
on whether type-erasing a container shrinks `.text`, why LTO doesn't do it for you,
how trivially-relocatable element ops collapse, whether the shared core gets cloned
per type (no), and what keeping C++20 `constexpr` actually costs (it conflicts with
the size-optimal implementation). Several common assumptions are corrected there
under measurement.
