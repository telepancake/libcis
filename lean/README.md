# libcis-lean — the size/RAM profile

A parallel libcis codebase optimized for **code size and RAM** instead of
feature completeness. It is an **overlay** over the base library: this
directory contains only the headers it replaces; everything else falls
through to `../include`.

```sh
g++ -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
    -Ilean/include -Iinclude -Os \
    your_program.cpp src/support.cpp lean/src/kernels.cpp \
    -nodefaultlibs -lpthread -lm -lc -lgcc_s -lgcc -o your_program
```

The `-Ilean/include -Iinclude` order is the whole mechanism: `<vector>`
resolves to the lean one, and anything lean does not override (`<type_traits>`,
`<string_view>`, `<utility>`, ...) resolves to the base library. Never mix the
two include orders across TUs of one binary.

**`lean/src/kernels.cpp` is mandatory**, exactly like `src/support.cpp`. The
fat, non-template structural/algorithm kernels (string byte-splice, vector
grow/gap/erase, rb-tree rebalance/iterate, hash rehash/relink, list
reverse/merge-sort, deque table growth, the variant special-member walkers,
the introsort/heap/merge algorithm kernels, and the **runtime `std::format`
engine**) are **out-of-line** in this single translation unit — the lean headers
are NOT header-only. The structural/algorithm kernels are non-template functions
declared in `bits/lean_*.h`; the format engine is the per-CharT
`fmt::vformat_engine<char>` / `<wchar_t>`, declared `extern template` in
`<format>` and explicitly instantiated here (the libc++ `.so` strategy).
Link it into every lean binary (one copy of each kernel per binary), or build it
into a library once (one copy per system, below). Omitting it is a link error
(undefined `std::detail::lean_*` / `tree_*` / `hash_*` / `list_*` /
`variant_*` / `std::fmt::vformat_engine<...>` symbols).

### Building the kernels as a library (one copy per system)

The kernels TU has no per-type code, so it can ship as a static or shared
library that every lean binary links against instead of recompiling:

```sh
# static — liblean.a
g++ -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
    -Ilean/include -Iinclude -Os -c lean/src/kernels.cpp -o kernels.o
ar rcs liblean.a kernels.o

# shared — liblean.so
g++ -std=gnu++20 -fcoroutines -fno-exceptions -fno-rtti -nostdinc++ \
    -Ilean/include -Iinclude -Os -fPIC -shared \
    lean/src/kernels.cpp -o liblean.so
```

Then link a program with `-L. -llean` in place of `lean/src/kernels.cpp`. Use
`-ffunction-sections -fdata-sections -Wl,--gc-sections` on the final link so a
binary carries only the kernels it actually reaches.

## What lean gives up (deviations from the standard, all deliberate)

1. **Custom allocators.** Container templates keep their `Allocator` parameter
   for source compatibility, but instantiating one with anything other than
   `std::allocator<T>` fails a `static_assert`. All storage comes from
   `malloc`/`realloc`/`free`. `get_allocator()` returns a default-constructed
   `std::allocator<T>`. `std::pmr` containers are not available in this
   profile.
2. **constexpr containers and algorithms.** Container operations and the heavy
   algorithms are not `constexpr` (the base library targets gcc-10, which has
   no constexpr allocation anyway; lean also drops constexpr from the fat
   `<algorithm>` entry points so they can be type-erased). Cheap algorithms
   that stay templated keep `constexpr`.
3. **SSO.** `std::string` always heap-allocates (or points at the shared
   static empty representation). A small string costs one `malloc`; in return
   `sizeof(std::string) == sizeof(void*)` and the string code shrinks a lot.
4. **Over-aligned element types.** Container elements must satisfy
   `alignof(T) <= alignof(max_align_t)` (enforced by `static_assert`).
5. **Allocation failure traps** (`__builtin_trap`), consistent with the base
   library's `-fno-exceptions` throw sites.
6. **`end()` stability on node containers**: see the per-container notes;
   where a container object embeds its own end sentinel the standard rules
   hold exactly.
7. **Frugal `shared_ptr`/`weak_ptr` (one-pointer handles).** `sizeof(shared_ptr<T>)
   == sizeof(weak_ptr<T>) == sizeof(void*)`: the handle is a single pointer to a
   `{ void* elem; uint32_t strong; uint32_t weak; void(*dispose)(cb) }` control
   block (24 B on LP64, 16 B on ILP32), and the element pointer is stored once in
   the block rather than per handle. The refcounts are 32-bit and the increment
   kernels **trap on overflow** (2^32 live handles is impossible on the target).
   `make_shared<T>` uses **one** allocation (`T` inline after the block; over-aligned
   `T` is rejected by `static_assert`, like the containers). This buys several
   deliberate departures from `[util.smartptr]`:
   - **No aliasing constructor** — the second word a standard `shared_ptr` spends
     on a per-handle element pointer is exactly what an aliasing handle needs;
     with one pointer there is nowhere to store it, so the overloads are deleted.
   - **Pointer-adjusting conversions trap.** Because every handle sharing a block
     reinterprets the block's single stored `elem`, a converting
     construction/assignment/`static_pointer_cast` to a base at a **nonzero**
     offset (multiple inheritance) would hand back a mis-adjusted pointer; instead
     the library `__builtin_trap()`s. Zero-offset conversions (all single
     inheritance — the overwhelming case) are free. `weak_ptr` conversions run the
     same check on the pointer *value* only, never dereferencing the (possibly
     dead) object — with one carve-out: converting to a **virtual** base has a
     statically-unknowable offset that can only be computed by reading the
     object's vtable, so it traps via the offset check while the object is
     alive, and converting a **dangling** `weak_ptr` to a virtual base is
     undefined behavior (the check itself would read the dead object). A
     standard two-word handle captures the adjusted pointer at construction;
     the one-pointer handle has nowhere to keep it.
   - **No array support** (`shared_ptr<T[]>`, `make_shared<T[]>`, `operator[]`): the
     element count needed to destroy an array does not fit the fixed four-field
     block without a negative-offset trick judged not clean enough to ship.
   - **Omitted:** `get_deleter` (dead under `-fno-rtti`), `dynamic_pointer_cast`
     (needs RTTI), `allocate_shared`/`allocate_shared_for_overwrite`/`pmr`, and the
     whole `atomic<shared_ptr>` / `atomic_load`/`store`/`exchange`/`compare_exchange`
     family. **Kept:** `static_`/`const_`/`reinterpret_pointer_cast`,
     `owner_before`/`owner_less`, `enable_shared_from_this` with standard semantics
     (`shared_from_this` on an unowned object traps), and `unique_ptr` -> `shared_ptr`
     interop (it routes through the pointer/deleter constructor).
8. **Shared empty-string byte is racy under concurrent legal writes.** Because
   all empty strings of a given `charT` point at one shared immutable static
   rep, `s[s.size()]` on an *empty* string returns a reference to a byte shared
   by every empty string of that width. The standard permits writing `charT()`
   (and only `charT()`) to that reference. Two threads each performing that
   legal write of `charT()` to *different* empty strings therefore write the
   same shared byte — a data race unique to this design (the value written is
   always `charT()`, so no thread observes a wrong value, but it is a race per
   `[intro.races]`). Non-empty strings, and any write other than `charT()`,
   are unaffected (they force a heap allocation first).
9. **Two-word `std::function` with a small-object budget of one word.**
   `sizeof(function<...>) == 2 * sizeof(void*)` (`{ ctx, invoke }`), and the
   inline (heap-free) storage budget is exactly **one word**. This buys several
   deliberate departures from `[func.wrap.func]`:
   - **A `>=`-word-sized capture allocates.** Only a *one-word*, trivially
     copyable, ≤`alignof(void*)`-aligned callable is stored inline (plus plain
     function pointers, capture-less lambdas, and the extension ctor below).
     Any capture larger than one word — including a genuine **two-word**
     `[a, b]` capture — a non-trivially-copyable capture (e.g. a `std::string`
     by value), or an over-aligned one goes to a `malloc` block. This is the
     small-buffer trade for the two-word object, stated exactly like `string`'s
     SSO trade: the object shrinks, and the callables that no longer fit inline
     cost one `malloc`. (The budget is pointer-width-relative: an
     `[int, int]` capture is one word on LP64 but two words — hence heap — on
     ILP32.)
   - **`target()` / `target_type()` are omitted** — they require `typeid`
     (RTTI), which is off (`-fno-rtti`), exactly as the base library omits them.
   - **A lean extension constructor** `function(R (*fn)(T*, Args...), T* obj)`
     stores `obj` in `ctx` and uses `fn` *itself* as the invoker (reinterpreted
     to `R(*)(void*, Args...)`): zero per-type code, zero indirection hops. This
     is the GCC bound-pointer-to-member-function cast shape; it relies on `T*`
     and `void*` sharing an argument ABI (the same de-facto contract the GCC pmf
     cast relies on). `static_assert(is_object_v<T>)`.
   - **Mutating targets keep standard semantics** — at the cost of a heap
     block: the inline one-word mode is gated on *const-invocability*, so a
     `mutable` lambda (any callable whose call operator is non-const) routes to
     the heap block and mutates in place, even when its state is one word.
     Residual (undetectable) caveat: a const-invocable trivially-copyable
     one-word functor that mutates a `mutable` **member** inside its const call
     operator is invoked on a by-value copy and loses those mutations.
   - **Calling an empty `function` traps** (`__builtin_trap`), consistent with
     the `-fno-exceptions` profile — no `bad_function_call` is thrown (the type
     is still defined).
   - **Hidden-visibility DSO caveat.** The heap-mode discriminator is a single
     address compare of `invoke` against the per-signature `heap_invoke`
     trampoline, whose one-address-program-wide guarantee comes from C++ vague
     linkage. Passing a `function` object into a DSO that built `<functional>`
     under *hidden* visibility would give `heap_invoke` a second address and
     defeat the compare; the lean headers are compiled default-visibility so this
     never happens in-tree. Documented, not worked around.

10. **`std::variant` with table-driven special members.** Copy/move-construct,
    copy/move-assign, destroy and swap of a variant with any non-trivial
    alternative route through one static rodata table (a dispatch function
    pointer per alternative, null = trivially copyable) plus shared
    non-template walker kernels in `kernels.cpp`, instead of base's
    per-instantiation N-way visit switches. Observable deviations, all
    dead-code under this profile:
    - Non-trivial special members are **not `constexpr`** (they call the
      out-of-line kernels), consistent with deviation 2. All-trivial variants
      keep constexpr and fully trivial (defaulted) special members —
      byte-identical to base (`variant<int, long, double>` stays trivially
      copyable).
    - Cross-index assignment is destroy + construct-from-source; the
      strong-exception-guarantee temporary (`T(std::forward<Arg>(arg))` for a
      non-nothrow-copyable, nothrow-movable alternative) is dropped as
      unobservable under `-fno-exceptions` (it exists only to unwind a throw
      that cannot happen; `valueless_by_exception()` is always `false` after
      construction).
      **Same-index assignment uses the alternative's assignment operator**,
      exactly as the standard requires (verified by op-counting tests).
    - `swap` uses one uniform three-move dance for same- and different-index
      (base special-cases same-index to an element `swap`; the result is
      identical since variant's `swap` already requires move-constructible
      alternatives, and the `move_nothrow` ordering dance is likewise
      unobservable without exceptions).
    - The discriminant is the **smallest unsigned type** that fits the
      alternative count (`unsigned char` up to 255 alternatives), so
      `sizeof(variant<char, short>) == 4` where base spent an `unsigned int`
      index (8 bytes total).

11. **Locale severance — lean iostreams are CLASSIC-LOCALE ONLY.** The lean
    `<ios>`/`<ostream>`/`<istream>` overlays are byte-for-byte the base headers
    **except** the stream code path consults **no** locale facet: nothing in it
    calls `use_facet` / `has_facet`, references `ctype<CharT>` / `numpunct<CharT>`,
    or forces `locale::classic()`'s facet set to be constructed. Concretely:
    - **`imbue(loc)` is a documented NO-OP** that returns the classic locale and
      does **not** store `loc`. `getloc()` always returns a default
      `std::locale{}` (classic). A program that defensively `imbue()`s keeps
      working — it just gets classic formatting. `ios_base` stores **no**
      `std::locale` member.
    - **Whitespace / character classification is done DIRECTLY** against the
      fixed C/classic set (`detail::classic_isspace<CharT>`: a byte is space iff
      it is one of `' ' \t \n \v \f \r`), for both `char` and `wchar_t` — never
      through `ctype<CharT>` and never through the (C-locale-dependent)
      `iswspace`. `narrow`/`widen`/`fill` are the direct classic mappings
      (identity for `char`; `(wchar_t)(unsigned char)c` / `wctob`-classic for
      `wchar_t`).
    - **`boolalpha` uses hardcoded `"true"`/`"false"`** (and their widened
      `wchar_t` forms), never `numpunct::truename()`/`falsename()`.

    **This is a SIZE deviation, not a correctness cut.** All formatted I/O stays
    correct **in the classic locale**: integer/float/bool/char/string/pointer
    insertion and extraction; width/fill/adjustfield; showbase/showpos/uppercase/
    hex/oct/dec; precision/fixed/scientific/hexfloat; skipws; get/getline/peek/
    unget/putback/ignore/read; tellg/seekg/tellp/seekp; sentry semantics;
    failbit/eofbit/badbit; `operator>>` of `std::string` + `std::getline`;
    `endl`/`ends`/`flush`; the manipulators; and stringstreams — all verified
    byte-for-byte in `lean/test/iostream.cpp`. **Only LOCALE-DEPENDENT behavior
    changes**, and it changes to *classic*: thousands separators, locale digit
    grouping, locale bool names and locale-specific ctype **silently do not
    occur**. `imbue` was chosen as a NO-OP rather than a trap deliberately —
    a no-op keeps a defensively-imbuing program running (with classic output),
    whereas a trap would break it; the cost is that locale-specific formatting is
    silently classic. See the "Locale severance" section below for the size
    story (and the honest ceiling: the stream *code* is severed, but the classic
    facet *vtables* remain pinned into every binary by two eager initializers
    that live outside the stream headers).

Everything else — iterator categories, complexity guarantees, reference
stability of node containers, the public API surface — follows the standard.

## Representation contract

### The lean block (`bits/lean.h`)

Every array-like lean container stores **one pointer** to the payload of a
*lean block*:

```
                     payload pointer (the container object)
                     v
[ base ... header ][ payload ......................... spare tail ]
          ^size_t used at payload - sizeof(size_t)
```

- `used` (an element count or byte length — the owner decides) lives at a
  fixed negative offset from the payload pointer, inside the same allocation.
- **Capacity is not stored anywhere.** It is recomputed from
  `malloc_usable_size(base)`, so the spare tail glibc rounds allocations up to
  is usable capacity for free.
- The header offset is `max(sizeof(size_t), alignof(T))`, so the payload is
  correctly aligned for any non-over-aligned `T`.

### Containers

| type | object size | representation |
|---|---|---|
| `string` | 1 pointer | chars in a lean block, `used` = length; empty strings point at a shared immutable static rep (never freed, capacity 0); `c_str()` is just the pointer |
| `vector<T>` | 1 pointer | elements in a lean block, `used` = size; null pointer = empty; growth uses `realloc` when `T` is trivially relocatable (`bits/relocatable.h`), else malloc+relocate+free through type ops |
| `map`/`set` | ~32 bytes | red-black tree; node = `{parent-with-color-in-bit-0, left, right, value}`; the container embeds the libstdc++-style header node (root/leftmost/rightmost) + size, and the header doubles as the `end()` sentinel |
| `unordered_map`/`_set` | 1 pointer | control block holding `{size, max_load_factor, head-of-all-elements list}` followed by the bucket array (libc++-style single intrusive list through all elements so `begin()` is O(1); bucket count derived from the block, not stored) |
| `list<T>` | ~24 bytes | embedded sentinel `{prev,next}` + `size_t` size (C++11 requires O(1) `size()`) |
| `deque<T>` | small control | fixed-byte blocks + block table; reference stability at both ends per the standard |
| `shared_ptr<T>` | 1 pointer | a single pointer to a control block (`bits/lean_sp.h`); null = empty. The block is `{ void* elem; uint32_t strong; uint32_t weak; void(*dispose)(cb) }` = **24 B on LP64** (16 B on ILP32). `elem` is first so `get()`/`*`/`->` load it at zero displacement. `make_shared<T>` places `T` inline in the SAME allocation right after the block |
| `weak_ptr<T>` | 1 pointer | the same one-pointer handle onto the same control block; holds a weak reference. `enable_shared_from_this<T>` embeds one `weak_ptr<T>`, so it too is one pointer |
| `function<R(Args...)>` | 2 pointers | `{ void* ctx; R (*invoke)(void*, Args...) }` = **16 B on LP64** (8 B on ILP32); empty = `invoke == nullptr`. `operator()` is `invoke(ctx, args...)` after a null check — **no vtable in the hot path**. Four storage modes, discriminated only by `invoke`'s identity (never by pointer tag bits — unsafe on Thumb): (1) a one-word trivially-copyable callable (`[this]`/`[ptr]`/`[int]`) lives in `ctx`, `invoke` a per-type invoker; (2) a plain fn pointer / capture-less lambda: `ctx` is the fn pointer, `invoke` one shared per-signature trampoline; (3) the lean extension ctor `function(R(*)(T*,Args...), T*)`: `ctx` is the object, the user fn IS the invoker (zero per-type code, zero hops); (4) anything larger/over-aligned/non-trivially-copyable (incl. `>=2`-word captures) is a `malloc` block reached through `ctx`. Trivially relocatable |
| `variant<Ts...>` | storage + narrow index | base's recursive-union storage, but the discriminant is the smallest unsigned type that fits the alternative count (`unsigned char` for ≤255 alternatives): `sizeof(variant<char, short>) == 4` (base: 8). Special members are table-driven (`bits/lean_variant.h`, deviation 10); all-trivial variants keep fully trivial (defaulted) members and are `is_trivially_copyable` |

### Type erasure (`bits/lean.h` type ops + the algorithm kernels)

The rule: **erase a template into a single-instance kernel when its
per-instantiation code is big (≳200 bytes) and the indirection is amortized
over a whole container/range operation. Never erase a per-element hot path**
(`operator[]`, the `push_back` fast path, comparisons in a tight search loop
stay inline).

- Container structural work (vector grow/insert/erase shuffles, rb-tree
  rebalance/erase-fixup/iteration, hash-table rehash, list reverse/merge-sort,
  deque table growth) is implemented as plain non-template functions,
  **declared in `bits/lean_*.h` and defined out of line in
  `lean/src/kernels.cpp`** (one copy per binary, or one per system when built as
  `liblean.a`/`liblean.so`) — parameterized by `lean_ops` (element size/align +
  relocate/destroy/copy function pointers, null when trivial ⇒ kernels take the
  `memcpy` path with no calls). Only genuinely tiny hot accessors
  (`lean_used`/`lean_base`/`lean_alloc`, the tree bit-0 color ops, `push_back`
  fast paths, iterator ops, the `lean_ops_for` thunk templates) stay `inline` in
  the headers.
- The fat `<algorithm>` entry points (`sort`, `stable_sort`, `nth_element`,
  `partial_sort`, `inplace_merge`, `make_heap`/`sort_heap`) route
  contiguous-iterator ranges into single-instance kernels (also defined in
  `lean/src/kernels.cpp`) that take `(elem_size, compare thunk, context)`; the
  per-type code that remains is a ~2-instruction comparator thunk.
  Non-contiguous iterators keep a small templated fallback. The rb-tree's
  *descent* (comparator in a loop) stays a thin template; only the
  type-independent rebalancing is erased — that is exactly the split libstdc++
  ships in its .so.
- `std::function` erases a *callable* rather than a container operation, so it
  follows the same rule from the other side: its **hot path is a single indirect
  call** (`invoke(ctx, args...)`), never routed through a kernel, while only the
  cold heap-block clone/free (the fat, type-independent `malloc`/`memcpy`/`free`)
  is out-of-line in `kernels.cpp` as `detail::fn_block_clone` / `fn_block_free`.
  Each heap-stored callable type adds only three tiny thunks (copy-construct,
  destruct, invoke); the two shared kernels are one copy per binary. Inline
  callables (the one-word, fn-pointer and extension-ctor modes) reach `malloc`
  never and `kernels.cpp` not at all.
- `std::variant` erases its **special members** the same way: base emitted a
  per-instantiation N-way visit switch for each of copy/move-construct,
  copy/move-assign and destroy (measured ~853 B per variant type at -Os with
  one string-member alternative). Lean replaces them with one static rodata
  table per variant type — a single dispatch function pointer per alternative
  (`detail::vt_op_fn`, null = trivially copyable ⇒ the walker memcpys the
  storage bytes / skips destroy, the `lean_ops` convention) — consumed by five
  shared non-template walkers in `kernels.cpp` (`variant_destroy`,
  `variant_{copy,move}_construct`, `variant_{copy,move}_assign`). The only
  per-type code left is one small `vt_op<T>` action-switch thunk per
  *alternative type*, deduped program-wide across every variant that lists it.
  Measured (overhead_matrix, variant axis): per-type **853 B → 554 B (-35%)**.
  One dispatch pointer per alternative, not five per-op pointers, is
  load-bearing: a 5-pointer row costs 40 B of rodata per alternative and
  measured as a net LOSS (871 B/type) before the collapse. **When every
  alternative is trivially copyable the table is never instantiated** and the
  special members stay defaulted/trivial — an all-trivial variant compiles to
  zero engine code and references no kernel (verified via `nm -u`). `std::visit`
  is deliberately unchanged from base: it is already table-based, and the
  per-callsite cost (~735 B) is the user's visitor lambda — typed code that
  cannot be erased without losing its result; the measured per-callsite delta
  is ~0, as expected.

  **`optional` and `tuple` stay base — measured rationale.** The same
  finite-difference probe (string-member payload) put their per-type cost at
  ~103 B (base include order) and ~2 B (lean order — the payload's `string`
  members were the real cost, and lean `string` already absorbed them into its
  own kernels), far below the ≳200 B erasure threshold above. And no conforming
  *size* lever exists: an `optional<T>` sentinel compression would break
  engaged-null semantics (`optional<T*>` must distinguish `nullopt` from a null
  pointer value), and `tuple`'s layout IS its members. Nothing to erase, nothing
  to shrink — an overlay would be copy-paste with risk.

### `std::format` (`<format>` overlay + the `vformat_engine` kernel)

`std::format`'s cost is almost entirely a **fixed runtime engine**, not
per-callsite code: measured, the FIRST format callsite in a program is ~**41.5
KB** of text and the second is only **+0.3 KB**. That 41.5 KB is the runtime
formatting engine — the `fmt::vformat_to` dispatch loop, every builtin
`formatter<T,CharT>::format()` body, `fp_to_chars`' snprintf float path, and the
integer/grouping/padding/fill write helpers those call. Base emits that engine,
via header templates, **once per binary**. The lean `<format>` overlay is
byte-identical to base except it relocates that engine out-of-line so it ships
**once per SYSTEM** (in `liblean.a` / `liblean.so`) instead.

The lever is a single per-CharT choke point, `fmt::vformat_engine<CharT>`,
declared `extern template` in `<format>` (body in `kernels.cpp`, explicitly
instantiated for `char` + `wchar_t` — the only types `fmt_char_type` admits).
Every runtime path (`vformat`/`vformat_to`/`format_to`/`format_to_n`/
`formatted_size`, locale and non-locale) funnels the **type-erased** `arg_t`
dispatch through the ONE runtime context per CharT
(`basic_format_context<back_insert_iterator<fmt::output_buffer<CharT>>, CharT>`;
`output_buffer<CharT>` is a function-pointer-erased sink, so a custom output
iterator is *wrapped* rather than re-instantiating the engine). Because the
dispatch switches on the runtime tag, the whole engine is instantiated per-CharT
— exactly twice — inside that one TU. This is precisely how libc++ ships
`__format` in its `.so`.

Measured (marginal text+data+bss over an empty `main`, `-Os`, `--gc-sections`;
also `overhead_matrix.py`'s `std::format` axis):

| deployment | FIXED per binary (first callsite) | per extra callsite |
|---|---|---|
| base (engine header-only) | ~41.5 KB | ~0.3 KB |
| lean-static (engine in `kernels.o`) | ~41.2 KB | ~0.3 KB |
| lean-so (engine in `liblean.so`) | **~2.8 KB** | ~0.3 KB |

So the ~**38.5 KB** fixed engine moves from *per binary* to *per system*: a
single lean binary that uses `std::format` drops from ~41 KB to ~3 KB of
format text, at the cost of one shared `liblean.so`. The engine's contribution
to that shared object is the whole-TU delta of `kernels.o` text: **+66 KB**
(≈14 KB → ≈80 KB) — larger than the per-binary 41.5 KB because `kernels.o`
carries **both** the `char` and `wchar_t` engines *and* the Unicode
width/grapheme rodata tables in full, whereas a single char-only binary
gc-sections the `wchar_t` half away. That +66 KB is paid **once**, no matter how
many binaries link it.

**Compile-time format-string checking is fully preserved.** Only the *runtime*
engine leaves the header: the consteval `basic_format_string` validator, every
`formatter<T>` specialization (thin per-type parse/format wrappers whose
`format()` may *call* the externalized engine but whose type stays header-only),
and the constexpr parse context all stay inline. A malformed format string is
still a hard **compile error** — verified in `test/format.cpp`, where the
consteval validator caught a genuinely ill-typed dynamic-width argument during
development.

**Float-path drop: investigated, verdict inherently pinned (not faked).** The
type-erased dispatch's `arg_t` switch statically references the `double`/`float`/
`long double` arms (→ `formatter<double>::format` → `fp_to_chars`), so
`fp_to_chars` is ODR-reachable from the engine whether or not a program formats
a float. In an int-only lean-**static** binary `fp_to_chars` is therefore
**present** (confirmed with `nm`; an int-only binary is within ~0.1 KB of an
int+float one), and `--gc-sections` cannot drop it. Un-pinning it would require
routing the builtin float arm through a per-callsite indirection populated only
when a float is actually in `make_format_args` — a non-conforming redesign that
would also push the float formatter *back* into per-callsite code, defeating the
per-system win. It is left pinned, and honestly reported. For the lean-**so**
deployment the point is moot: `fp_to_chars` lives in `liblean.so` regardless, so
an int-only lean-so binary carries **none** of it (also confirmed with `nm`).

### Locale severance (`<ios>`/`<ostream>`/`<istream>` overlays)

Deviation 11: lean streams are **classic-locale only**. The overlays are
byte-for-byte the base headers except the stream code path calls **no** locale
facet — no `use_facet`/`has_facet`, no `ctype<CharT>`/`numpunct<CharT>`, no
stored `std::locale`. `<ios>` still `#include`s base `<locale>` for the
`std::locale` *type* named in the `getloc()`/`imbue()` signatures, but nothing
in the stream path consults a facet; `<ostream>`/`<istream>` drop the `<locale>`
include entirely. Whitespace is `detail::classic_isspace`, `narrow`/`widen` are
direct classic mappings, `boolalpha` is hardcoded `"true"`/`"false"`, `imbue` is
a no-op returning classic.

**What this removes from the stream code (proven with `nm`).** The base stream
path calls `use_facet<ctype<char>>` for `widen`/`narrow`, whitespace skipping and
token boundaries, and `ios_base` stores + copies a `std::locale`. The lean
overlay removes all of it: a lean `cout` object file (`nm`) references **no**
`use_facet<ctype<char>>` instantiation and **no** `std::locale` copy/assign from
the stream code. A TU that includes only `<locale>` (no streams) already carries
the `ctype`/`numpunct` vtables, proving the residual vtables in a linked binary
come from `<locale>` itself, **not** the stream code.

**Measured (marginal text+data+bss over empty `main`, `-Os`, `--gc-sections`,
standard `src/support.cpp` + `lean/src/kernels.cpp` link):**

| | base | lean | delta |
|---|---|---|---|
| empty-main baseline (absolute) | 113 101 | 107 494 | **−5 607** |
| `cout << int/float/bool/string` (absolute) | 135 137 | 127 862 | **−7 275** |
| `cout` **marginal** over empty | 22 036 | 20 368 | **−1 668 (−7.6 %)** |

The baseline itself shrinks because `src/support.cpp`, compiled in the lean
include order, now instantiates the *lean* (locale-severed) streams for its own
`<sstream>` use. The `cout` marginal drop (−1.7 KB) is exactly the locale
ctor/copy/assign + `use_facet<ctype<char>>` widen/narrow that left the stream
path.

**Honest ceiling — the ~65 KB facet suite is NOT in the marginal, and severance
alone cannot drop it.** The `cout` marginal (~21 KB base) is the **direct
number-formatting engine** (`format_integer_impl`, `put_float`, `pad_and_output`,
the base-10/base-16 LUTs) + the `cout`/`cin` static init — it is **not** locale
machinery (base already formats numbers directly, no `num_put`/`num_get`), so
severing locale cannot shrink it toward printf. The classic facet suite
(`ctype`/`numpunct`/`num_get`/`num_put`/`money`/`time`/`collate`/`codecvt`
vtables + `do_get`/`do_put` bodies) measures **~65 KB and is pinned into EVERY
binary** — including the empty-main baseline (measured: 64.8 KB of facet/locale
symbols in the base empty binary, 61.1 KB in the lean empty binary) — so it
**cancels in the marginal**. It is pinned by **two eager initializers that live
OUTSIDE the stream headers**:

- `std::detail::stream_facet_init_instance` (in `src/support.cpp`) — eagerly
  constructs + registers `num_get`/`num_put`/`money*`/`time*`/`messages` (and
  `num_get::do_get` transitively references `use_facet<ctype>`/`<numpunct>`,
  pinning those too);
- `std::detail::locale_classic_init_instance` (in `include/locale`) — eagerly
  constructs + registers the classic `ctype`/`numpunct`/`collate`/`codecvt`.

Because the lean streams no longer call any facet, those inits are the *only*
thing keeping the ~65 KB alive in a plain `cout`/`cin` program. **Ceiling
experiment** (neutralize both eager inits — a throwaway build, since both files
are outside the owned stream overlay): a lean `cout` binary drops
**127 862 → 45 036 bytes (~83 KB recovered)** and **still runs correctly**,
precisely because the severed streams never call `use_facet` at runtime. That
~83 KB is the real prize; reaching it needs those two eager initializers made
lazy (register-on-first-`use_facet`) so `--gc-sections` can drop the unused
facets. That is a **handoff** to the localization/base-support owner — the
severance here is the prerequisite (streams must stop pulling the facets before
lazy registration can drop them), and it is complete.

### Smart pointers (`bits/lean_sp.h` control block + the `sp_*` kernels)

The `shared_ptr`/`weak_ptr` refcount engine follows the same rule: the
type-independent count manipulation is a handful of non-template kernels
declared in `bits/lean_sp.h` and defined out of line in `lean/src/kernels.cpp`,
operating on the type-erased `detail::sp_cb` block (`{ void* elem; uint32_t
strong; uint32_t weak; void(*dispose)(sp_cb*) }`):

- `sp_retain` / `sp_weak_retain` — relaxed atomic increment, trap on 32-bit
  overflow.
- `sp_release` — release-decrement the strong count with an acquire fence on the
  zero transition (the standard reference-counting idiom); on zero it calls
  `cb->dispose(cb)` then drops the collectively-held weak ref.
- `sp_weak_release` — same for the weak count; on zero it `free()`s the whole
  block (including make_shared's inline object storage).
- `sp_lock` — CAS-increment-if-nonzero for `weak_ptr::lock`.
- `sp_use_count` — relaxed load of the strong count.

The **only** per-type code is the `dispose` thunk (a ~2-instruction template in
`<memory>`: destroy-in-place for `make_shared`, `delete` for the pointer ctor,
invoke-then-destroy for a custom deleter). Everything else — copy, move, reset,
lock, the whole handle — is inline pointer moves over the shared kernels.

## Testing and measurement

- `lean/test/*.cpp` — one file per component, each with its own `main()`.
  Failures **trap** (`CHECK` from `lean/test/lean_test.h`); no result
  printing, no fallbacks. A test binary exiting 0 means every check ran and
  passed.
- `lean/tools/run_tests.sh [filter]` — builds and runs every (or matching)
  test at `-O0 -g` and `-Os`, against the overlay, with the standard link
  recipe. Exit 0 iff everything passed. `CXX=clang++` works too.

  **32-bit targets.** `CXXARCH` threads an architecture flag through every
  compile *and* link (empty by default, so the 64-bit build is unchanged).
  `CXXARCH=-m32 lean/tools/run_tests.sh` builds and runs the whole suite as
  32-bit i386 (needs `gcc-multilib`/`g++-multilib`). The flag is validated up
  front: if the toolchain cannot compile+run for it, the run aborts loudly
  rather than silently skipping the arch. The lean profile is pointer-width
  clean — every container is still one pointer (or the documented small
  multiple: rb-tree 4 pointers, deque/list 3, `shared_ptr`/`weak_ptr` one), the
  `shared_ptr` block is 16 B on ILP32 (24 on LP64), and the refcounts stay
  32-bit so no 8-byte atomic / `libatomic` is pulled into the `-nodefaultlibs`
  link.
- `lean/tools/size_report.py` — compiles a fixed matrix of feature programs
  against base and lean at `-Os` (every binary linked with
  `-ffunction-sections -fdata-sections -Wl,--gc-sections`, both include orders,
  so each program carries only the kernels it reaches), subtracts an
  empty-program baseline, and writes the marginal text/data/bss table, the
  full-TU `size(1)` text of `lean/src/kernels.cpp` (the one-copy-per-system
  library cost), and `sizeof` comparisons to `lean/SIZES.md`. Numbers are
  measured, never estimated.

The full conformance story (the transferred libc++ suite under `test/std/`)
applies to lean the same way as to base — run the pipeline with the overlay
include order — minus tests for the documented deviations above. That gating
requires the bootstrapped toolchain and is not part of the quick loop.
