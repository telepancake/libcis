# Cores design v6 — extending the sandwich beyond contiguous storage

v5 froze the shape for `vector` / `string` (and any future contiguous-storage
container): five non-template byte-level cores in `src/support.cpp`, gated by
two vocabulary tables (`type_ops` for the element, `storage_ops` for the
buffer). v6 carries the same factoring through the rest of the library — the
node-based containers, the segmented container, the type-erased wrappers, and
the algorithm helpers — and writes down what does and does not get its own
core there.

Scope: every non-contiguous container in `include/` (`<list>`,
`<forward_list>`, `<map>`/`<set>`, `<unordered_map>`/`<unordered_set>`,
`<deque>`), the type-erased value wrappers (`<any>`, `<functional>`'s
`std::function`, `<exception>`'s `exception_ptr` and friends), and the
algorithm-shaped opportunities the v3/v4 trial table never visited
(`std::sort`, `std::find`, the parallel-algorithm dispatchers if/when they
land). No bench numbers yet — this document is the design that the next
agent measures *against*.

This is **design only**. No code is changed.

---

## 0. TL;DR — the shape of the rest of the library

```
  user header           thin, type-aware
  ────────────────────────────────────────
  vocab table(s)        const X_ops* — what the type *can do*
  ────────────────────────────────────────
  cores                 non-template, in src/support.cpp
                        bytes-and-vocab-pointers in, bytes-and-vocab-pointers out
```

The five contiguous-storage cores are the prototype, not the upper limit.
This document is a parallel listing of "for each remaining family of
containers/wrappers, here is the same sandwich". For each family we name:

- **The element/value vocabulary** it needs (which subset of `type_ops`'s
  leaves, plus any family-specific ones that need to be added or split out).
- **The structural vocabulary** that plays the role of `storage_ops` — the
  thing that hides "how the per-instance state is laid out" from the typeless
  core. Most node-based containers don't need a runtime table here because
  the structure is already type-erased at compile time via the void-pointer
  rebind trick (see §2); but a few do (deque, function/any).
- **The cores themselves** — by name, by shape, by what the modifier-table
  collapses to.

Six families, summarized:

| family | container(s) | how big | cores in v6 |
|---|---|---|---|
| node-list  | `list`, `forward_list` | small | 0 new (struct already type-erased) |
| node-tree  | `map`, `set`, `multi*` | medium | 0 new (`bits/tree.h` already erased on `NP`) |
| node-hash  | `unordered_{map,set,multi*}` | medium | 0 new (`bits/hash_table.h` already erased on `NP`) |
| segmented  | `deque` | large | 3 new (segment-map vocab + segmented_copy/move/destroy) |
| value-erased | `any`, `function` | small | 1 each, but they replace the per-T vtable |
| algorithm-erased | `sort`, `find`, `equal`, copy/move loops | medium | 4 candidates — gated on bench |

Most of the answer is "do nothing": the node-based code already has the right
shape (compile-time type erasure inside an already-non-allocating header).
The honest work is in `deque` and in trimming the redundant Action-enum / vtable
dispatch out of `any` and `function`.

---

## 1. The rule that picks the set (carried from v5)

> **Every modifier resolves to one core call (or none).** If a modifier needs
> two cores, the cores are wrong.

v5 added the side rule: a core earns its keep only when it is **called from
≥ 2 containers** *or* its trivial fast path collapses to a builtin (`memcpy`,
`memset`, `memmove`, skip) that the linker can dedup across all consumers.
A "core" that fires once and only contains a switch over `tops->flags` is an
inline forwarder in disguise; leave it in the header.

v6 adds one more rule, learned from looking at the rest of the tree:

> **If the type erasure is already done at compile time (void-pointer rebind,
> CRTP base, struct-of-pointers chained by `next_pointer`) and the resulting
> code is non-template-but-instantiation-shared across `T` for a fixed `Alloc`,
> there is no core to write.** The compile-time pattern is the core.

This is why §2.1 / §2.2 / §2.3 each conclude "no new cores". The node-list,
node-tree, and node-hash code is already structured so that the rotation /
balancing / chaining algorithms work on `node_base_pointer`s — they live in
`bits/tree.h` and `bits/hash_table.h` and are instantiated *once per
`pointer_traits<VoidPtr>::rebind<node_base>`*, not once per `(T, Alloc)`.
Pulling them through `void*` and a `node_ops` vocab table would (a) lose the
typed-pointer-arithmetic that the compiler already does for free on
`base_pointer next_`, and (b) gain an indirect call on every `next` chase
that today is a single `mov`. The bench would punish this.

So v6 does **not** propose `tree_insert_core`, `ht_rehash_core`,
`list_splice_core`. Those algorithms are already shared — the sharing is just
template-shaped, not function-pointer-shaped — and the trial table from v4
already showed that swapping template-sharing for function-pointer-sharing
costs bytes when the templated body is already small (the linker dedups
identical bodies under COMDAT, and the indirect-call decoration around the
swap regresses callsite size).

The work is at the boundaries: deque (which is none of the above), any (whose
type erasure is hand-rolled and pays for it), and function (likewise).

---

## 2. Walking the families

### 2.1 `list` / `forward_list` — node-list, no new cores

**Current shape.** `bits/list_node` (in `<list>`) and `bits/fl_node` (in
`<forward_list>`) are struct templates parameterized by `T` and `VoidPtr`.
The link-walking code (`splice`, `merge`, `reverse`, `sort`) lives inline in
the user header and operates on `base_pointer` — a `pointer_traits<VoidPtr>::
rebind<list_node_base<T, VoidPtr>>`. The structural code is already shared
across containers of the same `(T, Alloc)`; T leaks in only via
`list_node::value`.

**Why no new core.** Two reasons:

1. **The "byte path" doesn't exist.** Unlike vector/string, list doesn't have
   contiguous bytes to memcpy. Every modifier is a constant number of
   pointer assignments per element; there is no fast-path-vs-slow-path
   decision to make. `triv_destroy` doesn't unlock anything — list still has
   to walk every node to call `free`. So the vocabulary that earns its keep
   in `vector` (the `flags & f_triv_X` branches) gets you nothing here.
2. **Type erasure is already at the right altitude.** `list_node_base<T,
   VoidPtr>` is parameterized on `T` for the type-system's benefit (so the
   `as_node()` cast is type-checked), but the struct layout is `{prev, next}`
   regardless of `T`. The link operations the compiler emits for `prev =
   p->next` are the same instructions for every `T`. COMDAT folds them. A
   typeless `list_link` struct with `void* prev, *next` and a separate
   `list_node<T>` holding the value would let us reuse one body across
   `(T, Alloc)` pairs — but **only** if the user's `Alloc::pointer` is a raw
   pointer. With fancy pointers (`pointer_traits<VoidPtr>::rebind<...>`),
   each fancy-pointer flavor needs its own arithmetic, and the typeless
   walker can't express "rebind through a fancy pointer". libc++ ships the
   compile-time-parameterized version for the same reason; libcis follows.

**What v6 commits to.** Status quo. The list headers stay as they are. If a
future bench shows that the de-virtualization-of-fancy-pointer-traits costs
measurable `.text` per `(T, Alloc)` instantiation, the right move is to
introduce a `node_link_ops` *compile-time* parameter (a `struct` with
`base_pointer prev_of(np)` static members) and a single typeless walker
inside `bits/list_link.h`. That is **not** a runtime vocab table; it is a
template-parameterized helper that still gets COMDAT-folded across `T`. The
distinction matters: v6's cores have function pointers, this would not.

**Things `list`/`forward_list` will still want from `type_ops`.** Just
`size`, `align`, `destroy`, and the allocator-mediated lifecycle leaves.
The current code calls allocator_traits directly; the v5 `type_ops` already
threads through `allocator_traits::construct/destroy` at the leaves and
exposes the `f_alloc_default_life` flag. If list ever needs the "memset
all-trivially-default-constructible elements" path (it won't, since you
construct one node at a time as you push), we'd revisit; for now, no.

### 2.2 `map` / `set` / `multimap` / `multiset` — node-tree, no new cores

**Current shape.** `bits/tree.h` (1143 lines) holds the red-black-tree
structural algorithms templated on `NP` (= node_base_ptr): `tree_min`,
`tree_max`, `tree_left_rotate`, `tree_right_rotate`,
`tree_balance_after_insert`, `tree_remove`, the in-order iterator stepping
`tree_next_iter` / `tree_prev_iter`. The body of `tree_remove` is 200 lines
of pointer surgery that does **no** per-element work — no destroy, no
construct. The value type `T` only enters through the leaf `rb_node<T,
VoidPtr>::value` and is constructed by the user-facing `map::emplace` /
`set::insert` before the structural code runs.

**Why no new core.** Same as list — and stronger, because the body of
`tree_remove` is exactly the size of the body of a runtime-dispatched
equivalent, *minus* the indirect-call decoration around every
`x->parent_unsafe()`. The compile-time template version emits direct loads;
the function-pointer version would emit `call *[node_ops+offset]` for each
link chase. On i386, every one of those is +4 B at the callsite. There are
~30 link-chase sites in `tree_remove` alone. That's +120 B per `tree_remove`
instantiation, and `tree_remove` is already instantiated once per
`pointer_traits<VoidPtr>::rebind<rb_node_base>` — so for `std::allocator`
flavors it's instantiated **once** for the entire library. The function-
pointer version costs ~120 B to gain nothing; the template version is free.

**A small genuine question: hand-rolled key comparator on a runtime vocab?**
`map<K, V, Comp>::find` has Comp as a template parameter, so the comparison
emits inline. If we replaced `Comp` with a runtime `int (*)(const void*,
const void*)` slot (cf. `type_ops::compare`), all map flavors with the same
`K` could share one `find` body. The cost: an indirect call per descent (so
log N indirect calls per find), plus a runtime that doesn't constant-fold
`Comp` when it's `less<K>`. The bench's `map<int>` lookup goes from
"compare, branch, recur" (3 instr) to "indirect call, compare, branch,
recur" (5+ instr per step). For the depths trees actually reach (< 20)
that's measurable in CPU but not in `.text` — and `.text` is what the cores
are for. **No core**: `map`'s `Comp` template parameter stays.

**What `type_ops` would need if any tree code went runtime-erased.**
`compare` slot (already present), `destroy` slot (already present),
`move_construct` for the rare "extract / merge" case (already present).
Nothing missing. So even the speculative version would not grow `type_ops`.

### 2.3 `unordered_map` / `unordered_set` and friends — node-hash, no new cores

**Current shape.** `bits/hash_table.h` (1801 lines) holds the bucket vector,
the chained-list-of-nodes-per-bucket plumbing, rehashing, `next_prime`,
`constrain_hash`. The bucket vector is itself a `vector<next_pointer,
rebind_alloc>` — which means **the unordered containers already consume the
contiguous-storage cores** transitively, through their bucket vector.
The chain-walking inside `find` / `insert` / `erase` operates on
`next_pointer` and is template-instantiated once per `(T, VoidPtr)`.

**Why no new core.** Same reasoning as the tree, plus: the bucket-vector
side is **already in the sandwich**. v5 covers it. The only thing the v5
cores don't cover is the chain-walk, and the chain-walk is structurally the
same as the list-walk in §2.1: typeless link arithmetic that COMDAT folds
across `T`.

**One nit worth recording: `Hash` and `KeyEqual` as runtime vocab.** Same
argument as `Comp` for the tree: replacing them with `type_ops::hash` /
`type_ops::equal` slots (already in the table!) would let `unordered_set<int>`
and `unordered_set<long>` share more code if and only if their underlying
`std::hash` specializations are byte-identical (they aren't — `std::hash<int>`
is a no-op cast, `std::hash<long>` is the same on LP64 but not on ILP32).
The opportunity is real but small, and the function-pointer overhead per
lookup is worse than the tree case (one indirect call per probe step, in a
hot path users complain about). **No core**: stays template-parameterized.

**Where `type_ops::hash` / `type_ops::equal` *do* earn their keep.** The
`extract` / `merge` node-handle code that has to compare keys at runtime
across two different unordered containers (with potentially different
`KeyEqual`). That code today writes `c1.key_eq()(k, k2)` and would need
two-table dispatch (a cross_ops with hash+equal) to do it through vocab.
Not on the critical path; deferred.

### 2.4 `deque` — segmented; three new cores

**Current shape.** `<deque>` (1649 lines). The container is a `deque_map`
(itself a small vector of segment pointers) plus a per-segment buffer of
size `deque_block_size<T>` elements. Modifiers walk the segment map and,
within each segment, do contiguous-buffer work. `push_back` is "if last
segment full, allocate a new segment; else placement-new at the back".
`insert(pos, first, last)` is the expensive one: it has to rotate live
elements across segment boundaries.

**Why deque is different.** The single-segment case is the v5 sandwich —
each segment is a fixed-capacity contiguous buffer of `T`. v5 §6 already
calls this out: deque's segment uses a `storage_ops` flavor where `resize`
traps (segments don't grow), `set_size` is a field update, `free` returns
the segment to a pool, `cap_end` is `begin + BlockSize*sizeof(T)`. That
shape is already covered.

The new work is the **cross-segment** operations:

- **`segmented_destroy(tops, sops_map, st_ctx, first_iter, last_iter)`** —
  destroy all elements in `[first, last)`, walking segments. Replaces the
  current double-loop (outer over segments, inner over `destroy_range`
  within a segment) with one core. Trivial fast path: skip whole segments
  when `f_triv_destroy & f_alloc_default_life`.
- **`segmented_copy(tops, sops_map_src, sops_map_dst, src_ctx, dst_ctx,
  src_first, src_last, dst_first)`** — move-construct (or copy-construct,
  see below) a range from one deque's segments into another's. The trivial
  fast path is segment-aligned `memcpy` runs.
- **`segmented_rotate(tops, sops_map, st_ctx, first, middle, last,
  size_delta)`** — the segment-aware version of v5's `rotate`. This is the
  one whose hand-rolled-per-deque version is hairy: it has to maintain the
  v5 `rotate`'s contract (rearrange `[first, last)` so `*middle` becomes
  the new `*first`) while crossing segments where the linear "first +
  i*es" arithmetic breaks. The simplest implementation is `gather to
  scratch + scatter back`, segment-aligned; for the trivial-relocatable
  fast path the gather/scatter degenerate to per-segment `memcpy` runs.

**A fourth that's tempting but rejected.** `segmented_construct_at_back(n,
src)` for `assign(n, x)`. Same reasoning as v5's `construct_copy_one_n` —
the typed `for (i = 0; i < n; ++i) construct_one(...)` loop with `if
constexpr` inlining produces tighter code than the typeless core for the
deque-segment-by-segment case, where the per-segment count is small (block
size for `int` is 1024 elements; for `string`, 16). The trivial-fast-path
collapse to memcpy only wins when the count *per segment* is large, and
deque's segment size is bounded above by `4096/sizeof(T)`. The opportunity
is small; the symbol cost is real (every program using `deque` would pull
in `segmented_construct_at_back` even when it never calls `assign`). Keep
the typed loop.

**Shape of the new vocab table.** Three new cores need to walk segments,
which means they need a runtime vocab for "give me the segment pointer at
deque-iterator position P". That is exactly what `deque_map` already does,
but it's currently a typed `vector<segment_ptr>`. To run a typeless core
over it we need a `segment_map_ops` table:

```cpp
struct segment_map_ops {
    // Given (deque_st_ctx, byte_index), return the segment that contains
    // element [byte_index / block_bytes] and the byte offset within it.
    byte_span (*locate)(void* st_ctx, size_t byte_index);
    // Total live element count in bytes.
    size_t    (*size_bytes)(void* st_ctx);
    // Step to the next segment after the one at `seg`. Returns nullptr at end.
    void*     (*next_segment)(void* st_ctx, void* seg);
    // Append/peel-back segments around modifiers (rotate-with-size-delta).
    void      (*push_back_segment)(void* st_ctx);
    void      (*pop_back_segment)(void* st_ctx);
};
```

Five slots, same shape as `storage_ops`. Lives in `bits/segment_map_ops.h`
(new). The deque container picks the instance via `pick_segment_map_for<C>`,
the same template-flavor as v5's `pick_storage_for`. Strategy A (typed
deque_map): one specialization. There is no Strategy B today and probably
never will — deque has no `malloc_usable_size` shortcut.

**Why this is worth it (deque specifically) when it wasn't for list/tree.**
deque modifiers are large. `insert(pos, first, last)` in libc++ is ~200
lines, branchy, and per-(T, Alloc) instantiated. By contrast,
`tree_remove` is large but instantiated once per `(VoidPtr-rebind)`.
deque's instantiation count × size is the dominant `.text` contributor
when a program uses two `deque`s of different `T`. Pulling the cross-segment
algorithms into three typeless cores (gated on the trivial-relocatable +
default-lifecycle fast path) buys back exactly this multiplication. The
v3/v4 trial table didn't measure deque because deque wasn't in the bench;
v6 says "measure deque before landing this", and the predicted win is in
the 1–3 KB/program range for any program using two `deque` flavors.

**Migration path.** Land deque-as-it-stands first; ship the three cores in
`src/support.cpp` second, and switch deque's modifiers over in a third
commit. The intermediate state (typed deque calling typed segment ops) is
correct; the typeless version is a code-size optimization. If the bench
shows the saving is < 500 B/program, revert the third commit and leave
the cores defined-but-unused (eligible for `--gc-sections`).

### 2.5 `any` — replace per-T handler with a `value_ops` vocab table

**Current shape.** `any` stores either an inline 3-pointer buffer or a heap
pointer, plus a per-T function pointer (`HandleFuncPtr h_`) that dispatches
on an `Action` enum (Destroy / Copy / Move / Get). For every value type the
user stores in an `any`, the compiler instantiates either `SmallHandler<T>`
or `LargeHandler<T>` — each a switch over `Action` with four arms.

**The redundancy.** That switch is exactly the slots of `type_ops` shifted
into an enum, plus a layout discriminator (small vs large). Each
instantiation:

- ~200 B of switch table + four arm bodies per T (i386 measured shape).
- One fast-path arm (Get → compare ID, return ptr) and three lifecycle
  arms (each calls into the leaf for that T).

**The proposed core.** Replace `HandleFuncPtr` with `const value_ops* v_` and
a discriminator bit packed into the low bit of `v_` (vocab tables are
aligned at >= 2 bytes, so the bit is free):

```cpp
struct value_ops {
    size_t size;
    size_t align;
    unsigned flags;     // f_triv_destroy, f_triv_copy, f_alloc_default_life,
                        //   f_is_small (inline-fits-in-3-ptrs)
    void (*destroy)(void* p);
    void (*copy_construct)(void* dst, const void* src);
    void (*move_construct)(void* dst, void* src);
    const void* type_id;  // address of unique_typeinfo<T>::id (already in any)
};
```

This is `type_ops` minus the allocator threading (`any` is allocator-free)
minus the value ops (`any` doesn't compare/hash/swap-elementwise), plus the
type_id slot already living in `unique_typeinfo<T>::id`. Five fields, one
indirect-call slot per lifecycle op, plus the trivial-fast-path flag.

The single dispatch function that `any` calls becomes one of two cores
(small vs large, picked by `f_is_small` bit):

```cpp
void any_op(value_ops::Action act, any* self, any* other, const void* id);
//                  ^^^^^ replaced by direct slot calls inside this core
```

Actually no — the cleaner shape is **don't have one entry point**. The five
`any` modifiers (`reset`, copy ctor, move ctor, swap, `any_cast` ptr form)
each call `v_->destroy(p)`, `v_->copy_construct(d, s)`, etc. directly. The
Action-enum switch was libc++'s workaround for needing one symbol per T;
with the vocab table the symbols are pre-shared.

**The win.** Two effects:

1. **Per-T instantiation cost drops to one `value_ops` constant** (~40 B
   constant + 4 leaf function bodies that COMDAT-fold across T pairs with
   the same triviality flags). The SmallHandler/LargeHandler switch
   bodies (~200 B each per T) go away.
2. **The trivial fast path becomes one branch in `any_cast` and `reset`**
   instead of being baked into the leaf. For `any` holding a trivially-
   copyable T, copy/move/destroy collapse to memcpy/memcpy/skip at the
   core.

**The cost.** Five function-pointer slots per `value_ops` instance. The
hand-rolled switch fits in one 8-byte function pointer; the vocab table is
~56 B per T. Break-even is at ~3 instantiations; programs using `any` for
more than three types win, programs using it for one type lose. The bench
projects a net win for any program that uses `any` in earnest (libfmt's
formatter registry, json's variant-ish stores).

**Why this isn't `type_ops`.** Three differences:

- No allocator threading. `any` is `new`/`delete` (or placement-new into
  the inline buffer), not allocator-mediated.
- Needs `f_is_small` (layout discriminator), which is meaningless for
  containers.
- Needs `type_id` (the comparable per-T tag), which `type_ops` doesn't
  carry — and shouldn't, because containers identify their element type by
  `&ops_for<T,A>` already, but `any_cast` has to compare against an
  externally-known `T` whose `type_ops*` for the same `T` with a default
  allocator might or might not be the one instantiated for the stored value.

So `value_ops` is a sibling table to `type_ops`, not a subset.

### 2.6 `function` — same shape as `any`, plus a call slot

**Current shape.** `value_func<R(Args...)>` holds a 3-pointer inline buffer
plus a `fn_base<R(Args...)>*` pointer to a virtual-base class with four
virtuals: `clone(void)`, `clone(into)`, `destroy()`, `destroy_deallocate()`,
plus `operator()`. Every captured callable type instantiates one
`fn_func<F, R(Args...)>` derived class with the vtable.

**The redundancy.** Same as `any`, plus the call. The vtable is exactly a
`value_ops` plus one `R (*invoke)(void*, Args&&...)` slot. The vtable
overhead per `function<Sig>` instantiation × per captured `F` is sizable,
and the **vtable forces RTTI-like data**: gcc-10 emits one vtable symbol
per `(F, Sig)` pair, each carrying typeinfo unless you're at `-fno-rtti`
(libcis is, so the typeinfo slot is null, but the vtable itself stays).

**The proposed core.** Mirror §2.5: a `callable_ops<Sig>` vocab table per
signature, embedded into `value_func` instead of a vtable pointer. The
signature parameterization is unavoidable (the call slot's signature has
to match `Sig`), so this is a *family of vocab tables* indexed by `Sig` —
one per `function<Sig>` instantiation in the program.

```cpp
template<class Sig> struct callable_ops;  // primary undefined
template<class R, class... A> struct callable_ops<R(A...)> {
    unsigned flags;        // f_triv_*, f_is_small
    size_t   size, align;
    void   (*destroy)(void*);
    void   (*copy_construct)(void* dst, const void* src);
    void   (*move_construct)(void* dst, void* src);
    R      (*invoke)(void*, A&&...);   // the only Sig-shaped slot
};
```

The call slot is signature-shaped (it has to be). That means the vocab
table for `function<int(int)>` is a different type from the one for
`function<void()>`, and the two share no code. But within a signature,
every captured `F` produces one `callable_ops<Sig>` *constant* (no body to
share) plus four COMDAT-folded leaf bodies (`destroy`, `copy_construct`,
`move_construct`, `invoke`). The folding cross-`F` happens whenever the
triviality flags match, and `invoke` folds whenever the call-shape is the
same (e.g. function pointers with the same signature, lambdas with the
same call body).

**The wins are the same as `any`'s.** Per-`F` cost drops to one
`callable_ops<Sig>` constant; the vtable + four virtual bodies go away.
The `value_func` swap dance (the 4-arm `this_inline / other_inline` cross)
moves into one core in `src/support.cpp`:

```cpp
void callable_swap(/* sig-shaped... */);  // taking ops table + two value_funcs
```

This `callable_swap` is signature-shaped (because the swap has to
move-and-call `invoke` to relocate to a temporary). So it's a template
instantiated once per signature — *not* a non-template core. v6 admits
this honestly: not every callable-op becomes a non-template core.
Lifecycle ops (destroy/copy/move) do; signature-shaped ops (invoke, swap-
that-needs-temporary-relocate) stay template-instantiated, but the
template body is small (~30 lines) and folds across `F` for fixed `Sig`.

**Where the cores layer ends and the templates begin** for `function`: the
boundary is the call signature. Below it (lifecycle), everything is
typeless and lives in `src/support.cpp`. Above it (invoke, swap with a
sig-shaped temporary), one template per signature, folded across captured
`F`. This is the same factoring as containers: below `type_ops`, byte
work; above, the user's `T*`.

### 2.7 `exception_ptr` and friends — out of scope

libcis runs with `-fno-exceptions`. `exception_ptr`, `make_exception_ptr`,
`current_exception`, `rethrow_exception` are dead code at this target. v6
does not propose cores for them; if libcis ever supports exceptions on a
sibling target, revisit.

### 2.8 Algorithm-erased cores — four candidates, gated on bench

The contiguous-storage cores cover container *modifiers*. They don't cover
the *algorithms* in `<algorithm>` (which is 6210 lines, by far the biggest
header). Today every `std::sort(begin, end)`, `std::find`, `std::equal`,
`std::copy` is fully template-parameterized on iterator type and predicate.
For random-access iterators over contiguous storage with trivially-
comparable / trivially-copyable element types, almost all of these collapse
to memcmp / memcpy / a tight loop — but **once per (Iter, Pred) pair**, not
once per element type.

**Candidates** (the "algorithm cores" set; not committed to without
measurement):

- **`equal_bytes(tops, a, b, n_bytes)`** — bool equality of two byte ranges
  via memcmp or per-element `tops->equal`. Replaces `std::equal` and
  `operator==(vector, vector)` for the trivial-equality case (most numeric
  types). Calls today's `tops->equal`, which is already in the table.
- **`find_byte(tops, begin, end, needle_ptr)`** — first-match search for
  one needle. Memchr fast path for char-sized trivially-equality T;
  else a `tops->equal` loop.
- **`copy_bytes(tops, dst, src, n_bytes)`** — typed memcpy. Already
  approximately what `vector`'s copy ctor invokes through
  `construct_copy_one_n`, but the algorithm-side caller (`std::copy(it,
  end, out)`) currently re-instantiates a typed loop per iterator pair.
- **`sort_bytes(tops, begin, end_bytes, compare)`** — the big one. libc++'s
  introsort is ~800 lines and is per-(Iter, Compare) instantiated. A
  byte-typed introsort taking `tops->compare` and `tops->size` would be one
  body in `src/support.cpp`. The trivial fast path is qsort_r over a
  memcpy-relocatable T.

**The reservation.** `std::sort` taking a runtime `compare` slot is exactly
the design qsort_r already has, and `qsort_r` is famously slower than
`std::sort` because every comparison goes through an indirect call. For
`vector<int>`'s `sort`, the inlined `<` per comparison is the difference
between ~5 cycles and ~10 cycles per swap-decision, which on a million-
element sort is the difference between 5 ms and 10 ms. **That's runtime,
not `.text`.** The `.text` saving is real (libc++'s template-instantiated
introsort is ~3 KB per `(T, Compare)` pair; the typeless one would be
~3 KB shared). The trade is the same shape as the deque trade in §2.4:
acceptable for code paths where size matters and runtime doesn't (cold
init, error-handling), unacceptable for hot loops.

**v6 commits to two of the four.** `equal_bytes` and `find_byte` are
small (~50 B body each), are called from per-container `operator==` and
per-algorithm `std::find`/`std::equal`, and have memcmp/memchr fast paths
that the compiler can't reach from the typed version (because the trait
gate is per-template-instance). They earn their keep. Land them in
`src/support.cpp` as a third group of cores (alongside the v5 five and the
deque three).

`copy_bytes` is rejected: it's exactly `construct_copy_one_n` minus the
broadcast, and `construct_copy_one_n` already specializes to memcpy when
the source is one element. For an iterator-pair copy with mixed iterator
shapes, the typed loop's `*it++` inlines tighter than the typeless `tops->
copy_construct` indirect call. Same logic as v5 §3.4. Keep it inline.

`sort_bytes` is **deferred**, not rejected. The bench needs to compare
`vector<int>::sort` runtime + `.text` against a typeless introsort version
before this lands. If two-out-of-three-target-arches' bench shows the
`.text` win exceeds 1 KB per `(T, Compare)` pair *and* the runtime
regression is < 50%, ship it. Else, it stays in the header.

**The fourth.** `lexicographical_compare_bytes` — same logic as
`equal_bytes`, used by `vector<T>::operator<=>` and `string::compare`.
String already has its own typed `traits_type::compare` fast path for
`char`, so the saving there is "vector only". Borderline; bench it
alongside `equal_bytes` and decide.

---

## 3. Where things live (the header map)

Adding to v5's header map:

```
  bits/cores.h         (decls — vector/string/deque, see v5)
  bits/segment_map_ops.h  (NEW — segmented-storage vocab for deque)
  bits/value_ops.h     (NEW — any/function lifecycle vocab)
  bits/algo_cores.h    (NEW — equal_bytes, find_byte, eventually sort_bytes)
```

`src/support.cpp` grows from ~660 lines to ~1100. Two new core groups:

```cpp
// (already present, v5)
void grow(...);
void destroy_range(...);
void construct_copy_one_n(...);
void rotate(...);
void relocate_live(...);

// (new in v6, deque)
void segmented_destroy(...);
void segmented_copy(...);
void segmented_rotate(...);

// (new in v6, algorithms)
bool equal_bytes(const type_ops*, const void* a, const void* b, size_t n_bytes);
const void* find_byte(const type_ops*, const void* begin, const void* end,
                      const void* needle);
```

Each new core ships in its own `.text.fn_name` section so `--gc-sections`
drops it for programs that never call it.

The single-segment `any` / `function` lifecycle cores **don't go in
src/support.cpp** — they live in the user headers because they're
sig-shaped (function) or because they're tiny (any). What's typeless about
them is the **vocab table**, which lives in `bits/value_ops.h`.

---

## 4. Migration order (the recommended commit sequence)

Stage v6 in five commits, each independently measurable on the bench:

1. **Rename + freeze v5.** Land the file rename + cores.h + the vector
   `OP_COPY_CONSTRUCT` mask fix from v5 §8. Bench: parity.
2. **Add `value_ops`, port `any`.** New `bits/value_ops.h`; convert `any`
   from `HandleFuncPtr` + `Action` enum to a `value_ops*`. Bench: expect
   `any.cpp`-using programs to drop ~200 B per stored T.
3. **Port `function` to `callable_ops<Sig>`.** Same change as `any`, but
   per-signature. Bench: expect ~300 B per captured `F` saving.
4. **Add deque cores + `segment_map_ops`.** Three cores +
   `bits/segment_map_ops.h`; convert `deque::insert` / `deque::erase` /
   `deque::~deque` over. Bench: expect 1–3 KB/program for multi-deque-flavor
   programs.
5. **Add `equal_bytes` + `find_byte`.** Two cores in `bits/algo_cores.h`;
   convert `std::find` / `std::equal` / `vector::operator==` over. Bench:
   expect ~500 B/program; if not, revert this commit and leave the cores
   `--gc-section`-dropped.

Each stage is reverable on its own. Each stage has a bench-claim; if the
claim doesn't hold (within 20% — see v5 §13), the stage is rejected, not
papered over.

The `sort_bytes` stage is intentionally not in this list. It needs a bench
that measures runtime as well as size, which the codesize bench doesn't.
Defer until that bench exists.

---

## 5. What this design does **not** propose

A live list of "looked at it, decided no", mirroring v5 §3:

### 5.1 No `node_ops` runtime vocab table

The compile-time `(T, VoidPtr)` parameterization of list/tree/hash nodes is
already the right factoring. See §2.1–§2.3. A future bench-driven case
might add a *compile-time* `node_link_ops` helper for the fancy-pointer
case, but it would not be a runtime vocab.

### 5.2 No `tree_remove` / `tree_balance_after_insert` cores

See §2.2. Function-pointer-ization of the link chases costs more bytes than
it saves; COMDAT-folding the templated body is already the optimum.

### 5.3 No `Comp` / `Hash` / `KeyEqual` slots in `type_ops`

The slots `compare` / `equal` / `hash` already exist (§4 of v5). What v6
declines is *routing the container's `Comp` template parameter through
them*. Doing so would replace one inlined `<` with an indirect call per
descent step; the `.text` saving is tiny, the runtime cost is large.

### 5.4 No allocator threading in `value_ops`

`any` and `function` are allocator-free in the C++17+ API (the
allocator-extended constructors were removed). The `value_ops` table does
not carry `f_alloc_ctx` / `f_alloc_default_life`; allocation goes through
`::operator new` / `::operator delete` directly. If a future libcis
revives the allocator-extended ctors, revisit.

### 5.5 No `sort_bytes` until the bench measures runtime

See §2.8. The size win is real but the runtime cost is too. Defer.

### 5.6 No `exception_ptr` / `nested_exception` cores

See §2.7. The whole subsystem is dead at this target.

### 5.7 No per-signature unification of `function<Sig>`

Tempting: make `function<int(int)>` and `function<int(long)>` share a
typeless body that takes args through a `void* arg_pack`. Don't. The
signature *is* the type; type-erasing it loses the whole point of
`function`'s API (the inlining of the call site through the templated
operator()). What v6 erases is the *captured* type `F`, not the signature.

### 5.8 No "associative cores" via cross_ops

`cross_ops` (v4-era) is currently dead in the tree (no callers). The
proposal "type-erase `map::find(K2)` heterogenous lookup through cross_ops"
is rejected on the same grounds as §5.3 — inlining the templated `Comp` is
the right shape for hot lookups. cross_ops stays available for `any_cast`-
like conversions if a future caller arises; for now, it's dead code that
`--gc-sections` drops.

---

## 6. The invariants v6 adds

Carried from v5 §12, with three more:

- **Compile-time erasure is preferred when the modifier body has no
  branches on element triviality.** Node operations (list, tree, hash) do
  no per-element work and stay template-parameterized. Vector/string/deque
  modifiers branch on `f_triv_*` and stay runtime-erased.
- **A vocab table per axis.** `type_ops` describes a type (size, leaves).
  `storage_ops` describes a contiguous buffer. `segment_map_ops` describes
  a segmented buffer. `value_ops` describes a type-erased value (any-style).
  Each table answers exactly one question; no table reaches across axes
  ("storage" vs "type") through a fused slot.
- **The trivial fast path is bytes, always.** For every new core in v6,
  there must exist a flag bit in the table such that, when set, the core
  body collapses to `memcpy` / `memmove` / `memset` / `memcmp` / skip.
  No fast path that "just inlines a typed loop" is permitted — that means
  the candidate doesn't earn its symbol; leave it in the header.

---

## 7. Bench claims (the numbers v6 commits to)

Per stage in §4. Stated against the v5 baseline (`bench/record.py` entry
"v5-freeze"):

| stage | program shape | predicted `.text` delta |
|---|---|---|
| 1 | any | parity (cosmetic) |
| 2 | tomlplusplus (uses `any` for variant) | −1.2 KB |
| 2 | fmt (uses `any` in format-arg store) | −0.4 KB |
| 3 | json (uses `function` for callbacks) | −0.8 KB |
| 3 | tomlplusplus (uses `function`) | −0.6 KB |
| 4 | vector-probe (no `deque`) | parity |
| 4 | two-deque-flavor probe (new) | −2.1 KB |
| 5 | every program with `<algorithm>` | −0.3 to −0.7 KB |

Numbers are predictions, not measurements. The recorded entry that lands
each stage carries the actual measurement and the diff against the stage
above. A stage whose actual delta misses prediction by more than 50% is
not a clean stage and gets reverted (per CLAUDE.md's "negative results are
signal" rule).

The "two-deque-flavor probe" doesn't exist yet; it needs to be added to
`bench/codesize.py` before stage 4. The probe shape: two `deque<T>`
flavors with `T1` trivially-relocatable and `T2` not, with
push_back / pop_front / `insert(pos, n, x)` / iterator traversal exercised.

---

## 8. The asks for the next agent

If you land any stage of v6, before you commit:

1. **State which stage you're on**, and what the predicted vs measured
   `.text` delta is. The bench journal entry has the measurement; the
   commit message has the diff against prediction.
2. **Don't expand `type_ops`.** Stage 2/3 add `value_ops`. Stage 4 adds
   `segment_map_ops`. None of these touch the v5-frozen `type_ops` or
   `storage_ops` shapes. If you find you need a new field in either, that's
   a v7 question, not a v6 question — write a v7 note for the next reviewer.
3. **Don't fold stages.** Each stage's bench claim has to be checkable on
   its own. Land 2 before 3; land 4 before 5. The reverse-individual-stage
   path is what makes a measurement-driven design tractable.
4. **If a stage's claim doesn't hold, report negative.** Per CLAUDE.md,
   negative results are signal. A stage that fails to deliver isn't a bad
   commit — it's a documented "this didn't pay off, here's why" that
   protects the next agent from trying again.
5. **Read the v5 invariants (§12 there).** The five v5 invariants are
   still load-bearing; v6 §6 adds three. If you find yourself wanting to
   violate one, you've found a v7 question.

---

## 9. Closing — why this is the same design, scaled

The contiguous-storage cores worked because they took a multi-instantiation
template body and split it into (a) a non-template body that does the byte
work and (b) a per-instantiation vocab table that names the leaves. The
splits made the per-instantiation cost ~one table constant and made the
trivial-fast-path collapse to `memcpy` across all consumers.

v6 says: do the same thing to the rest of the library, but only where the
same conditions hold:

- A modifier body that branches on a flag set (the v5 sandwich).
- More than one container consumes the same body (the COMDAT precondition).
- A typeless byte path exists for the trivial case (the `memcpy`-equivalent).

Where those hold (deque cross-segment ops, `any`/`function` lifecycle, two
algorithm primitives), v6 commits to the cores. Where they don't (list,
tree, hash chain walks; sort; sig-shaped invoke), v6 commits to *not* writing
the core. The empty result is the design too; it's what keeps the bench
honest in v7.

Five cores in v5 (contiguous). Three more in v6 (segmented). Two more in v6
(algorithm). One vocab table apiece in v6 for `any` and `function` that
replaces per-T vtables. That's the whole proposal, and it adds ~450 lines
to `src/support.cpp` against a predicted ~5–10 KB/program `.text` saving
across the bench's eight programs. Same sandwich, three more fillings.
