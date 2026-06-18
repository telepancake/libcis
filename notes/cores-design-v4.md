# Cores design (v4)

A focused revision of the type-erased "core" vocabulary for libcis's
contiguous-storage containers (`vector`, `basic_string`, and the contiguous
slabs that will eventually back `deque`). Written with the tree as it stands
(`bits/type_ops.h`, `bits/storage_ops.h`, `bits/vector_cores.h`,
`src/support.cpp`) plus the measured findings in
`notes/callsite-analysis.md` plus the open questions left by v3.

v4 differs from v3 in three respects:

1. **Acknowledge the trial table.** v3 picked an arg-order convention
   (`tops` first) and a fused `replace_range` shape against an unmeasured
   model of the calling code. The trial table (`notes/callsite-analysis.md`,
   "Trial summary") already showed that `st_ctx`-first regressed by
   +13 B per callsite on x86_64, and that the lazy `alloc_ctx` slot lookup
   gains call-site bytes if forced on cores that *don't* already take
   `sops`. v4 keeps the tree's measured convention (`tops`, `sops`,
   `st_ctx`, then per-call payload) instead of inventing a new one.
2. **Don't lose the rotate-with-delta result.** The trial table's
   "rotate(…,delta) folded into core" row says: collapsing erase/insert
   post-amble into the core saves bytes per real-binary on all three
   target architectures. v3's `replace_range` re-folded *more* into the
   core (destroy + shift + maybe-grow + set_size + return-pointer) on
   the strength of the same logic, but never verified that the win
   compounds rather than saturates. v4 splits the difference: keep
   `rotate(…,delta)` (measured win), promote it to *the* mutating
   middle-of-buffer primitive, and add only the new cores whose
   payoff is independently arguable. No measurement, no new core.
3. **Wire cross_ops or delete it — pick one, in this revision.** v1/v2/v3
   all left this open. v4 picks: *delete it*. Rationale below (§3.2).

## 1. The contract a core has to meet

A *core* is a non-template free function defined exactly once in
`src/support.cpp` and declared in `bits/vector_cores.h`. To live in that
file rather than be inlined in the container header, a candidate body
must satisfy *every* one of:

1. **Real shared work.** Body is dominated by a loop, a branch on a
   `type_ops::flags` bit, and at least one leaf-or-`memcpy` slow/fast
   path — not by argument-shuffling. A 12-instruction body whose
   epilogue dominates does not earn its symbol.
2. **At least two callers across containers.** Bodies that one container
   method calls in one place can stay inline in the header without
   bloating the per-(C, T, A) instantiation. ("Two callers" means two
   distinct *container methods*, not two textual call sites of the same
   helper from one method.)
3. **Tables differ, body doesn't.** The slow-path leaf calls dispatch
   through `tops->X` / `sops->Y`; the fast paths dispatch through
   `tops->flags`. No `if constexpr` on `T` lives inside the core — that
   is what the leaves are for.
4. **The trivial fast path is a real `memcpy`/`memset`/`memmove`/skip.**
   The cores' whole reason to exist on i386 + ARM-32 is that the trivial
   path collapses N per-instantiation loops into one `__builtin_mem*` at
   the use site after the linker's deduplication.

`grow` in the current tree fails (1): it is a one-line forwarder to
`sops->resize`. We keep it anyway because four headers already forward
through that name and renaming-or-removing it would churn six call sites
without changing the codegen. But the v4 set will not gain new entries
of the same shape.

## 2. The set

Six entries: four kept from the current tree (`grow`, `destroy_range`,
`rotate`, `construct_copy_one_n`) plus the internal-only `relocate_live`
(moved out of the public menu — see §2.5) plus one new
`copy_construct_range` for the iterator-pair construct paths in vector
and the buffer-copy paths in string.

### 2.0 Argument-order convention (kept from the tree, validated by trials)

```
core(const type_ops* tops, const storage_ops* sops, void* st_ctx,
     /* per-call payload */ ...);
```

Three vocabulary args first, payload last. Rationale from the trial
table:

- `st_ctx-first` was measured at +13 B per `vector::erase` callsite on
  x86_64 (compiler saved `this` on stack across the intermediate
  `end_ptr_()` call rather than holding it in `rbx`). i386 was a wash
  (all args spill).
- `tops`/`sops` are loaded from rodata; their addresses are constant
  per-(C, A) and the compiler hoists the loads. Putting payload first
  would force a payload-then-tops `push` sequence on i386, in the
  wrong order for the compiler's `push imm32` of the table address.
- `el_ctx` is **not** an arg. Cores that need it call `sops->alloc_ctx
  (st_ctx)` lazily, gated by `(tops->flags & f_alloc_ctx)`. Trial:
  threading `el_ctx` as a fifth arg cost +20 B per `destroy_range`
  callsite on x86_64 for the stateless-allocator common case (an
  unnecessary `xor edx, edx` per call).

The rule for adding payload args: register-arg slots first, then any
spill candidate last. Pick the spill candidate so it's written once
at the call site and read once in the core (e.g. a freshly-computed
`min_bytes`).

### 2.1 `grow` — capacity ensure (kept; promoted note added)

```cpp
void grow(const type_ops* tops, const storage_ops* sops, void* st_ctx,
          size_t min_bytes);
```

Body is `sops->resize(tops, st_ctx, min_bytes)`. Body fails the
"real shared work" test in §1 — it is one indirect call. We keep it
for two reasons:

1. **One name across headers.** Six call sites in `<vector>` and
   `<string>` say `detail::grow(...)`. Inlining the body
   (`sops->resize(...)`) replaces one name with another and does not
   shrink anything. The forwarder-symbol cost is paid once.
2. **A natural home for future growth-policy.** The "+1 for the spare
   slot" check currently lives in the headers (`reallocate_grow`,
   `vallocate`). If we ever consolidate it into the core, this is
   the right symbol; v4 does not consolidate (no measurement to
   justify it).

**Caller list (existing).** `vector::vallocate`,
`vector::reallocate_grow`, `vector::shrink_to_fit`,
`string::vallocate`, `string::reallocate_grow`,
`string::shrink_to_fit`. Six call sites.

**No change vs. tree.** Listed for completeness.

### 2.2 `destroy_range` — destroy a constructed byte range

```cpp
void destroy_range(const type_ops* tops, const storage_ops* sops,
                   void* st_ctx, void* begin, void* end);
```

**Semantics.** Destroys every constructed element in `[begin, end)`
using `tops->destroy`. `begin > end` traps. Does **not** update
`sops`'s recorded size — the caller that wants size update either
calls `sops->set_size` after, or routes through `rotate(…,delta<0)`
which fuses both.

**Trivial fast path.** `triv_destroy(tops)` → return. Path taken
by every `basic_string<C>` and every trivially-destructible
`vector<T>` with stateless allocator.

**Slow path.** Lazy `el_ctx` fetch, walk back `end → begin` in
`tops->size` strides calling `tops->destroy(el_ctx, p)`.

**Callers.** `vector::vdeallocate`, `vector::destruct_at_end`,
`string::vdeallocate`, transitively from `rotate(…,delta<0)` for the
truncation tail. Three direct callers (qualifies under §1 criterion 2)
plus the transitive one inside `rotate`.

**ABI.** 5 args. On x86_64 fits in `rdi/rsi/rdx/rcx/r8`; on ARM-32
the fifth (`end`) spills; on i386 all spill. Acceptable as
measured.

**Why not fused with `set_size` ("`truncate_to`" in v3).**
v3 proposed combining `destroy_range + set_size` into one
`truncate_to(new_size_bytes)` call to save a per-callsite `set_size`
arg-prep. v4 rejects this: of the three direct callers,
`vdeallocate` follows the destroy with `sops->free` (not `set_size`),
and `destruct_at_end` is followed by `set_size` only at *some* of its
callers (e.g. `clear` calls it then sets size to 0; `~vector` calls
it then `free`s). The fusion either (a) overspecifies the post-state
and forces the callers that don't want `set_size` to set it back, or
(b) gains nothing because the truly-fused callers already collapse to
one indirect-call slot via `rotate(…,delta<0)` (the case v3 wanted
to optimize). Empirically, this is one slot of `storage_ops` per
fusion-eligible callsite — within the noise.

### 2.3 `rotate` — rotate-with-delta (kept; *the* middle-of-buffer mutator)

```cpp
void rotate(const type_ops* tops, const storage_ops* sops, void* st_ctx,
            void* first, void* middle, void* last,
            ptrdiff_t size_delta_bytes);
```

**Semantics.** Standard `std::rotate` semantics over the byte range,
then adjust container size by `size_delta_bytes`. On `delta < 0` the
trailing `|delta|` bytes are destroyed via `destroy_range` AND
`sops->set_size` shrinks by `|delta|`. On `delta > 0` `set_size`
grows by `delta` (caller has pre-constructed the
new-end elements outside the rotated window). Delta = 0 leaves size
unchanged (used when the caller did its own bookkeeping around an
in-place rotation).

**Trivial fast path.** When `triv_reloc(tops)` is set the block-swap
uses `memcpy/memmove` on a 256-byte stack scratch + `::malloc`
fallback. When also `triv_destroy(tops)`, the delta<0 destroy is a
no-op and only `set_size` runs.

**Callers.** `vector::insert`, `vector::emplace`, `vector::erase`,
`vector::erase(first, last)`, `vector::insert(iter-pair)`. **String
modifiers do NOT currently call `rotate`.**

**v4 stance on string modifiers via rotate.** v1/v2/v3 each proposed
migrating `string::insert`/`erase`/`replace` onto `rotate`. v4 does
*not* recommend the migration without the `bench/drivers/str_mixed.cpp`
measurement, for one reason: string's modifiers already follow the
`triv_reloc + default_lifecycle` path in the trivial case (memmove +
null-terminate). `rotate`'s block-swap-via-scratch is *strictly more
work* than that simple memmove when the shift is one-directional —
the scratch buffer is unused in the simple case. If the measurement
says block-swap+`set_size` fuses smaller across all three target
architectures, migrate; if it says the simple memmove + null-write
wins, leave it. Until then, string keeps its hand-roll.

**ABI.** 7 args — the only core where x86_64 spills. The trial table
explicitly measured this and ruled in favor of `delta`-as-spill
(`+174 / +75 / +68 B per real binary` smaller on x86_64 / i386 /
ARM-32 with rotate-with-delta vs. separate set_size follow-up).
**Do not drop the delta.**

### 2.4 `construct_copy_one_n` — broadcast copy-construct

```cpp
void construct_copy_one_n(const type_ops* tops, const storage_ops* sops,
                          void* st_ctx, void* dst, const void* src,
                          size_t n);
```

**Semantics.** Construct `n` copies of `*src` at `dst[0..n)`. Caller
guarantees capacity. Trivial fast path memcpys the source bytes into
each slot.

**Callers (existing).** `vector::insert(pos, n, x)` — one caller in the
tree today.

**One-caller question.** Does `construct_copy_one_n` survive the
two-caller bar from §1? Two routes to "yes":

- **`vector::assign(n, x)`** is the natural second caller. The
  current header hand-rolls a `for` loop. Migration is a one-line
  change, and the result memsets/memcpys the same fast path as
  insert. Per-(C, T) save: the assign-loop instantiation. Not
  measured; not gating on it for v4.
- **`vector::resize(n, x)`** growth leg, same shape.
- The new `copy_construct_range` (§2.6) does NOT subsume this — it
  reads from a *buffer*, not a *broadcast source* (see v3 §4.2
  for the per-callsite reason).

**v4 stance.** Keep `construct_copy_one_n`. Migrate `vector::assign(n,
x)` and `vector::resize(n, x)` growth leg to it as a follow-up so the
two-caller bar is met without measurement debt.

**ABI.** 6 args. On x86_64 fits in `rdi/rsi/rdx/rcx/r8/r9`; on i386
all spill; on ARM-32 `n` spills.

### 2.5 `relocate_live` — internal byte-level move (NOT a public core)

```cpp
void relocate_live(const type_ops* tops, void* el_ctx,
                   unsigned char* dst, unsigned char* src,
                   size_t live_bytes);
```

**Not in the cores menu.** Called only by `storage_a_resize<C>` and
`storage_b_resize<C>` (the template bodies inside `bits/storage_ops.h`).
Currently declared in *both* `bits/vector_cores.h` and
`bits/storage_ops.h`; v4 removes the declaration from
`bits/vector_cores.h` and keeps it solely in `bits/storage_ops.h`
(where its caller lives). Hygiene: it is not part of the container
forwarder vocabulary.

**Signature peculiarity (kept).** Takes pre-resolved `el_ctx` rather
than `(sops, st_ctx)`. This is intentional: at the call site the
resize template has already fetched the ctx once at entry; recursing
through `sops->alloc_ctx` here would re-fetch through the same
indirection it is implementing.

### 2.6 NEW: `copy_construct_range` — iterator-pair / buffer construct

```cpp
void copy_construct_range(const type_ops* tops, const storage_ops* sops,
                          void* st_ctx,
                          void* dst, const void* src, size_t n_bytes);
```

**Semantics.** Construct `n_bytes / tops->size` elements at `dst[0..)`,
each from the matching slot in `src`. Pre: `src` and `dst` do not
overlap; `dst` is uninitialized storage of at least `n_bytes`. Post:
`dst` holds copy-constructed objects. Trivial fast path: `(triv_copy
& alloc_default_life)` → `__builtin_memcpy(dst, src, n_bytes)`.

**Why it is not `construct_copy_one_n` with a stride.** v2 considered
that. The trivial fast paths diverge: broadcast is `memcpy(dst+i*sz,
src, sz)` looped (or memset for `sz==1`); range is one
`memcpy(dst, src, n_bytes)`. Folding both into one symbol with a
"stride==0 means broadcast" branch puts a runtime test on the hot
path that the linker cannot eliminate (the cores are not inlined).
v3 §4.2 expected separating to save bytes in the body and per
call-site; that prediction is the right one.

**Callers (proposed wiring).**

- **`vector::vector(InputIt first, InputIt last)`** for forward
  iterators of contiguous element type. Currently a per-(C, T, A,
  Iter) loop in the header. The cross-type case (`vector<long>` from
  `int*`) keeps its header loop (the cross construct is one element
  per template instantiation, not type-erased).
- **`vector::insert(pos, first, last)`** forward overload — same
  shape, fills the gap opened by `rotate(…, +n*es)`.
- **`vector::vector(const vector&)`** copy ctor. Currently a per-T loop
  in the header; migration replaces it with `vallocate(size)` +
  `copy_construct_range(…, n_bytes)` + `set_size`. **In the
  trivial-`T` case this folds to one `memcpy`.**
- **`basic_string::basic_string(const C*, size_t)`** and friends —
  after `vallocate` they currently call `traits_type::copy` to fill the
  buffer. Migration would route through `copy_construct_range`'s
  trivial path (which IS `memcpy`). The visible win is symbol
  consolidation across the four string-buffer-copy ctors and the
  six append/assign(buffer) variants; the codegen is the same memcpy.

**Why this is the new entry and not "wait, measure, decide" (v3).**
The wait-and-measure stance left a dead `cross_ops` table in the tree
across three design revisions. The two payoff modes for
`copy_construct_range` are independently real:

1. The vector iterator-pair paths in the header today each instantiate
   their own copy loop per `(C, T, A, Iter)`. Hoisting them into
   `copy_construct_range` collapses the trivial case to one memcpy per
   binary, paid in `support.cpp` once. **This is the same shape of
   win that `destroy_range` has**, and the measured `destroy_range`
   delta in the trial table is positive.
2. The string buffer-copy paths all reduce to a memcpy already, but
   today every string ctor/append/assign(buf) emits its own little
   `traits_type::copy` thunk. Going through `copy_construct_range`
   collapses those, again with the same memcpy in the fast path.

**No cross_ops.** This core takes only `type_ops`, not `cross_ops`.
The cross-type construct case (`vector<long>` from `int*`) is rare
enough (and ill-defined enough — converting via `static_cast` is
caller policy, not a type-erased contract) that v4 keeps it in the
header per-template-instantiation loop. See §3.2.

**ABI.** 6 args, same as `construct_copy_one_n`. Same convention,
same spill behavior.

## 3. What v4 explicitly drops

### 3.1 v3's `replace_range`

v3 proposed fusing `rotate + destroy_range + maybe-grow + set_size +
return-pointer` into one 6-arg core. The promise: one indirect call
per vector mutator (push_back, insert, erase, emplace, resize,
clear).

The trial table already validated *part* of this — `rotate(…,delta)`
beats split rotate+set_size by `+174 / +75 / +68 B per real binary`.
v3's claim is that fusing *more* compounds the win. v4 does not
believe that without measurement, because:

- The growth leg invalidates `begin`; `replace_range`'s post-call
  pointer return is a load-through-`sops->data` either way (the
  caller already needs the new base for follow-up construct), so the
  callsite saving is "one less argument prep on the
  `set_size` call that `rotate` already does." That is one
  push-imm on i386, ~3 B; one MOV on x86_64.
- `replace_range`'s 6-arg signature pushes `(tops, sops, st_ctx,
  pos_off, old_n, new_n)`, all six register-arg-eligible on x86_64.
  Adding the `new_size_bytes` 7th arg (which v3 said spills) brings
  the spill back. Compared to `rotate`'s same 7 args, the savings is
  just whether we fold `destroy_range` in. Real callers of `rotate`
  that need `destroy_range` are the `delta<0` branch — *which already
  fuses it*.

If the measurement (`bench/drivers/vector_erase_mix.cpp` per v3 §4.1)
shows `replace_range` wins on i386 + ARM-32 by ≥40 B per binary, do
the migration. v4's claim is: that measurement is the entry
condition, not a follow-up sanity check.

### 3.2 `cross_ops` — delete it

`cross_ops` has lived in `bits/type_ops.h` with zero callers across
v1, v2, v3. Each design proposed a different way to wire it.

v4 deletes it. Justification:

- **`copy_construct_range` does NOT take `cross_ops`.** The
  iterator-pair case for `vector<T>` from `U*` (where T != U) is rare;
  the standard requires only `T(*it)`, which the header-side per-template
  loop expresses one element at a time. No type-erased core, no
  cross-type vocabulary.
- **The only other plausible consumer was string-from-string-of-
  different-CharT,** which the standard does not require. We can stay
  conformant without it.
- **Hetero-lookup in `unordered_map`/`set`** does not consume the cores
  vocabulary; it consumes hasher/equal customization. `cross_ops`'s
  shape (`construct_from`, `assign_from`, `compare`, `equal`, `cast`)
  is not a hetero-lookup shape.

Removing the `cross_ops` block from `bits/type_ops.h` is ~100 LOC
of pure dead code. Do it.

### 3.3 No `truncate_to`, no `replace_range`, no `insert_open_gap`,
no `erase_close_gap`, no `fill_construct` (v2/v3 proposals).

Each of these is `destroy_range`-or-`rotate`-or-
`construct_copy_one_n` plus one indirect `sops->set_size` call. The
measurement v3 cited for fusing `destroy + set_size` is the same
measurement that already validated rotate-with-delta — and rotate
*already does the fusion* in the only callsites where it pays
(erase/insert/emplace). Promoting the rest of the vector mutators
to do it too is the v3 proposal; v4 sends that to the
"measure first" bucket.

### 3.4 No `grow_to(min_bytes)` separate from `grow`.

`grow` already takes `min_bytes`; v3's `grow_to` was the same call
with a one-line `if (cap_end - begin >= min_bytes) return;` short-circuit.
That short-circuit is what the header `reserve()` and
`vallocate()` already do at the call site. Adding it to the core
hides the constant comparison from the inliner.

### 3.5 No `equal_bytes` / `compare_bytes`.

v3 proposed type-erased equal/compare for `vector::operator==`,
`basic_string::operator<=>`. The cores would be short-circuit
loops over `tops->equal` / `tops->compare`. v4 says: leave them in
the headers per-template. Reasons:

- The trivial-`memcmp` fast path applies to *every* `basic_string<C>`
  (always trivially copyable) and lives at the call site. Inlining
  `__builtin_memcmp` at the comparison call site is one call
  instruction either way.
- The non-trivial path is a leaf-loop with short-circuit on the first
  mismatch, body ~15 instructions per element. The per-(T) version is
  not measurably bigger than the type-erased version (the per-element
  loop dispatches through `tops->equal` either way).
- The total body bytes saved across all of `vector::operator==` and
  `string::operator==` is the loop-frame for one instantiation.
  Verified by inspection of the headers: the call sites are not hot
  enough to warrant a symbol.

If a follow-up measurement on a real consumer (multiple
`vector::operator==` instantiations across different `T`) shows the
per-(T) loop adds up, revisit. For now: not a core.

## 4. Mapping containers to cores

The check that the set above is sufficient — every mutator in
`vector` and `basic_string` lowers to a short sequence of cores plus
one or two header-side leaf calls.

### 4.1 `vector<T, A>`

```
push_back(v):           grow if needed; ::new (end_) T(v); set_size += es

emplace_back(args...):  grow if needed; ::new (end_) T(args...); set_size += es

pop_back():             destroy_range(end_-es, end_); set_size -= es

clear():                destroy_range(begin_, end_); set_size = 0

resize(n):              if (shrink) destroy_range(begin_+n*es, end_); set_size = n*es
                        else if (grow) grow; ::new placement-loop default-construct;
                              set_size = n*es

resize(n, v):           shrink: same; grow: grow; construct_copy_one_n(end_, &v, n-old);
                              set_size = n*es

reserve(n):             if (cap-begin < n*es) grow(n*es)

shrink_to_fit():        if (size < cap) grow(size_bytes)
                        (storage_ops::resize handles "smaller" case; if it
                         doesn't, vector's free-and-reallocate path stays
                         per-template.)

insert(pos, v):         grow if needed; ::new (end_) T(v); rotate(pos, end_, end_+es, +es)

insert(pos, n, v):      grow if needed; construct_copy_one_n(end_, &v, n);
                              rotate(pos, end_, end_+n*es, +n*es)

insert(pos, first,last):grow if needed; copy_construct_range(end_, &*first, n_bytes);
                              rotate(pos, end_, end_+n_bytes, +n_bytes)

emplace(pos, args...):  grow; ::new (end_) T(args...); rotate(pos, end_, end_+es, +es)

erase(pos):             rotate(pos, pos+es, end_, -es)

erase(first, last):     rotate(first, last, end_, -(last-first))

assign(n, v):           if (n <= size): destroy [begin+n, end), set size to n,
                              construct_copy_one_n(begin_, &v, n) over live   *
                        else:          destroy_range(begin_, end_), grow,
                              construct_copy_one_n(begin_, &v, n), set size to n

* The "destroy then construct over" pattern is one
  destroy_range + one construct_copy_one_n. v2/v3 each invented an
  assign core to fuse them; the savings is one indirect call slot
  per assign() callsite. The header forwarder can sequence them.

assign(first, last):    same shape with copy_construct_range

operator=(const&):      same as assign(begin, end) on rhs

== / !=:                size != size -> false; header-side loop over tops->equal
                              (with memcmp fast path in the trivial case)

<=>:                    header-side loop; memcmp fast path
```

### 4.2 `basic_string<C, T, A>` (`C` always trivially copyable)

```
push_back(c):           grow if needed; *end_ = c; set_size += sizeof(C)

pop_back():             set_size -= sizeof(C)  (no destroy: trivially destructible)

clear():                set_size = 0

resize(n):              shrink: set_size = n*sz  (no destroy)
                        grow:   grow; fill region with C() (memset for size==1);
                                set_size = n*sz

resize(n, c):           grow case: grow; construct_copy_one_n(end_, &c, n-old);
                                set_size

append(s, n):           grow if needed; copy_construct_range(end_, s, n*sz);
                              set_size += n*sz

append(n, c):           grow if needed; construct_copy_one_n(end_, &c, n);
                              set_size += n*sz

operator+=(s):          append-shape

operator+=(c):          push_back-shape

insert(pos, s, n):      grow if needed; copy_construct_range(end_, s, n*sz);
                              rotate(pos, end_, end_+n*sz, +n*sz)

insert(pos, n, c):      grow if needed; construct_copy_one_n(end_, &c, n);
                              rotate(pos, end_, end_+n*sz, +n*sz)

erase(pos, n):          rotate(pos, pos+n*sz, end_, -n*sz)

replace(pos, n1, s, n2):
  if (n2 == n1):        memcpy(pos, s, n2*sz)   (no rotate needed)
  if (n2 <  n1):        memcpy(pos, s, n2*sz); rotate(pos+n2*sz, pos+n1*sz, end_,
                                                       -(n1-n2)*sz)
  if (n2 >  n1):        grow if needed;
                        copy_construct_range(end_, s, n2*sz);
                        rotate(pos+n1*sz, end_, end_+(n2-n1)*sz, +(n2-n1)*sz);
                        memcpy(pos, s, n1*sz)

* See §3.0 below for why string-modifiers-via-rotate is *not*
  unconditionally adopted in v4.

operator==:             memcmp (charT is trivially copyable)

compare / <=>:          memcmp / size compare
```

### 3.0 (referenced above) String-modifiers-via-rotate

The §4.2 listing migrates string's `insert`/`erase`/`replace` onto
`rotate`. v3 and v2 proposed the same. The downside, recorded as a
caveat in v1 §3.1: `rotate`'s block-swap-via-scratch is heavier than a
single `memmove` when the shift is one-directional, which is the
common case for string.

v4's recommendation: **do the migration, gated by the
`bench/drivers/str_mixed.cpp` measurement.** The migration is sound
(string's spare-slot null terminator falls out of
`sops->set_size`, which is what `rotate` already calls); the only
open question is the byte ledger. Two paths:

a. Migration wins on i386 + ARM-32: string drops six hand-rolled
   shift loops, gains one rotate call per modifier. Roll out.
b. Migration loses (block-swap costs more than memmove on the
   no-overlap-in-one-direction case): keep the hand-roll, accept
   that string's mutators don't share the cores vocabulary, and
   note in `CONVENTIONS.md` that this is a measured exemption.

v3 picked (a) implicitly; v4 explicitly defers to the measurement.

## 5. ABI / header layout (kept from the tree)

**Where the symbols live.** All public cores declared in
`bits/vector_cores.h`, defined in `src/support.cpp`. Non-template,
non-inline, free functions in `namespace std::detail`. They are
ordinary library symbols and participate in libcis's same internal-
to-the-library ABI as everything else.

**`noexcept`.** All cores are `noexcept` (per the project-wide
`-fno-exceptions` invariant: any leaf throw terminates the program).
v3 §6 spelled out the trade — the unwound state of `rotate`/insert/
`copy_construct_range` partial application after a throw is "the
program is going down anyway." v4 inherits that stance verbatim;
nothing in this design changes it.

**No `constexpr`.** Cores are runtime-only. Constexpr container
support is dropped per `CONVENTIONS.md`.

**Header dependencies.** `bits/vector_cores.h` includes
`<cstddef>`, `bits/type_ops.h`, `bits/storage_ops.h`. The cores need
no `<memory>`, `<algorithm>`, or `<iterator>` — the byte-pointer
boundary at the forwarder erases all of that.

**Pointer stability.** `sops->resize` invalidates `begin`. Cores
that may grow (`grow` only) re-fetch via `sops->data`; the others
do not call resize.

**Spare-slot invariant.** `sops->set_size(st_ctx, k)` writes
`tops->size` bytes of zeros at `begin + k` when capacity allows.
This is how string null-terminates and how vector preserves its
spare slot for the `emplace_back` placement-new path. The cores
write to the spare slot only via `set_size`; never directly.

## 6. What v4 would measure

Five focused measurements. Each one is per-arch (x86_64 + i386 + ARM-32
minimum). Order by expected payoff.

1. **`copy_construct_range` win.** Build a vector-range-ctor driver
   (`bench/drivers/vec_range_ctor.cpp`) instantiating `vector<int>`,
   `vector<long>`, `vector<string>`, `vector<Big>` from `int*`, `long*`,
   `string*`. Compare per-callsite bytes for `vector(first, last)` and
   `vector::insert(pos, first, last)` with the new core vs. the
   header-side per-template loop. Win condition: ≥10 B saved per
   callsite on i386 AND ARM-32, no regression on x86_64.

2. **String modifiers via `rotate`.** Build
   `bench/drivers/str_mixed.cpp` exercising `insert(pos, s, n)`,
   `erase(pos, n)`, `replace(pos, n1, s, n2)` for `string` and
   `wstring`. Compare hand-rolled `traits_type::move` vs. rotate-based
   forwarders. Decision matrix in §3.0.

3. **`construct_copy_one_n` second caller.** Migrate
   `vector::assign(n, x)` and `vector::resize(n, x)` growth leg to
   `construct_copy_one_n`. Compare per-(C, T) instantiation cost
   before/after on the same driver. Expectation: small but positive
   on all three archs.

4. **v3's `replace_range` fusion.** If somebody insists on revisiting
   v3, the directed measurement is the v3 §4.1 driver
   (`bench/drivers/vector_erase_mix.cpp` with ~30 sites). Threshold:
   per-callsite save ≥40 B on i386 + ARM-32. Until that measurement
   exists and clears the bar, `replace_range` is not in the set.

5. **Deletion of `cross_ops`.** Build the bench projects before/after
   removing the `cross_ops` definition block from
   `bits/type_ops.h`. Expectation: small win on every arch (dead code
   in rodata, dead leaves in deduplicated comdat) and no callers to
   break. This is hygiene; the measurement is to make the deletion
   defensible against "what if some downstream wants it."

## 7. Migration from the current tree

The current tree implements:
`grow`, `destroy_range`, `construct_copy_one_n`, `rotate`,
`relocate_live` (declared in both `bits/vector_cores.h` and
`bits/storage_ops.h`).

v4 changes are:

- **Add** `copy_construct_range` declaration to `bits/vector_cores.h`
  and definition to `src/support.cpp`. Body is `construct_copy_one_n`
  with a single source-cursor stride: the trivial path is one
  `__builtin_memcpy(dst, src, n_bytes)`, the slow path is a loop with
  `tops->copy_construct(el_ctx, dst+i*sz, src+i*sz)`.
- **Wire** `copy_construct_range` at the listed callers:
  `vector(first, last)` (forward, same-T), `vector::insert(first,
  last)` (forward, same-T), `vector::vector(const vector&)`,
  `string::basic_string(const C*, size_t)` and friends.
- **Wire** `construct_copy_one_n` at the new callers:
  `vector::assign(n, x)`, `vector::resize(n, x)` growth leg.
- **Migrate** string modifiers (`insert`/`erase`/`replace`) to
  `rotate` if and only if the §6.2 measurement supports it.
- **Remove** the `relocate_live` declaration from
  `bits/vector_cores.h` (keep the one in `bits/storage_ops.h`).
- **Delete** `cross_ops`, `cross_for`, `construct_from_op`,
  `assign_from_op`, `compare_cross_op`, `equal_cross_op`, `cast_op`,
  `make_cross_ops` from `bits/type_ops.h`. They have no callers and
  no plan to acquire one.

Each migration is mechanical and can land in its own commit. The
v4 set is reached without breaking any current code path: the four
existing cores stay with their current signatures.

## 8. Future work

- **Deque slabs.** When `deque<T, A>` lands, its per-slab buffer
  vocabulary is *not* the same `storage_ops` (no monolithic resize,
  capacity is per-slab fixed). The likely shape: a
  `slab_storage_ops` table with a different contract; the cores
  themselves (`destroy_range`, `rotate`, `construct_copy_one_n`,
  `copy_construct_range`) stay. The middle-of-buffer cores need new
  versions only if the cross-slab shift is meaningfully different
  from `rotate`'s scratch-buffer block-swap — which it is, so plan
  on `slab_replace_range` or similar at that point. Speculative;
  flagged for the deque port.

- **Custom-`std::allocator` testing.** The four-allocator audit
  (default, min_allocator, stateful, pmr-style) is largely covered
  by the existing test files. No work needed here; the v4 set's
  contract is identical to v3's on this axis.

- **Trivial fast path for `f_triv_default`.** Today only
  `f_triv_copy`, `f_triv_destroy`, `f_triv_reloc` are consumed at
  the cores' fast paths. A `f_triv_default` fast path would let
  `resize(n)` growth-leg memset zero rather than loop placement-
  new. The size payoff is potentially large on `vector<int>`-style
  consumers; v3 §11 sketches the trade. Not a v4 entry, but a
  natural follow-up.

## Summary (one paragraph)

Five public cores: `grow`, `destroy_range`, `rotate`,
`construct_copy_one_n`, plus the new `copy_construct_range` for
iterator-pair / buffer construct. `relocate_live` stays internal to
`bits/storage_ops.h`. The arg-order convention (`tops`, `sops`,
`st_ctx`, payload) is kept from the tree's measured behavior, *not*
re-derived from a clean room. `rotate(…, delta)` stays — the trial
table confirmed it saves bytes on every target arch. `cross_ops` is
deleted (zero callers across three design revisions; no plan to
acquire one). String-modifiers-via-`rotate` is gated on a measurement
that v4 names but does not pre-judge. v2/v3's larger fusion
proposals (`truncate_to`, `replace_range`, `insert_open_gap`,
`erase_close_gap`, `fill_construct`, `equal_bytes`, `compare_bytes`)
are deferred to the same measurement bar — each adds a symbol, each
has to clear the same "≥40 B saved per callsite on i386 + ARM-32" gate
before joining the set.
