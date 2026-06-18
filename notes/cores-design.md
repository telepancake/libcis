# Core primitives for libcis contiguous-storage containers

A coherent set of type-erased "cores" that `vector`, `basic_string`, and a
future deque-like (or `flat_*`) container can be implemented on top of.

This doc is descriptive of the *target* set, not the current tree. The
current tree (cores in `src/support.cpp`, forwarders in `include/vector` and
`include/string`) grew organically; the goal here is to write down a small,
defensible vocabulary and identify what's missing, what's dead, and what's
in the wrong place.

## 1. Goal & principles

**Goal.** Provide every contiguous-storage container with the same five-ish
shared, non-template, byte-level primitives, parameterized only by
`const type_ops*` (element vocabulary) + `const storage_ops*` (buffer
vocabulary) + `void* st_ctx` (the container). Containers become thin headers
that translate `T*`/`size_type` into `void*`/`size_t` at the boundary and
forward into one symbol per operation.

**Driving principles** (in priority order):

1. **One implementation per core.** No `#ifdef ARCH_*` on signatures and
   no parallel "x86_64 path" / "ARM path" bodies. The price of decisions
   that favor one ABI shows up as bytes; that's a normal cost. The price
   of two implementations is bugs.
2. **Size on i386 + ARM-32 first.** Measured per `notes/callsite-analysis.md`:
   the three-decomposition rule (shared cores / per-(C,A) instantiation /
   per-callsite × count). x86_64 is the dev box; cores must not regress it
   visibly but it is not the gate.
3. **Reuse the abstractions we already trust.** `type_ops`, `storage_ops`,
   `cross_ops` are mature. New primitives should be expressible *through*
   them, not parallel to them. A would-be core that just queries one slot
   of `storage_ops` (e.g. "give me end_ptr") is a slot of `storage_ops`, not
   a core.
4. **Each core has at least one real caller in vector OR string today.**
   Speculative cores (the `cross_ops`-using ones below) are proposed only
   together with the specific container method they would replace.
5. **Readability over the last 4 KB.** A core whose body is 12 instructions
   and whose only purpose is to fold one `mov` at one callsite does not earn
   its symbol. The rule of thumb: if the body is dominated by the
   prologue/epilogue, inline it back into the forwarder.

## 2. The proposed core set

Six cores, in two groups: the three "lifecycle/relocation" primitives
(destroy, copy-broadcast, rotate-with-delta) plus the grow/free pair plus
one new "iterator-pair-construct" primitive. `relocate_live` stays
internal-only (called by `storage_ops::resize`); it is NOT a public core.

The shared shape on every core is `(const type_ops*, const storage_ops*,
void* st_ctx, ...)`. `el_ctx` (the allocator instance) is fetched lazily
inside the core via `sops->alloc_ctx(st_ctx)` when `tops->flags & f_alloc_ctx`
is set, exactly as today. This survived the trial (per
`notes/callsite-analysis.md` trial table) only when the core *already*
takes `sops`; cores that don't take `sops` keep `el_ctx` as a compile-time
arg.

### 2.1 `grow` — capacity-ensure

```cpp
void grow(const detail::type_ops* tops,
          const detail::storage_ops* sops,
          void* st_ctx,
          size_t min_bytes);
```

**Semantics.** Ensure the buffer is large enough to hold `min_bytes` live
bytes by calling `sops->resize(tops, st_ctx, min_bytes)`. The post-state:
`begin_` points at a fresh allocation, live bytes are relocated, capacity
is at least `min_bytes`, the spare-slot zero is written. Currently a
thin wrapper around `sops->resize`; that is fine — it gives every caller
one symbol to forward through and lets the wrapper grow logic (e.g. the
"+1 for spare slot" check) move from forwarders to the core later if we
decide to.

**Trivial fast path.** None at this layer; lives inside `relocate_live`
inside `resize`.

**Callers.** `vector::vallocate`, `vector::reallocate_grow`,
`vector::shrink_to_fit`, `string::vallocate`, `string::reallocate_grow`,
`string::shrink_to_fit`. Six call sites today.

**ABI notes.** Three pointer args + one size = 4 args; in-register on
every target. Nothing spills.

### 2.2 `destroy_range` — destroy a constructed range

```cpp
void destroy_range(const detail::type_ops* tops,
                   const detail::storage_ops* sops,
                   void* st_ctx,
                   void* begin, void* end);
```

**Semantics.** Destroy every element in `[begin, end)` using
`tops->destroy`. Begin > end traps.

**Trivial fast path.** `triv_destroy(tops)` → return. This is the path
taken by all `char`-typed string and all trivially-destructible vector
elements under `std::allocator`.

**Callers.** `vector::vdeallocate`, `vector::destruct_at_end`,
`string::vdeallocate`, and inside `rotate(...,delta<0)` for the
truncation tail. The `rotate`-internal call is the only "transitive" one
and is what makes `destroy_range` worth a separate symbol from inlining
into the truncate path.

**ABI notes.** 5 args. Fits in registers on x86_64/AArch64; on ARM-32 the
fifth arg spills to stack (1 slot). On i386 everything spills (`push`
order is the compiler's). Acceptable as-is. The trivial-fast-path check
folds the body to ~5 bytes of test-and-return code, so the "wasted"
spill is dwarfed by saved bytes when the fast path applies (string<char>,
vector<int>, ...).

### 2.3 `rotate` — rotate-with-delta

```cpp
void rotate(const detail::type_ops* tops,
            const detail::storage_ops* sops,
            void* st_ctx,
            void* first, void* middle, void* last,
            ptrdiff_t size_delta_bytes);
```

**Semantics.** Standard `std::rotate` over the byte range, then adjust
container size by `size_delta_bytes`. On negative delta the tail
`[last + delta, last)` is destroyed via `destroy_range` AND
`sops->set_size` shrinks by `|delta|`. On positive delta `set_size` grows
by `delta` (caller has already constructed those elements outside the
rotated window; rotate's job is to roll them into place). Delta = 0
leaves size unchanged.

**Trivial fast path.** When `triv_reloc(tops)` is set the block-swap uses
`memcpy`/`memmove` on a scratch buffer (256-byte stack scratch + `malloc`
fallback). When also `triv_destroy(tops)`, the delta<0 destroy is a no-op
and only the `set_size` call fires.

**Callers.** `vector::insert`, `vector::emplace`, `vector::erase`,
`vector::erase(first, last)`, `vector::insert(iter-pair)`. **String
modifiers do NOT call `rotate` today** — they hand-roll the shift via
`traits_type::move` + null-terminate. See §3 for the proposal.

**ABI notes.** 7 args = the only core that spills on x86_64 (6 register
args). `size_delta_bytes` is the right thing to spill: it's used after
the move-heavy body, so the spill-load is hidden behind the latency of
the relocation loop. The empirical evidence (`notes/callsite-analysis.md`,
"rotate(...,delta) verdict on three architectures") is that
`with-delta vs no-delta` is `+174 / +75 / +68 B per binary` smaller on
x86_64 / i386 / ARM-32 respectively, with `vec_mixed.cpp` as the
realistic consumer. **Keep the delta.**

### 2.4 `copy_construct_one_n` — broadcast-copy

```cpp
void copy_construct_one_n(const detail::type_ops* tops,
                          const detail::storage_ops* sops,
                          void* st_ctx,
                          void* dst, const void* src, size_t n);
```

(Renamed from current `construct_copy_one_n` for verb-first symmetry with
the `cross_ops` `construct_from` op. The current name is fine; if rename
costs bytes in symbol size, skip it.)

**Semantics.** Construct `n` copies of `*src` at `dst[0..n)`. Caller
guarantees capacity. Trivial fast path memcpys the source bytes into
each slot.

**Callers.** `vector::insert(pos, n, x)` — one caller in tree today.

**Should it stay a separate symbol?** Yes, marginally. The body has a
clean memcpy-loop fast path and a leaf-loop slow path; inlining it into
the one caller would duplicate both at any future second caller (e.g.
`assign(n, x)` if/when assign migrates to a core path, or
`vector::vector(n, x)` constructor). It is also the natural symmetry
partner for the proposed §2.6 `copy_construct_from_range`. Keep, but
revisit if six months pass with still one caller.

**ABI notes.** 6 args; fits in registers on x86_64/ARM64. On ARM-32
`n` spills (5th of 6 register-or-stack args). Acceptable.

### 2.5 `relocate_live` — INTERNAL byte-level move

```cpp
// Defined in src/support.cpp, called by storage_*_resize<C>.
void relocate_live(const detail::type_ops* tops, void* el_ctx,
                   unsigned char* dst, unsigned char* src, size_t live_bytes);
```

**Not a public core.** Called only by the `storage_a_resize<C>` and
`storage_b_resize<C>` template bodies. Declared in `bits/vector_cores.h`
today; the right move is to move the declaration to `bits/storage_ops.h`
(where its caller lives) and stop listing it in the cores menu — it is
not part of the container forwarder vocabulary.

Note the signature uniquely takes a *pre-resolved* `el_ctx` (the resize
template fetches via `storage_a_alloc_ctx<C>` once at entry) rather than
`sops` + `st_ctx`. That asymmetry is intentional: at the point of call,
the resize template has already done the strategy-specific `void*` work
and there is no `storage_ops` to thread (we are *implementing* one).
Adding a `sops`+`st_ctx` arg here would force a recursive call back
through the table for no win.

### 2.6 NEW: `copy_construct_from_range` — iterator-pair bulk construct

```cpp
void copy_construct_from_range(const detail::type_ops* tops,
                               const detail::cross_ops* xops,   // T <- U
                               const detail::storage_ops* sops,
                               void* st_ctx,
                               void* dst,
                               const void* src_begin,
                               const void* src_end);
```

**Semantics.** Construct `(src_end - src_begin) / xops->second->size`
elements at `dst[0..)`, each from the corresponding `src[i]` via
`xops->construct_from`. Trivial fast path: when `T == U` (the
`xops->first == xops->second` test) AND the elements are
trivially-copyable + default-life, memcpy the byte range.

**Callers** (proposed; this is the wiring of the currently-dead
`cross_ops`):

- `vector::construct_at_end(InputIter first, Sentinel last, n)` (the
  forward-iterator overload). Currently a per-(C, T, A, Iter) template
  loop calling `construct_one(begin_+i, *first)`. Replacing it for the
  contiguous-iterator case folds N template loops into one shared body.
- `vector::insert(pos, InputIter first, InputIter last)` (forward
  overload, contiguous-iterator case). Currently the same per-template
  loop.
- `vector(InputIter first, InputIter last)` (forward, contiguous case).

The case where iterator and element types are *byte-identical* (`vector<int>`
range-ctor from `int*`) is the common one and gets the memcpy fast path.
The cross case (`vector<long>` from `int*`) gets the leaf loop.

**Why a new core and not the existing `copy_construct_one_n`?**
Different semantics: `_one_n` reads one source for N destinations;
`_from_range` reads N distinct sources for N destinations. The bodies
overlap (both broadcast vs. range-iterate), but the source-stride is
different and folding both into one symbol with a "stride = 0 means
broadcast" trick obscures the fast paths and costs branches on the hot
path.

**Why this is not premature.** `cross_ops` exists in tree today with
zero callers; either we wire it or we delete it. Wiring it pays off only
if it folds enough per-template-instantiation code (the per-(C, Iter)
range loop) to beat one new core symbol. The right gate is the
measurement in §5.

**ABI notes.** 7 args — same arity as `rotate`. On x86_64 the natural
spill is `src_end` (used only for the loop bound, computed once from
the first six args before any leaf call). Spill choice is the
compiler's; we can hint via parameter order but should measure before
committing. Default-order recommendation: `(tops, xops, sops, st_ctx,
dst, src_begin, src_end)` — matches reading order "with these vocab
tables on this container, copy from src into dst." Don't reorder
without measurement, per the trial-table evidence that `st_ctx-first`
regressed on x86_64.

## 3. Cores to merge, drop, or rename

### 3.1 String modifiers should go through `rotate`

`string::insert`, `string::erase`, `string::replace`, `string::resize`
(shrink case) all hand-roll the shift via `traits_type::move(p+dst,
p+src, n)` and an explicit null-terminate. That is *exactly* what
`rotate(...,delta)` does — and string is what motivated the
"set_size also zeroes the spare slot" invariant in `storage_ops` in
the first place.

**Proposal.** Migrate string's mutating modifiers to call `rotate`
through a `rotate_(...)` helper analogous to vector's. The hand-roll's
`null_terminate_()` step disappears because `storage_ops::set_size`
already writes the zero. The trivial-relocation path inside `rotate`
already collapses to the same `memmove` the hand-roll does, so the
common-case codegen is comparable; the wins are (a) one fewer set of
"sized shift" loops in the per-(CharT, Traits, A) string instantiation,
and (b) string no longer reinvents the spare-slot-zero invariant.

**Caveat / measurement gate.** `replace(pos, n1, s, n2)` is the
interesting case: `n1 != n2` means the shift distance and length both
vary, and `rotate` does block-swap-via-scratch which is heavier than
`memmove` for the simple "shift tail by k" subcase. The right shape
is probably: forwarder picks between `traits_type::move` (no insert/
erase, just shift) and `rotate` (when delta != 0). Decide by measuring
`bench/drivers/str_*.cpp` (if it exists; if not, add one with
insert/erase/replace patterns) before committing.

### 3.2 `cross_ops` — wire it or delete it

Currently zero callers in tree. Two options:

a. Wire it via §2.6 `copy_construct_from_range`. This pays off only
   if §2.6 reduces enough per-(C, Iter) template loops to beat the
   constant-table cost of `cross_for<T, U>` instantiations. The
   constant table is small (one struct of pointers, mostly null).
b. Delete it. Saves ~100 lines of `bits/type_ops.h`; loses the
   abstraction for any future container that wants `flat_map`-style
   T-from-U construction.

**Recommendation.** (a), gated on the measurement in §5. If the
measurement says no, do (b) — `cross_ops` doesn't pay rent.

### 3.3 `copy_construct_one_n` rename

Cosmetic: rename to `copy_construct_one_n` → `copy_construct_n_from`
or `broadcast_copy_construct`. Either matches the
`copy_construct_from_range` symmetry. Not load-bearing; only do this
if §2.6 lands so the names line up. Otherwise leave it.

### 3.4 No new "destroy_at_end" / "pop_back" / "set_size" cores

Those would just be `sops->set_size(st_ctx, ...)` with a destroy in
front. Adding wrappers gives the compiler less information at the call
site (now it can't see the constant `delta` that `rotate(...,delta<0)`
folds). The current shape — `rotate` collapses erase to one call, all
other size changes are direct `set_size_elems_` in the forwarder — is
correct.

### 3.5 `vector::construct_at_end` family stays per-(C, T, A) for now

The "construct n defaults at end" path has no cross-container body to
share (default-construct is the most type-specific op). The only
common shape is the loop-and-bump-size, which is two lines. Don't
hoist; not worth a symbol.

## 4. What NOT to do

- **Per-arch signatures or `#ifdef` shims.** Single impl rule.
- **Dozens of micro-shim entries that tail-call one big impl** (e.g.
  `destroy_range_trivial_fastpath` exposed as a separate symbol).
  Trades source-readability for marginal callsite bytes that the
  inliner already gets. CLAUDE.md says we are writing C++.
- **Reorder so `st_ctx` is first.** Measured to regress
  `vec_many.cpp` rotate callsites by +13 B each on x86_64 (no change
  on i386). Source: trial table in `notes/callsite-analysis.md`.
- **Drop `rotate(...,delta)`.** NET cost +174 / +75 / +68 B per
  `vec_mixed.cpp` binary on x86_64 / i386 / ARM-32 — the support.cpp
  saving from a simpler rotate body does NOT offset the per-callsite
  cost of splitting erase/insert/emplace into rotate + follow-up.
  Source: `notes/callsite-analysis.md` "rotate verdict" section.
- **Merge `copy_construct_one_n` and `copy_construct_from_range` into
  one stride-parameterized core.** The trivial-fast-path bodies are
  genuinely different (memcpy-of-one vs memcpy-of-range) and folding
  forces a runtime branch on the hot path.
- **Hoist `vector::construct_at_end` family into a core.** Per-(T)
  default-construct has no shared shape.
- **Add a separate "pop_back" core in front of `sops->set_size`.**
  Inlines fine in the forwarder; the wrapper symbol loses the
  constant-folding the compiler would otherwise do.
- **Pass `el_ctx` as an explicit arg to public cores.** The lazy fetch
  via `sops->alloc_ctx` was measured in the trial table to keep
  callsites smaller for stateless allocators (the common case): no
  fifth-arg threading on push_back/destroy callsites. The trade is
  that cores that *do* fetch it pay one indirect call inside
  `sops->alloc_ctx` — measured acceptable because that case is rare
  and the fetch is gated by a flag bit the compiler already had to
  load. Keep the current design.

## 5. Open questions (measurement-gated)

Listed with the measurement that would settle each.

1. **Does §2.6 `copy_construct_from_range` pay for itself?**
   Measurement: build `bench/drivers/vec_mixed.cpp` + a new
   `vec_range_ctor.cpp` driver that exercises range-ctor and
   range-insert for `vector<int>`, `vector<long>`, `vector<string>`,
   `vector<Big>` from `int*`, `long*`, `string*`, `vector<int>::iterator`.
   Measure per-arch (x86_64, i386, ARM-32) project `.text` with the
   range loop hoisted into the core vs. left per-template. The win
   condition is "smaller on i386 AND ARM-32, no worse than +5% on
   x86_64." If it doesn't pay, delete `cross_ops` (§3.2 option b).

2. **Should string modifiers route through `rotate`?**
   Measurement: build a `str_mixed.cpp` driver that does
   `insert(pos, s, n)`, `erase(pos, n)`, `replace(pos, n1, s, n2)` for
   `string`, `wstring`. Compare hand-rolled `traits_type::move` path
   vs. `rotate_` forwarder. Same per-arch + win condition. Expected
   result: insert/erase migrate, `replace` stays hand-rolled or
   picks per-shape inside the forwarder (see §3.1).

3. **Should `copy_construct_one_n` be inlined into its single
   caller (`vector::insert(pos, n, x)`)?**
   Measurement: per-callsite size of `vector::insert(pos, n, x)` on
   x86_64 + i386 + ARM-32 with the core present vs. with the body
   inlined into the template. If inlining is within ~10 B per
   callsite and saves the symbol (currently ~50 B in `support.cpp`),
   inline. If a second caller appears (e.g. §3.1 makes `string::resize`
   call it via `rotate`'s expand path), keep.

4. **Is the `alloc_ctx` slot on `storage_ops` worth keeping for
   Strategy B?**
   Strategy B is restricted to `std::allocator` (stateless), so the
   slot always returns nullptr. The slot exists for ABI uniformity
   with Strategy A. Cost: 1 pointer in the `storage_ops_b<C>` rodata
   table per `(C)`. Win from removal: 4 bytes × per-instantiation count
   × 32-bit-arch. Measurement: simple — count instantiations across
   `bench/projects/*` and multiply. If under 100 B per project, keep
   for uniformity. (Hand-wave estimate: typical project has 5–10 string
   shapes, ~40 bytes total. Keep.)

5. **Should `relocate_live` be moved out of `bits/vector_cores.h`?**
   It's not a public core (only resize implementations call it). Move
   the declaration to `bits/storage_ops.h` and stop documenting it in
   the cores menu. No measurement needed; this is hygiene.

## Summary

The proposed set is six symbols: `grow`, `destroy_range`, `rotate`,
`copy_construct_one_n` (existing four), plus one new
`copy_construct_from_range` that wires the currently-dead `cross_ops`
into vector's iterator-pair construct/insert paths, plus the
internal-only `relocate_live` (declaration moved to `bits/storage_ops.h`).
String migrates its `insert`/`erase` modifiers to call `rotate`; its
`replace` and find/compare families stay where they are. `cross_ops` is
wired or deleted (§3.2). Two trial measurements (`vec_range_ctor`,
`str_mixed`) settle whether the two migrations land. The set deliberately
preserves the load-bearing design choices from the trial record:
`rotate(...,delta)` stays, lazy `el_ctx` fetch through `sops->alloc_ctx`
stays, `st_ctx` is not first.
