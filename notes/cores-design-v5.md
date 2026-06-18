# Cores design v5 — the shared type-erased cores for libcis

Scope: the non-template, byte-level routines that live in `src/support.cpp`
and are called by `<vector>`, `<string>`, and any future contiguous-storage
sequence container (deque's segment vector, the small-buffer side of
`small_vector`, etc.). They are the second half of the type-erasure
sandwich whose first half is `bits/type_ops.h` + `bits/storage_ops.h`:

```
  container header    (template, thin)
  ─────────────────────────────────────
  type_ops    storage_ops    (vocabulary tables, in bits/)
  ─────────────────────────────────────
  cores       (non-template, in src/support.cpp)   ← this document
```

The container is type-aware and thin. The cores are typeless and do the
work. The two vocabulary tables are the contract between them. This
document picks the **smallest set of cores** that lets vector and string's
modifiers be one core call each, and pins down the calling convention so a
third container (deque's per-segment buffer, a small-vector spillover,
flat_map's underlying vector) drops in without growing the table.

This is design only. No code is changed.

---

## 0. TL;DR

Five cores, one shape, one rule.

```cpp
namespace std::detail {

// 1. Storage growth — ensure the buffer holds at least min_bytes live bytes.
void grow(const type_ops*, const storage_ops*,
          void* st_ctx, size_t min_bytes);

// 2. Destroy elements in [begin, end). Trivial-fast-path: skipped.
void destroy_range(const type_ops*, const storage_ops*,
                   void* st_ctx, void* begin, void* end);

// 3. Construct n copies of *src at dst. Trivial-fast-path: memcpy loop.
void construct_copy_one_n(const type_ops*, const storage_ops*,
                          void* st_ctx, void* dst, const void* src, size_t n);

// 4. std::rotate over a constructed range, with a post-rotate size delta.
//    Erase = rotate(p, p+es, end, -es). Insert = construct-at-end + rotate.
void rotate(const type_ops*, const storage_ops*, void* st_ctx,
            void* first, void* middle, void* last,
            ptrdiff_t size_delta_bytes);

// 5. Internal-to-storage_ops: relocate live bytes from old to new buffer.
//    Called by storage_ops::resize implementations; not by containers.
void relocate_live(const type_ops*, void* el_ctx,
                   unsigned char* dst, unsigned char* src, size_t live_bytes);

} // namespace std::detail
```

Five names, one calling convention (`(tops, sops, st_ctx, …op args in bytes)`
for the four container-facing cores; `relocate_live` is a leaf that the
storage_ops resize slot calls and takes the already-fetched `el_ctx`
directly). All argument widths are bytes; the container converts at the
boundary. The cores live in `src/support.cpp` as a single ELF section per
function so `--gc-sections` drops them in programs that never use them.
The forward declarations live in `include/bits/cores.h`; vector/string
include that header, not vector_cores.h (the v4 name retained nothing
vector-specific and was a misnomer).

The rule that picks the set: **every container modifier resolves to one
core call (or none)**. If a modifier needs two cores, the cores are wrong.

The bench-measured target: at parity with the v4 shape (`storage_ops`
Strategy A and B both wired), this set is what `src/support.cpp` already
ships modulo the rename. The v5 contribution is to **freeze** that set and
state why no sixth core is needed, so the next agent that wants to add a
"grow_with_gap" / "shift_down" / "construct_default_n" / "swap_to" slot
has a written reason to look elsewhere.

---

## 1. The calling convention

One shape, every core:

```
fn(const type_ops* tops,
   const storage_ops* sops,
   void* st_ctx,
   … op-specific args, all byte-typed …)
```

- **`tops`** — element vocabulary table. Lifecycle leaves
  (`default_construct/copy_construct/move_construct/destroy`), value ops
  (`copy_assign/move_assign/swap/compare/equal/hash`), `size`, `align`,
  and a `flags` bitset that includes triviality bits AND
  `f_alloc_default_life`, `f_alloc_ctx`. See bits/type_ops.h.
- **`sops`** — buffer vocabulary table. Five state slots
  (`data/set_size/cap_end/resize/free`) and one ctx slot (`alloc_ctx`).
  See bits/storage_ops.h.
- **`st_ctx`** — opaque pointer the storage_ops slots understand. Today,
  for vector and string it is `this`. For a future deque segment it is the
  segment header. Picked by the container.
- **op args** — pointers into the buffer (`void*`), counts in bytes
  (`size_t`), signed deltas in bytes (`ptrdiff_t`). The element size
  (`tops->size`) is the conversion factor; the container converts at the
  boundary so no `sizeof(T)` math leaks into the core.

The `el_ctx` (= the allocator instance pointer) is **fetched lazily** by
each core via `sops->alloc_ctx(st_ctx)`, **only** when
`tops->flags & f_alloc_ctx` says the allocator has per-instance state.
For stateless allocators (`std::allocator`, libcxx-test's `min_allocator`,
plenty of pmr-shaped instances whose state-equality is `true_type`) the
fetch never fires and the core never threads ctx through anything.

This is intentional: the threading is the only thing that distinguishes a
"trivial-fast-path" call (`triv_destroy(ops)` returns true → just return)
from the "real work" path. The fast path looks at `tops->flags` and exits;
it never touches `sops` and never asks for ctx. The bench wins from v4
came from making this fast path the common path; v5 freezes that.

### Why not pass `el_ctx` from the container?

Two reasons:

1. The container would compute `&alloc_` at every callsite, which means
   the address-of-alloc decoration appears `n` times per callsite × the
   number of templates × the number of allocator types. For stateless
   allocators it is a dead value, so the compiler strips it — but each
   strip is one optimization the compiler has to perform per
   instantiation. Lazy fetch via `sops->alloc_ctx` collapses this to one
   constant-folded null at instantiation time.
2. It puts the trivial-fast-path branch on the *caller* side of the call,
   which prevents `--gc-sections` from dropping `default_construct_op`,
   `destroy_op`, etc. for trivial T when the consumer never asks for the
   non-trivial path. With the fetch inside the core, the ctx-handling
   prologue is part of the core, and the leaves themselves get GC'd.

The cost is one indirect call (`sops->alloc_ctx`) per core invocation
when state is needed, which is rare. For stateless allocators the
indirect call is never made; for stateful ones it is one load + one
indirect call per modifier. Acceptable.

---

## 2. The five cores, one by one

### 2.1 `grow(tops, sops, st_ctx, min_bytes)`

Ensure the buffer holds at least `min_bytes` live bytes' worth of
capacity, relocating existing live bytes into a fresh allocation if
needed. Delegates entirely to `sops->resize`; the core is two lines and
exists so the container has one symbol to call instead of writing
`sops->resize(tops, st_ctx, min_bytes)` at every site.

**Why a wrapper instead of inlining the slot call at the container?**
The container would have to know which mask-bits of `tops->flags` matter
for ctx, which is a storage_ops detail. Today the slot itself does that
check internally. Keeping `grow` as a one-line forwarder also gives a
single hookpoint for instrumentation (size telemetry, allocation tracing)
without re-grepping every callsite.

**Trivial fast path**: none at this layer. `sops->resize` is itself the
fast path (Strategy A doesn't dispatch through `type_ops` for the
allocation; only the relocation does, via `relocate_live`). The core
returns void; the container reads back `begin_` / `end_` / `cap_`
through `sops->data` and `sops->cap_end` if it needs them, or via the
layout base in the Strategy A case.

Callers today: vector's `vallocate`, `reallocate_grow`, `shrink_to_fit`;
string's `vallocate`, `reallocate_grow`. Two per file — small enough to
inline, but the symbol pays for itself as a forwarder for instrumentation.

### 2.2 `destroy_range(tops, sops, st_ctx, begin, end)`

Run `tops->destroy` (which folds to `allocator_traits::destroy`) on
every element in `[begin, end)`, end-to-begin (libc++ order, in case the
destructor of element `i` depends on later elements still being alive).

**Trivial fast path**: `f_triv_destroy & f_alloc_default_life` → return.

**Precondition trap**: `begin > end` traps. This is the kind of bug that
used to silently destroy 2^64-N elements before; the trap is one branch
and pays for itself the first time it catches a sign-flip.

Callers today: vector's `vdeallocate`, `destruct_at_end`; string's
`vdeallocate`. The end-of-range cleanup at `~vector()` / `~basic_string()`
all funnels here.

### 2.3 `construct_copy_one_n(tops, sops, st_ctx, dst, src, n)`

Copy-construct `n` elements at `dst`, each from the single object at
`src` (one source, broadcast). Two cases:

- `f_triv_copy & f_alloc_default_life` → `memcpy` `n` times (the
  element size is `tops->size`).
- Otherwise → fetch `el_ctx` once, then `tops->copy_construct(el_ctx,
  dst+i*sz, src)` in a loop.

Callers today: vector's `insert(pos, n, x)`. String has its own
`traits_type::assign(p, n, c)` path that is faster than the generic
core for `char` (the compiler can SIMD-vectorize a typed broadcast in a
way it cannot vectorize a `memcpy`-in-a-loop). So string is not a caller
and won't be; this core is specifically for `vector<T>::insert(pos, n, x)`.

**Why not a more general "construct_n_from_iter" core?** Two reasons:

1. `vector(InputIter, InputIter)` and `insert(pos, InputIter, InputIter)`
   work with input iterators whose dereference type is parameterized on
   the iterator, not just the container. Type-erasing that means either
   passing an `(advance_fn, deref_fn)` pair (a second vocabulary table
   keyed on the iterator) or `memcpy`-from-a-buffer (which requires
   materializing the buffer first). Both lose more code size than they
   save.
2. The container's iterator loop is small (10–20 bytes per `T`) and
   inlines cleanly with the user's iterator. The construct-from-one-and-
   broadcast case is the only one where the core wins: `n` can be huge,
   the source is a single object, and the trivial fast path is a tight
   memcpy loop the compiler vectorizes.

So this core's scope is exactly `insert(pos, n, x)`. It is the only
"insert n" core, and there is no `assign_n` / `fill_n` / `construct_n`
sibling.

### 2.4 `rotate(tops, sops, st_ctx, first, middle, last, size_delta_bytes)`

The workhorse. `std::rotate` semantics — rearrange `[first, last)` so
`*middle` becomes the new `*first` — then apply a signed size delta
(in bytes, can be negative or zero).

This single core replaces three:

- `erase(pos)` → `rotate(p, p+es, end, -es)`.
- `erase(first, last)` → `rotate(p, p+n*es, end, -n*es)`.
- `insert(pos, ...)` → construct elements at `end` (via the spare-slot
  invariant — see `ensure_free_`), then `rotate(pos, old_end, new_end,
  +n*es)` to bring them into position.

Insert and erase no longer call any "shift" core. They reduce to "make
room or construct at end, then rotate". The size delta inside the rotate
handles the post-condition (destroy the now-tail-end leftovers on
shrink, expand the live range on grow).

**Algorithm**: block-swap via scratch. Pick the smaller of the two
sides (`left = m - f`, `right = l - m`), save it to a stack/heap
scratch (`alloca`-style: 256 bytes on the stack, `::malloc` above
that), shift the other side into its new position with the right
direction (forward for left-side shifts that move down, reverse for
right-side shifts that move up), then restore the saved side at the
other end.

**Trivial fast path**: `triv_reloc & f_alloc_default_life` → three
`memcpy/memmove` calls (no leaf dispatch). For non-trivial T,
`relocate_block` / `relocate_block_rev` walks the elements via
`move_construct` + `destroy`.

**Precondition trap**: `first > middle || middle > last` traps. Same
rationale as `destroy_range`'s trap.

**Why the size_delta_bytes in this core specifically?** Because the
delta is *always* applied immediately after a rotate. Splitting them
into separate calls means the container writes
`rotate(f, m, l); sops->set_size(st_ctx, n+delta)`. That is two API
points the container has to know about, and the post-condition
(destroy the truncated tail on negative delta) becomes a third
operation at the container. Folding the delta into rotate makes
erase one core call.

The hot-take version: the only thing a sequence container does that
isn't push/pop is rotate-with-a-size-delta. Make that a primitive.

### 2.5 `relocate_live(tops, el_ctx, dst, src, live_bytes)`

The byte-level move-then-destroy primitive that the storage_ops
`resize` slot calls to ferry live bytes from an old buffer to a freshly
allocated new one. Trivial-relocatable path is `memcpy`; otherwise walk
the elements via `move_construct` + `destroy`.

This is the only core that takes `el_ctx` directly rather than fetching
it via `sops->alloc_ctx`. The reason: the storage_ops resize slot has
already (a) called `alloc_ctx` to decide whether to look up an instance
and (b) has the result on hand. Re-fetching inside `relocate_live` would
double the work. Containers don't call this directly; only the
storage_ops resize slot does.

So strictly speaking this isn't a "container-facing" core; it's a
storage_ops-facing one. It lives in `src/support.cpp` alongside the
other four (it's the same shape of byte-walking loop) and is declared
in `bits/cores.h` next to them, but the calling convention asymmetry is
real and is the price of factoring the relocation loop into its own
function.

---

## 3. What the cores deliberately do **not** include

A live list of "looked at it, decided no". Each entry names the
alternative core and the reason it stays in the container.

### 3.1 No `shift_down` / `shift_up` cores

These were `rotate`'s predecessor. The argument for splitting them out
is that rotate "wastes" the scratch buffer for the trivial shift case
(`m == f + 1` for an `insert(pos, x)` — left side is one element, you
could just save it in a register, shift the right side down via
memmove, and write it back). The argument against is that this is what
the rotate fast path *already does*: the left-side scratch is a 256-byte
stack array; for `m == f + 1` it consumes `sizeof(T)` bytes of it; the
compiler is free to keep the saved element in a register across the
memmove. Special-casing buys a few branches in the cold path and
doesn't simplify the container — insert still has to call something
that rotates the new element from `end` to `pos`.

If the bench ever shows the small-rotate case is a measurable fraction
of `vector`'s `.text` *and* a special-case `shift_down` core would shave
it, revisit. For now: rotate handles it.

### 3.2 No `construct_default_n` core

`vector(n)` constructs `n` default-initialized elements. This is the
loop:

```cpp
for (size_type i = 0; i < n; ++i)
    construct_one(this->begin_ + sz + i);
```

The body is one line, the call to `construct_one` (which calls
`alloc_traits::construct(alloc_, p)`) is one line, and for trivial T
the compiler folds the whole thing to `memset(p, 0, n*sizeof(T))`. A
core would (a) need the trivial fast path (memset), (b) need
`tops->default_construct` for the leaf path, (c) save zero bytes at
the call site since there is one caller per container, and
(d) cost the indirect-call decoration.

Same logic for `construct_at_end(iter, iter, n)` — copy-construct from
an iterator pair. The iterator type isn't typeable in a non-template
core unless we add a second vocab table, see 2.3.

### 3.3 No `grow_with_gap` core (the v4 thought-experiment)

The idea: combine "grow the buffer" and "leave a gap of N elements at
position P in the relocated range" into one core, so insert becomes
`grow_with_gap(P, N); construct N at P; size += N`. The argument is
that the relocation copies elements once (into their final positions)
instead of twice (once into the grown buffer, then a rotate to make
room). For `vector<string>::insert(begin(), N, "...")` with a small N
that's a real win.

The argument against: this is an optimization for one shape
(`insert(pos, n, x)` with growth required). The common case is
`emplace_back` / `push_back`, which has no gap and would route through
plain `grow`. So `grow_with_gap` is a second symbol that fires rarely,
and its trivial fast path (`triv_reloc + default_life`) is `memcpy left
half + skip gap + memcpy right half` — three ranges instead of one.
That has to pay for the symbol's existence on every program that uses
vector, including the ones that never call `insert(pos, n, x)`. Net
loss on the bench, by the v4 trial.

The principle: a core earns its keep by being called often enough that
its size pays for itself across the programs that pull it in. A core
that fires only in `insert(pos, n, x)` with growth — a code path most
programs don't have — is a code-size loss masquerading as a perf win.
Keep the two-pass (grow, then rotate) shape for now.

### 3.4 No `assign_range` / `assign_with_size` core

The body of `assign_with_size` (see vector ~line 379) is a two-armed
loop: when `new_size > sz`, copy-assign over the existing range and
then copy-construct the tail; when `new_size <= sz`, copy-assign over
the live range and destroy the tail. It depends on the iterator type
and the user-supplied predicate (`first == last`). Type-erasing it
needs the same iterator vocab table that 2.3 rejected. The function is
~20 lines in the container, doesn't appear on the hot path (assign
isn't push_back), and contains no per-element work the compiler can't
already inline. Leave it in.

### 3.5 No `swap_to` / `alloc_only` slots (the prompt-1 v4 misfire)

The prompt-1 agent split `resize` into `alloc_only` (allocate but don't
relocate) + `swap_to` (relocate then free). It then needed two
`thread_local` variables to pass state from one call to the other.
That is the wrong factoring: `resize` is the operation; the relocation
is internal to it; there is no caller that wants "allocate, then run
arbitrary code, then relocate". The split shipped 7 slots instead of 5
and regressed code size everywhere because each slot is a symbol every
storage_ops constant pays for. Five slots, and the resize slot does the
relocation internally via `relocate_live`.

### 3.6 No `pop_back` / `set_size_minus_one` core

vector's `pop_back` is:

```cpp
alloc_traits::destroy(alloc_, std::to_address(this->begin_ + sz - 1));
set_size_elems_(sz - 1);
```

Two lines, one indirect-via-traits call, one inline field update. A
core wouldn't have less code; the indirect call would still happen
(through `tops->destroy` instead of `alloc_traits::destroy`); the field
update is the same. No win.

### 3.7 No `reserve_or_relocate` for the SSO case

string's small-buffer-optimization case (when libcis adopts it, see §6)
is the opposite of what type-erasure helps with: SSO is a layout
optimization, the in-place buffer is type-known, and the relocation
crosses the layout boundary. The cores will only be involved on the
heap side; the SSO side stays in the container. The cores don't need a
new entry for it.

---

## 4. The element vocabulary table (`type_ops`) — what the cores need

This is a recap of the contract from the core side. It is **not** a
proposal to change `bits/type_ops.h`.

The cores read:

- `tops->size`, `tops->align` — the element layout.
- `tops->flags & f_triv_destroy & f_alloc_default_life` — destroy fast path.
- `tops->flags & f_triv_copy & f_alloc_default_life` — copy fast path.
- `tops->flags & f_triv_reloc & f_alloc_default_life` — relocate fast path.
- `tops->flags & f_alloc_ctx` — does the allocator need its instance?
- `tops->destroy(ctx, p)` — single-element destroy leaf.
- `tops->copy_construct(ctx, d, s)` — single-element copy ctor leaf.
- `tops->move_construct(ctx, d, s)` — single-element move ctor leaf.

The cores **do not** read:

- `tops->default_construct` (no core constructs defaults; see 3.2).
- `tops->copy_assign`, `tops->move_assign`, `tops->swap` (assign is not
  in a core; see 3.4).
- `tops->compare`, `tops->equal`, `tops->hash` (these are for associative
  containers, not the contiguous cores).

What this means for `ops_for<T, A, MASK>`: vector and string currently
pass `OP_DESTROY | OP_MOVE_CONSTRUCT`. With v5's set, the right mask is
`OP_DESTROY | OP_COPY_CONSTRUCT | OP_MOVE_CONSTRUCT` — the
`construct_copy_one_n` core uses `copy_construct`. Today vector elides it
because `construct_copy_one_n` has a trivial-copy fast path; for
non-trivial T the leaf is missing and the core would trap. **This is a
real call-site bug in vector's mask — fix as part of landing v5** (one
line in `ops_table_()`).

String's mask is correct as-is (string never calls `construct_copy_one_n`;
its broadcast goes through `traits_type::assign`).

---

## 5. The buffer vocabulary table (`storage_ops`) — what the cores need

Also a recap of the contract; not a change.

The cores read:

- `sops->resize(tops, st_ctx, min_bytes)` — grow's only callee.
- `sops->data(st_ctx)` — rotate, to read the current live extent before
  applying the size delta. Returns a `byte_span` (`{data, size}`).
- `sops->set_size(st_ctx, new_bytes)` — rotate, to apply the delta.
- `sops->alloc_ctx(st_ctx)` — every core, when `f_alloc_ctx` is set,
  to fetch the allocator instance pointer.
- `sops->free(st_ctx)` — not called by any core directly; called by the
  container in `vdeallocate` after `destroy_range`.
- `sops->cap_end(st_ctx)` — not called by any core; container uses it
  for `capacity()`.

So the cores use 4 of the 6 slots. The other 2 (`free`, `cap_end`) are
container-facing only. That's fine — `storage_ops` was sized for the
container, not for the cores, and there is no win in trimming it.

---

## 6. Other containers that consume these cores

### vector — current consumer

Modifiers in v5 terms:

| modifier | core call |
|---|---|
| `vallocate(n)` / `reallocate_grow(n)` | `grow(_, _, this, (n+1)*es)` |
| `vdeallocate()` | `destroy_range(_, _, this, begin, end); sops->free(this)` |
| `destruct_at_end(p)` | `destroy_range(_, _, this, p, end); set_size` |
| `push_back` / `emplace_back` | none (in-place construct after `ensure_free_`) |
| `pop_back` | none (one destroy leaf + size update) |
| `emplace(pos)` | construct at end (`ensure_free_(2)`), `rotate(pos, p, p+1, +es)` |
| `insert(pos, n, x)` | `ensure_free_(n+1)`, `construct_copy_one_n(_,_,_,p,&x,n)`, `rotate(pos, p, p+n, +n*es)` |
| `insert(pos, first, last)` (fwd) | `ensure_free_(n+1)`, container-side construct loop, `rotate(pos, p, p+n, +n*es)` |
| `erase(pos)` | `rotate(p, p+es, end, -es)` |
| `erase(first, last)` | `rotate(p, p+n*es, end, -n*es)` |
| `clear()` | `destroy_range(begin, end); set_size 0` |
| `resize(n)` | grow or `destroy_range`, then container-side default-construct loop |
| `shrink_to_fit()` | `grow(_, _, this, (sz+1)*es)` |

Every modifier is one core call (or one core + an in-loop construct).
The "or none" cases are the in-place ones where the spare-slot
invariant means the construct happens in already-allocated room.

### string — current consumer

Same shape, minus `construct_copy_one_n` (string uses
`traits_type::assign` for the broadcast case — typed and faster for
`char`). String's modifiers in v5 terms:

| modifier | core call |
|---|---|
| `vallocate(n)` / `reallocate_grow(n)` | `grow(_, _, this, (n+1)*es)` |
| `vdeallocate()` | `destroy_range(_, _, this, begin, end); sops->free(this)` |
| `init_copy / init_fill / append / assign` | `grow` if needed, container-side typed copy |
| `insert(pos, ...)` | `ensure_free_`, container-side construct, `rotate(pos, p, p+n, +n*es)` |
| `erase(pos, n)` | `rotate(p, p+n*es, end, -n*es)` |

Same five cores; string just happens to bypass `construct_copy_one_n`
for the `char`-broadcast fast path. That's fine — the core stays for
vector and any future `flat_set<T>::insert(pos, n, x)`-style consumer.

### deque — future consumer

Each deque segment is a fixed-capacity buffer of `T`. The segment
*itself* doesn't grow (deque grows by allocating another segment), so
the cores are mostly overkill: deque doesn't need `grow`, doesn't need
`shrink_to_fit`. It does need:

- `destroy_range` at segment teardown — same core, same arguments.
- `rotate` is conditional: deque's `erase` and `insert` go through
  segments and rotate within a segment occasionally, but the
  cross-segment shifts are deque's own.

The cores are big enough for deque to use as-is, without growth. The
deque header would write a `storage_ops` flavor for "segment of fixed
size N" (allocate is a one-shot pool fetch; `resize` is unreachable;
`cap_end` is `begin + N*es`). That's three new lines in `storage_ops.h`,
no change to the cores.

### small_vector — future consumer

`small_vector<T, N>` has an inline buffer of `N`-element capacity and
spills to the heap. The inline side keeps the container's layout
type-known (the buffer is `aligned_storage_t` inside the container);
the heap side uses Strategy A's three-pointer layout.

Strategy: same five cores, two storage_ops flavors (one for "inline
side" — `resize` traps because growth means moving to heap; `set_size`
is a simple field update; `free` is a no-op; `cap_end` is `begin + N*es`
— and one for "heap side" identical to vector's Strategy A). The
container picks at runtime via a discriminator bit. Cores need no change.

### flat_map — future consumer

flat_map is a sorted vector under the hood. Its modifiers reduce to
vector's modifiers + a binary search at the container layer. No new
core; flat_map uses vector's cores transitively.

---

## 7. Where the cores live, what the header is named, who includes it

**File**: `src/support.cpp` (already). Each core gets its own
`.text.fn_name` section (`-ffunction-sections` is bench-default), so
`--gc-sections` drops the ones unused programs. The cores reference
each other (rotate calls relocate_block which calls the leaves; grow
calls sops->resize which calls relocate_live) so they pull each other
in as a graph; a program that uses `<vector>` for trivial T only will
pull the trivial fast paths of all five and drop the leaves.

**Forward-decl header**: `include/bits/cores.h` (new, ~30 lines). The
v4 name `bits/vector_cores.h` was a misnomer — string uses them too,
and deque will. Rename on landing v5. The header is included by
`<vector>` and `<string>` (transitively, via `<bits/storage_ops.h>`
which it already includes) and by `src/support.cpp`. The
include-graph is:

```
  <vector>, <string>
       │
       ▼
  bits/cores.h           (decls only)
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

`src/support.cpp` includes `bits/cores.h` for the forward decls and
defines the bodies. Containers include `bits/cores.h` for the forward
decls and call them. No other TU pulls them in.

---

## 8. The migration from v4 to v5

What's in the tree today (`src/support.cpp` + `bits/storage_ops.h` +
`bits/type_ops.h`) is **already v5**, modulo the dead include and the
vector mask bug. So v5 is mostly the act of *naming the design*
(this document) and writing it down.

The concrete code deltas are small:

1. **Add `include/bits/cores.h`** with the five forward declarations
   above (copied from the now-deleted `bits/vector_cores.h`). ~30 lines.
2. **Update `src/support.cpp` line 20** from
   `#include <bits/vector_cores.h>` (file removed in `debf4cd`) to
   `#include <bits/cores.h>`. **The current source doesn't compile**
   without this — `debf4cd` removed the header but didn't fix the
   include, which means the in-tree `src/support.cpp` is broken as-is
   relative to the v4 state restored by reverting `debf4cd`. The first
   step of landing v5 is to rename `bits/vector_cores.h` →
   `bits/cores.h` and update the one include site.
3. **Fix vector's `ops_table_()` mask** to add `OP_COPY_CONSTRUCT`
   (one line in `include/vector` ~line 218). Without it,
   `vector<NonTriviallyCopyableT>::insert(pos, n, x)` will see a null
   `tops->copy_construct` and trap when it falls off the trivial fast
   path. This is a latent bug in the current code that v5's mask audit
   surfaces; fix it.
4. **Pin the contract** by adding a `static_assert` in `bits/cores.h`
   that the storage_ops table is exactly 6 slots and the type_ops table
   has the leaves the cores need (`size`, `align`, `destroy`,
   `move_construct`, `copy_construct`, `flags`). Ten lines. The
   `static_assert` text is the bug report when a future agent adds a
   seventh slot — the next reviewer's brief.
5. **No bench journal entry** because the code didn't change (modulo
   the rename and the one-line mask fix). The mask fix should be its
   own commit so it can be measured in isolation if a sequence test
   surfaces (today's tests don't exercise the `insert(pos, n, x)` of
   a non-triv-copyable T — that's how the bug survived). The bench
   journal entry is for the mask fix, not for v5 as a whole.

Nothing else changes. The cores' shape, the storage_ops slot count,
the type_ops fields, the calling convention — all stay. v5 is the
document that says "this is what we have, and here is why no sixth
core, no seventh storage_ops slot, no additional type_ops leaf is
needed". If a future agent has a reason to add one, they have to argue
against this document, not against silence.

---

## 9. What this design assumes about gcc-10.2 / -fno-exceptions / -fno-rtti

- **No exceptions**: the cores trap on bad preconditions (`begin > end`),
  not throw. Allocation failure inside `sops->resize` traps the same way
  (it calls `::operator new` which terminates rather than throws). The
  cores have no `try/catch` and don't need the exception strong-guarantee
  rollback that libc++'s `vector::insert` does (libc++ records the old
  state, runs the modify, and restores on throw). Without exceptions
  there is nothing to roll back to, and `__builtin_trap` is the
  failure handler.
- **No RTTI**: the cores carry no `type_info`; the element identity is
  the `type_ops*` itself. Two distinct `T`s have distinct `&ops_for<T,A>`
  addresses, which is enough for the cores' purposes (it's also enough
  for `any` and `function` to identify their stored type; out of scope
  here but worth noting that the same vocab table generalizes).
- **gcc-10**: no `__builtin_bit_cast` (gcc-11+), but the cores don't
  use it; the byte-walking is `__builtin_memcpy/memmove`. No
  `__type_pack_element`, but the cores aren't templated. No
  `[[likely]]`/`[[unlikely]]`, not used. The cores compile clean at
  `-std=gnu++20 -fno-exceptions -fno-rtti -O0` and at `-Os -flto`.

---

## 10. Open question: should `set_size` also be a core?

`set_size_elems_` is a one-line container-side helper that calls
`sops->set_size(this, n*es)`. It's not in the cores' five because it has
no internal logic — it's a pure forwarder. But the container calls it at
~20 sites (every modifier that changes size has a `set_size_elems_(...)`
postfix).

Argument for promoting it: one extra core symbol, ~5 bytes of `.text`
each callsite saves (replacing the inline forwarder with a `call`),
~100 bytes saved across vector's modifiers per `vector<T,A>`
instantiation. At ~5 instantiations per program (vector<int>,
vector<string>, vector<char>, vector<MyType>, vector<unique_ptr<X>>) and
3 programs in the bench, that's ~1.5 KB total — measurable.

Argument against: the call breaks `pop_back`'s "decrement, destroy"
locality the compiler exploits today (the size update and the destroy
happen back-to-back; if both go through indirect calls, the compiler
loses register-allocation freedom). And the symbol is `void f(void*,
size_t)` shared across every container — it would fold under COMDAT
across vector and string anyway, so the saving is amortized whether
it's a core or an inline forwarder.

**Verdict**: don't add it as a core. If the bench shows ~500B per
instantiation in the size_t-arithmetic decoration around
`sops->set_size`, revisit; today the forwarder is the right shape.

---

## 11. Open question: should `relocate_live` move into storage_ops as a slot?

The argument: `relocate_live` is called by exactly one caller
(`storage_ops::resize` implementations) and is structurally part of
storage_ops's job. Making it a 7th storage_ops slot
(`relocate_live: void(*)(type_ops*, void*, void*, size_t)`) would let
storage_ops's resize implementation call through the table the same way
the cores do, and the storage_ops_a / storage_ops_b instances would
share the same `relocate_live` body (it's typeless).

The argument against: making it a slot adds one indirect call to every
`resize`, and the only reason to make it a slot is symmetry — there's
no consumer that picks a different `relocate_live` than the default
one. It would be a slot with one implementation, which is the worst
kind of slot.

**Verdict**: keep `relocate_live` as a free function in `src/support.cpp`,
declared in `bits/cores.h`. The storage_ops resize slot calls it
directly. The asymmetry (resize takes type_ops + st_ctx, relocate_live
takes el_ctx + raw bytes) is real and documented in 2.5.

---

## 12. The invariant the cores enforce

One sentence each:

- **Trivial-fast-path correctness**: the trivial-X bit AND the
  allocator-default-life bit BOTH must be set before a core may take a
  byte path; either alone is wrong. Recorded in `triv_destroy`,
  `triv_copy`, `triv_reloc`, `triv_default` helpers in support.cpp.
- **Lazy ctx fetch**: a core fetches `el_ctx` via `sops->alloc_ctx`
  exactly once, at most once per call, and only when `f_alloc_ctx` is
  set; the result is held in a local for the duration of the call.
- **Byte-granular arithmetic**: all pointer arithmetic inside the
  cores is in bytes; `tops->size` is the multiplier. No `T*` casts
  except inside the leaf body (which the core doesn't see).
- **Spare-slot invariant ownership**: `storage_ops::set_size` and
  `storage_ops::resize` write the spare slot at the new end (one
  element zeroed) when capacity permits. The cores **never** write
  the spare slot themselves; they trust the storage_ops slots to
  maintain it. (This is what makes string's null terminator free —
  it's not the cores' job, it's storage_ops's.)
- **One core call per modifier**: if a container modifier wants two
  cores in a row, the cores are wrong, not the modifier. The
  exception is `vdeallocate` (destroy_range then free), where the
  two operations are owned by different vocab tables and can't be
  combined without crossing the abstraction boundary.

These five invariants are the design's load-bearing claims. A future
agent breaking one is breaking the contract.

---

## 13. Closing — why this design is right-sized

Five cores is enough because:

- Every contiguous-storage modifier resolves to one core call.
- Every core call resolves to one `type_ops` leaf path *or* one trivial
  byte-op path, picked off `flags`.
- Every byte-op path is one of {`memcpy`, `memmove`, `memset`, a
  leaf-walking loop}.

The combinatorics: 5 cores × 2 paths (trivial/leaf) × 1 leaf kind
(byte-walking loop) = 10 distinct codegen shapes. That's small enough
to fit in `support.cpp` without bloating, large enough to cover every
container modifier libcis cares about today and the obvious extensions.

If a sixth core can demonstrate (a) a callsite count > 5 across
libcis's containers, (b) a `.text` saving > 500 B per program in
bench, AND (c) a shape that the existing five cores can't approximate
within 20% of the optimum, then it earns its keep. The bar is
deliberately high because the v4 trial showed that under-justified
cores regress code size before their per-instantiation savings show up.
