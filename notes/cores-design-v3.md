# Cores design (v3)

Derived from the design problem alone; not from any existing implementation in
this tree.

## 1. The principle

A function earns the title of *core* iff it is the **smallest unit of
contiguous-storage work whose body is invariant across `(Container, T, A)`
and whose call sites would otherwise generate distinct, near-identical
sequences of instructions per instantiation.** Two halves to that. First,
"smallest unit": a core must be a single, named byte-level operation —
make-room, destroy-tail, construct-into, compare-against. A composite that
is just "two cores called in order" is not a core; the call site can
sequence them. Second, "invariant body, distinct sequences": a function
whose machine code differs per `(C, T, A)` cannot be type-erased into one
symbol; a function whose body is one line of `storage_ops` dispatch has no
shared body to amortize. The cores live in the middle: the body has real
loops, real branches, real trivial fast paths gated on `type_ops::flags`,
and is identical for every instantiation — only the tables differ.

## 2. The set

Six cores. Each takes `(const type_ops*, ...)` so the trivial-fast-path
flags are always in hand; the four that mutate storage also take
`(const storage_ops*, void* st_ctx)`.

### 2.1 `truncate_to`

```cpp
void truncate_to(const type_ops* tops,
                 const storage_ops* sops,
                 void* st_ctx,
                 size_t new_size_bytes) noexcept;
```

**Arg order rationale.** Four args. Fits in i386's first four pushes,
ARM-32's r0-r3, x86_64 rdi/rsi/rdx/rcx, AArch64 x0-x3. The `tops` pointer
goes first because the very first thing the body does is read
`tops->flags` and short-circuit if `f_triv_destroy & f_alloc_default_life`.
On the i386 caller, the `tops` push is from a fixed `rodata` address and is
usually `push imm32` (5 bytes); the `sops` push is the same. `st_ctx` is
`this` and is generally already in a register at the call site for free.
`new_size_bytes` is the freshly computed shrink target.

**Semantics.** Destroys elements in `[new_size_bytes, current_size)` and
sets the buffer's recorded size to `new_size_bytes` via `sops->set_size`.
Pre: `new_size_bytes <= current_size_in_bytes`; trapped on violation.
Post: storage capacity unchanged, live byte count = `new_size_bytes`, the
spare-slot zeroing at the new end (the null terminator for string) is
re-asserted by `sops->set_size` for free.

**Callers.**
- `vector::pop_back`, `string::pop_back` — frequent.
- `vector::resize(n)` and `string::resize(n)` when `n < size` — common.
- `vector::clear`, `string::clear` (`new=0`) — common.
- `~vector`, `~basic_string` pre-`free` (`new=0`) — every instance.
- `vector::erase(end()-1, end())` — common.
- Internal step of `assign`/`operator=` when the source is shorter.

**Fast path.** If `(tops->flags & f_triv_destroy) && (tops->flags &
f_alloc_default_life)`, skip the destroy loop entirely and tail-call
`sops->set_size`.

**Slow path body sketch.** Read the live byte range via `sops->data`,
compute `el_ctx` lazily (call `sops->alloc_ctx(st_ctx)` only when
`f_alloc_ctx` is set, else `nullptr`), then loop from `begin +
new_size_bytes` to `begin + live` stepping by `tops->size`, calling
`tops->destroy(el_ctx, p)` at each step. Finally call `sops->set_size`.

### 2.2 `replace_range`

```cpp
void* replace_range(const type_ops* tops,
                    const storage_ops* sops,
                    void* st_ctx,
                    size_t pos_off,
                    size_t old_n,
                    size_t new_n) noexcept;
```

**Arg order rationale.** Six args. On x86_64 SysV that exactly saturates
rdi/rsi/rdx/rcx/r8/r9 with zero spill — the highest-arity core in the set,
designed against that ceiling. On AArch64 all six fit in x0-x5. On ARM-32
the last two (`old_n`, `new_n`) spill to stack; that's acceptable because
they are the freshly computed sizes — already in scratch registers, cheap
to `str sp,#imm`. On i386 every arg spills; the order `(tops, sops,
st_ctx, pos_off, old_n, new_n)` matches the natural left-to-right
evaluation order at the call site so `push` order is monotonic. If a
seventh arg were ever added it should be the one with the largest
short-immediate encoding — but no seventh arg is justified, see §3.

**Semantics.** Replace the byte range
`[begin + pos_off, begin + pos_off + old_n)` with a freshly-opened
**uninitialized** region of `new_n` bytes. Returns a pointer to that
opened region. Steps the function performs internally:

1. Destroy `[begin + pos_off, begin + pos_off + old_n)`.
2. If `new_n > old_n` and existing capacity is insufficient, call
   `sops->resize` to grow (the growth policy — geometric — is the
   storage strategy's responsibility; cores pass `min_bytes = new_live`
   to be respected as a floor). After grow, refresh local pointers.
3. Shift the suffix `[pos_off + old_n, live)` by `(new_n - old_n)` bytes
   (positive or negative) via the element's relocate primitive (memmove
   when `f_triv_reloc & f_alloc_default_life`, else
   loop-move-construct-then-destroy).
4. Update live size to `live - old_n + new_n` via `sops->set_size`.
5. Return `begin + pos_off`.

Pre: `pos_off + old_n <= live`. The returned region is uninitialized
storage; the caller constructs into it (typically via `fill_construct`,
`copy_construct_range`, or one default loop). Post: live size has
shifted by `(new_n - old_n)`; capacity may have grown; spare slot at the
new end is re-asserted.

**Callers.**
- `vector::insert(pos, v)` and `vector::insert(pos, n, v)` —
  `old_n=0`, `new_n=n*es`. Insert at end is `push_back`. **Frequent.**
- `vector::insert(pos, first, last)` — same; for forward iterators the
  size is known in one pass. Common.
- `vector::erase(first, last)` — `new_n=0`. Frequent.
- `vector::resize(n)` growing — `pos_off=live`, `old_n=0`,
  `new_n=(n*es - live)`. Then a default-construct loop on the return.
  Common.
- `vector::emplace`/`emplace_back` — `old_n=0`, `new_n=es`. Frequent.
- `string::append`, `string::push_back`, `string::insert`,
  `string::erase`, `string::replace`, `string::operator+=` — all
  collapse to one form. Frequent.

**Fast path.** Two fast paths share the body:

- *Destroy.* Skip step 1's loop when `f_triv_destroy & f_alloc_default_life`.
- *Relocate.* In step 3, use `__builtin_memmove` when `f_triv_reloc &
  f_alloc_default_life`; else the loop-move-construct-then-destroy form.

The growth check itself is unconditional but a single compare against
`sops->cap_end` minus `begin`; it is not a "fast path" so much as
branch-predicted.

**Slow path body sketch.** Fetch `data()` and `cap_end()` once. If
shrinking or in-place fits, destroy the old slice, memmove (or
relocate-loop) the suffix, set size, return. If growing past capacity:
compute new live, ask `sops->resize` to deliver at least that many bytes
(it relocates everything verbatim), then within the new buffer adjust:
shift the post-`pos_off` tail to the right by `new_n - old_n` using
relocate (after `resize` it is contiguous, but the gap is at the wrong
offset until shifted), set size, return.

### 2.3 `fill_construct`

```cpp
void fill_construct(const type_ops* tops,
                    void* dst,
                    size_t n_bytes,
                    const void* value) noexcept;
```

**Arg order rationale.** Four args. `tops` again first for the flag
read. `dst` and `n_bytes` are the destination span; `value` last because
on the calling side it is typically computed last (it is the address of
the freshly-passed `const T&` parameter). On i386, four pushes; on
ARM-32 all four register-fit; on x86_64 rdi/rsi/rdx/rcx.

No `storage_ops` arg: this is a pure construct-into-uninitialized-bytes
primitive. Its callers have already opened the destination via
`replace_range` or `sops->resize`.

**Semantics.** Construct `n_bytes / tops->size` copies of `*value` at
`dst`. Pre: `dst` is `n_bytes` of uninitialized storage. Post: those
bytes hold initialized copies. Used for "fill" patterns.

**Callers.**
- `vector(n, v)` ctor; `vector::insert(pos, n, v)`;
  `vector::assign(n, v)`; `vector::resize(n, v)` growing — moderate.
- `basic_string(n, c)` ctor; `string::append(n, c)`; `string::insert(pos,
  n, c)`; `string::assign(n, c)`; `string::resize(n, c)`; `string::replace
  (..., n, c)` — *very* frequent for character types because `c` is a
  literal and the trivial path is `memset`.

**Fast path.** If `(f_triv_copy & f_alloc_default_life)` and
`tops->size == 1`: `__builtin_memset(dst, *(unsigned char*)value,
n_bytes)`. If `(f_triv_copy & f_alloc_default_life)` and
`tops->size > 1`: write the first element with `memcpy` then double-and-
copy (`memcpy(dst+es, dst, es); memcpy(dst+2*es, dst, 2*es); ...`) until
done — `__builtin_memcpy` with constant size folds well, but the loop
form is fine too; subject to measurement, may simply do an
elementwise-`memcpy` loop. Either way no `tops->copy_construct` calls.

**Slow path body sketch.** Loop from `dst` to `dst + n_bytes` stepping
by `tops->size`, calling `tops->copy_construct(el_ctx, p, value)`. The
allocator ctx is **not** an arg to `fill_construct`; callers pass
`nullptr` and only the lifecycle leaves of allocators with
`f_alloc_default_life == 1` (which folds to placement-new) are valid
here. *Open question, §5: should `fill_construct` accept an
`el_ctx` arg to support stateful allocators in `assign(n, v)`?*

### 2.4 `copy_construct_range`

```cpp
void copy_construct_range(const type_ops* tops,
                          void* dst,
                          const void* src,
                          size_t n_bytes) noexcept;
```

**Arg order rationale.** Four args. Same justification as
`fill_construct`. `src` precedes `n_bytes` because the natural calling
convention reads "copy from src for n_bytes" left-to-right; matches the
standard `memcpy(dst, src, n)` ordering for cache familiarity.

**Semantics.** Copy-construct `n_bytes / tops->size` elements from
`src` to uninitialized `dst`. Pre: `dst` is uninitialized for
`n_bytes`; `src` and `dst` do not overlap (they live in independent
allocations). Post: `dst` holds `n_bytes` of copy-constructed elements.

**Callers.**
- `vector` copy ctor; `vector(first, last)` range ctor;
  `vector::assign(first, last)` (grow leg); `vector::operator=` (rhs
  copy) — moderate.
- `string` copy ctor; `string(first, last)`; `string(s, pos, n)`;
  `string::append(s)`; `string::assign(s)`; `string::operator=`;
  `string::insert(pos, s)`; substr; `string::replace(pos, n, s)` —
  frequent, almost always trivial path = memcpy.

**Fast path.** `(f_triv_copy & f_alloc_default_life)`:
`__builtin_memcpy(dst, src, n_bytes)`.

**Slow path body sketch.** Loop from offset 0 to `n_bytes` stepping
`tops->size`, calling `tops->copy_construct(nullptr, dst+i, src+i)`.
Same `el_ctx` open question as §2.3.

### 2.5 `equal_bytes`

```cpp
bool equal_bytes(const type_ops* tops,
                 const void* a,
                 const void* b,
                 size_t n_bytes) noexcept;
```

**Arg order rationale.** Four args, all register-fit on every target.

**Semantics.** Pre: `a` and `b` each name `n_bytes` of constructed
elements (sizes assumed equal — the caller's size check is the
short-circuit). Post: returns `true` iff every pair compares equal via
`tops->equal`. Trivial path: `memcmp`.

**Callers.** `vector::operator==`, `basic_string::operator==` (and the
`==` half of `<=>` for strings of equal length). Rare in core paths,
common in test code.

**Fast path.** `(f_triv_copy & f_alloc_default_life)` →
`__builtin_memcmp(a, b, n_bytes) == 0`. For string with
`is_trivially_copyable_v<CharT>` this is always the case; equality of
strings is one memcmp.

**Slow path body sketch.** Step through both ranges in parallel calling
`tops->equal(a+i, b+i)`; bail to `false` on first mismatch.

### 2.6 `compare_bytes`

```cpp
int compare_bytes(const type_ops* tops,
                  const void* a, size_t a_bytes,
                  const void* b, size_t b_bytes) noexcept;
```

**Arg order rationale.** Five args. On x86_64 fits in
rdi/rsi/rdx/rcx/r8; on AArch64 in x0-x4; on ARM-32 the fifth (`b_bytes`)
spills. The unequal-size case is common (string `<=>`), so passing both
sizes is required. Order: `(a, a_bytes)` together then `(b, b_bytes)`
together so the two operands stay co-located in the call-site
push/argument sequence.

**Semantics.** Three-way compare: return -1/0/+1 for less/equal/greater
under elementwise `tops->compare`, with shorter ordered first when one
is a prefix of the other.

**Callers.** `basic_string::compare` and `string::operator<=>` —
moderately frequent. `vector::operator<=>` — rare. Not a hot path but
the body would otherwise be inlined and duplicated.

**Fast path.** `(f_triv_copy & f_alloc_default_life)` and
`tops->size == 1`: `__builtin_memcmp(a, b, min(a_bytes, b_bytes))`,
then resolve the tie by size comparison. For multi-byte trivial
elements: a typed-element memcmp is not safe in general (endianness,
padding), so the slow loop is taken. *Subject to measurement: for
`tops->size > 1` with trivial layout and known endianness we *could*
use memcmp; deferred until justified.*

**Slow path body sketch.** Step `min(a_bytes, b_bytes)` in parallel
calling `tops->compare`; bail on first nonzero with that sign; if loop
exhausts and one range is longer return the sign accordingly.

## 3. Considered and rejected

- **`grow_to(tops, sops, st_ctx, min_bytes)`** ("reserve"). Body is one
  compare against `sops->cap_end` and a tail call to `sops->resize`.
  Inlined at the call site that is one cmp + one branch + one call; as
  a core it is one call to it plus two arg-setup pushes — net loss.
  Plus, `reserve` is rare in real code.
- **`destroy_and_free`** (destructor sequence). Two calls
  (`truncate_to(0)` then `sops->free`) at the destructor site cost
  exactly the same as one call to a combiner core; the combiner saves
  nothing.
- **`assign_range(tops, sops, st_ctx, src, n_bytes)`**. This is just
  "truncate to min(old, n) + replace_range or copy_construct_range +
  conditional construct loop" depending on which is bigger; no shared
  body that isn't already in the cores above. The two- or three-step
  callsite expansion is what matches the standard's
  copy-assign-then-construct-tail / destroy-tail dichotomy directly.
- **`move_construct_range`** as distinct from `copy_construct_range`.
  Inside the trivial path both are `memcpy`; outside it the leaf
  differs only by `&` vs `&&`. Could be a parameter but the savings
  are zero — `replace_range`'s relocate already handles the only place
  type-erased moves happen (suffix-shift during insert/erase). Outside
  that, callers that move-construct are doing copy-ctor from rvalue
  ranges, vanishingly rare in container code.
- **`fill_assign(tops, dst, n, value)`** (copy-`=` into already-live
  range). Used by `assign(n, v)` shrink leg. Body is a loop that's
  trivially folded to `memset`/`memcpy` at the call site — and the
  trivial path is the only path that matters for char/int. Not worth
  the symbol.
- **`swap_storage`**. The whole work of swapping vector/string state
  is three pointer swaps (Strategy A) or one pointer swap (Strategy
  B). Cores don't help.
- **`relocate_range`** as a standalone callable. It is the *body*
  of `replace_range` step 3 and `sops->resize`'s internal step; it
  already exists at storage_ops layer via `relocate_live`. Exposing
  it independently to consumers would only invite misuse.

## 4. Validation plan

**Methodology baseline.** A consumer TU named `consume.cpp` that
instantiates `std::vector<int>`, `std::vector<std::string>`,
`std::basic_string<char>`, and `std::vector<X>` where `X` is a
non-trivially-relocatable class with a user dtor; the TU contains 40-60
calls covering `push_back`, `emplace_back`, `insert(pos, v)`,
`insert(pos, n, v)`, `insert(pos, first, last)`, `erase`, `clear`,
`resize` (both directions), `assign(n, v)`, `assign(first, last)`,
`operator=`, `==`, and `<=>`. Built `-Os -fno-exceptions -fno-rtti` for
i386, arm-linux-gnueabihf (Thumb-2), x86_64, and aarch64. Measure both
per-call-site bytes (objdump the consumer; cut at arg-setup start to
post-`call`) and the `support.cpp` core symbol sizes (`nm --size-sort
support.o`). The metric: `Σ(callsites × per-call bytes) +
Σ(core body bytes) + Σ(per-(C,A) storage_ops rodata)`. Each
non-obvious choice below has a directed measurement.

**4.1 `replace_range` collapsing five cores into one.** Compare against
a five-core split (`insert_room`, `erase_range`, `resize_grow`,
`resize_shrink`, `emplace_room`). Predict the unified core wins on
i386 (fewer instantiations of arg setup) and arm-32 (shared dispatch
hot path) and approximately ties on x86_64 (where the 6th arg costs
one extra MOV). Numbers we'd cite: total `text` on i386 across the
consumer + core, per-callsite bytes for `insert` and `erase` separately.

**4.2 `fill_construct` and `copy_construct_range` as two cores vs one
with stride.** Build the consumer twice. Hypothesis: separating them
saves bytes in the body (no `if (stride == 0) ... else ...` branch,
no extra arg) and the caller's setup cost is unchanged because every
caller statically knows which it wants. Measurement: i386 core-symbol
size for the two-vs-one variant.

**4.3 The trivial fast paths in `truncate_to`, `replace_range`,
`fill_construct`, `copy_construct_range` are flag-checked at runtime in
the core rather than at compile time in the caller.** This is the
type-erasure trade. Predict: the runtime check costs ~3 instructions
(test, branch on zero, fall through to memset/memcpy), one-time per
call to the core; the alternative (compile-time check in the caller)
duplicates the fast and slow path at every call site. Measurement:
build a variant where the caller inlines an `if constexpr` on
`is_trivially_copyable_v<T>` and dispatches to two different cores; the
caller-side bloat on i386 should clearly lose.

**4.4 `equal_bytes` and `compare_bytes` as separate cores.** Predict
the equality short-circuit on size mismatch is a 2-byte cost at the
callsite and the body is then a fixed `n_bytes` memcmp — strictly
smaller than a combined comparator with three-way semantics. Measure:
total bytes for the four operators in the consumer.

**4.5 `replace_range` returning `void*`.** Predict: returning the
insertion pointer saves the caller from re-fetching `data() + pos_off`
post-call (a load through `sops->data` if the pointer is volatile, plus
an add). On i386 that's ~7 bytes saved per call to `replace_range` that
is followed by a construct call; cumulative win across `push_back`,
`insert`, `emplace`. Measure: callsite bytes for `vector<int>::insert`
with and without the return.

**4.6 Arg position of `tops` first.** Predict: every core reads
`tops->flags` immediately, so on x86_64 `rdi` is consumed first and
freed for scratch quickly. On i386 it doesn't matter — every arg is on
the stack. On ARM-32 `r0` likewise. Confirm via objdump on the bodies.

## 5. Open questions

- **Allocator ctx threading through `fill_construct` and
  `copy_construct_range`.** Stateful allocators (`f_alloc_ctx == 1`)
  need an instance pointer for `tops->copy_construct`. Either (a) add a
  fifth `void* el_ctx` arg to both cores, or (b) require these cores to
  be called only when `f_alloc_default_life` so they take the trivial
  path; the slow path uses a different core that does take ctx. Settle
  by counting how many real call sites are inside containers with
  stateful allocators (probably ≤ 5 in the test suite); if (a) costs
  the trivial fast path one wasted register or push everywhere, prefer
  (b). The measurement is the i386 callsite byte delta of the two
  variants on the consumer TU.

- **Does `replace_range` need a separate "construct fast" return form
  for `emplace_back`?** `emplace_back` is `replace_range(..., live, 0,
  es)` followed by placement-new of one element. The push-back path is
  so hot that even one extra register-spill on x86_64 (the 6th arg)
  may justify a 4-arg `extend_one(tops, sops, st_ctx)` -> `void*`
  variant. Decide by measuring i386 and ARM-32 binary size of a TU
  that does only `push_back` in a tight loop, both ways.

- **`compare_bytes` memcmp shortcut for `sizeof(T) > 1` trivial
  types.** Endianness-safe for native arithmetic comparison of `T`
  iff `T` is `unsigned char` (which is the size-1 case anyway). For
  `wchar_t`/`char16_t`/`char32_t` element-by-element comparison
  matters semantically. Resolution: the fast path stays at `size==1`;
  re-examine if a profile shows `string<char16_t>::compare` as a
  measurable cost.

- **Spare-slot maintenance during `replace_range` step 3.** When the
  suffix shift overwrites the old trailing zero, the recomputation at
  step 4 (`sops->set_size`) restores it. If a future use of
  `replace_range` ever reads the spare slot mid-call this assumption
  breaks. Confirm by inspection of the storage_ops contract; settled
  for now.

- **Whether `vector::resize(n)` (default-construct growth leg) deserves
  a seventh core `default_construct_range` instead of constructing
  inline at the caller after `replace_range`.** Body would be 8 lines
  (memset fast path + leaf loop). Caller saves one call-arg sequence
  per `resize` site. Measure on a TU with three `resize(n)` calls
  versus three inlined construct loops; if savings <8 bytes per
  callsite on i386, leave it inline.
