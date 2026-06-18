# Cores design — the type-erased sandwich for libcis

This is the consolidated design we implement against. It supersedes the
discussion in `notes/cores-design-v5.md`, `notes/cores-design-v6.md`,
`notes/cores-design-v7.md`, and integrates the in-tree corrections from
`notes/cores-critique.md`. Where the prior notes disagreed with the tree,
this document follows the tree; where they were consistent, this document
states the consensus once.

The design is **measurement-first**: stages don't ship without a bench
probe and a journal entry. Stages that miss their predicted delta by
>50% are reverted, not papered over (per `CLAUDE.md`'s "negative results
are signal" rule).

---

## 0. TL;DR

The shape:

```
  container/wrapper header   thin, type-aware
  ────────────────────────────────────────────
  vocab table(s)             const X_ops* — what the type can do
  ────────────────────────────────────────────
  cores                      non-template, in src/support.cpp
                             bytes + vocab pointers in, ditto out
```

Already shipped (in-tree today, frozen):

- **Five contiguous-storage cores** in `src/support.cpp`: `grow`,
  `destroy_range`, `construct_copy_one_n`, `rotate`, `relocate_live`.
- **Two vocab tables**: `type_ops` (element vocab) in
  `include/bits/type_ops.h`, `storage_ops` (buffer vocab) in
  `include/bits/storage_ops.h`.
- **Forward-decl header**: `include/bits/vector_cores.h` (current name).

To-be-frozen later (each gated on a bench probe + a journal entry):

- **Deque (segmented storage)**: 3 cores + `segment_map_ops` vocab.
- **`any` and `function`**: 1 vocab each (`value_ops`, `callable_ops<Sig>`).
- **Algorithm primitives**: `equal_bytes`, `find_byte`.
- **Sum types**: `sum_ops` vocab for `variant` lifecycle.
- **Smart pointers**: `sp_cb_ops` replaces `shared_ptr` control-block vtable.
- **Cold corners**: a `valarray_resize_exact` core, a
  `path_lexical_normal_bytes` core, a `future_callback_ops` vocab.

Explicitly **not** proposed: anything in §4 (the rejection list).

---

## 1. The rule that picks the set

> A core earns its keep when ALL of the following hold:
> 1. Its body is **typeless byte work** (size in bytes, not count of `T`).
> 2. **More than one caller** consumes the same body (across containers,
>    or across template instantiations of the same wrapper).
> 3. The **trivial fast path** collapses to `memcpy`/`memmove`/`memset`/
>    `memcmp`/skip/atomic-builtin — something the linker can fold across
>    consumers.
> 4. The compile-time alternative is **not already the COMDAT optimum**.

Corollary (carried from v6): if the type erasure is already done at
compile time (void-pointer rebind, CRTP base, struct-of-pointers chained
by `next_pointer`) and the resulting code is non-template-but-
instantiation-shared across `T` for a fixed `Alloc`, the compile-time
pattern is the core. Don't add a runtime vocab on top.

Corollary (added in v7): a virtual-function API is already a runtime
vocab table. `memory_resource`, `streambuf`, `locale::facet`,
`error_category` are vocab tables; don't propose a parallel one. The
inversion also holds — when the library ships an internal virtual
hierarchy whose subclasses fold pairwise under COMDAT if the vtable is
removed (`sp_counted_base`, `assoc_sub_state`), replacing the vtable
with a vocab table is the sandwich.

And the load-bearing rule from v5:

> **Every container modifier resolves to one core call (or none).** If a
> modifier needs two cores, the cores are wrong.

---

## 2. The five contiguous-storage cores (frozen)

In `src/support.cpp`, declared in `include/bits/vector_cores.h`:

```cpp
namespace std::detail {

// 1. Ensure the buffer holds at least min_bytes live bytes.
void grow(const type_ops*, const storage_ops*,
          void* st_ctx, size_t min_bytes);

// 2. Destroy elements in [begin, end). Trivial-fast-path: skipped.
void destroy_range(const type_ops*, const storage_ops*,
                   void* st_ctx, void* begin, void* end);

// 3. Construct n copies of *src at dst. Trivial-fast-path: memcpy loop.
void construct_copy_one_n(const type_ops*, const storage_ops*,
                          void* st_ctx, void* dst, const void* src, size_t n);

// 4. std::rotate over a constructed range, plus a post-rotate size delta.
//    Erase = rotate(p, p+es, end, -es). Insert = construct-at-end + rotate.
void rotate(const type_ops*, const storage_ops*, void* st_ctx,
            void* first, void* middle, void* last,
            ptrdiff_t size_delta_bytes);

// 5. Internal to storage_ops: relocate live bytes from old to new buffer.
//    Called by storage_ops::resize implementations; not by containers.
void relocate_live(const type_ops*, void* el_ctx,
                   unsigned char* dst, unsigned char* src, size_t live_bytes);

} // namespace std::detail
```

### 2.1 Calling convention

The four container-facing cores all share:

```
fn(const type_ops* tops,
   const storage_ops* sops,
   void* st_ctx,
   ... op-specific args, all byte-typed ...)
```

- **`tops`** — element vocabulary (size, align, lifecycle leaves, value
  ops, flags including triviality bits + `f_alloc_default_life` +
  `f_alloc_ctx`).
- **`sops`** — buffer vocabulary (5 state slots: `data`, `set_size`,
  `cap_end`, `resize`, `free`; 1 ctx slot: `alloc_ctx`).
- **`st_ctx`** — opaque pointer the storage_ops slots understand. Today
  it is `this` for vector and string; for a future deque segment it is
  the segment header.
- **op args** — `void*` pointers into the buffer, byte counts (`size_t`),
  signed byte deltas (`ptrdiff_t`). The element size (`tops->size`) is
  the conversion factor; the container converts at the boundary so no
  `sizeof(T)` math leaks into the core.

`el_ctx` (the allocator instance pointer) is **fetched lazily** by each
core via `sops->alloc_ctx(st_ctx)`, **only** when
`tops->flags & f_alloc_ctx` is set. For stateless allocators the fetch
never fires.

`relocate_live` is the leaf exception: the storage_ops resize slot has
already (a) called `alloc_ctx` and (b) holds the result, so
`relocate_live` takes `el_ctx` directly. This asymmetry is real and
documented; it is the price of factoring the relocation loop into its
own function.

### 2.2 The five cores, one paragraph each

**`grow`** — one-line forwarder onto `sops->resize`. Exists for
instrumentation and to keep the container from needing to know which
mask-bits of `tops->flags` matter for the resize slot.

**`destroy_range`** — calls `tops->destroy` on every element in
`[begin, end)`, end-to-begin. Trivial fast path:
`f_triv_destroy & f_alloc_default_life` → return. Traps on `begin > end`.

**`construct_copy_one_n`** — copy-constructs `n` elements at `dst`,
each from the single object at `src` (one source, broadcast). Trivial
fast path: `f_triv_copy & f_alloc_default_life` → memcpy loop. Only
vector's `insert(pos, n, x)` calls it today; string uses
`traits_type::assign` for `char`-broadcast.

**`rotate`** — `std::rotate` over `[first, last)` plus a signed
`size_delta_bytes`. Algorithm: block-swap via stack/heap scratch
(`alloca`-style: 256 B on the stack, `::malloc` above that). Trivial
fast path: `triv_reloc & f_alloc_default_life` → three memcpy/memmove
calls. The size-delta-in-this-core decision is load-bearing: it makes
every erase one core call and every insert "construct at end + rotate"
(two calls only because the construct happens in already-allocated room
via the spare-slot invariant). Traps on `first > middle || middle > last`.

**`relocate_live`** — move-then-destroy primitive that the storage_ops
resize slot calls. Trivial path is memcpy/memmove; otherwise walks
elements via `move_construct` + `destroy`.

### 2.3 What the cores read from `type_ops`

The cores read: `size`, `align`, `flags & {f_triv_destroy,
f_triv_copy, f_triv_reloc, f_alloc_default_life, f_alloc_ctx}`,
`destroy`, `copy_construct`, `move_construct`.

The cores do **not** read: `default_construct`, `copy_assign`,
`move_assign`, `swap`, `compare`, `equal`, `hash`.

### 2.4 What the cores read from `storage_ops`

The cores read: `resize`, `data`, `set_size`, `alloc_ctx`.

The cores do **not** read directly: `free`, `cap_end` (container-facing
only).

### 2.5 Where the cores live

`src/support.cpp`. Each core is its own `.text.fn_name` section so
`--gc-sections` drops unused ones. Forward decls in
`include/bits/vector_cores.h` (the current name — the rename to
`bits/cores.h` is a cosmetic cleanup; see §10 A1).

The include graph:

```
  <vector>, <string>
       │
       ▼
  bits/vector_cores.h    (decls only)  [or bits/cores.h post-rename]
       │
       ▼
  bits/storage_ops.h     (5 vocab slots + glue)
       │
       ▼
  bits/type_ops.h        (element vocab + leaves)
       │
       ▼
  bits/relocatable.h     (the one trait the leaves need)
```

### 2.6 The container modifier table

| vector modifier | core call(s) |
|---|---|
| `vallocate(n)` / `reallocate_grow(n)` | `grow(_, _, this, (n+1)*es)` |
| `vdeallocate()` | `destroy_range(begin, end); sops->free(this)` |
| `destruct_at_end(p)` | `destroy_range(p, end); set_size` |
| `push_back` / `emplace_back` | none (in-place construct after `ensure_free_`) |
| `pop_back` | none (one destroy leaf + size update) |
| `emplace(pos)` | construct at end (`ensure_free_(2)`), `rotate(pos, p, p+1, +es)` |
| `insert(pos, n, x)` | `ensure_free_(n+1)`, `construct_copy_one_n(...)`, `rotate(pos, p, p+n, +n*es)` |
| `insert(pos, first, last)` (fwd) | `ensure_free_(n+1)`, container-side construct loop, `rotate(...)` |
| `erase(pos)` | `rotate(p, p+es, end, -es)` |
| `erase(first, last)` | `rotate(p, p+n*es, end, -n*es)` |
| `clear()` | `destroy_range(begin, end); set_size 0` |
| `resize(n)` | grow or `destroy_range`, then container-side default loop |
| `shrink_to_fit()` | `grow(_, _, this, (sz+1)*es)` |

String mirrors this minus `construct_copy_one_n` (it uses
`traits_type::assign`). String **does not call** `construct_copy_one_n`
today and isn't expected to. (v5 §6's "happens to bypass" wording in the
v5 note is misleading — see critique E1; the truth is "never wired".)

### 2.7 The invariants the cores enforce

1. **Trivial-fast-path correctness** — `f_triv_X & f_alloc_default_life`
   both required before any core takes the byte path.
2. **Lazy ctx fetch** — `el_ctx` fetched at most once per call, only when
   `f_alloc_ctx`.
3. **Byte-granular arithmetic** — all pointer math in bytes;
   `tops->size` is the multiplier; no `T*` casts except inside leaves.
4. **Spare-slot ownership** — `storage_ops::set_size` and
   `storage_ops::resize` write the spare slot at the new end. Cores
   never write it themselves. (Verified against
   `storage_ops.h::storage_{a,b}_{set_size,resize}` — all four memset the
   spare slot; critique D1.)
5. **One core call per modifier** — exception: `vdeallocate`
   (`destroy_range` then `sops->free`) crosses the two vocab tables and
   cannot be combined without breaking the abstraction.

### 2.8 The known bug to fix as a standalone commit

`include/vector` lines 216–218 — `ops_table_()` mask is

```cpp
detail::OP_DESTROY | detail::OP_MOVE_CONSTRUCT
```

but vector calls `construct_copy_one_n` (in `insert(pos, n, x)`), which
on the non-trivial path calls `tops->copy_construct`. With the current
mask, `copy_construct` is null for any `T` not trivially-copy-default-
life. Trivial T (`vector<int>::insert(p, 5, 42)`) hits the memcpy fast
path and survives; `vector<std::string>::insert(p, 3, "abc")` traps.

Fix: add `OP_COPY_CONSTRUCT` to the mask. Own commit, bench-journal
entry as a correctness fix (critique B1).

---

## 3. The other containers and wrappers

### 3.1 No new cores for `list` / `forward_list` / `map` / `set` / `unordered_*` / `tree` / `hash_table`

The link-walking algorithms in `bits/list_node`, `bits/tree.h`, and
`bits/hash_table.h` are already template-parameterized on `NP` / `node_
base_pointer`, instantiated **once per `pointer_traits<VoidPtr>::rebind`**
(once total for `std::allocator` flavors). Pulling them through a runtime
`node_ops` vocab would replace direct loads with `call *[node_ops+offset]`
per link chase — +4 B per callsite × ~30 chases in `tree_remove` alone =
+120 B per instantiation to gain nothing. The compile-time pattern is
the core.

The same applies to `Comp` / `Hash` / `KeyEqual` template parameters of
`map`, `set`, `unordered_*` — inlining them is the right shape; routing
them through `type_ops::compare` / `equal` / `hash` would replace one
`<` with an indirect call per descent step.

If a future bench shows the fancy-pointer rebind costs measurable `.text`
per `(T, Alloc)` pair, the right move is a **compile-time**
`node_link_ops` helper (a struct with static members) inside
`bits/list_link.h`. That is still template-parameterized, not a runtime
vocab.

### 3.2 Deque — 3 cores + `segment_map_ops` vocab (stage 4)

The single-segment case is the v5 sandwich (each segment is a
fixed-capacity contiguous buffer; storage_ops's resize traps, set_size
is a field update, free returns to a pool, cap_end is
`begin + BlockSize*sizeof(T)`).

The new work is cross-segment:

```cpp
// Destroy [first, last) walking segments. Trivial path: skip whole
// segments where f_triv_destroy & f_alloc_default_life.
void segmented_destroy(const type_ops*, const segment_map_ops*,
                       void* st_ctx, void* first_iter, void* last_iter);

// Move (or copy) construct a range from one deque's segments into
// another's. Trivial path: segment-aligned memcpy runs.
void segmented_copy(const type_ops*, const segment_map_ops*,
                    const segment_map_ops*, void* src_ctx, void* dst_ctx,
                    void* src_first, void* src_last, void* dst_first);

// Segment-aware rotate with size delta. Implementation: gather to
// scratch + scatter back, segment-aligned. Trivial path degenerates to
// per-segment memcpy.
void segmented_rotate(const type_ops*, const segment_map_ops*,
                      void* st_ctx, void* first, void* middle, void* last,
                      ptrdiff_t size_delta);
```

The new vocab:

```cpp
struct segment_map_ops {
    // (deque_st_ctx, byte_index) -> segment containing element at
    // byte_index / block_bytes, plus byte offset within it.
    byte_span (*locate)(void* st_ctx, size_t byte_index);
    size_t    (*size_bytes)(void* st_ctx);
    // Step to the next segment after `seg`. Returns nullptr at end.
    void*     (*next_segment)(void* st_ctx, void* seg);
    // Append / peel-back segments around modifier size deltas.
    void      (*push_back_segment)(void* st_ctx);
    void      (*pop_back_segment)(void* st_ctx);
};
```

Lives in `include/bits/segment_map_ops.h`. Five slots, same shape as
`storage_ops`.

Rejected (do not add): `segmented_construct_at_back(n, src)` for
`assign(n, x)`. Per-segment count is small (block size for `int` is
~1024; for larger `T`, smaller — the precise upper bound depends on
`deque_block_size`'s formula in `<deque>` line 30; the argument carries
without pinning the exact number — critique D5). Typed loop is tighter
than the typeless core. Don't ship the symbol.

Migration: land deque-as-it-stands first; ship the three cores in
`src/support.cpp` second; switch deque's modifiers over third. The
intermediate state (typed deque) is correct; the typeless version is a
code-size optimization.

### 3.3 `any` — `value_ops` vocab (stage 2)

Confirmed against `include/any` lines 256-263: the current
`SmallHandler<T>::handle` / `LargeHandler<T>::handle` is a per-T
function with a 4-way switch over `Action` enum
(Destroy/Copy/Move/Get). Critique D2 verified this.

Replace `HandleFuncPtr` with `const value_ops*`:

```cpp
struct value_ops {
    size_t size;
    size_t align;
    unsigned flags;     // f_triv_destroy, f_triv_copy, f_alloc_default_life,
                        //   f_is_small (inline-fits-in-3-ptrs)
    void (*destroy)(void* p);
    void (*copy_construct)(void* dst, const void* src);
    void (*move_construct)(void* dst, void* src);
    const void* type_id;  // address of unique_typeinfo<T>::id
};
```

The five `any` modifiers (`reset`, copy ctor, move ctor, swap, `any_cast`
ptr form) call `v_->destroy` / `v_->copy_construct` / `v_->move_construct`
directly. The Action-enum switch goes away.

Per-T cost: down from ~200 B (switch + four arm bodies per `T`,
small-or-large flavor) to one `value_ops` constant (~56 B) plus four
COMDAT-folded leaf bodies. Break-even at ~3 distinct stored `T`. Per-call
overhead is **lower** than today's (one indirect call to a leaf vs one
indirect call into the switch + one switch jump — critique D2 footnote).

Lives in `include/bits/value_ops.h`. Sibling to `type_ops`, not a subset
(no allocator threading, plus `f_is_small` and `type_id` slots that
`type_ops` doesn't carry).

### 3.4 `function` — `callable_ops<Sig>` vocab (stage 3)

Same idea as `any`, plus a sig-shaped invoke slot. The vtable becomes:

```cpp
template<class Sig> struct callable_ops;  // primary undefined
template<class R, class... A> struct callable_ops<R(A...)> {
    unsigned flags;     // f_triv_*, f_is_small
    size_t   size, align;
    void   (*destroy)(void*);
    void   (*copy_construct)(void* dst, const void* src);
    void   (*move_construct)(void* dst, void* src);
    R      (*invoke)(void*, A&&...);
};
```

Lifecycle ops (destroy/copy/move) become typeless cores in
`src/support.cpp`. Invoke and the swap-via-temporary stay
template-instantiated **per signature** but the template body is small
(~30 lines) and folds across captured `F` for fixed `Sig`. The boundary
between typeless cores and per-Sig templates is the call signature.

Lives in `include/bits/value_ops.h` (alongside `value_ops`).

Vtable removal rationale (more precise than v6 stated, per critique
D3 / v7 §2.3.2): under `-fno-rtti`, gcc still emits the typeinfo pointer
slot in the vtable (populated with null), and the vtable is keyed on the
concrete-class type. A vocab table replaces this with per-flavor
constants whose bodies fold across template parameters at the byte level.

### 3.5 Algorithm primitives — `equal_bytes`, `find_byte` (stage 5)

```cpp
bool equal_bytes(const type_ops*, const void* a, const void* b,
                 size_t n_bytes);
const void* find_byte(const type_ops*, const void* begin, const void* end,
                      const void* needle);
```

`equal_bytes`: memcmp fast path for triv-equality T; else `tops->equal`
loop. Used by `std::equal`, `operator==(vector, vector)`,
`vector<T>::operator==`.

`find_byte`: memchr fast path for char-sized triv-equality T; else
`tops->equal` loop. Used by `std::find` and string/string_view searches.

Lives in `include/bits/algo_cores.h`.

Rejected (do not add):
- `copy_bytes` — `construct_copy_one_n` already specializes to memcpy
  for one-element source; for iterator-pair copies the typed loop's
  `*it++` inlines tighter.
- `sort_bytes` — deferred. Size win is real (~3 KB per `(T, Compare)`
  pair → ~3 KB shared), runtime cost is real too (indirect call per
  comparison, qsort_r-shaped slowdown). Gate is a runtime+size bench
  that doesn't exist. If two-of-three arches show `.text` win > 1 KB
  per pair AND runtime regression < 50%, ship.
- `lexicographical_compare_bytes` — borderline; bench alongside
  `equal_bytes` if it lands cleanly.

### 3.6 Sum types — `sum_ops` vocab for variant lifecycle (stage 6)

`variant<Ts...>` currently builds an fmatrix of N function pointers per
visit (one per alternative), keyed on the exact (Visitor, Ts...) tuple.
For lifecycle modifiers (destroy/copy/move/swap), the visitors are
trivial — "call the Is-th alternative's leaf" — and the fmatrices are
pure dispatch overhead.

```cpp
struct sum_ops {
    size_t   n_alternatives;
    size_t   size;       // sizeof(variant)
    size_t   align;
    unsigned flags;      // AND of triviality across alternatives
    const struct alt_ops* alts;
};

struct alt_ops {
    size_t offset;
    void (*destroy)(void* alt);
    void (*copy_construct)(void* dst_alt, const void* src_alt);
    void (*move_construct)(void* dst_alt, void* src_alt);
};

void sum_destroy(const sum_ops*, void* self, size_t index);
void sum_copy_construct(const sum_ops*, void* dst, const void* src,
                        size_t src_index);
void sum_move_construct(const sum_ops*, void* dst, void* src,
                        size_t src_index);
```

Per-flavor cost: down from ~256 B fmatrix + private bodies (for N=4) to
~`(N+1) × 32` B table. The `std::visit` / comparison / cross-index
assignment paths stay template-instantiated.

Lives in `include/bits/sum_ops.h`. `optional` and `expected` stay as-is
— their bool-discriminator + payload is layout-shaped, not vocab-shaped,
and there is no shared body to factor (`if (engaged_)` collapses to
memcpy(T) when T is trivially copyable; no slot is unlocked by a core).

### 3.7 `shared_ptr` — `sp_cb_ops` replaces control-block vtable (stage 7)

`<memory>` ships three concrete `sp_counted_base` subclasses
(`sp_counted_ptr_deleter<P,D,A>`, `sp_counted_emplace<T,A>`, plus the
abstract base). Each instantiation emits a vtable + two non-trivial
virtual bodies. The refcount manipulation itself is non-virtual and
already shared.

Replace the vtable with:

```cpp
struct sp_cb_ops {
    uint16_t ptr_offset;        // 0xffff = no separate ptr (emplace)
    uint16_t deleter_offset;    // 0xffff = default delete
    uint16_t alloc_offset;      // 0xffff = ::operator new/delete
    uint16_t cb_size;
    unsigned flags;             // f_alloc_default, f_no_deleter,
                                //   f_inline_emplace
    void (*on_zero_shared)(void* cb);
    void (*on_zero_shared_weak)(void* cb);
};
```

`sp_counted_base` stores `const sp_cb_ops*` in place of the vtable
pointer. `release_shared` becomes
`if (...) ops_->on_zero_shared(this)` — one indirect call, same cost as
the virtual it replaces.

Win shape: bodies fold across template parameters at the byte level
(the "trivially-destructible T, default delete" `on_zero_shared` body
is one shared symbol for all such T). Cost: one `sp_cb_ops` constant
per distinct *(deleter, allocator, inline-emplace)* combination
(typically 2-4 per program). Break-even at ~3 distinct `T`.

Lives in `include/bits/sp_cb_ops.h`. Sibling to `value_ops`.

`unique_ptr` stays as-is — pointer + EBO deleter is already the
optimum.

### 3.8 `<future>` shared state — `future_callback_ops` (stage 9, contingent)

Same shape as §3.7: `<future>` builds an `assoc_sub_state` virtual base
with derived classes for `promise::set_value`, `packaged_task`,
deferred `async`. Pull the lifecycle + invoke into:

```cpp
struct future_callback_ops {
    size_t value_size;
    size_t value_align;
    unsigned flags;
    void (*destroy_value)(void* state);
    void (*invoke)(void* state);  // signature-erased: callbacks are always R(void)
};
```

The invoke slot is **always** `R(void)` because args are bound at
`packaged_task::operator()` time — one signature, no per-Sig table.

Lives in `include/bits/future_cb_ops.h`. Marginal saving (~0.5 KB on
async-heavy programs); land only if `sp_cb_ops` (§3.7) lands cleanly
and the v8 author confirms `sp_cb_ops` infrastructure can't be shared
directly (it can't, per v7 §2.11.4 — the layout assumptions differ).

### 3.9 Cold corners

- **`<sstream>`** — v6/v7 proposed `sb_xsputn_bytes`. Per critique A2,
  this is built on a **false premise**: `basic_stringbuf` in this tree
  has **no `xsputn` override** (only `overflow`, `underflow`,
  `pbackfail`, `setbuf`, `seekoff`, `seekpos`, `sync`). The default
  `streambuf::xsputn` loops on `sputc` and routes through `overflow`,
  which itself grows the underlying `basic_string` via the v5 cores
  transitively. **Decision**: drop the `sb_xsputn_bytes` core from the
  plan. If a future bench shows that adding an `xsputn` override AND
  factoring it pays off, it's an "add an override and core it"
  proposal, not a "factor existing duplication" one — different scope.

- **`<valarray>`** — `valarray_resize_exact` core for the
  `resize` / size-mismatch `operator=` / move-from-shorter case.
  Trivial fast path: `f_triv_destroy & f_alloc_default_life` →
  skip destroy loop. Three callers, one body. Saving ~150 B
  per-`T` instantiation, bounded by valarray usage (typically < 0.5 KB
  per program). Land only if a downstream user measurably uses
  valarray.

- **`<filesystem>`** — `path_lexical_normal_bytes` core for the
  state-machine walk used by `lexically_normal`, `lexically_relative`,
  `lexically_proximate`, `parent_path`. Pure byte work (POSIX `char`
  only). Saving ~0.3 KB per program using `<filesystem>` non-trivially.
  Small but clean.

---

## 4. The "no" list

These were considered and rejected. Each entry exists so a future
agent has a written reason to look elsewhere instead of relitigating.

### 4.1 Contiguous-storage shape

- **`shift_down` / `shift_up`** — rotate handles the trivial single-
  element case already; special-casing doesn't simplify the container.
- **`construct_default_n`** — one caller (`vector(n)`), one-line body,
  no shared work.
- **`grow_with_gap`** — fires only on `insert(pos, n, x)` with growth;
  symbol cost outweighs per-instance saving across all programs that
  pull in vector but don't use this path.
- **`assign_range` / `assign_with_size`** — needs an iterator vocab
  that loses more code than it saves.
- **`swap_to` / `alloc_only` slots** — the prompt-1 v4 misfire. Five
  storage_ops slots, not seven; the resize slot owns its relocation
  internally via `relocate_live`.
- **`pop_back`** — two-line body, no shared work, hurts locality the
  compiler exploits today.
- **`reserve_or_relocate` for SSO** — SSO is layout, not type-erasure.
  The in-place buffer is type-known.
- **`set_size` as a sixth core** — six callsites in vector (not the
  ~20 v5 claimed; critique A3), the forwarder is the right shape.
- **`relocate_live` as a 7th storage_ops slot** — only one
  implementation; would be a slot with one instance, which is the worst
  kind of slot.

### 4.2 Node containers

- **`tree_remove` / `tree_balance_after_insert` cores** — link-chase
  indirect calls regress code size.
- **`Comp` / `Hash` / `KeyEqual` as `type_ops` slots routed from
  container template params** — runtime cost is too high (one indirect
  call per descent step), size win is too small.
- **`node_ops` runtime vocab** — compile-time parameterization is
  already the optimum; if fancy-pointer cost measurably hurts, a
  *compile-time* `node_link_ops` helper is the answer, not a runtime
  vocab.

### 4.3 Wrappers and type-erased values

- **`optional_ops`** — bool test is layout-shaped, not vocab-shaped.
  Trivial-copy case is already `memcpy(sizeof(optional<T>))`.
- **`tuple_ops` / `pair_ops`** — product types are compile-time only.
- **`unique_ptr` core** — EBO + pointer is the optimum.
- **Per-signature unification of `function<Sig>`** — the signature is
  the type. Erasing it through a `void* arg_pack` loses the inlining
  that is the entire point.

### 4.4 The "virtual-base APIs"

These are already vocab tables by virtue of their public ABI; do not
propose parallel vocabs:

- **`<memory_resource>`** (memory_resource).
- **`<streambuf>`** / **`<istream>`** / **`<ostream>`** /
  **`<iostream>`** / **`<fstream>`** (basic_streambuf).
- **`<locale>`** (facets).
- **`<system_error>`** (error_category).

### 4.5 Compile-time fusion

- **`<iterator>` / `<ranges>`** — compile-time view fusion is the
  performance model. Runtime erasure breaks the inlining.
- **`<chrono>` core, `<random>`, `<scoped_allocator>`, `<tuple>`,
  `<pair>`** — pure compile-time.
- **`<atomic>` / `<barrier>` / `<semaphore>` / `<latch>` / `<mutex>` /
  `<shared_mutex>` / `<condition_variable>` / `<stop_token>` /
  `<thread>`** — bodies are builtins or thin forwarders, no library
  code to share.
- **`<coroutine>` / `<source_location>`** — compiler intrinsics, no
  library code.

### 4.6 No shared work to factor

- **`<charconv>`** — per-arithmetic-type bodies are distinct
  algorithms.
- **`<random>`** — engine / distribution algorithms are mutually
  distinct.
- **`<array>` / `<bitset>` / `<span>` / `<string_view>`** — by
  construction, no allocation / lifecycle to share.
- **`<stack>` / `<queue>` / `<priority_queue>`** — thin forwarders.
- **`<chrono>` tzdb data** — data, not code.

### 4.7 Dead at this target

- **`<exception>` / `exception_ptr`** — `-fno-exceptions`.
- **`<typeinfo>` / `<typeindex>`** — `-fno-rtti`.
- **`<stdexcept>` bodies** — exist for ABI, not thrown.

### 4.8 Deferred (needs measurement)

- **`<algorithm>::sort` / `<numeric>` reductions** — `.text` win is
  real, runtime cost is real, gate is a runtime+size bench that doesn't
  exist.
- **`<regex>` walker** — promising shape, but the traits dispatch is
  inside the hot loop and the bench has no regex probe. Add the probe
  first; decide later.
- **`<format>` parser** — instantiated per `(CharT, formatter<T>)`;
  parser body could be shared via a typeless `parse_format_spec` core,
  but predicted saving is < 1 KB. Defer.
- **`<chrono>` formatter dispatch** — ride on `<format>`'s outcome.
- **Cross-subsystem `num_put` ↔ `<charconv>` ↔ `<format>` fusion** —
  worthwhile but about routing existing logic, not introducing a core.
- **`cross_ops`** — dead in tree (zero callers, confirmed); kept
  available, GC-dropped today.

---

## 5. The header map

```
  bits/vector_cores.h       (v5 — contiguous cores; rename to bits/cores.h
                             is a cosmetic cleanup, see §10)
  bits/type_ops.h           (v5 — element vocab; FROZEN)
  bits/storage_ops.h        (v5 — buffer vocab; FROZEN)
  bits/relocatable.h        (v5 — the one trait the leaves need; FROZEN)
  bits/segment_map_ops.h    (stage 4 — deque segment vocab)
  bits/value_ops.h          (stages 2,3 — any + function vocabs)
  bits/algo_cores.h         (stage 5 — equal_bytes, find_byte)
  bits/sum_ops.h            (stage 6 — variant lifecycle vocab)
  bits/sp_cb_ops.h          (stage 7 — shared_ptr control-block vocab)
  bits/future_cb_ops.h      (stage 9 — future shared-state vocab; contingent)
```

`src/support.cpp` grows from current ~660 lines to ~1500 lines by the
end of all stages. Each new core ships in its own `.text.fn_name`
section so `--gc-sections` drops unused ones.

---

## 6. The migration order

Sequenced so each stage has a bench probe **before** it lands. Stages
are independently revertable; each has a bench-claim, and a stage
whose actual delta misses prediction by >50% is reverted (per
`CLAUDE.md`'s "negative results are signal" rule).

**Stage 0 — probes.** Add the missing bench probes to
`bench/codesize.py`. Without these, the size claims in subsequent
stages cannot be honestly journal-recorded (critique C). Required:

- `variant_probe.cpp` — three flavors of
  `variant<int, string, double, vector<int>>`-shaped types.
- `shared_ptr_probe.cpp` — eight `T` flavors with `make_shared` /
  `shared_ptr(new T)` / `shared_ptr(new T[], default_delete<T[]>)`.
- `two_deque_probe.cpp` — two `deque<T>` flavors with `T1`
  trivially-relocatable and `T2` not; push_back / pop_front /
  `insert(pos, n, x)` / iterator traversal exercised.
- `regex_probe.cpp` — measurement-only baseline for v8.

Optional but recommended: `valarray_probe.cpp`,
`filesystem_probe.cpp`, `async_probe.cpp`, `algo_probe.cpp`.

**Stage 1 — freeze v5.** Two commits, each measured:
  - (a) Rename `bits/vector_cores.h` → `bits/cores.h` (cosmetic; no
    behavior change). Note (critique A1): the in-tree code compiles
    as-is; v5's "the current source doesn't compile without this" was
    wrong, presumably because the `debf4cd` removal it cited was
    reverted or never landed.
  - (b) Fix vector's `OP_COPY_CONSTRUCT` mask bug (§2.8 / critique B1).
    Own commit, journal-tagged as a correctness fix. Bench: parity.

**Stage 2 — `value_ops`, port `any`.** New `bits/value_ops.h`. Bench
claim: programs with > 3 distinct stored T see −200 B/T (~−1.2 KB on
tomlplusplus, ~−0.4 KB on fmt).

**Stage 3 — `callable_ops<Sig>`, port `function`.** Bench claim:
~−300 B per captured F (~−0.8 KB on json, ~−0.6 KB on tomlplusplus).

**Stage 4 — deque cores + `segment_map_ops`.** Convert deque's
`insert`/`erase`/destructor over. Bench claim: −1 to −3 KB on the
two-deque-flavor probe.

**Stage 5 — `equal_bytes` + `find_byte`.** Convert `std::find` /
`std::equal` / `vector::operator==` over. Bench claim: −0.3 to −0.7 KB
per program. If the measured delta is below 0.3 KB, leave the cores
defined-but-unused (eligible for `--gc-sections`) and revert the
call-site changes.

**Stage 6 — `sum_ops`, port `variant` lifecycle.** Bench claim:
−0.5 to −1.5 KB on programs with > 3 variant flavors.

**Stage 7 — `sp_cb_ops`, port `shared_ptr` control blocks.** Bench
claim: −1 to −3 KB on the shared_ptr probe.

**Stage 8 — `valarray_resize_exact` + `path_lexical_normal_bytes`.**
Cold corners, only ship if a downstream caller exists. Bench claim:
~−0.3 KB each, parity on programs that don't use them.

**Stage 9 — `future_callback_ops`.** Contingent on stage 7 landing
cleanly. Bench claim: ~−0.5 KB on async-heavy programs.

**Stage 10 — regex probe (measurement only).** Records the current
cost so v8 can decide on the walker port.

Explicitly NOT in this sequence (deferred to v8 or later):
`sort_bytes`, `<format>` parser core, chrono format dispatch core,
`<numeric>` runtime erasure, `<regex>` walker port,
`num_put`/`charconv`/`format` fusion.

Total predicted saving across the bench's eight programs is in the
5–15 KB range, mostly from stages 2/3/4/7.

---

## 7. The invariants (consolidated)

Carried forward; the list is closed at ten. A future agent breaking one
is breaking the contract; if they think they need to, that's a v8
question that requires a written argument.

**From the contiguous-storage layer:**

1. **Trivial-fast-path correctness** — both the trivial-X bit and
   `f_alloc_default_life` are required before a core takes a byte path.
2. **Lazy ctx fetch** — `el_ctx` fetched at most once per call, only when
   `f_alloc_ctx`.
3. **Byte-granular arithmetic** — pointer math in bytes, `tops->size` is
   the multiplier, no `T*` casts except inside leaves.
4. **Spare-slot ownership** — `storage_ops::set_size` and
   `storage_ops::resize` own the spare slot; cores never write it.
5. **One core call per modifier** — exception: `vdeallocate` crosses
   the two vocab tables.

**From the rest of the library:**

6. **Compile-time erasure is preferred when the modifier body has no
   branches on element triviality.** Node operations stay
   template-parameterized; vector/string/deque modifiers stay
   runtime-erased.
7. **A vocab table per axis.** `type_ops` describes a type;
   `storage_ops` describes a contiguous buffer; `segment_map_ops`
   describes a segmented buffer; `value_ops` describes a type-erased
   value; `callable_ops<Sig>` describes a callable; `sum_ops` describes
   a sum type; `sp_cb_ops` describes a shared-ptr control block. Each
   table answers one question; no fused slots across axes.
8. **The trivial fast path is bytes, always.** For every new core,
   there must exist a flag bit such that, when set, the body collapses
   to memcpy/memmove/memset/memcmp/skip/atomic-builtin. "Just inlines a
   typed loop" is not a trivial fast path.

**From the meta layer:**

9. **Vtables that already exist in the public ABI count as the vocab
   table.** Don't propose `streambuf_ops` or `facet_ops`. The cores
   question for those subsystems is whether the library's concrete
   subclasses share byte work with each other — typically they don't.
10. **Vtables internal to the library are fair game.** When the library
    ships a virtual hierarchy whose subclasses fold pairwise under
    COMDAT if the vtable is removed (`sp_counted_base`,
    `assoc_sub_state`), replacing the vtable with a vocab table is the
    sandwich. **This rule does not authorize growing v5-frozen tables**
    — `type_ops` and `storage_ops` are out of scope (clarification of
    critique D4). If you find you need a new slot in either, that's a
    v8 question.

---

## 8. The bench discipline

Per `CLAUDE.md`: "Size claims without a journal entry are not claims."

For every stage:

1. **Probe exists** in `bench/codesize.py` *before* the stage lands
   (stage 0).
2. **Journal entry** via `bench/record.py "desc"` after the change,
   `bench/record.py --diff` against the prior baseline.
3. **Numbers must be measured.** Not predicted, not estimated, not
   reasoned-about-from-IR. The journal records actual sizes.
4. **Predicted vs measured delta in the commit message.** If they
   diverge by > 50%, the stage is rejected.
5. **Negative results are signal.** A stage that fails to deliver
   isn't a bad commit — it's a documented "this didn't pay off, here's
   why" that protects the next agent. Don't spin.
6. **Per-stage isolation.** Don't fold stages; each stage's claim has
   to be checkable on its own.

---

## 9. Assumptions about the build target

- **`-fno-exceptions`** — cores trap on bad preconditions
  (`__builtin_trap`); allocation failure inside `sops->resize` traps.
  No `try`/`catch`; no exception strong-guarantee rollback.
- **`-fno-rtti`** — cores carry no `type_info`. Element identity is the
  `type_ops*` address. Vtable typeinfo slots are still emitted as null
  (the gcc reality, per critique D3); this is exactly why replacing the
  vtable with a vocab table is a net win on `shared_ptr`'s and
  `function`'s internal hierarchies.
- **gcc-10.2** — no `__builtin_bit_cast`, no `__type_pack_element`, no
  `[[likely]]`/`[[unlikely]]`. Cores use `__builtin_memcpy/memmove` for
  byte-walking. Compile clean at `-std=gnu++20 -fno-exceptions
  -fno-rtti -O0` and `-Os -flto`.

---

## 10. Corrections to the v5/v6/v7 notes (kept for the record)

Pulled from `notes/cores-critique.md`. The design above already
incorporates these; this section is the audit trail.

**A1.** v5 §8 step 2 claimed `src/support.cpp` doesn't compile because
`bits/vector_cores.h` was removed in `debf4cd`. False against today's
tree: the header exists (89 lines), `src/support.cpp:20` includes it,
the tree compiles. The rename to `bits/cores.h` is a cosmetic cleanup,
not a compile-state fix. Stage 1(a) above reflects this.

**A2.** v6 §2.4.3 claimed `basic_stringbuf<CharT>` overrides `xsputn` to
do shared byte work. False: this tree's `basic_stringbuf` defines only
`overflow`, `underflow`, `pbackfail`, `setbuf`, `seekoff`, `seekpos`,
`sync` — no `xsputn` override. The `sb_xsputn_bytes` core would have to
*introduce* duplication and then factor it, which is a different
proposal. Dropped from this design (§3.9 first bullet).

**A3.** v5 §10 claimed `set_size_elems_` is called at ~20 sites in
vector. Actual: 6 sites (lines 281, 290, 301, 309, 741, 749 per
critique). The open question — "should `set_size` be a sixth core" —
closes faster: no.

**B1.** Vector's `ops_table_()` mask is missing `OP_COPY_CONSTRUCT`.
Real bug, confirmed against `include/vector` lines 216-218 and 783.
Standalone commit in stage 1(b).

**C.** Multiple v6/v7 stages claim deltas against bench probes that
don't exist. Stage 0 above adds them before subsequent stages can ship.

**D1.** Spare-slot ownership claim verified against
`storage_ops.h::storage_{a,b}_{set_size,resize}` (lines 114, 188, 272,
336 — all memset the spare slot). Invariant 4 above carries a citation.

**D2.** `any`'s structure as v6 described it is correct (per
`include/any` 256-263). Footnote on call-overhead being slightly *lower*
than today's incorporated into §3.3.

**D3.** v6 §2.6's "vtable forces RTTI-like data" is loose; the precise
statement (typeinfo pointer slot emitted as null, vtable keyed on
concrete type) is in v7 §2.3.2 and §9 above.

**D4.** v7 §6's "vtables fair game" rule needed an explicit note that
it doesn't authorize growing v5-frozen tables. Invariant 10 above
includes that clarification.

**D5.** v6 §2.4 cited a deque block-size upper bound of `4096/sizeof(T)`
without re-deriving from the tree. The argument carries; the exact
constant comes from `<deque>` line 30's `deque_block_size` formula.

**E1.** v5 §6 said string "happens to bypass `construct_copy_one_n`" —
truth is string never calls it. Wording fixed in §2.6.

**E2.** v7 §2.7 said `<regex>` is "2000+ lines"; actual `wc -l` is
4977. Other size claims in v7 verified accurate.

**E3.** v5 §13 arithmetic ("10 distinct codegen shapes") is loose;
counting actual `support.cpp`, it's ~9 with rotate's leaf being
two-headed (forward and reverse block-walk). Doesn't change the
conclusion.

---

## 11. What this design is, and what it isn't

This is the design we implement against. The five contiguous-storage
cores are frozen as-is in the tree; everything else is a sequenced
proposal gated on a bench probe and a journal entry.

The design **is not** an implementation plan that can ship without
measurement. Every stage past stage 1 is a hypothesis; the bench is
the gate. Stages that fail to deliver are reverted, and the reversion
is documented so v8 doesn't relitigate.

The design **is** a written-down "yes this, no that" so the next
agent has a contract to argue against. Most of the library does not
need a core; §4 catalogues which families and why. The sandwich was
the right shape for contiguous storage; it carries to deque, the
type-erased wrappers, and a handful of cold corners. The rest of the
library has its own answer (compile-time fusion, public-ABI virtuals,
no shared work, dead-at-this-target), and the answer is "no core".

Read `CONVENTIONS.md` and `README.md` for build/test discipline;
`bench/code-size-techniques.md` for the codesize playbook; this file
for which slots to fill and which to leave alone.
