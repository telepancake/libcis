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

## Artificial probes (no submodule)

Two self-contained drivers stress the *library* directly rather than a real
codebase — `std::vector` instantiated over many element types, the workload a
type-erased container aims to shrink:

| probe | what it builds |
|---|---|
| `vec_mixed` | `vector<T>` over a few dissimilar types (`int`/`long`/`double`/`string`/a struct) — a typical program's type count |
| `vec_many` | `vector<T>` over 16 distinct, non-trivially-relocatable, different-sized types (so ICF can't fold the instantiations) — the favorable case for erasure |

Both exercise the full mutating surface (`push_back`/`insert`/`erase`/`resize`/
copy/assign). They are in the default measured set, so they appear in the table
and journal next to the external codebases.

## Running

```sh
bench/codesize.py                  # table for every project + the artificial probes
bench/codesize.py fmt              # just one
bench/codesize.py vec_mixed vec_many   # just the artificial probes
bench/codesize.py --json           # machine-readable
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
bench/record.py "what I changed / am measuring"   # measure all + append a journal line
bench/record.py --print                           # index of all journal entries
bench/record.py --diff                            # .text deltas: previous entry -> latest
bench/record.py --diff abc491e                    # ...vs a specific commit prefix
bench/record.py --diff "null-sentinel"            # ...vs an entry by desc substring
```

The append is done under an exclusive file lock, so many agents can record
concurrently without corrupting the log. `--diff` prints a per-project
`old / new / delta` table of `.text` across the artificial probes *and* the
external codebases at once — the canonical way to read a change's size impact:
`record` before, `record` after, `--diff`.

## Techniques

`code-size-techniques.md` is a cited design memo on how a standard-library
*implementation* can shrink the `.text` of programs that use it — the catalog the
"try stuff" agents should draw experiments from. It ends with a ranked, libcis-
specific action plan (phase 0 = free build flags, then error-path consolidation,
then instantiation-volume work).

## Type-erasure primitives (`include/bits/`)

The reusable vocabulary an erased container core is written against — declared
once, allocator-aware, and **independent of any particular container**:

- **`bits/type_ops.h`** — the operation tables.
  - `type_ops` — single element type `T` (under allocator `A`): `size`/`align`/
    `flags` plus function-pointer leaves for the lifecycle (default/copy/move
    construct, destroy) and the value ops (copy/move assign, swap, 3-way compare,
    equal, hash). `ops_for<T, A>` is the constant instance. A null leaf is the
    "trivial or invalid" sentinel — the core handles that case inline
    (memcpy/memmove/nothing). Lifecycle leaves route through `allocator_traits`,
    so a custom `construct`/`destroy` is honored; the allocator instance is passed
    only when it is *stateful* (the `f_alloc_ctx` flag), so the common case carries
    no allocator pointer.
  - `cross_ops` — ops genuinely between two types `T` and `U` (construct/assign a
    `T` from a `U`, compare/equal across, `cast`), embedding the two single-type
    tables so a two-type algorithm still receives one pointer. `cross_for<T, U>`.
  - `realloc_op` — the storage axis: a realloc-style function type bound to a
    container via an opaque ctx that resizes a buffer and preserves its bytes
    (`(ctx, cur, &size) -> new_base`, `size` in/out bytes). It is given no
    `type_ops` and does not touch elements. Convention documented at the type.
- **`bits/relocatable.h`** — `is_trivially_relocatable_v<T>` (opt-in for the
  library's own movable-by-bytes types), which gates the memcpy/realloc fast paths.

These ship as standalone, documented headers. The measured conclusion of wiring
them into `std::vector` was that vector's per-element bodies are too small for
erasure to pay — the shared core + per-type leaves + call marshalling exceed the
inlined typed loops they replace — so the vector conversion was **not** kept on
this branch; the probes and tables remain as the tooling to evaluate the
technique on better-suited targets.

## Provenance: the harness is authoritative, prose is not

Every `make size` result stamps the exact library it measured (`MEASURED LIBRARY:
<branch> @ <commit>` in the summary and `.test_results/latest/env.txt`). That stamp
exists because a size/speed number is meaningless — and dangerous — without knowing
which branch, commit, and arch produced it.

Treat **every** size/perf claim in commit messages, notes, this file, or chat as an
**unverified, perishable claim** until you re-run `make size` on the named branch and
read `.test_results/latest/` (including the annotated `.asm`). Do not promote a
measurement to a design rule ("never X", "the design forbids Y"): measurements are
branch/arch/compiler-specific and expire. The re-runnable harness is the durable
truth; words about it are a log, often stale, sometimes measured on the wrong tree.
