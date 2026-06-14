# libcis

A faithful but minimal, easy-to-read reimplementation of LLVM's libc++,
targeting **gcc-10.2** with `-std=gnu++20 -fno-exceptions -fno-rtti`. It is
**header-only**: everything lives in `namespace std` under `include/`, with no
underscore-prefixed names and none of libc++'s portability/ABI macros. See
[`CONVENTIONS.md`](CONVENTIONS.md) for the coding rules.

The test suite is **transferred** from upstream libc++: a libclang-based tool
(`tools/transfer.py`) rewrites each libc++ test (wraps it in a per-file
namespace, drops RTTI/exception sites, records its entry point) so the same
tests can validate libcis. The repo therefore has two halves: the **library**
(`include/`, `src/`) and the **transfer + test pipeline** (`tools/`, `test/`,
`build/`).

---

## 1. Requirements

| Tool | Used for |
|------|----------|
| `g++-10` (10.2) | the one supported compiler for the library and the `libcis` backend |
| `python3` + `ninja` (≥1.10) | the transfer + test build graph |
| `libclang` (Python bindings) | `tools/transfer.py` AST rewriting (only needed to **re-run the transfer**) |
| A checkout of libc++ at `/home/user/llvm-project/libcxx` | the source of the tests (path is set as `LIBCXX` in `tools/transfer.py`) |
| *(optional)* `clang++-20`, `g++-14` | the discriminator backends (`libcxx`, `libstdcxx`, `gcc10std`) |

The committed `test/std/` tree and `test/std/manifest.json` are **generated**
(git-ignored). On a fresh checkout you must run the transfer (section 3) before
running tests.

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

The transfer turns `/home/user/llvm-project/libcxx/test/std/**` into
`test/std/**` plus `test/std/manifest.json` (the list of transferred tests with
their recorded entry points). It is wired as an incremental ninja graph.

```sh
# 1. Generate the build graph (build/build.ninja + build/transfer.ninja).
#    With no args it covers every std/ subtree; pass subtrees to scope it.
python3 tools/gen_transfer.py                 # all subtrees
python3 tools/gen_transfer.py thread time     # just these

# 2. Run it: rewrites the tests, writes the manifest, runs the tripwire,
#    then builds + runs the consolidated "group" test binaries.
ninja -f build/build.ninja
```

What `ninja -f build/build.ninja` does, in order:

1. **transfer** — `tools/transfer.py` rewrites each test directory (one
   libclang+PCH startup per directory) into `test/std/`.
2. **manifest** — aggregates the per-file records into `test/std/manifest.json`.
3. **tripwire** — `tools/tripwire.py` asserts the rewrite actually happened
   (RTTI sites excised, entries recorded, expected run-test counts) so a
   degraded transfer fails the build instead of silently copying files.
4. **groups** — `tools/gen_groups.py` emits `build/groups.ninja`, which builds
   `build/groups/libcis/libsupport.a`, compiles one consolidated TU per source
   directory (≈7 tests share one header parse), links and runs each.

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

## Layout

```
include/        the library (header-only; user headers + bits/ internals)
src/support.cpp runtime glue: operator new/delete etc. (linked as libsupport.a)
test/std/       generated: transferred tests + manifest.json + support/  (git-ignored)
tools/
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
