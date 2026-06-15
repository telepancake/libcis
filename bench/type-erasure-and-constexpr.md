# Type erasure, constexpr, and where C++ container code size actually goes

Empirical findings from probing one question: **can a type-erased container
implementation meaningfully shrink `.text`, and what does keeping C++20
`constexpr` cost?** Every number here was measured against libcis (g++-10,
`-std=gnu++20 -fno-exceptions -fno-rtti`) at `-Os`, mostly under the bench's
flags-only LTO baseline (see `bench/README.md`). Reproduce via the drivers in
`bench/experiments/evector/` and `bench/codesize.py`.

The headline: type erasure is real but bounded, the compiler does more of it for
free than folklore claims, *and* keeping `constexpr` is fundamentally at odds with
the size-optimal implementation. Several widely-believed "facts" turned out wrong
under measurement; those corrections are the valuable part.

---

## 0. The technique under test

The classic type-erased container (the user's original proposal):

- a `constexpr` **`type_ops` table** per element type — function pointers for the
  type-dependent leaf ops (relocate/copy/destroy/…) plus `sizeof`/`alignof`;
- the heavy container logic as **non-template `core_*` functions** taking a
  `const type_ops&`, operating on raw bytes;
- a thin `evector<T>` wrapper that calls the core with `ops_for<T>`.

Files: `type_ops.hpp`, `evector.hpp`, `evector_dual.hpp` (the constexpr-keeping
variant), drivers `vector_std/erased/dual.cpp`.

---

## 1. The win is real, but bounded (and LTO eats part of it)

`evector` vs `std::vector` over 10 element types, **marginal `.text`** over the
empty-`main` baseline:

| build | std::vector | evector | saving |
|---|---|---|---|
| plain `-Os` | +4226 | +3066 | **−1160 (−27%)** |
| LTO baseline | +3592 | +2876 | **−716 (−20%)** |

So ~20% under our real (LTO) baseline on a *minimal* vector surface. LTO narrows
the gap because it does cross-TU work the naïve build doesn't.

**Correction banked here:** "19% is only a small win" was wrong framing — it's the
same order as the entire LTO win. But "this scales to 60%" is equally wrong: there
is no single library refactor that yields 60% on already-LTO'd, no-EH/no-RTTI code.

---

## 2. constexpr fundamentally requires *typed* storage

What is and isn't allowed during constant evaluation (verified on g++-10):

| operation | constexpr? |
|---|---|
| `reinterpret_cast<T*>(bytes)` | ❌ hard error — **unconditional**, "correct type" irrelevant |
| placement-new `::new(buf) T(...)` | ❌ `operator new(size_t,void*)` isn't constexpr |
| `malloc`/`free`, `memmove` over `void*` | ❌ |
| `void*`→`T*` via **`static_cast`** (round-trip to a real T) | ✅ |
| `allocator<T>::allocate` + `construct_at`/`destroy_at` | ✅ |

This is *why* `std::vector` is constexpr in C++20: it keeps typed `T*` storage from
`allocator<T>::allocate` and builds elements with `construct_at`. The constant
evaluator models *typed objects*, not bytes. A type-erased container (bytes +
`reinterpret_cast` + `memmove`) is, by construction, not constexpr.

---

## 3. Why LTO does NOT auto-share the "type-independent" code

It's tempting to think LTO already outlines the type-independent growth/capacity
logic. It does not. Same routine, `SZ` (standing in for `sizeof(T)`) supplied three
ways, `-Os -flto`:

| `SZ` is… | distinct bodies | `.text` |
|---|---|---|
| a template parameter (the `vector<T>` model) | **5** | 729 |
| a runtime argument | **1** | 416 |
| a runtime argument, `noclone` | **1** | 472 |

A→B is **−43%**, and LTO left all of it on the table. Reasons:

1. **"Type-independent" in source ≠ identical machine code.** Growth/`__recommend`/
   relocate bake `sizeof(T)` in as a *compile-time constant*, so each instantiation
   is genuinely different bytes.
2. **LTO's dedup tools don't apply.** ICF folds only *byte-identical whole
   functions*; these aren't identical. The pass that extracts a common
   *instruction subsequence* from differing functions is the **machine outliner —
   clang/LLVM only; GCC has none.**
3. **There is no de-specialization pass, by design.** LTO exists to specialize and
   inline for speed; collapsing N specializations into one runtime-parameterized
   function is the opposite, a size-for-speed trade only a human makes.

So sharing requires the programmer to make `sizeof`/ops **runtime** values — i.e.
type erasure. The compiler will not do it for you.

**Correction banked:** my earlier "the type-independent code can just be outlined
to shared helpers (and stays constexpr)" was wrong — if `sizeof(T)` still flows in
as a constant, LTO re-specializes the helper per T (the 5-body row). Real sharing
*is* the runtime-parameterization that breaks constexpr.

---

## 4. Element ops are NOT an irreducible per-type tax

The bigger correction. copy/move/destroy/etc. look type-specific, but:

- **Trivially-relocatable types** (scalars, pointers, POD aggregates — most element
  types): copy=`memcpy`, move=`memmove`, destroy=nop — **size-only**. With size as
  a runtime parameter (which erasure already provides) plus an
  `is_trivially_copyable` dispatch, **all of them route through one `memmove`
  core**. Measured: **9 distinct trivial element types collapsed to 1 shared body.**
  Per-type `.text` for trivial types → ≈0 (just the ops-table constant).
- **Non-trivial types** (`string`, user copy/move/dtor): the per-type "leaf" is a
  thin **adapter thunk** that calls T's *own* copy/move/dtor — and those real
  bodies exist **once per type regardless of containers**.

Caveat the disassembly exposed: the compiler will **not** do the trivial collapse
from a naïve move-loop — it emits type-specific register moves (`int`/`unsigned`
folded at 0x17 bytes, but `float`=0x1b, `long`=0x19, `double`=0x1b differ). The
**library** must explicitly dispatch to `memmove`. Once it does (what
libstdc++/libc++ already do as the trivial-relocatability optimization), the
collapse is total.

So the genuinely irreducible per-type container tax is: an ops-table constant
always, plus a thin thunk for *non-trivial* element types only. Trivial element
types pay essentially nothing — which raises the technique's ceiling well above the
naïve prototype's 20%.

---

## 5. The generic type-erased core is NOT duplicated per type

A real worry: with `ops_for<T>` a compile-time constant at each call site, would
IPA-CP clone the shared core per type (to devirtualize), re-monomorphizing it?
**No — on any compiler at `-Os`.** 16 element types through one table-driven core,
DCE-proof workload, `-Os -flto`:

| compiler | core bodies | `.text` | strategy |
|---|---|---|---|
| g++-10 | **1** | 789 | one shared out-of-line core, dispatch through table |
| g++-13 | **1** | 818 | same |
| clang-18 | **0** | **560** | inlines + devirtualizes + fuses (smaller) |

GCC keeps one shared core (same at `-O2`); clang fuses and comes out smaller.
Neither duplicates per type.

**Correction banked:** the prototype originally marked the core
`__attribute__((noinline, noclone))` to "force" sharing. That was solving a
non-problem — at `-Os`, IPA-CP is size-conservative and won't clone the core 16×
anyway. The attributes only *fought* the compiler (never smaller, sometimes
bigger), and were removed. (Open caveat, *not* validated: clang's inline-everything
could inline a *large* real core per **call site** in multi-TU code — a different
axis than per-type.)

---

## 6. What keeping constexpr actually costs

The `dual-path` attempt to have both — one method, two implementations chosen by
`std::is_constant_evaluated()`: a typed `allocator<T>`/`construct_at` path for
constant evaluation, the erased byte core at runtime. The `reinterpret_cast` in the
runtime branch is legal to *appear* because it's never *evaluated* at compile time.
`constexpr` is genuinely retained (a `static_assert` over `dvector<int>` compiles
against libcis).

Its cost, measured two ways:

**(a) Whole-program-LTO executable** — the dead typed branch mostly DCEs:

| | marginal `.text` (LTO) | vs std |
|---|---|---|
| std::vector | +3592 | — |
| erased | +2876 | −716 (full win) |
| dual (constexpr kept) | +3052 | −540 |

Dual keeps **~75%** of the win; constexpr costs ~25%. Under plain `-Os` it keeps
only **~28%** (the dead branch DCEs poorly without whole-program view).

**(b) Exported / separately-compiled library** (`template class dvector<int>` +
`<std::string>`, i.e. the extern-template technique) — the typed code does **not**
disappear:

| | `.text` (all sections) |
|---|---|
| erased instantiation | 1389 |
| dual instantiation (constexpr kept) | **1984 — +595 B, +43%** |

`nm` confirms `dvector<int>::grow_typed()` and `dvector<std::string>::grow_typed()`
survive as exported weak symbols. **Exported symbols can't be garbage-collected**,
so the entire typed `constexpr` machinery rides along in the ABI.

**Correction banked (the big one):** "the dead constexpr branch is magically DCE'd"
is false. Intra-function, the branch's *instructions* are removed (standard). But
template instantiation still emits the typed branch's *callees* as weak symbols,
reclaimed only if **unreferenced AND not exported**. In a whole-program-LTO exe:
mostly reclaimed (the ~75% case). In an exported library: **kept** (+43%).

---

## 7. Conclusion

For a constrained-production, size-focused standard library that is header-only
*or* ships exported instantiations:

1. The size-optimal container is **type-erased + trivial-relocatability `memmove`
   core + a per-type ops table**, with thunks only for non-trivial element types.
   That is fundamentally a **runtime** construct.
2. C++20 `constexpr` containers require **typed** storage and are therefore
   incompatible with #1 except via `dual-path`, which:
   - **doubles the source** of every container method (against a "minimal,
     readable" charter);
   - keeps only ~75% of the win under whole-program LTO, ~28% under plain `-Os`;
   - **conflicts with extern-template instantiation** — exporting a constexpr
     container drags its full typed machinery into the ABI, un-collectible (+43%).
3. Therefore **dropping C++20 constexpr container support is a justified,
   now-quantified trade** for this kind of library: it buys the last ~25%, avoids
   doubling the source, and unblocks extern-templates. The cost is conformance with
   a feature whose value to constrained production code is marginal.

The deeper lesson: `constexpr`, as standardized, assumes a *typed-object* execution
model; the *size*-optimal model is *untyped bytes + runtime dispatch*. They pull in
opposite directions, and no optimizer reconciles them for free.

---

## 8. Reproduce

```sh
# the win, and the dual-path cost, vs std::vector (over 10 element types)
bench/codesize.py baseline vector_std vector_erased vector_dual          # LTO baseline
bench/codesize.py --no-opt baseline vector_std vector_erased vector_dual # plain -Os

# §3 (LTO won't de-specialize), §4 (trivial ops collapse), §5 (core not cloned),
# §6 (constexpr legality, exported-instantiation cost): see the one-off probes in
# this investigation; all are a few-line g++-10/g++-13/clang-18 compiles.
```

All `.text` figures are from `size -A` on g++-10 unless a compiler is named.
Numbers move with libcis and the toolchain; re-measure before relying on any.
