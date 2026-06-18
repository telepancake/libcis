# cores-design v5/v6/v7 — what doesn't survive a check against the tree

Read 1:1 against the in-tree code (`include/bits/{vector_cores.h,type_ops.h,
storage_ops.h}`, `src/support.cpp`, the user headers each note cites).
Most descriptions of the *current* code are accurate; what fails is mostly
in (a) the v5 migration plan, (b) one factual claim about `<sstream>`, and
(c) a handful of consistency / measurability issues in v6/v7. Catalogued
below, ordered most-to-least load-bearing.

---

## A. Failures of fact about the tree

### A1. v5 §8 step 2 is wrong: the tree compiles as-is

v5 says:

> Update `src/support.cpp` line 20 from `#include <bits/vector_cores.h>`
> (file removed in `debf4cd`) to `#include <bits/cores.h>`. **The current
> source doesn't compile** without this — `debf4cd` removed the header but
> didn't fix the include.

Check the tree:

- `include/bits/vector_cores.h` exists (89 lines, fully populated with
  the five forward declarations).
- `src/support.cpp` line 20 reads `#include <bits/vector_cores.h>`.
- `include/bits/cores.h` does not exist.

So either `debf4cd` was reverted (most likely) or never landed. Either way,
v5's framing — "the first step of landing v5 is to fix a broken include"
— is false against today's tree. v5 is not "naming what already shipped";
it is proposing a rename that has not happened. The header is still called
`vector_cores.h`, and the only delta to "land v5" is the rename + the mask
fix (A2). The rename is cosmetic (no behavior change); the mask fix is
real.

### A2. v6 §2.4.3 — `basic_stringbuf::xsputn` does not exist in this tree

v6 says:

> `basic_stringbuf<CharT>` overrides `xsputn` / `overflow` to grow its
> internal `basic_string<CharT>` buffer. The `xsputn(s, n)` body is "if
> room, memcpy n*sizeof(CharT) bytes and bump pptr; else overflow".

Check `include/sstream`: the class defines `overflow`, `underflow`,
`pbackfail`, `setbuf`, `seekoff`, `seekpos`, `sync` — but **no `xsputn`
override**. `basic_stringbuf` here inherits the default `streambuf::xsputn`,
which loops on `sputc` and routes through `overflow`. The "shared byte
work" v6 wants to factor into `sb_xsputn_bytes` does not exist as a unit
in this codebase; it's a per-character fallback distributed across
`overflow` + the inherited `xsputn`. The v7 stage-3 commit (port
`<sstream>` to `sb_xsputn_bytes`) would have to first *add* the override
and then port it. The bench claim (−0.4 KB on dual-CharT programs) is
gated on changing the implementation strategy, not on factoring existing
duplication. Either re-frame the stage as "add an xsputn override and
core it" or drop it.

(The `overflow` body itself does grow the underlying `basic_string` via
`str_.push_back / str_.resize`; that already routes through the v5 cores
transitively. There is no shared work missed.)

### A3. v5 §10 open question conflates two things

v5 §10 ("should `set_size` also be a core?") describes `set_size_elems_`
as the candidate. The check:

- `set_size_elems_` exists in `<vector>` (line 112) and is called at 6
  sites in vector (lines 281, 290, 301, 309, 741, 749). Not "~20 sites"
  as v5 claims.
- String has its own `null_terminate_()` defined but UNUSED (1 occurrence,
  the definition; 0 callers). That confirms v5 §12's "spare slot is the
  null terminator, storage_ops zeroes it" — but the still-extant function
  is a dead-code smell, not a load-bearing piece.

The 6-vs-20 difference matters because v5's bench projection ("~5 instan­
tiations × 3 programs × 100 B = 1.5 KB") was already marginal at 20
sites; at 6 it's not measurable. The open question can be closed faster
than v5 implies: no.

---

## B. The latent bug v5 surfaces is real and worth fixing

### B1. vector's `ops_table_()` mask is missing `OP_COPY_CONSTRUCT`

Confirmed against `include/vector` lines 216-218:

```cpp
static const detail::type_ops* ops_table_() noexcept {
    return &detail::ops_for<value_type, allocator_type,
                            detail::OP_DESTROY | detail::OP_MOVE_CONSTRUCT>;
}
```

And `<vector>` line 783 calls `construct_copy_one_n` (in
`insert(pos, n, x)`), whose `support.cpp` body (line 79) calls
`ops->copy_construct(...)` on the non-trivial path. With the current mask,
`copy_construct` is null for any `T` that isn't trivially-copyable +
default-life, and the call will crash.

The trivial fast path (lines 72-76) does cover the common case
(`vector<int>::insert(pos, 5, 42)` etc.) which is why the bug hasn't
shown up in tests. `vector<std::string>::insert(pos, 3, "abc")` would
trip it.

v5 §4 names this correctly; v5 §8 step 3 prescribes the one-line fix.
This is genuine, independent of the rest of v5.

(String's mask is genuinely safe — it never calls `construct_copy_one_n`;
its broadcast goes through `traits_type::assign`. v5 says the same.)

---

## C. Claims that depend on bench probes that don't exist

The v5/v6/v7 commit sequences and bench projections are predicated on
probes the bench doesn't yet contain. From `bench/codesize.py` (not
verified here, but flagged in the notes themselves):

- v6 stage 4: "two-deque-flavor probe" — v6 §7 says "doesn't exist yet;
  needs to be added".
- v6 stage 5: claims `equal_bytes`/`find_byte` save 0.3-0.7 KB on "every
  program with `<algorithm>`" — no probe identified.
- v7 stage 1: "needs a variant-heavy probe first".
- v7 stage 2: "shared_ptr_probe.cpp" — doesn't exist.
- v7 stage 3: "sstream_probe.cpp" — doesn't exist; and see A2, it would
  measure zero against the current implementation.
- v7 stage 4: "valarray_probe.cpp" — doesn't exist; v7 even hedges
  ("may be a wash on programs that don't use valarray (most)").
- v7 stage 5: "filesystem_probe.cpp" — doesn't exist.
- v7 stage 6: "async_probe.cpp" — doesn't exist.
- v7 stage 7: explicitly a measurement-only commit.

Per CLAUDE.md ("Size claims without a journal entry are not claims"):
seven of the v7 stages cannot ship as written, because the bench has
nothing to measure them against. v6 stage 4 and stage 5 have the same
problem. The stages are real proposals but the bench gates have to be
built first; the migration order conflates "design a stage" with "ship a
stage" and so over-promises ship-ability.

This is mostly a sequencing fix in the notes ("v7 stage 0: add probes
A-F before any other v7 stage lands"). It's not a wrongness in the design
proposals themselves — but a reader following the notes literally would
ship stages that cannot be honestly bench-recorded.

---

## D. Soft claims worth tightening

### D1. v5 §12 "spare-slot ownership" needs a tree-pointer

v5 §12 says "the cores **never** write the spare slot themselves; they
trust the storage_ops slots to maintain it." Check `storage_ops.h`:

- `storage_a_set_size` (line 114) memsets the spare slot.
- `storage_a_resize`   (line 188) memsets the spare slot.
- `storage_b_set_size` (line 272) memsets the spare slot.
- `storage_b_resize`   (line 336) memsets the spare slot.

The rotate core in `support.cpp` lines 150-310 (skimmed) calls
`sops->set_size(...)` to apply the size delta, so the spare-slot
maintenance does ride on `storage_ops`. Claim survives. Worth a one-line
citation in v5 §12 so a future reader doesn't have to re-derive it.

### D2. v6 §2.5 misstates `any`'s structure: per-T handler vs union dispatch

v6 says "each Handler is a switch over `Action`". Check `include/any`
lines 256-263:

```cpp
struct SmallHandler {
    static void* handle(Action act, const any* self, any* other,
                        const void* fallback_id) {
        switch (act) {
        case Action::Destroy: ...
        case Action::Copy:    ...
        case Action::Move:    ...
        case Action::Get:     ...
```

Confirmed. The redundancy v6 names is real. The proposed `value_ops`
table — 5 fields, lifecycle leaves + type_id + flags — would replace the
per-T switch. The proposal is sound.

One thing v6 doesn't say but should: the current `Handler` already takes
~3 fields' worth of context (`const any*`, `any*`, `const void*`), so the
per-call overhead of the proposed scheme (one indirect call per leaf
instead of one indirect call into the switch + one switch jump) is
slightly LOWER than today's, not just equal. The bench claim ("Programs
using `any` for more than three types win") is conservative.

### D3. v6 §2.6 `function`'s vtable cost under `-fno-rtti`

v6 §2.6 says:

> the **vtable forces RTTI-like data**: gcc-10 emits one vtable symbol
> per `(F, Sig)` pair, each carrying typeinfo unless you're at
> `-fno-rtti` (libcis is, so the typeinfo slot is null, but the vtable
> itself stays).

This is correct as a high-level summary, but the v7 §2.3.2 version of
the same point (about `sp_counted_base`) is more careful:

> Under `-fno-rtti` gcc still emits the typeinfo pointer slot in the
> vtable as null, plus the vtable itself is keyed on the concrete-class
> type ...

v6 should match v7's precision: the "RTTI-like data" is the typeinfo
pointer slot (still emitted, populated with null), not separate typeinfo
records. The motivation for replacing the vtable stands either way.

### D4. v7 §6 invariant adds a contradiction with v5 §3.5

v7 §6 says:

> **Vtables that exist because the library couldn't share bodies are
> fair game.**

But v5 §3.5 ("No `swap_to` / `alloc_only` slots") says the storage_ops
should NOT grow even when there'd be a shared body. The v5 rule is a
single-instance "no, the prompt-1 misfire is rejected"; the v7 rule is a
general "when bodies fold pairwise, slot them". They're consistent on the
specific cases (storage_ops is already a vocab table, not a vtable, so
v5 §3.5 isn't an exception to v7 §6) — but a future agent reading v7 §6
in isolation might use it as license to add a `swap_to` slot. Either
v5 §3.5 needs forward-referencing in v7, or v7 §6 needs an explicit
note that it doesn't authorize growing the v5-frozen tables.

### D5. v6 §2.4 deque cores — `segmented_construct_at_back` rejection has a wrong reason

v6 §2.4 says:

> The trivial-fast-path collapse to memcpy only wins when the count *per
> segment* is large, and deque's segment size is bounded above by
> `4096/sizeof(T)`.

Check `<deque>` line 30 — `deque_block_size` is defined but the actual
upper bound the comment cites isn't 4096. (The actual constant depends
on `sizeof(T)`; libc++'s policy is `max(16, 4096 / sizeof(T))` if I
remember right.) The argument carries — small per-segment count, memcpy
fast path doesn't help — but the specific bound should be re-derived
from the actual `deque_block_size` formula in the tree.

### D6. v7 §2.4.3 sstream — see A2

Crossreferenced; the core proposal is built on a false premise.

---

## E. Minor inaccuracies

### E1. v5 §6 lists string modifiers using `construct_copy_one_n`

v5 §6 ("string — current consumer") says string "happens to bypass
`construct_copy_one_n` for the `char`-broadcast fast path" — but in fact
string does not call this core at all today (`grep construct_copy_one_n
include/string` is empty). The wording "happens to bypass" implies a
fall-through; the truth is it's never wired. Cosmetic.

### E2. v7 §0 line count for `<regex>` is wrong

v7 §2.7: "per-`(CharT, Traits)` instantiated. For `char` and `wchar_t`
users (the common case), that's two copies of a 2000+ line algorithm."

Actual `wc -l include/regex` = 4977. The argument doesn't depend on the
exact size, but the "2000+" suggests v7 estimated from memory rather
than checking. Other v7 size claims (algorithm 6210, tree 1143,
hash_table 1801, chrono_tzdb_data 7208, format 8284, locale 6830,
charconv 1116, random 3935, ranges 3973, deque 1649, filesystem 2961,
future 1219, memory 2877, variant 1705, any 409) all match exactly.

### E3. v5 §13 "10 distinct codegen shapes" arithmetic

v5 §13: "5 cores × 2 paths (trivial/leaf) × 1 leaf kind (byte-walking
loop) = 10 distinct codegen shapes". Counting the actual support.cpp:

- `grow` — no triv path of its own (delegates to resize).
- `destroy_range` — triv (skip) + leaf (loop).
- `construct_copy_one_n` — triv (memcpy loop) + leaf (loop).
- `rotate` — triv (memcpy/memmove triad) + leaf (block-swap via scratch
  + relocate_block / relocate_block_rev). Two leaf shapes, not one.
- `relocate_live` — triv (memmove) + leaf (relocate_block forward only).

So it's closer to 9 shapes, not 10, and one of them (rotate's leaf) is
two-headed. The "1 leaf kind" assertion is a simplification; the
relocate_block/rev split is a leaf-shape divergence that the v5 invariant
list doesn't acknowledge. Doesn't change the conclusion, but the
combinatorics paragraph is loose.

---

## F. The good news

The bulk of v5/v6/v7 descriptions of the *current code* are accurate:
- type_ops / storage_ops shapes match exactly.
- The five cores' calling convention matches `bits/vector_cores.h`.
- vector and string callsites match the modifier tables in v5 §6.
- Node-container compile-time-erasure analysis (v6 §2.1/§2.2/§2.3) is
  accurate against `bits/list_node`, `bits/tree.h`, `bits/hash_table.h`.
- `any`'s `HandleFuncPtr`/`Action` shape, `function`'s `fn_base`/`fn_func`
  vtable, `shared_ptr`'s `sp_counted_base`/derived hierarchy, `future`'s
  `assoc_sub_state` — all confirmed.
- `cross_ops` is dead in the tree (v7 §5.8's "no callers" claim verified).
- variant's `fmatrix` / `visitation_variant` machinery is present as
  described (v7 §2.1.2).
- deque's block-segment structure matches v6 §2.4.

The invariants in v5 §12, v6 §6, and v7 §6 are internally consistent and
match the code, with the one drafting issue in D4.

---

## G. Recommended actions before "landing" any of v5/v6/v7

1. **Fix B1 (vector mask) as a standalone commit.** This is the only
   real bug exposed. One line; independent of the rest of the design
   docs. Worth a bench-journal entry tagged as a correctness fix.

2. **Decide on the rename (A1) without v5's framing.** The choice is
   "rename `vector_cores.h` to `cores.h` for accuracy" — fine, but it's
   a one-commit cosmetic change with no compile-state urgency.

3. **Strike A2 from v7.** The `sb_xsputn_bytes` core's premise is false;
   either replace the proposal with "add an `xsputn` override to
   basic_stringbuf, then core it" (different scope) or drop it.

4. **Add the missing bench probes before scheduling v6/v7 stages.** Per
   CLAUDE.md the size claims aren't claims until they're measured.

5. **Tighten the soft drafting issues (D1-D6, E1-E3).** None block a
   stage; all reduce future agent confusion.

The core design — the v5 sandwich, frozen — is sound. v6 and v7 extend
it consistently. The catalog above is exclusively "the writeup got X
wrong about the code"; the design intent in each case carries.
