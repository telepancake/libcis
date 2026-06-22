# libcis

A faithful but minimal, easy-to-read reimplementation of LLVM's libc++,
targeting **gcc-10.2** with `-std=gnu++20 -fno-exceptions -fno-rtti`. It is
**header-only**: everything lives in `namespace std` under `include/`, with no
underscore-prefixed names and none of libc++'s portability/ABI macros. See
[`CONVENTIONS.md`](CONVENTIONS.md) for the coding rules.

## Quick start

A `Makefile` wraps the pipeline so you don't have to memorize the stage order.
It fails loudly on the first real error (unlike the conformance board, which
counts missing results as red):

```sh
make bootstrap # download the pinned toolchain into ./toolchain (first run only)
make doctor   # toolchain present AND a real smoke build of the library
make smoke    # just: does the library compile+link+run? (no test corpus needed)
make support  # build the mandatory libsupport.a on its own
make test     # full pipeline: transfer -> build groups -> board
make gate SUBTREE=thread   # per-file CLEAN/NOT-CLEAN check for one subtree
```

**`make bootstrap` fetches everything the pipeline needs into `./toolchain`**
(see section 1), so you do not have to install — or version-match — g++-10,
clang, libc++ headers, and the test corpus yourself. Every other target depends
on it, so a bare `make test` bootstraps first. To override the library compiler
instead of using the bootstrapped one: `make CXX=g++-13`. The sections below
document each underlying stage.

---

The test suite is **transferred** from upstream libc++: a libclang-based tool
(`tools/transfer.py`) rewrites each libc++ test (wraps it in a per-file
namespace, drops RTTI/exception sites, records its entry point) so the same
tests can validate libcis. The repo therefore has two halves: the **library**
(`include/`, `src/`) and the **transfer + test pipeline** (`tools/`, `test/`,
`build/`).

---

## 1. Requirements

The pipeline needs a **version-matched** toolchain — g++-10 for the library, and
libclang + a same-version libc++ test corpus for the transfer (an older libclang
cannot parse a newer corpus). Rather than make you install and version-match it,
**`make bootstrap` provisions it into `./toolchain`** — without touching the
host's package manager or any prebuilt distro/arch-specific binary:

```sh
make bootstrap        # or: tools/bootstrap.sh
```

| Provisioned into `./toolchain` | What it is | How |
|------|------------|-----|
| `gcc/` — `g++-10` | the library compiler | **built from the GNU source release** (no portable prebuilt gcc exists; `configure` detects the host arch) |
| `pylibs/` — `libclang` | the transfer's parser | the **PyPI `libclang` wheel** via `uv`/`pip` (wheels are per-arch and self-contained, so pip resolves your CPU) |
| `llvm-project/` — corpus, libc++ headers, clang builtin headers, `cindex.py` | the transfer's inputs | one sparse **git checkout** (arch-independent source); libc++'s two generated headers are produced from their `.in` templates |

This is **not** "bazel hermetic": the built compiler still runs against the
host's glibc + binutils. But it makes **no architecture or distro assumption**
and uses **no host package manager**. The host needs `bash`, `git`, `curl`,
`tar`, `make`, a C/C++ compiler (to build gcc), and `uv` or `pip` — that's it.
Building gcc takes a while on the first `make bootstrap`; it is cached after.

The optional reference/discriminator backends are not bootstrapped:

| Tool | Used for | Override |
|------|----------|----------|
| *(optional)* `clang++-20`, `g++-14` | the reference/discriminator backends (`libcxx`, `libstdcxx`, `gcc10std`) | `$CXX_LIBCXX`, `$CXX_LIBSTDCXX`, `$CXX_GCC10STD` |

Nothing is pinned to an absolute path: every tool reads its toolchain from
[`tools/config.py`](tools/config.py), which **prefers `./toolchain`** when
present, then an explicit environment variable, then system autodetection. **Run
the doctor** to see exactly what is resolved (and what, if anything, is missing):

```sh
make doctor        # or: python3 tools/doctor.py
```

The committed `test/std/` tree and `test/std/manifest.json` are **generated**
(git-ignored). On a fresh checkout you must run the transfer (section 3) before
running tests. `./toolchain` is git-ignored too; `make distclean` removes it to
force a re-bootstrap.

### Using your own libc++ test corpus

`make bootstrap` fetches a matching corpus automatically. To use a different
one, point `$LIBCXX` at any LLVM checkout's `libcxx/test` (it needs the `std/`
and `support/` subtrees) — but keep it in step with the clang that parses it:

```sh
export LIBCXX=~/llvm-project/libcxx/test
```

If unset, `config.py` probes `./toolchain/llvm-project/libcxx/test` (the
bootstrapped one), then `third_party/llvm-project/libcxx/test`,
`~/llvm-project/libcxx/test`.

---

## 2. Compiling against the library

libcis is header-only. Compile any program with:

```sh
g++-10 -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti \
       -nostdinc++ -Iinclude -O0 \
       your_program.cpp src/support.cpp \
       -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc -o your_program
```

Key flags:

- `-nostdinc++ -Iinclude` — use libcis's headers instead of the system C++ library.
- `-fcoroutines` — gcc-10 gates the C++20 `co_*` keywords behind this; harmless if unused.
- `-fno-exceptions -fno-rtti` — the library is built for this mode (throw paths become `__builtin_trap`).
- `src/support.cpp` — provides `operator new`/`delete` and other runtime glue, **mandatory** under `-nodefaultlibs`.
- The `-nodefaultlibs … -lgcc` tail links the C runtime explicitly.

Expect `-Wliteral-suffix` warnings from `<chrono>` (the UDL operators
deliberately omit the leading `_` per the project conventions); they are benign.

A quick smoke test:

```sh
printf '#include <vector>\n#include <algorithm>\nint main(){std::vector<int> v{3,1,2};std::sort(v.begin(),v.end());return v[0]!=1;}\n' > /tmp/t.cpp
g++-10 -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ -Iinclude -O0 \
       /tmp/t.cpp src/support.cpp -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc -o /tmp/t && /tmp/t && echo ok
```

---

## 3. Running the transfer (generating the tests)

The transfer turns `$LIBCXX/std/**` (the libc++ test corpus, section 1) into
`test/std/**` plus `test/std/manifest.json` (the list of transferred tests with
their recorded entry points). It is wired as an incremental ninja graph.

```sh
# 1. Generate the build graph (build/build.ninja + build/transfer.ninja).
#    With no args it covers every std/ subtree; pass subtrees to scope it.
python3 tools/gen_transfer.py                 # all subtrees
python3 tools/gen_transfer.py thread time     # just these

# 2. Run the transfer: rewrites the tests, writes the manifest, runs tripwire.
ninja -f build/build.ninja

# 3. Build + run the consolidated "group" binaries.  THIS IS A SEPARATE STEP:
#    build/build.ninja pulls groups.ninja in via `subninja`, and ninja only
#    regenerates the TOP-LEVEL -f file, never a subninja'd one -- so the
#    gengroups edge in build/build.ninja never fires on its own and the groups
#    stage is silently skipped (you'll see "ninja: no work to do" with no
#    libsupport.a).  Drive it explicitly:
python3 tools/gen_groups.py --ninja           # materialize the real groups.ninja
ninja -f build/groups.ninja libcis            # libsupport.a + every group result
```

> The whole sequence above is what `make test` runs for you.

What the steps do, in order:

1. **transfer** — `tools/transfer.py` rewrites each test directory (one
   libclang+PCH startup per directory) into `test/std/`.
2. **manifest** — aggregates the per-file records into `test/std/manifest.json`.
3. **tripwire** — `tools/tripwire.py` asserts the rewrite actually happened
   (RTTI sites excised, entries recorded, expected run-test counts) so a
   degraded transfer fails the build instead of silently copying files.
4. **groups** — `tools/gen_groups.py` emits `build/groups.ninja`, which builds
   `build/groups/libcis/libsupport.a`, compiles one consolidated TU per source
   directory (≈7 tests share one header parse), links and runs each.

#### The transfer PCH

Step 1 above is preceded by a `pch` edge that builds `build/transfer.pch` once:
a precompiled header of every top-level libc++ header, so each of the ~10k test
parses skips re-chewing the standard headers. The headers come from
`$LIBCXX_INCLUDE` (autodetected from `/usr/lib/llvm-*/include/c++/v1` or
`/usr/include/c++/v1`). It is wired into the ninja graph, but you can also build
it on its own:

```sh
python3 tools/transfer.py --build-pch        # writes build/transfer.pch
```

The PCH is a pure speed optimization: if `$LIBCXX_INCLUDE` is absent or the PCH
fails to build, `transfer.py` prints a notice and parses each file without it
(slower, identical output).

`transfer.py` also needs clang's **resource dir** (the builtin `stddef.h` etc.)
— libclang often miscomputes its own, so the parse can't find the builtins.
The dir is autodetected (via `clang -print-resource-dir`, then the usual install
paths) and passed to clang as `-resource-dir`, which orders it correctly (libc++
headers before the C library, as libc++'s `<cstddef>` requires). Override the
root with `$CLANG_RESOURCE_DIR`. When it can't be found, parses fail with
`'stddef.h' file not found`, and `tools/doctor.py` flags it.

The transfer also copies the libc++ support headers into `test/std/support/`
and patches them for gcc-10 (see `patch_support()` in `tools/gen_transfer.py`).

---

## 4. Running the resulting tests

There are two ways to run the transferred tests; both read
`test/std/manifest.json` and honor the exclusion list (section 5).

### 4a. Per-file gate — `tools/run_files.py` (fast, the primary CLEAN/NOT-CLEAN check)

Compiles and runs **each** test of a subtree **individually** (so one broken
test can't blank a whole consolidated TU), then prints any failures and a single
`CLEAN` / `NOT CLEAN` verdict.

```sh
# Prerequisite: the libcis support archive must exist.
g++-10 -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
       -Iinclude -Itest/std -Itest/std/support -O0 -w \
       -c src/support.cpp -o build/groups/libcis/__support.o
ar rcs build/groups/libcis/libsupport.a build/groups/libcis/__support.o
# (or just run `ninja -f build/build.ninja` once, which builds it.)

# Run the gate over a subtree (prefix-matched against manifest paths):
python3 tools/run_files.py thread
python3 tools/run_files.py input.output
python3 tools/run_files.py time
```

Subtree names are the top-level manifest prefixes: `algorithms`, `atomics`,
`containers`, `depr`, `diagnostics`, `experimental`, `input.output`,
`iterators`, `language.support`, `localization`, `modules`, `numerics`,
`ranges`, `re`, `strings`, `thread`, `time`, `utilities`. A more specific path
prefix like `thread/thread.condition` also works. Exit status is `0` iff CLEAN.

> Run the gates **one or two at a time**. Running many in parallel
> oversubscribes the CPU and can produce spurious timing/locale flakes in the
> concurrency-sensitive tests (e.g. `atomics.ref`, locale construction).

### 4b. Consolidated build + conformance board

After `ninja -f build/build.ninja` has built and run the group binaries, get a
green/red summary:

```sh
python3 tools/board.py libcis        # GROUPS + TESTS conformance board
python3 tools/board.py libcis -v     # verbose (lists red groups)
```

### 4c. Per-test ninja graph (optional, full incremental dependency tracking)

`tools/gen_ninja.py` emits a graph with one compile+link+run edge per test and a
`-MMD` depfile per source, so editing a library header reruns exactly the
dependent tests.

```sh
python3 tools/gen_ninja.py libcis            # writes build/libcis.ninja
ninja -f build/libcis.ninja                  # build + run all tests
ninja -f build/libcis.ninja thread           # just one subtree
```

### Discriminator backends

The same tests can run against reference toolchains to tell a libcis bug from a
compiler limitation:

- `libcxx` — `clang++-20` + libc++
- `libstdcxx` — `g++-14` + its libstdc++
- `gcc10std` — `g++-10` + **its own** libstdc++ (the key discriminator: a test
  red on both `gcc10std` and `libcis` is a gcc-10 limitation, not a libcis bug;
  red only on `libcis` is a real defect)

These backends are defined in `tools/gen_groups.py` (groups build) and
`tools/gen_ninja.py` (`python3 tools/gen_ninja.py libstdcxx`, etc.).

---

## 5. Excluded tests

`tools/exclusions.json` lists tests **removed from the CLEAN/NOT-CLEAN
denominator**, each with a written justification. A test belongs there only when
it is *not* a libcis defect — typically a gcc-10 compiler limitation proven by
the `gcc10std` discriminator, or a facility genuinely impossible on the target
(e.g. `import std;` module tests, which are `UNSUPPORTED: gcc`). Every test
**not** listed there is expected to pass. `run_files.py` re-applies these
automatically, so the gate is self-consistent regardless of transfer order.

---

## Compiled code-size benchmark

`bench/` tracks how much `.text` STL-heavy programs pull in against libcis at
`-Os` (plus a flags-only optimized/LTO baseline). It measures real header-only
codebases (fmt, json, tomlplusplus, …) *and* two artificial `std::vector`
probes, and journals results so a change's size impact is read back as a diff:

```sh
git submodule update --init --depth 1            # pull the benchmark codebases
bench/codesize.py                                # table for every project + probes
bench/record.py "what I changed"                 # measure all + append a journal line
bench/record.py --diff                           # per-project .text deltas vs the prev entry
```

The reusable type-erasure primitives explored for size work live in
`include/bits/type_ops.h` (`type_ops` / `cross_ops` single- and two-type op
tables, and `realloc_op` — the storage axis) and `include/bits/relocatable.h`.
See `bench/README.md` for both.

## Layout

```
include/        the library (header-only; user headers + bits/ internals)
src/support.cpp runtime glue: operator new/delete etc. (linked as libsupport.a)
bench/          compiled code-size benchmark + size journal (see bench/README.md)
test/std/       generated: transferred tests + manifest.json + support/  (git-ignored)
tools/
  config.py          single source of truth for toolchain/paths (env-overridable)
  doctor.py          probe the toolchain; report what's present/missing + the fix
  transfer.py        libclang AST rewrite of one test (the transfer worker)
  gen_transfer.py    emit build/build.ninja + build/transfer.ninja
  gen_groups.py      emit build/groups.ninja (consolidated group TUs, all backends)
  gen_ninja.py       emit build/<backend>.ninja (one edge per test)
  run_files.py       per-file CLEAN/NOT-CLEAN gate for a subtree
  board.py           conformance board for a backend
  exclusions.json    justified test exclusions
  gen_tzdb.py        build-time generator for the compact <chrono> tzdb
build/          generated ninja graphs + build outputs
```
