# libcis-lean — the size/RAM profile

A parallel libcis codebase optimized for **code size and RAM** instead of
feature completeness. It is an **overlay** over the base library: this
directory contains only the headers it replaces; everything else falls
through to `../include`.

```sh
g++ -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
    -Ilean/include -Iinclude -Os \
    your_program.cpp src/support.cpp lean/src/kernels.cpp \
    -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc -o your_program
```

The `-Ilean/include -Iinclude` order is the whole mechanism: `<vector>`
resolves to the lean one, and anything lean does not override (`<type_traits>`,
`<string_view>`, `<utility>`, ...) resolves to the base library. Never mix the
two include orders across TUs of one binary.

**`lean/src/kernels.cpp` is mandatory**, exactly like `src/support.cpp`. The
fat, non-template structural/algorithm kernels (string byte-splice, vector
grow/gap/erase, rb-tree rebalance/iterate, hash rehash/relink, list
reverse/merge-sort, deque table growth, the introsort/heap/merge algorithm
kernels) are **out-of-line** functions declared in `bits/lean_*.h` and defined
once in this single translation unit — the lean headers are NOT header-only.
Link it into every lean binary (one copy of each kernel per binary), or build it
into a library once (one copy per system, below). Omitting it is a link error
(undefined `std::detail::lean_*` / `tree_*` / `hash_*` / `list_*` symbols).

### Building the kernels as a library (one copy per system)

The kernels TU has no per-type code, so it can ship as a static or shared
library that every lean binary links against instead of recompiling:

```sh
# static — liblean.a
g++ -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
    -Ilean/include -Iinclude -Os -c lean/src/kernels.cpp -o kernels.o
ar rcs liblean.a kernels.o

# shared — liblean.so
g++ -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
    -Ilean/include -Iinclude -Os -fPIC -shared \
    lean/src/kernels.cpp -o liblean.so
```

Then link a program with `-L. -llean` in place of `lean/src/kernels.cpp`. Use
`-ffunction-sections -fdata-sections -Wl,--gc-sections` on the final link so a
binary carries only the kernels it actually reaches.

## What lean gives up (deviations from the standard, all deliberate)

1. **Custom allocators.** Container templates keep their `Allocator` parameter
   for source compatibility, but instantiating one with anything other than
   `std::allocator<T>` fails a `static_assert`. All storage comes from
   `malloc`/`realloc`/`free`. `get_allocator()` returns a default-constructed
   `std::allocator<T>`. `std::pmr` containers are not available in this
   profile.
2. **constexpr containers and algorithms.** Container operations and the heavy
   algorithms are not `constexpr` (the base library targets gcc-10, which has
   no constexpr allocation anyway; lean also drops constexpr from the fat
   `<algorithm>` entry points so they can be type-erased). Cheap algorithms
   that stay templated keep `constexpr`.
3. **SSO.** `std::string` always heap-allocates (or points at the shared
   static empty representation). A small string costs one `malloc`; in return
   `sizeof(std::string) == sizeof(void*)` and the string code shrinks a lot.
4. **Over-aligned element types.** Container elements must satisfy
   `alignof(T) <= alignof(max_align_t)` (enforced by `static_assert`).
5. **Allocation failure traps** (`__builtin_trap`), consistent with the base
   library's `-fno-exceptions` throw sites.
6. **`end()` stability on node containers**: see the per-container notes;
   where a container object embeds its own end sentinel the standard rules
   hold exactly.
7. **Shared empty-string byte is racy under concurrent legal writes.** Because
   all empty strings of a given `charT` point at one shared immutable static
   rep, `s[s.size()]` on an *empty* string returns a reference to a byte shared
   by every empty string of that width. The standard permits writing `charT()`
   (and only `charT()`) to that reference. Two threads each performing that
   legal write of `charT()` to *different* empty strings therefore write the
   same shared byte — a data race unique to this design (the value written is
   always `charT()`, so no thread observes a wrong value, but it is a race per
   `[intro.races]`). Non-empty strings, and any write other than `charT()`,
   are unaffected (they force a heap allocation first).

Everything else — iterator categories, complexity guarantees, reference
stability of node containers, the public API surface — follows the standard.

## Representation contract

### The lean block (`bits/lean.h`)

Every array-like lean container stores **one pointer** to the payload of a
*lean block*:

```
                     payload pointer (the container object)
                     v
[ base ... header ][ payload ......................... spare tail ]
          ^size_t used at payload - sizeof(size_t)
```

- `used` (an element count or byte length — the owner decides) lives at a
  fixed negative offset from the payload pointer, inside the same allocation.
- **Capacity is not stored anywhere.** It is recomputed from
  `malloc_usable_size(base)`, so the spare tail glibc rounds allocations up to
  is usable capacity for free.
- The header offset is `max(sizeof(size_t), alignof(T))`, so the payload is
  correctly aligned for any non-over-aligned `T`.

### Containers

| type | object size | representation |
|---|---|---|
| `string` | 1 pointer | chars in a lean block, `used` = length; empty strings point at a shared immutable static rep (never freed, capacity 0); `c_str()` is just the pointer |
| `vector<T>` | 1 pointer | elements in a lean block, `used` = size; null pointer = empty; growth uses `realloc` when `T` is trivially relocatable (`bits/relocatable.h`), else malloc+relocate+free through type ops |
| `map`/`set` | ~32 bytes | red-black tree; node = `{parent-with-color-in-bit-0, left, right, value}`; the container embeds the libstdc++-style header node (root/leftmost/rightmost) + size, and the header doubles as the `end()` sentinel |
| `unordered_map`/`_set` | 1 pointer | control block holding `{size, max_load_factor, head-of-all-elements list}` followed by the bucket array (libc++-style single intrusive list through all elements so `begin()` is O(1); bucket count derived from the block, not stored) |
| `list<T>` | ~24 bytes | embedded sentinel `{prev,next}` + `size_t` size (C++11 requires O(1) `size()`) |
| `deque<T>` | small control | fixed-byte blocks + block table; reference stability at both ends per the standard |

### Type erasure (`bits/lean.h` type ops + the algorithm kernels)

The rule: **erase a template into a single-instance kernel when its
per-instantiation code is big (≳200 bytes) and the indirection is amortized
over a whole container/range operation. Never erase a per-element hot path**
(`operator[]`, the `push_back` fast path, comparisons in a tight search loop
stay inline).

- Container structural work (vector grow/insert/erase shuffles, rb-tree
  rebalance/erase-fixup/iteration, hash-table rehash, list reverse/merge-sort,
  deque table growth) is implemented as plain non-template functions,
  **declared in `bits/lean_*.h` and defined out of line in
  `lean/src/kernels.cpp`** (one copy per binary, or one per system when built as
  `liblean.a`/`liblean.so`) — parameterized by `lean_ops` (element size/align +
  relocate/destroy/copy function pointers, null when trivial ⇒ kernels take the
  `memcpy` path with no calls). Only genuinely tiny hot accessors
  (`lean_used`/`lean_base`/`lean_alloc`, the tree bit-0 color ops, `push_back`
  fast paths, iterator ops, the `lean_ops_for` thunk templates) stay `inline` in
  the headers.
- The fat `<algorithm>` entry points (`sort`, `stable_sort`, `nth_element`,
  `partial_sort`, `inplace_merge`, `make_heap`/`sort_heap`) route
  contiguous-iterator ranges into single-instance kernels (also defined in
  `lean/src/kernels.cpp`) that take `(elem_size, compare thunk, context)`; the
  per-type code that remains is a ~2-instruction comparator thunk.
  Non-contiguous iterators keep a small templated fallback. The rb-tree's
  *descent* (comparator in a loop) stays a thin template; only the
  type-independent rebalancing is erased — that is exactly the split libstdc++
  ships in its .so.

## Testing and measurement

- `lean/test/*.cpp` — one file per component, each with its own `main()`.
  Failures **trap** (`CHECK` from `lean/test/lean_test.h`); no result
  printing, no fallbacks. A test binary exiting 0 means every check ran and
  passed.
- `lean/tools/run_tests.sh [filter]` — builds and runs every (or matching)
  test at `-O0 -g` and `-Os`, against the overlay, with the standard link
  recipe. Exit 0 iff everything passed. `CXX=clang++` works too.
- `lean/tools/size_report.py` — compiles a fixed matrix of feature programs
  against base and lean at `-Os` (every binary linked with
  `-ffunction-sections -fdata-sections -Wl,--gc-sections`, both include orders,
  so each program carries only the kernels it reaches), subtracts an
  empty-program baseline, and writes the marginal text/data/bss table, the
  full-TU `size(1)` text of `lean/src/kernels.cpp` (the one-copy-per-system
  library cost), and `sizeof` comparisons to `lean/SIZES.md`. Numbers are
  measured, never estimated.

The full conformance story (the transferred libc++ suite under `test/std/`)
applies to lean the same way as to base — run the pipeline with the overlay
include order — minus tests for the documented deviations above. That gating
requires the bootstrapped toolchain and is not part of the quick loop.
