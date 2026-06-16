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

## Extending the ops table (adding a new element leaf)

A core can only call the element operations that `detail::type_ops`
(`include/bits/type_ops.h`) actually carries. Today that is `size`,
`move_construct`, `copy_construct`, `destroy`. When a heavy method needs an
element operation that is **not yet a member** — e.g. value/default-construct
(`resize(n)`, `construct_at_end(n)`), copy-/move-assign (`assign`, in-place
`insert`/`emplace` that overwrites a live slot), swap, equality/less, hash —
do **not** reach for the static type inside the core or invent a parallel
table. Add the leaf to the one `type_ops`:

1. **Add the function pointer** to `struct type_ops`. Keep the signature
   uniform: `void (*op)(void* ctx, void* dst, …)` with the element(s) passed as
   `void*`/`const void*`, `ctx` first. Add it after the existing members so the
   aggregate layout only grows.
2. **Update every brace-initializer** of a `type_ops`. They are aggregate
   literals (`make_type_ops` starts `type_ops o{sizeof(T), nullptr, …}`), so a
   new member must be added to each initializer — the compiler will not warn if
   you forget and leave it value-initialized to a confusing `nullptr`.
3. **Write the leaf template** `<op>_op<T, A>`. Lifecycle operations
   (construct/destroy/assign of elements) route through `allocator_traits<A>`
   via `ctx` exactly as the existing leaves do; pure value operations
   (comparison, hash, swap) act on the `T`s directly. Keep it tiny — leaves are
   shared program-wide by LTO/ICF, so one good leaf serves every container and
   algorithm.
4. **Wire it into `make_type_ops<T, A>`** under the matching `if constexpr`
   validity guard (`is_default_constructible_v`, `is_copy_assignable_v`, …) so
   the pointer is null when the operation is ill-formed for `T`, and **define
   that null's meaning**: either "operation unavailable for this `T`" (the core
   must not be asked to perform it) or a trivial-sentinel "the core may inline
   the builtin" — pick the one the doctrine's fast-path rule (invariant 2)
   already uses for relocation, and apply it consistently.
5. A leaf that participates in the **trivial fast path** must be gated the same
   doubly-conditional way as relocation: null only when the element is trivial
   for that operation **and** the allocator has a default lifecycle, so a
   customizing allocator is never bypassed.

The table is the single extension point; growing it is preferred to adding a
core argument or a second table.

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
