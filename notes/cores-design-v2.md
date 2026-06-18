# cores-design-v2

Clean-room derivation of the type-erased "core" primitive set for libcis's
contiguous-storage containers (`vector`, `basic_string`, and — eventually —
the contiguous slabs that back `deque`). Designed without reference to any
existing core implementation; the inputs are the C++ container contract, the
two abstractions already in the tree (`type_ops`, `storage_ops`), and the
measured findings handed down in the brief.

## 1. The principle

A core is a free function — one symbol in `libcis.so` — whose body is a
typed-byte-range loop large enough that we do not want N copies of it,
parameterised by a `const type_ops*` (the element vocabulary) and a
`const storage_ops*` + opaque `void* st_ctx` (the buffer vocabulary). What
qualifies a body for promotion is not "could be shared," but "is shared
non-trivially" — the loop has at least one branch, at least one leaf call on
the slow path, a memmove/memset/memcpy fast path on the trivial path, and at
least two distinct container methods call it. A core that wraps one
`storage_ops` slot, or that compiles to fewer instructions than its own
arg-setup at the call site, is not a core.

## 2. The set

Five cores. Argument-order convention (justified per finding #2):
**per-call payload args first** (register-arg slots), then `tops`, then `sops`,
then `st_ctx` last. The container forwarder receives `this` in rdi, calls
helper methods (`size()`, `end_ptr_()`, `recommend()`) that clobber rdi, and
*then* passes `st_ctx = this` to the core. With `st_ctx` as the last
register-arg (or first spill), the compiler holds it in a callee-saved
register cheaply across those helper calls. Where a 7th arg is needed
(finding #1), it spills to the stack at the call site; the spill arg is
chosen so that "written once, read once in the core" makes the spill the
cheapest possible — typically the `new_size_bytes` value, which the caller
computed once and the core reads once. Allocator instance is *not* passed:
cores fetch it lazily via `storage_ops::alloc_ctx` gated by
`tops->flags & f_alloc_ctx` (finding #3).

### 2.1 `destroy_range`

```cpp
void destroy_range(void* first,
                   void* last,
                   const type_ops* tops,
                   const storage_ops* sops,
                   void* st_ctx);
```

**Semantics.** Destroys every constructed element in the byte range
`[first, last)` by invoking `tops->destroy` on each `sizeof(T)`-strided slot.
Pre: `first` and `last` are aligned, `last >= first`, both lie within a single
live allocation governed by `st_ctx`. Post: those slots are uninitialised
storage; `st_ctx`'s recorded size is **unchanged** (this core does not own
the size update — callers that simultaneously want a size update either also
call `sops->set_size` directly or use `erase_close_gap` which fuses both).

**Callers.** `~vector`, `~basic_string` (via the deallocate path), `clear`,
`pop_back`, `resize`-shrink, `erase` (both overloads — sole-call destroy is
the small case, fused destroy+shift handled by `erase_close_gap`), the
assign-shrink branch, the move-assignment-overwrites-old-content branch,
`basic_string::resize`-shrink. ~8 distinct call sites per
(`vector`, `basic_string`).

**Trivial fast path.** `if ((tops->flags & (f_triv_destroy|f_alloc_default_life))
== (f_triv_destroy|f_alloc_default_life)) return;` — no work needed at all.

**Slow path.** `void* el_ctx = (tops->flags & f_alloc_ctx) ? sops->alloc_ctx(st_ctx) : nullptr;`
then a `for` loop walking `first` to `last` in `tops->size` strides invoking
`tops->destroy(el_ctx, p)`. Lazy alloc-ctx fetch avoids the per-call-site
xor-zero for stateless allocators (finding #3).

### 2.2 `fill_construct`

```cpp
void fill_construct(void* dst,
                    size_t n,
                    const void* value,
                    const type_ops* tops,
                    const storage_ops* sops,
                    void* st_ctx);
```

**Semantics.** Constructs `n` copies of the prototype `*value` (as a `T`) at
`dst, dst + tops->size, ...`, allocator-mediated via
`tops->copy_construct`. Pre: `dst` points to uninitialised storage of at
least `n * tops->size` bytes within the live allocation; `value` points to a
properly-constructed `T`. Post: `n` constructed objects at the destination.
This core does **not** update size; callers that grew the buffer themselves
update size before/after. (Reason for not fusing: fill is reached from
constructors that have *just* allocated and have size==0, from `assign(n,x)`
which writes over existing live elements, and from `insert(pos, n, x)` which
writes into a previously-opened gap. The size update is different in each
case; fusing would force a "new_size" arg whose value the caller has to
compute either way.)

**Callers.** `vector(n)`, `vector(n, x)`, `vector::assign(n, x)`,
`vector::resize(n)`, `vector::resize(n, x)`, `vector::insert(pos, n, x)`,
`basic_string(n, c)`, `basic_string::assign(n, c)`, `basic_string::resize(n, c)`,
`basic_string::insert(pos, n, c)`. ~10 distinct call sites.

**Trivial fast path.** `if ((tops->flags & (f_triv_copy|f_alloc_default_life))
== (f_triv_copy|f_alloc_default_life))` then, if `tops->size == 1`, `memset`;
else a tight `memcpy`-each-slot loop the compiler vectorises. (No
unconditional `memset` because for `size > 1` the per-slot bit pattern is
arbitrary.)

**Slow path.** Fetch `el_ctx` lazily as above, walk `n` slots calling
`tops->copy_construct(el_ctx, dst + i*size, value)`.

### 2.3 `append_from_buffer`

```cpp
void append_from_buffer(const void* src,
                        size_t n_bytes,
                        const type_ops* tops,
                        const storage_ops* sops,
                        void* st_ctx);
```

**Semantics.** Constructs `n_bytes / tops->size` elements at the *current
end* of the live range (read via `sops->data`), copy-constructing each from
the corresponding slot of `src`, and updates the recorded size to
`old_size_bytes + n_bytes`. Pre: capacity is sufficient (caller has reserved
at least `n_bytes` of headroom plus the spare slot); `src` is a
`tops->size`-aligned typed buffer disjoint from the destination. Post: live
range extends by `n_bytes`; spare slot at the new end is zeroed (the
`storage_ops::set_size` slot guarantees this when there is room) — that's
how string's null-terminator and vector's emplace_back fast-path keep
working.

**Why size update IS fused here.** Every caller of "append a contiguous
range" wants the size to advance by exactly the bytes copied; there is no
caller that copies and then decides a different size. Fusion saves the
per-callsite `set_size` arg-prep on every push/append/copy-ctor.

**Callers.** `basic_string::append(const C*, n)`,
`basic_string::operator+= (const C*)`, `basic_string::push_back`,
`basic_string` ctors from `C*`/`string_view`/`basic_string` (after
`vallocate`), `basic_string::assign(const C*, n)` (after shrink-and-set-empty),
`vector::vector(const vector&)` (copy ctor — same shape:
copy-from-other's-buffer to one's own end), `vector::assign(InputIt,
InputIt)` for contiguous iterators (the iterator-aware overloads downcast).
~8 distinct call sites.

**Trivial fast path.** `if ((tops->flags & (f_triv_copy|f_alloc_default_life))
== (f_triv_copy|f_alloc_default_life))`: a single `memcpy` of `n_bytes` plus
`sops->set_size(st_ctx, old_size + n_bytes)`. This is the path taken for
**every** `basic_string<C>` instantiation (CharT is always trivially
copyable) — string's hottest mutator is one branch and one `memcpy`.

**Slow path.** Read `(begin, old_size)` via `sops->data`, fetch `el_ctx`
lazily, loop calling `tops->copy_construct`, then `sops->set_size`.

### 2.4 `erase_close_gap`

```cpp
void erase_close_gap(size_t dst_off,
                     size_t src_off,
                     size_t move_bytes,
                     size_t destroy_bytes,
                     const type_ops* tops,
                     const storage_ops* sops,
                     void* st_ctx,
                     size_t new_size_bytes);
```

**Semantics.** Implements `erase(first, last)` and the shrink branch of
`resize`/`assign`. Conceptually:
1. Move-construct the tail `[begin + src_off, begin + src_off + move_bytes)`
   down to `[begin + dst_off, begin + dst_off + move_bytes)` (forward walk —
   `dst_off <= src_off`, so the moves leave already-moved-from slots behind
   them).
2. Destroy the trailing `destroy_bytes` of source that were not overwritten
   by step 1's moves.
3. Update recorded size to `new_size_bytes`.

Pre: `dst_off <= src_off`; `dst_off + move_bytes + destroy_bytes == old_size`;
`new_size_bytes == dst_off + move_bytes`. Post: size shrunk to
`new_size_bytes`; spare slot at the new end is zeroed by `set_size` (string's
null terminator, vector's spare).

**Why 7 args is the right shape.** This fuses what would otherwise be two
core calls (`relocate` + `truncate`/`destroy_range`) plus an explicit
`sops->set_size`. The 7-arg signature is justified by trial finding #1:
the spill arg (chosen here as `new_size_bytes`, last) is one stack write at
the call site; splitting into two cores costs *two* separate arg setups, each
with its own `tops`/`sops`/`st_ctx` load. The 8th argument (`new_size_bytes`)
spills on x86_64 SysV; on i386 SysV all args spill, so it makes no
difference; on ARM-32 AAPCS it spills too. The fused win is the call-site
collapse.

**Callers.** `vector::erase(it)`, `vector::erase(first, last)`,
`vector::resize(n)` shrink branch, `vector::assign(n, x)` shrink branch,
`vector::assign(InputIt, InputIt)` shrink branch, `basic_string::erase`,
`basic_string::resize(n)` shrink branch, `basic_string::replace` when the
replacement is shorter. ~8 distinct call sites.

**Trivial fast path.** `if ((tops->flags & (f_triv_reloc|f_alloc_default_life))
== (f_triv_reloc|f_alloc_default_life))`: one `memmove(begin+dst_off,
begin+src_off, move_bytes)` (no destroy needed — trivially destructible),
then `sops->set_size(st_ctx, new_size_bytes)`. This is *the* string-erase
fast path.

**Slow path sketch.** Fetch `el_ctx` lazily. Walk forward `move_bytes` /
`tops->size` iterations: `move_construct(dst+i*sz, src+i*sz)` then
`destroy(src+i*sz)`. Then destroy the trailing `destroy_bytes /
tops->size` not-yet-destroyed slots. Then `set_size`.

### 2.5 `insert_open_gap`

```cpp
void insert_open_gap(size_t pos_off,
                     size_t move_bytes,
                     size_t gap_bytes,
                     const type_ops* tops,
                     const storage_ops* sops,
                     void* st_ctx,
                     size_t new_size_bytes);
```

**Semantics.** The dual of `erase_close_gap`. Used by every middle-of-buffer
insert that has already ensured capacity. Conceptually:
1. Move-construct the tail `[begin + pos_off, begin + pos_off + move_bytes)`
   *up* to `[begin + pos_off + gap_bytes, begin + pos_off + gap_bytes +
   move_bytes)` (backward walk — `gap_bytes > 0`, so dst overlaps source from
   above; reverse walk leaves source slots intact behind the destructive
   destroy).
2. Destroy the now-vacated `[begin + pos_off, begin + pos_off + gap_bytes)`
   source slots (they were moved-from, so destruction is just running the
   destroy leaf on the moved-from objects — the standard says "all elements
   beyond the insertion point are now in moved-from state at their *new*
   positions").
3. Update recorded size to `new_size_bytes = old_size + gap_bytes`.

The gap `[pos_off, pos_off + gap_bytes)` is left uninitialised storage; the
caller fills it via `fill_construct` (for `insert(pos, n, x)`),
`copy_construct_from_buffer` (a separate use of `append_from_buffer`'s leaf
loop on the gap region — see open question 5.2 — or via direct
`construct_one` calls when the caller is a single-element `insert`).

Pre: `pos_off + move_bytes == old_size`; capacity has at least `gap_bytes`
of headroom plus the spare slot. Post: live size extended by `gap_bytes`;
gap content uninitialised.

**Why fused.** Same finding-#1 argument as `erase_close_gap`. The
alternative is `relocate_up` + `set_size`, two cores with full arg setups.

**Callers.** `vector::insert(pos, x)`, `vector::insert(pos, n, x)`,
`vector::insert(pos, first, last)` for forward iterators of known size,
`vector::emplace(pos, args...)`, `basic_string::insert(pos, n, c)`,
`basic_string::insert(pos, const C*, n)`, `basic_string::replace` when the
replacement is longer. ~7 distinct call sites.

**Trivial fast path.** As `erase_close_gap`: one `memmove(begin+pos_off+gap_bytes,
begin+pos_off, move_bytes)` then `set_size(new_size_bytes)`.

**Slow path sketch.** Walk backward: for `i = move_bytes/sz - 1` down to 0,
`move_construct(begin + pos_off + gap_bytes + i*sz, begin + pos_off + i*sz)`
and `destroy(begin + pos_off + i*sz)`. Then `set_size`.

## 3. What was considered and rejected

- **`grow_to(min_bytes)`** — a thin wrapper that calls `sops->resize` if
  the current capacity is short. Body is ~4 instructions; container can
  inline the check against `cap_end()` and call `sops->resize` directly.
- **`reserve(min_bytes)` / `shrink_to_fit()`** — same: the policy
  (amortised growth, exact fit) is container-side; the buffer mechanics are
  already in `storage_ops::resize`.
- **`push_back_one(args)`** — would need to forward `Args...` through type
  erasure, which we can't (no perfect-forwarding through a `void*`). The
  container method calls placement-new directly on the spare slot and then
  `sops->set_size`. Two instructions of body; no core.
- **`emplace_at(pos, args)`** — same `Args...` problem; the
  insert-shaped callers do `insert_open_gap` to make room, then placement-new
  into the gap.
- **`construct_default_n` separate from `fill_construct`** — the trivial fast
  path for default-init is "`memset` to zero," which is a special case of
  the fill fast path with a zero prototype; making it a separate core
  duplicates the structure to save one branch. Better to have callers pass
  a zero prototype to `fill_construct` when they want default-init of a
  trivially-default type, and to have the slow path branch on whether
  prototype is "null"-encoded for value-init. (See open question 5.3.)
- **`copy_construct_from_buffer` separate from `append_from_buffer`** —
  every caller wants the size to advance. The "construct into a pre-sized
  buffer without updating size" case shows up exactly once (in the gap
  filler for `insert(pos, first, last)` of contiguous iterators) and is
  cheaply expressible as a `fill`-shaped loop in the container. (See open
  question 5.2.)
- **`relocate_live`** alone — it's already declared as a leaf primitive in
  `bits/storage_ops.h` and used by `storage_ops::resize`'s implementation.
  Containers don't need to call it directly; `erase_close_gap` and
  `insert_open_gap` bundle "relocate-and-update-size" properly for their
  callers.
- **Element-equality / comparison / hash cores** — `operator==`,
  `operator<=>` over containers are not byte-loop-shaped (short-circuit on
  first mismatch, walk in element steps). The leaves they need
  (`tops->equal`, `tops->compare`) are already on `type_ops`; the loop is
  small enough that one per (T) is acceptable in the container header.
  (Could change if measurement disagrees.)
- **`cross_ops`-based cores** — `cross_ops` is currently unconsumed. The
  cases where a vector op naturally takes a different `U`-typed source —
  `assign` from a foreign iterator, `insert` from a foreign range — are
  rare enough at the call-site level that I do not plan to introduce
  cross-typed cores until a measurement asks for it. Container methods
  loop in their own header for the foreign-iterator case.

## 4. What I would measure to validate

For the cores whose shape is the non-obvious call (fusion, arg ordering),
each measurement is a build of a focused driver TU and a binary-size
delta against the alternative shape.

1. **Erase fusion.** `bench/drivers/vector_erase_mix.cpp` instantiating
   `vector<int>`, `vector<std::string>`, `vector<small_pod>` and calling
   `erase`, `resize(n)` shrink, `assign(n,x)` shrink across ~30 sites.
   Compare per-(`erase_close_gap` callsite) bytes on i386 + ARM-32 + x86_64
   vs. a split implementation that calls `destroy_range` + `relocate_down`
   + `sops->set_size` separately. Expect i386 saving ≥50 B per binary,
   ARM-32 ≥40 B (the analogues of the finding-#1 numbers).
2. **Insert fusion.** Symmetric: `bench/drivers/vector_insert_mix.cpp`.
3. **Argument-order: `st_ctx` last.** Build a driver where the forwarder
   calls `recommend()` + `end_ptr_()` between receiving its args and
   calling `erase_close_gap`. Compare a build where `st_ctx` is arg 1 vs.
   the proposed arg-last. On x86_64 expect +10-13 B per callsite for the
   arg-1 build; on i386 no change (all spill). Reproduces finding #2.
4. **Lazy alloc-ctx vs. explicit threading.** Add `void* el_ctx` as an
   explicit arg to all five cores; instantiate a `vector<int>` (stateless
   `std::allocator`) driver with ~20 callsites. Expect every callsite to
   gain 2-3 B (the `xor`/`mov rdx, 0` on x86_64, `eor r2, r2` on ARM-32,
   `mov dword [esp+8], 0` on i386). Reproduces finding #3.
5. **`append_from_buffer` size-update fusion.** Compare
   `bench/drivers/string_append.cpp` against a build with `set_size` split
   out. Expect savings on the string-hot path because `memcpy + set_size`
   collapses to one call.
6. **`destroy_range` NOT fused with `set_size`.** Symmetric of #5: if
   measurement shows the truncate-after-destroy case (`clear`, `pop_back`,
   `resize`-shrink) dominates over the destroy-only case (`~vector`,
   `~basic_string`), reconsider the design and either fuse a `truncate`
   variant or pass an optional `new_size_bytes` (using
   `(size_t)-1` as the sentinel for "don't touch size"). I expect the
   destroy-only case to dominate in real binaries because `erase` /
   `resize`-shrink already go through `erase_close_gap`; but this is a
   prediction, not a measurement.

## 5. Open questions

1. **Should `erase_close_gap` take pointer arguments or byte offsets?**
   I've drafted it with offsets (`size_t dst_off`, `size_t src_off`, ...)
   because the alternative — three `void*` plus a `begin` for the size
   update — passes one extra argument that the core can derive from
   `sops->data`. Offsets are also cheaper than pointers on i386 in the
   common case where the offset is a small constant the compiler holds in
   a register without sign-extending. But the trial findings did not
   compare these shapes. **Settle by:** building both variants of one core
   and measuring on i386 + ARM-32.

2. **Do we need a `construct_from_buffer_no_size` core for the
   `insert(pos, first, last)` contiguous-iterator gap-filler?** The gap
   left by `insert_open_gap` needs to be filled; today (this design)
   `fill_construct` doesn't suit because the source is a buffer, not a
   prototype. Options: (a) make a sixth core; (b) generalise
   `fill_construct` to take a stride arg (0 = repeat prototype, sz = walk
   source buffer); (c) loop in the container method (small body per
   instantiation but not type-erased). I'm currently picking (c) on the
   assumption that the foreign-buffer-insert case is rare enough not to
   pay for a sixth core or a stride branch on the hot path. **Settle by:**
   counting call sites in the libcxx test suite that hit this
   path and measuring per-instantiation overhead of (c).

3. **Value-init vs. default-init prototype passing.** `vector(n)` (no `x`)
   wants value-init (`new (p) T()`); for a trivially-default `T` this
   memset-zero-es. `vector(n, x)` wants copy-construct from `x`. With one
   `fill_construct` core, distinguishing the two requires either a flag
   arg or a sentinel `value == nullptr` meaning "value-init." The
   sentinel adds a branch on the slow path (cheap) but disambiguates
   memset(0) vs. memcpy-each-slot on the trivial path (where the prototype
   pointer is null, memset(0) is correct iff trivially-default; where the
   prototype is non-null, memcpy-each-slot is correct iff trivially-copyable).
   Either is workable; the null-sentinel variant is the smaller signature.
   **Settle by:** building one of each and diffing the leaf-loop emit.

4. **Does `basic_string::replace` warrant its own core?** Replace decomposes
   into "open gap of size n_new (or close gap of size n_old - n_new)" plus
   "fill the gap from the source buffer." That's
   `insert_open_gap`/`erase_close_gap` + `append_from_buffer`-shaped fill.
   The container method already calls both; not obviously worth a fused
   `replace_with_buffer` core unless `bench/drivers/string_replace.cpp`
   shows the split is per-callsite-expensive. **Settle by:** measuring.

5. **`deque` slab cores.** `deque<T,A>` will need exactly the same five
   primitives applied to a per-slab buffer rather than a single contiguous
   range. The trick: the `storage_ops` for a deque-slab is not the same
   thing as for `vector` — it doesn't have a single growing buffer, it has
   a ring of fixed-size slabs. The five cores listed above take a
   `storage_ops` that exposes `(data, set_size, cap_end, resize, free)`;
   a deque-slab specialisation of `storage_ops` would need a different
   contract (no resize; cap_end and set_size are per-slab). The
   cleanest path is probably a second `storage_ops`-shaped table for
   deque-slabs, and the cores stay the same. **Settle when** the deque
   port begins; until then this is speculative.

6. **Should `cross_ops` ever consume a core?** Today it has no caller. The
   plausible future caller is `basic_string`'s appends/comparisons of a
   `basic_string<CharU>` (charT-mismatched), which the C++ standard does
   not actually require to be supported. So `cross_ops` may exist solely
   for the `unordered_map`/`set` hetero-lookup contracts and never grow a
   container-core consumer. **Settle by:** auditing libc++'s
   heterogeneous-lookup paths; we may be able to delete `cross_ops`
   entirely without losing functionality.
