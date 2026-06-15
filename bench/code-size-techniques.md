# Shrinking C++ standard-library code size — a design memo for libcis

How a *standard-library implementation* can reduce the compiled code (`.text`) of
programs that use it. Synthesized from a multi-agent sweep of libc++, libstdc++,
EASTL, ETL, Microsoft STL, picolibc / newlib / llvm-libc, Fuchsia's embedded
toolchain, and the GCC/binutils/LLVM manuals. Primary sources are linked in §8.

**Scope discipline.** Every technique here is something *the library does* — its
headers, type design, instantiation and dispatch strategy, error paths, and the
way it cooperates with the compiler and linker. Advice aimed at the *application*
author ("don't `#include <iostream>`", "`reserve()` your vectors") is deliberately
excluded. Where a technique has both a library form and a user form, only the
library form appears.

---

## 0. TL;DR

If you read nothing else:

**Two wins are already banked** by libcis's setup — but only *partly*:
`-fno-exceptions` and `-fno-rtti` remove most EH/RTTI code. Residue remains
(out-of-line `__throw_*` helpers, bounds-check branches, and — surprisingly —
`.eh_frame`, which `-fno-exceptions` does *not* remove on its own).

**Two things g++-10 simply cannot do** — don't design around them: `-Oz`
(GCC 12+) and the LLVM MachineOutliner (clang-only). A third lever, **ICF
(identical-code folding), is missing only from the *default* `bfd` linker** —
g++-10 drives `gold`/`lld` fine via `-fuse-ld=gold`/`-fuse-ld=lld`, so ICF *is*
available to libcis, just not by default (E3).

**The five highest-value moves for libcis, in order:**

1. **Free build-flag wins, testable today, zero library change:**
   `-ffunction-sections -fdata-sections -Wl,--gc-sections`,
   `-fno-asynchronous-unwind-tables -fno-unwind-tables`,
   `-fvisibility=hidden -fvisibility-inlines-hidden`.
2. **One shared `[[noreturn, gnu::cold]]` failure handler** behind every error/
   precondition site (the libc++ `__libcpp_verbose_abort` / MSVC `_Xlength_error`
   pattern).
3. **Explicit `extern template` instantiation of `basic_string<char>`** (then the
   hottest `vector`/`unordered_map`) into the `support.cpp` libcis already ships.
4. **Outline parameter-independent code** out of the hottest templates (the MSVC
   `basic_string` fast-path/slow-path split).
5. **Partition float↔string conversion** so a program that never formats a float
   never links the decimal-conversion tables.

Items 1, 2, 4 are pure header/build work; 3 and the float partition reuse the
existing compiled TU. Nothing here forces libcis to abandon its header-only model.

---

## 1. The cost model: the life of a byte of stdlib `.text`

A stdlib symbol travels through four stages; a size technique intervenes at one of
them. Knowing *which* stage a technique targets is how you avoid stacking two
tools that do the same job.

```
   (1) INSTANTIATE          (2) INLINE             (3) FOLD              (4) GC
   template<T> emits     -Os inlines small      COMDAT merges        --gc-sections
   one body per T,       bodies into each       byte-identical or    drops sections
   per TU                call site              vague-linkage copies no longer reached
   ─────────────────     ─────────────────      ─────────────────    ─────────────────
   A1 type erasure       A2 outline non-dep.    E2 COMDAT (auto)     E1 gc-sections
   A3 void* allocator    B3 noinline            E3 ICF (gold/lld)    B2 hidden vis.
   A4 if constexpr       C2 cold/outline                            C4 drop eh_frame
   B1 extern template
```

Three load-bearing facts about the pipeline:

- **Stage 1 is where header-only libraries hemorrhage.** Every consuming TU
  re-instantiates everything it touches. The later stages (fold, GC) claw some of
  it back, but the cheapest byte is the one never instantiated. Hence the
  emphasis on type erasure (A1), outlining (A2), and extern templates (B1).
- **Inlining at `-Os` is a double-edged sword.** A body inlined at twenty call
  sites can be larger than one out-of-line copy plus twenty calls. `-Os` already
  tunes for this, but `noinline`/outlining (B3, A2) give the library explicit
  control where the compiler guesses wrong.
- **Fold (3) and GC (4) only reclaim what the library lets them.** COMDAT needs
  bodies to be *identical* (or vague-linkage); GC needs *per-section* granularity
  and unreferenced symbols. A library that emits subtly-different copies, or that
  keeps symbols artificially reachable, defeats both for free.

---

## 2. What libcis's configuration already decides

| Status | Item | Detail |
|---|---|---|
| ✅ Banked | RTTI off | `-fno-rtti`: no `type_info` graph / name strings for polymorphic types; vtable `-1` slot nulled. |
| ✅ Banked | Exceptions off | `-fno-exceptions`: no `.gcc_except_table`, no landing pads; `throw` lowers to terminate. |
| ⚠️ Residual | EH unwind tables | `.eh_frame`/`.eh_frame_hdr` **survive** `-fno-exceptions`; need `-fno-asynchronous-unwind-tables` (C4). |
| ⚠️ Residual | `__throw_*` helpers | Still real out-of-line calls + message strings + bounds-check branches (C1). |
| ✅ Free | COMDAT folding | ELF + GNU ld ≥ 2.8 dedups vague-linkage copies automatically (E2). |
| 🚫 Unavailable | `-Oz` | GCC 12.1+. g++-10 has `-Os` only. |
| 🚫 Unavailable | MachineOutliner | clang/LLVM only; no GCC equivalent. |
| 🟢 Opt-in | Linker ICF | default `bfd` ld has none; g++-10 drives `gold`/`lld` via `-fuse-ld=` for `--icf=safe` (E3). |
| 🟢 Available | `support.cpp` | A compiled TU already exists → extern-template instantiation (B1) is on the table without leaving header-only. |

The practical reading: libcis has already paid for the EH/RTTI wins, so the
*remaining* opportunity is concentrated in **template instantiation volume**
(stage 1) and **error-path glue** (C) — plus a handful of **free build flags**
the bench isn't yet passing.

---

## 3. The technique catalog

Legend: **[header]** stays header-only · **[flag]** a build/link flag · **[lib]**
needs the compiled component (libcis has one) · **[auto]** already in effect.

### A — Cut template-instantiation volume (stage 1; the biggest header-only lever)

**A1. Type-erased, size-/type-independent cores ("hoisting").** **[header]**
Write the bulk of a container against a base independent of the parameter that
varies most, so variants share one body.
- *Mechanism:* fewer distinct instantiations reach `.text` in the first place.
- *In the wild:* **ETL** layers `vector_base` (size/capacity, value-type
  independent) → `ivector<T>` (type-dependent, capacity-independent algorithms) →
  `vector<T,N>` (storage only); "shared base classes (based on type) help reduce
  overall container code size," and `ivector<T*>` is "just one instantiation of
  code for all pointer types." **libc++**'s narrower analog is `__split_buffer`
  as a shared growth core for `vector`/`deque`.
- *Impact:* large for container-heavy programs; the dedup is multiplicative across
  `(T, N)` or `(T, Cmp)` pairs.
- *libcis fit:* high value, high effort. Prefer the **non-virtual** form (outline
  to a free function templated only on the varying type) over ETL's virtual base,
  to respect `-fno-rtti` and avoid reintroducing vtables.
- *Myth check:* libc++ does **not** implement `vector<T*>` over `vector<void*>`.
  That `void*`-core idiom is EASTL/ETL/MSVC-adjacent, not libc++.

**A2. Outline parameter-independent code out of templates.** **[header]**
Move the part of a template body that doesn't depend on the parameters into a
non-template (or less-parameterized) helper; split hot/cold and fast/slow.
- *In the wild:* **MSVC STL** factors `basic_string` mutators into "a non-allocating
  fast path and an allocating slow path … making it more likely for the common
  no-reallocate case to be inlined," and pulled the shrink path out of the realloc
  path "reducing code size for all mutating members." **Chromium**'s guide: "move
  template-parameter-independent code into non-templated helpers."
- *libcis fit:* directly actionable in `string`, `vector`, `unordered_*`, `format`.
  Low risk, no ABI implications.

**A3. A single non-templated, `void*`-based allocator interface.** **[header]**
- *In the wild:* **EASTL** replaces `std::allocator`'s `rebind` member template
  (which "can result in an explosion of templates, code, and performance
  problems") with one non-templated allocator over `void*`.
- *libcis fit:* a real conformance deviation; informative but probably not worth
  adopting for a conforming port. Listed for completeness.

**A4. `if constexpr` / concepts to prune branches.** **[header, auto-ish]**
Compile-time dead-path elimination means the path never reaches `.text`. Already
libcis's house style (per `CONVENTIONS.md`); the actionable bit is *auditing* for
residual runtime `if`/SFINAE that could become `if constexpr`.

### B — Out-of-lining and the header-only tension (stages 1–2)

**B1. Explicit / `extern template` instantiation of heavy common specializations.**
**[lib]** Declare `extern template` in the header; emit the definition once in
`support.cpp`. Consumers reference the symbol instead of re-emitting member bodies.
- *In the wild:* **libc++** does exactly this for `basic_string<char>` (and
  `wchar_t`): ctors/dtor, `assign/append/insert/replace/erase/find/compare/at/
  resize/reserve`, via `__string/extern_template_lists.h` + `src/string.cpp`; also
  some locale facets and `ios`. **MSVC STL** ships throw helpers, iostreams and
  locales in a small separately-compiled DLL.
- *Impact:* removes per-TU duplication of the heaviest, most-used bodies; `string`
  is in essentially every program.
- *libcis fit:* **the highest-leverage "real library" move available without
  leaving header-only**, because `support.cpp` already exists. Start with
  `basic_string<char>`.
- *Risk:* you pick specific specializations (a `u16string`-only program gains
  nothing); definitions must stay ODR-identical to the header.

**B2. Hidden visibility on implementation symbols.** **[flag]**
`-fvisibility=hidden -fvisibility-inlines-hidden` lets the compiler internalize/
devirtualize more, exports fewer symbols, and makes `--gc-sections` and ICF more
effective.
- *In the wild:* **Fuchsia** builds libc++ with `symbol_visibility_hidden`. libc++'s
  `_LIBCPP_HIDE_FROM_ABI` is *hidden visibility + `exclude_from_explicit_instantiation`
  + `abi_tag`* — note it is **no longer** `always_inline`/`internal_linkage` (dropped
  for "debuggability and code size," D49240).
- *libcis fit:* cheap to try as a flag; the `abi_tag` half is irrelevant for a
  static single-version freestanding lib, but the visibility half still helps GC.

**B3. `[[gnu::noinline]]` — and when it actually helps at `-Os`.** **[header]**
*Not* a blanket "stop inlining big bodies" tool. At `-Os` GCC's size-aware inliner
already refuses to duplicate a genuinely large body across many callers (measured:
identical `.text` with and without `noinline` for an 8-caller heavy function), and
it deliberately *inlines* single-caller functions (`-finline-functions-called-once`)
and deletes the standalone copy — where forcing `noinline` makes code *larger*.
`noinline` earns its place in two narrow cases: **(1)** a body the inliner *scores*
as cheap on early IR but which *explodes per call site* after
constant-propagation-driven unrolling / specialization / downstream inlining —
`noinline` stops that multiplication; **(2)** as *enforcement* that code you
deliberately outlined (C1/A2) stays outlined when it has few callers (only a win
when separateness buys something else: `.text.unlikely` placement, cross-TU COMDAT
sharing, or keeping the hot caller small). For the usual "keep rare error code off
the hot path" goal the real levers are **outlining** (C1) + **`cold`** (C2), not
`noinline`.

### C — Error / precondition / termination paths (the under-appreciated win)

**C1. One shared termination handler for *all* error sites.** **[header/lib]**
Funnel every unrecoverable site through a single `[[noreturn]]` function rather
than per-site throwing/aborting code with its own message.
- *In the wild:* **libc++**'s `std::__libcpp_verbose_abort` — one printf-style
  `[[noreturn]]` that all `__throw_*` and assertion paths reach; in
  `-fno-exceptions` builds `__throw_length_error` becomes
  `_LIBCPP_VERBOSE_ABORT("length_error … \"%s\"", msg)`. **MSVC STL** uses tiny
  `[[noreturn]]` per-container wrappers (`_Xlength_error("vector too long")`) that
  all call one exported helper.
- *libcis fit:* **high value, natural fit.** Collapse scattered abort/throw glue
  into one `std::detail::fail(const char*)`, or — where no message is needed — a
  single `__builtin_trap()` (one `ud2`/`brk` instruction). One body, called
  everywhere; the per-site cost drops to a `call` (or a trap).

**C2. Mark error/throw helpers `[[gnu::cold, gnu::noinline]]`.** **[header]**
- *Mechanism (GCC manual):* `cold` → "optimized for size rather than speed,"
  placed in a separate `.text.unlikely` subsection, with branches to it marked
  unlikely; `noinline` → one shared copy. Together they evict rare error code
  from the hot path.
- *Note:* `-freorder-blocks-and-partition` (basic-block hot/cold split) is
  auto-disabled when EH is *on* — but libcis is `-fno-exceptions`, so it's
  available; still, the function attributes are the reliable lever.
- *libcis fit:* trivial; apply to the §C1 handler and to bounds-check helpers.

**C3. Tunable hardening, default to the cheapest handler.** **[header/flag]**
- *In the wild:* libc++'s `_LIBCPP_HARDENING_MODE`. Production modes map a failed
  check to "a single instruction" (`__builtin_trap`) — "minimizes the code size
  penalty"; debug/enforce modes route to the verbose abort. Fuchsia's embedded
  build uses `LIBCXX_HARDENING_MODE none`.
- *libcis fit:* when libcis adds precondition checks, gate them so the default is
  a single trap, not a formatted per-site message.

**C4. Drop the unwind tables that survive `-fno-exceptions`.** **[flag]**
- *Mechanism (MaskRay, EH-ABI):* `-fno-exceptions` removes `.gcc_except_table` and
  landing pads, but `.eh_frame`/`.eh_frame_hdr` remain unless you also pass
  **`-fno-asynchronous-unwind-tables`** (and on some targets `-fno-unwind-tables`).
  Minimal bare-metal builds use `-fno-exceptions -fno-rtti -fno-unwind-tables`.
- *libcis fit:* the bench passes only `-fno-exceptions -fno-rtti` — adding the
  unwind-table flags is a **free, measurable** test (shrinks the binary; `.eh_frame`
  is outside `.text` but counts). Trade-off: no backtraces through those frames.

### D — Partition heavyweight subsystems (the embedded / Fuchsia model)

**D1. Make localization optional; keep `<format>`/`charconv` locale-free.**
**[header/flag]**
- *In the wild:* libc++'s `LIBCXX_ENABLE_LOCALIZATION=OFF` "disables `<iostream>`,
  `<regex>`, `<locale>`" — "useful when porting to platforms that don't support the
  C locale API." Fuchsia's embedded cache also turns off
  unicode / wide-characters / filesystem / random-device.
- *libcis fit:* the facet graph behind iostream/format is the costliest subsystem;
  ensure the default `"C"` formatting path never instantiates facets.

**D2. Partition float↔string conversion (the printf-float analog).**
**[header/lib]** Decimal float formatting (Ryū/Grisu-style) is one of the single
largest blobs a small C++ program drags in via `to_chars`/`format`.
- *In the wild (with numbers):* **picolibc** ships five printf variants — dropping
  double→integer saves ~**5.6 KB** text, double→float ~**2.6 KB** on a real app.
  **newlib-nano** omits float I/O by default, re-added via `-u _printf_float`
  (~**10–15 KB**). **llvm-libc** has `LIBC_CONF_PRINTF_DISABLE_FLOAT` and a
  *modular* printf that lazily links float (float monomorphization ~**1.5 KB** on
  armv6m).
- *libcis fit:* keep the float→decimal path in its own out-of-line function (in
  `support.cpp`) so `--gc-sections` removes it when a program never formats a float.

**D3. Silent terminate / no demangling.** **[header]**
Don't pull a demangler or rich diagnostics into the default terminate path.
- *In the wild:* Embedded Artistry's libc++ defaults `libcxx-silent-terminate` on
  ("reduces I/O and demangling overhead on terminate").

### E — Make the compiler & linker do the work (stages 3–4)

**E1. `-ffunction-sections -fdata-sections` + `-Wl,--gc-sections`.** **[flag]**
- *Mechanism:* the linker GCs at *section* granularity; one section per function/
  datum lets it drop the unreferenced ones (mark-and-sweep over relocations from
  the entry symbol). The library's job is not to keep symbols artificially
  reachable.
- *Measured:* ShellCheck **35.9 MB → 14.5 MB**; Linux kernel **~10%**; FreeBSD
  `devd` ~900 KB → ~600 KB. (Highly workload-dependent.)
- *libcis fit:* **the cheapest big win to test now** — the ~80 KB empty-`main`
  baseline (static libc + `support.cpp`) is exactly what GC trims.

**E2. COMDAT folding of vague-linkage entities.** **[auto]**
Inline functions, vtables, `type_info`, template instantiations and template
statics have "vague linkage"; on ELF with GNU ld ≥ 2.8 "duplicate copies … are
discarded at link time" (`SHT_GROUP`/`GRP_COMDAT`). Free on libcis's target — the
only action is *not defeating it* (avoid subtly non-identical bodies; avoid forcing
internal linkage on things that could be merged).

**E3. Identical Code Folding (ICF).** **[flag — check the linker]**
Folds byte-identical functions — powerful for template instantiations that compile
to the same code.
- *Availability:* `gold --icf=safe` (relocation-scan for address-taken) and
  `lld --icf=safe` (precise, via `-faddrsig` address-significance tables) only.
  The **default `bfd` ld has none** — but g++-10 switches linker with
  `-fuse-ld=gold`/`-fuse-ld=lld` (both verified working with g++-10 on this
  toolchain), so this is an opt-in, not a hard limitation.
- *Measured:* gold "Safe ICF" ~**6%** of text on large Google binaries (safe ≈ as
  good as all); one lld clang-binary datapoint ~**-0.8%** safe / ~**-2%** all.
- *libcis fit:* a real experiment, not blocked — try
  `-ffunction-sections -fuse-ld=gold -Wl,--icf=safe` in the bench. Just don't
  assume the *default* link has it.

**E4. `-Os`, and the levers that don't exist here.** **[flag]**
- `-Os` = `-O2` minus the size-increasing passes (`-falign-{functions,jumps,labels,
  loops}`, `-fprefetch-loop-arrays`, `-freorder-blocks-algorithm=stc`), tuned for
  size. Already in use.
- **`-Oz` is not in g++-10** (GCC 12.1+). **No MachineOutliner in GCC** (clang-only;
  ~4.5% at `-Oz` on AArch64). Don't plan around either.

---

## 4. Three design philosophies, contrasted

The implementations converge on the same goals (no-EH/RTTI shipping code, minimal
per-instantiation bloat) but split on *how* to kill template duplication — a useful
map when deciding libcis's direction:

| | EASTL | ETL | libc++ |
|---|---|---|---|
| Per-instantiation dedup | **Avoid virtuals**; lean on inlining + COMDAT/ICF | **Type-erased virtual bases** (`ivector<T>`) emit body once per type | **Extern-template** the heavy specializations into the dylib |
| Allocator | single non-templated `void*` allocator (no `rebind`) | fixed-capacity, no heap | standard `std::allocator` |
| Error paths | exceptions optional, off by default | configurable (exception/assert/log/off) | one `__libcpp_verbose_abort`; `__throw_*` wrappers |
| Needs compiled lib? | no (header + folding) | no | **yes** (the `.so`/`.a`) |
| Fits `-fno-rtti`? | yes (no virtuals) | uses *sparing* virtuals | yes |
| Best lesson for libcis | rely on COMDAT; keep bodies foldable | hoist size-independent code (non-virtually) | extern-template `string` via `support.cpp` |

libcis can cherry-pick: **non-virtual hoisting** (ETL's idea, EASTL's
vtable-avoidance) for stage-1 dedup, **extern templates** (libc++) for the few
hottest specializations, and **one verbose-abort** (libc++/MSVC) for error paths.

---

## 5. Measured impact (cited; treat as orders of magnitude)

| Technique | Reported impact | Source |
|---|---|---|
| `-ffunction-sections` + `--gc-sections` | ShellCheck 35.9→14.5 MB; kernel ~10% | vidarholen; LKML |
| ICF (gold, safe) | ~6% of text (~64 MB binaries; safe ≈ all) | Google "Safe ICF" |
| ICF (lld) | ~-0.8% safe / ~-2% all (clang binary) | llvm-dev thread |
| float→int printf | ~5.6 KB; double→float ~2.6 KB (real app) | picolibc; keithp |
| newlib-nano float add-back | ~10–15 KB | NXP |
| llvm-libc modular printf float | ~1.5 KB (armv6m) | LLVM PR #147426 |
| `-fno-rtti -fno-exceptions -fno-unwind-tables` | 13 KB → 1.3 KB image; ~10 KB combined | bare-metal C++ guide |
| `-fno-exceptions` (unwinding) | ~5–10%; can be dwarfed at `-O2/3` | Momtchev |
| MachineOutliner (`-Oz`, AArch64) | ~4.5% on top of `-Oz` (clang-only) | LLVM D45916 |

Most figures are from secondary sources on different toolchains/workloads. The
only numbers that matter for libcis are the ones `bench/codesize.py` produces.

---

## 6. The libcis action plan

Ranked by (expected `.text` win × low effort/risk) for header-only + g++-10 +
no-EH/RTTI + `-Os`. Each item names the experiment and how to journal it.

### Phase 0 — free build-flag wins (no library change; do first)
Establishes how much is "shape of the build" vs. "shape of the library" before any
header work. Add to a throwaway variant of the bench compile line and measure:

```
# in addition to the current -Os -fno-exceptions -fno-rtti line:
-ffunction-sections -fdata-sections -Wl,--gc-sections          # E1
-fno-asynchronous-unwind-tables -fno-unwind-tables             # C4
-fvisibility=hidden -fvisibility-inlines-hidden                # B2
```
Record each independently and combined:
```
bench/record.py "phase0: +gc-sections"
bench/record.py "phase0: +no-unwind-tables"
bench/record.py "phase0: +hidden-visibility"
bench/record.py "phase0: all three"
```
*Expectation:* gc-sections is the big mover (trims the static-libc baseline);
unwind-tables shrinks binary size off `.text`; visibility is a force-multiplier
for the other two.

### Phase 1 — error-path consolidation (header-only)
2. **One `[[noreturn, gnu::cold, gnu::noinline]]` `std::detail::fail(const char*)`**
   (or `__builtin_trap()` where no message is needed) behind every abort/throw/
   bounds-fail site. *(C1+C2.)* Broad, low-risk, big.
   `bench/record.py "phase1: single verbose-abort handler"`

### Phase 2 — instantiation volume (header-only + the existing compiled TU)
3. **`extern template basic_string<char>`** declared in `<string>`, defined in
   `support.cpp`. *(B1.)* Then measure adding hot `vector`/`unordered_map`
   specializations.
   `bench/record.py "phase2: extern-template basic_string<char>"`
4. **Outline `string`/`vector`/`format` mutators** into fast/slow + non-dependent
   helpers. *(A2.)*
   `bench/record.py "phase2: outline string mutator slow paths"`
5. **Partition float→decimal** in `to_chars`/`format` into an out-of-line body so
   GC drops it when unused. *(D2.)*
   `bench/record.py "phase2: float-format partition"`

### Phase 3 — structural (highest effort, do once 0–2 quantify the prize)
6. **Non-virtual size-independent container cores** (ETL "hoisting" without the
   vtable). *(A1.)*
7. **Optional:** `-fuse-ld=gold -Wl,--icf=safe` if available *(E3)*; keep the
   default locale path facet-free *(D1)*; sustain `if constexpr` discipline *(A4)*.

**Header-only vs. compiled-component:** everything except #3 (and the `support.cpp`
half of #5) is pure header/build work. #3 and float partitioning reuse the
`support.cpp` libcis already ships — no new architectural commitment.

---

## 7. Mechanism glossary

- **Monomorphization** — the compiler emits a separate concrete body for each
  template instantiation; the root cause of stage-1 bloat.
- **Vague linkage** — GCC's term for entities (inline fns, vtables, `type_info`,
  template instantiations/statics) that may be defined in many TUs; reconciled by
  COMDAT (discard duplicates) or weak symbols (pick one, keep the rest).
- **COMDAT group** — ELF `SHT_GROUP`/`GRP_COMDAT`: identically-signatured groups
  across objects collapse to one at link; the standard "discard duplicate
  instantiations" mechanism.
- **ICF (Identical Code Folding)** — linker merges byte-identical *function bodies*
  (a superset of COMDAT, which keys on symbol/signature). gold/lld only.
- **`--gc-sections`** — linker mark-and-sweep over sections reachable from the
  entry symbol; drops the unreached. Needs `-ffunction-sections`/`-fdata-sections`.
- **`.text.unlikely`** — the cold subsection where `[[gnu::cold]]` functions land,
  keeping rare paths off the hot path's cache lines.
- **LSDA / `.gcc_except_table` / `.eh_frame`** — exception/unwind data. `-fno-
  exceptions` drops the first two; `.eh_frame` needs the unwind-table flags too.
- **Extern template** — a header `extern template` *declaration* suppresses local
  instantiation; the matching definition in one TU emits the bodies once.

---

## 8. References (primary)

**Compiler / linker.**
GCC Optimize-Options (`-Os`/`-Oz`/`-ffunction-sections`):
https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html ·
g++-10: https://gcc.gnu.org/onlinedocs/gcc-10.5.0/gcc/Optimize-Options.html ·
Common Function Attributes (`cold`/`hot`/`noinline`):
https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html ·
Code-Gen Options (`-fexceptions`/unwind tables):
https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html ·
C++ Dialect Options (`-fno-rtti`):
https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html ·
Vague Linkage: https://gcc.gnu.org/onlinedocs/gcc/Vague-Linkage.html ·
GNU ld `--gc-sections`: https://sourceware.org/binutils/docs/ld/Options.html ·
gold ICF: https://manpages.debian.org/testing/binutils/gold.1.en.html ·
lld 7.0 release notes: https://releases.llvm.org/7.0.0/tools/lld/docs/ReleaseNotes.html ·
MaskRay, EH ABI: https://maskray.me/blog/2020-12-12-c++-exception-handling-abi ·
ELF section groups: https://docs.oracle.com/cd/E23824_01/html/819-0690/chapter7-26.html

**libc++.**
Visibility Macros: https://libcxx.llvm.org/DesignDocs/VisibilityMacros.html ·
Hardening: https://libcxx.llvm.org/Hardening.html ·
`__verbose_abort`: https://github.com/llvm/llvm-project/blob/main/libcxx/include/__verbose_abort ·
`stdexcept`: https://github.com/llvm/llvm-project/blob/main/libcxx/include/stdexcept ·
extern-template lists: https://github.com/llvm/llvm-project/blob/main/libcxx/include/__string/extern_template_lists.h ·
`src/string.cpp`: https://github.com/llvm/llvm-project/blob/main/libcxx/src/string.cpp ·
`__split_buffer`: https://github.com/llvm/llvm-project/blob/main/libcxx/include/__split_buffer ·
D49240 (`_LIBCPP_HIDE_FROM_ABI`): https://reviews.llvm.org/D49240 ·
LIBCXX_ENABLE_LOCALIZATION: https://releases.llvm.org/12.0.0/projects/libcxx/docs/ReleaseNotes.html

**EASTL / ETL / MSVC STL.**
EASTL N2271: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html ·
FAQ: https://github.com/electronicarts/EASTL/blob/master/doc/FAQ.md ·
Design: https://github.com/electronicarts/EASTL/blob/master/doc/Design.md ·
ETL: https://www.etlcpp.com/ · vector: https://www.etlcpp.com/vector.html ·
ivector (mirror): https://os.mbed.com/users/bobbery/code/ETL/docs/tip/classetl_1_1ivector_3_01T_01_5_01_4.html ·
MSVC throw helpers: https://github.com/microsoft/STL/blob/main/stl/inc/xutility ·
https://github.com/microsoft/STL/blob/main/stl/src/xthrow.cpp ·
MSVC `basic_string` outlining: https://devblogs.microsoft.com/cppblog/c17-features-and-stl-fixes-in-vs-2017-15-3/

**Embedded / Fuchsia.**
Fuchsia C++ (Zircon): https://fuchsia.dev/fuchsia-src/development/languages/c-cpp/cxx ·
LLVM Fuchsia-stage2 cache: https://github.com/llvm/llvm-project/blob/main/clang/cmake/caches/Fuchsia-stage2.cmake ·
Embedded Artistry libc++: https://github.com/embeddedartistry/libcpp/blob/master/meson_options.txt

**printf-float partitioning.**
picolibc: https://github.com/picolibc/picolibc/blob/main/doc/printf.md ·
https://keithp.com/blogs/picolibc-float/ ·
newlib-nano: https://community.nxp.com/t5/LPCXpresso-IDE-FAQs/Newlib-Nano-Support/m-p/472759 ·
llvm-libc: https://libc.llvm.org/dev/printf_behavior.html · https://github.com/llvm/llvm-project/pull/147426

**Optimization guides / measurements.**
Chromium binary size: https://chromium.googlesource.com/chromium/src/+/main/docs/speed/binary_size/optimization_advice.md ·
vidarholen (`--gc-sections`): https://www.vidarholen.net/contents/blog/?p=729 ·
Google "Safe ICF": https://research.google/pubs/safe-icf-pointer-safe-and-unwinding-aware-identical-code-folding-in-gold/ ·
LLVM MachineOutliner: https://llvm.org/doxygen/MachineOutliner_8cpp.html ·
bare-metal C++ (RTTI/EH numbers): https://alex-robenko.gitbook.io/bare_metal_cpp/compiler_output/rtti ·
Momtchev, cost of exceptions: https://mmomtchev.medium.com/the-true-cost-of-c-exceptions-7be7614b5d84

---

*Method: parallel literature agents over the sources above; claims cross-checked
against the primary docs and de-duplicated. Known corrections folded in: g++-10
has no `-Oz` and no MachineOutliner; GNU `bfd` ld has no ICF; `_LIBCPP_HIDE_FROM_ABI`
is no longer `always_inline`; libc++ does not implement `vector<T*>→void*`;
`-fno-exceptions` alone does not remove `.eh_frame`. Verify any figure against
`bench/codesize.py` before relying on it for libcis.*
