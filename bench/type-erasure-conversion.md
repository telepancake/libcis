# Type-erasure conversion doctrine

How to convert any libcis component (`<vector>`, `<string>`, `<deque>`,
`<map>`, an `<algorithm>`, …) from per-type template code to shared
non-template cores. The procedure is uniform; nothing here is specific to a
particular container. Point it at a component and an agent applies it.

## The principle

A class template's heavy methods bake the element type into machine code:
every `vector<T>`, `basic_string<C>`, `map<K,V>` instantiation re-emits the
same loops specialized on its type. The conversion moves each heavy,
type-dependent operation **out of the template into one non-template core**,
defined once in `src/support.cpp`, parameterized at runtime by:

- a `const detail::type_ops&` — the element's `size` plus its lifecycle
  leaves (`move_construct`, `copy_construct`, `destroy`), each routed through
  `allocator_traits<A>::construct/destroy`; and
- where storage is involved, a `const detail::storage_ops&` —
  allocate/deallocate/reallocate bound to the allocator.

Both are reached through a single `void* ctx` (the allocator). The template
method becomes a **thin forwarder**: compute the few scalars (counts,
offsets), hand the core the byte pointer(s) and the ops tables, return. The
only per-type code left is the leaf bodies — tiny, and shared program-wide by
LTO/ICF across every instantiation — plus trivially-inlined scalar glue.

This is why it scales: the heavy body is emitted **once** for all `T`, and the
leaves it calls are shared by all containers and all algorithms, not just the
one being converted.

## What to convert, what to leave

- **Convert** any method whose body is a loop or block over elements that the
  compiler would otherwise specialize per `T`: relocation on growth, gap
  open/close (the shift for insert/erase), range construct/copy/destroy,
  assign, and so on.
- **Leave inline** the methods that are pure scalar/pointer arithmetic and
  thus already type-independent in effect: `size()`, `capacity()`,
  `operator[]`, `begin()`/`end()`, comparisons that defer to a shared
  algorithm. Type-erasing these would only add an indirect call.

The agent **inventories the component itself** and buckets each method into
these two; the doctrine does not enumerate methods.

## The ops vocabulary — a small set of shapes, not one function and not one-per-method

Cores are keyed by the *shape* of the work, and there are only a few shapes.
A method maps to the core whose shape it has; unrelated operations do **not**
collapse into one function.

- **relocate-all into fresh/grown storage** (reserve, shrink_to_fit,
  append-grow): `detail::core_grow` (realloc fast path, else
  allocate + `core_relocate` + free).
- **grow with a hole at an offset** (insert/emplace that must reallocate):
  one grow-with-gap core — relocates `[0,pos)` and `[pos,end)` into the new
  buffer leaving a raw gap. This is a *different* relocate from `core_grow`;
  do not fold it in.
- **in-place shift up / down** (insert/erase that does **not** reallocate, for
  trivially-relocatable elements): `detail::vec_open_gap` /
  `detail::vec_close_gap`.
- **range construct / copy / destroy**: loops over the element leaves.

Add a new core only when a genuinely new shape appears; otherwise reuse one.
`erase` is not `core_grow`; `assign` is not a single call; each lands on the
core that matches it.

## The ops tables already carry the full leaf set

`detail::type_ops` (`include/bits/type_ops.h`) is **complete**, not a stub to
grow per method. It carries the whole single-type leaf set — `default_construct`,
`destroy`, `copy_construct`, `move_construct`, `copy_assign`, `move_assign`,
`swap`, `compare`, `equal`, `hash` — plus `size`, `align`, and diagnostic
`flags`; `detail::cross_ops` carries the two-type leaves (`construct_from`,
`assign_from`, `compare`, `equal`, `cast`). So converting a method is "call the
leaf that is already in the table" — `resize(n)` uses `default_construct`,
`assign` uses `copy_assign`/`copy_construct`, a comparison uses `compare`/
`equal`. You should **not** need to add a member to convert a standard container.

If some genuinely novel operation ever needs a leaf the table lacks, the table
is the single extension point (add the pointer, update every `make_*` aggregate
initializer, write the `<op>_op` leaf — lifecycle ops route through
`allocator_traits<A>` via `ctx`, value ops act on the `T`s directly — and wire
it into `make_*` under the matching `if constexpr` guard with the same
doubly-gated trivial sentinel as relocation). But treat that as rare: reaching
for it usually means the table was wrongly trimmed, not that the operation is new.

## Invariants every core and forwarder MUST preserve

1. **Allocator-aware lifecycle.** Element construct/move/copy/destroy go
   through `allocator_traits<A>` via `ctx` (the allocator). A core must never
   placement-new / direct-destruct when the allocator may customize those.
2. **Trivial fast path is doubly gated.** memcpy/realloc relocation fires only
   when the element is trivially relocatable **and** the allocator has a
   default lifecycle (`detail::alloc_default_lifecycle`). The null leaf is
   exactly that combined sentinel, so an instrumenting/counting allocator is
   never bypassed.
3. **`ctx` is the allocator** for both the element leaves and the storage
   callbacks — one pointer, one source of truth.
4. **No exceptions, no RTTI** (`-fno-exceptions -fno-rtti`): no try/catch;
   allocation/length failures trap or propagate exactly as the existing code
   does.
5. **constexpr is deliberately dropped** from converted methods. The standard's
   constexpr-container model is incompatible with byte-level relocation; this
   is an accepted trade, decided already. Do not reintroduce constexpr to
   "fix" the resulting `static_assert` test failures.
6. **Ordering and aliasing guarantees still hold.** Where the standard allows
   an argument to alias the container (`v.emplace(v.begin(), v[1])`,
   `v.push_back(v[0])`), capture it first and construct the new element before
   freeing the old buffer — the core must be written so the forwarder can keep
   this guarantee.

## Procedure for a component

1. Inventory its methods; bucket each as scalar-inline (leave) or
   heavy-type-dependent (convert).
2. For each heavy method, identify which existing core shape it is. If none
   fits, design the **minimal** new core: declare it in a `bits/…​.h` header,
   define it **once** in `src/support.cpp`, taking `type_ops` [+ `storage_ops`]
   + `ctx`.
3. Replace the method body with a forwarder; per-type leaves come from
   `detail::ops_for<T, A>` (see `include/bits/type_ops.h`).
4. Preserve the invariants above, aliasing/order included.

## Gate — identical commands for every component

Substitute the component's conformance subtree for `<subtree>`
(e.g. `containers/sequences/vector`, `strings/basic.string`).

**Conformance — no new failures vs the pre-change baseline:**
```
g++-10 -std=gnu++20 -nostdinc++ -Iinclude -fno-exceptions -fno-rtti -Os -w \
    -c src/support.cpp -o build/groups/libcis/__support.o
ar rcs build/groups/libcis/libsupport.a build/groups/libcis/__support.o
python3 tools/run_files.py <subtree>
```
Capture the `FAIL` set, `git stash`, rebuild, run again, diff. The only
permitted difference is the deliberate constexpr `static_assert` failures, and
their count must **not increase**.

**Runtime sanitize — the conformance binaries themselves, never a toy test.**
Compile `src/support.cpp` and the tests with
`-fsanitize=address,undefined -fno-sanitize-recover=all`, and **`LD_PRELOAD`
the ASan runtime** (under `-nodefaultlibs` it otherwise links after libc and
aborts with "ASan runtime does not come first"):
```
LD_PRELOAD=$(g++-10 -print-file-name=libasan.so) ASAN_OPTIONS=detect_leaks=0 ./test
```
Every test that compiles must still **PASS**. The custom-allocator and
element-counting tests are precisely what prove the allocator-aware lifecycle;
they are the validation, so a hand-written check is redundant.

## Honesty (non-negotiable)

State what every number is **of**: which build, which types, whole-program vs a
single container. Do not say "done / clean / passes / fails" about anything not
actually built and run. Report the real FAIL count, not a remembered one. Make
**no** size claim without a `bench/codesize.py` measurement in which the shared
core is counted once.

## Done

Every heavy method of the component forwards to a shared core; the conformance
`FAIL` set is unchanged except the already-accepted constexpr `static_assert`s
(count not increased); every compiling test passes under ASan+UBSan; commit
messages state exactly what was verified; any size change is measured
separately and reported as what it is of.
