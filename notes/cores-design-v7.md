# Cores design v7 — extending the sandwich to the rest of the library

v5 froze the contiguous-storage cores (`grow`, `destroy_range`,
`construct_copy_one_n`, `rotate`, `relocate_live` against `type_ops` and
`storage_ops`). v6 carried the same sandwich through the node-based
containers (no cores: compile-time erasure is already optimal), deque
(three cores + `segment_map_ops`), `any` / `function` (one vocab table
each), and two algorithm primitives (`equal_bytes`, `find_byte`). v6
explicitly deferred or rejected a handful of others (sort, exception_ptr,
node_ops, per-signature unification of `function`).

v7 walks the families v6 did **not** analyse and applies the same rule
(§1 below): is the modifier body a typeless byte path that more than one
caller shares, with a trivial-fast-path collapse to a builtin? If yes,
core. If not — and this is the more common answer — say why not, in
writing, so v8 doesn't relitigate it.

This is **design only**. No code is changed.

---

## 0. TL;DR — twenty more families surveyed

The verdict table. "core?" means a non-template body in `src/support.cpp`
or a new vocab table in `bits/`. "no" means the existing shape is already
the optimum and the section explains why.

| family | header(s) | shape | core in v7? |
|---|---|---|---|
| sum types | `<optional>`, `<variant>`, `<expected>` | tag + payload union | **1 vocab** (`sum_ops`) — variant only |
| product types | `<tuple>`, `<pair>` | EBO chain | no — pure compile-time |
| smart pointers | `<memory>` (`shared_ptr`/`weak_ptr`) | virtual control block | **1 vocab** (`sp_cb_ops`) replaces vtable |
| smart pointers | `<memory>` (`unique_ptr`) | pointer + EBO deleter | no — EBO already optimal |
| pmr | `<memory_resource>` | virtual `memory_resource*` | no — already vocab-shaped |
| iostreams | `<streambuf>`, `<istream>`, `<ostream>`, `<iostream>` | virtual streambuf | no — virtuals are the API |
| string streams | `<sstream>` | streambuf + `string` buffer | **1 core** (`sb_xsputn_bytes`) |
| file streams | `<fstream>` | streambuf + FILE | no — already one impl per CharT |
| chrono | `<chrono>` (core) | duration + time_point math | no — pure compile-time arithmetic |
| chrono format | `bits/chrono_format.h` | format-spec dispatch | **deferred** — depends on `<format>` outcome |
| chrono tzdb | `bits/chrono_tzdb.h` | data tables | no — data, not code |
| format | `<format>` | tagged arg + visit_format_arg | no — already type-erased to `arg_t` |
| regex | `<regex>` | NFA + per-CharT engine | **deferred** — needs bench probe |
| container adaptors | `<stack>`, `<queue>`, `<priority_queue>` | thin wrappers | no — empty thunks |
| views | `<string_view>`, `<span>` | (ptr, size) | no — by construction |
| fixed-size | `<array>`, `<bitset>`, `<valarray>` | inline buffer / bit ops | no for array/bitset; **1 core** for valarray (cold) |
| iterator/ranges | `<iterator>`, `<ranges>` | CRTP view chains | no — compile-time fusion is the point |
| concurrency atoms | `<atomic>`, `<barrier>`, `<semaphore>`, `<latch>` | intrinsics | no — there is no byte work |
| concurrency higher | `<future>`, `<thread>`, `<mutex>`, `<shared_mutex>`, `<condition_variable>`, `<stop_token>` | OS handles | **1 vocab** for `<future>` shared-state callback |
| filesystem | `<filesystem>` | `path` + iterators | **1 core** (`path_lexical_normal`) + status quo on the rest |
| locale | `<locale>` | facet hierarchy (virtuals) | no — facets are the API |
| random | `<random>` | engine + distribution templates | no — pure compile-time |
| typeinfo/typeindex | `<typeinfo>`, `<typeindex>` | RTTI | no — RTTI off at this target |
| errors | `<system_error>`, `<stdexcept>` | category vtable / string | no for system_error; **status quo** for stdexcept |
| charconv | `<charconv>` | algorithm bodies | no — already one impl per arithmetic type, COMDAT-folded |
| numeric | `<numeric>` | accumulate/reduce templates | no — same trade as `std::sort`, deferred |
| coroutine | `<coroutine>` | compiler intrinsic | no — there is no library code to share |
| source_location | `<source_location>` | compiler intrinsic | no — same |
| scoped_allocator | `<scoped_allocator>` | template wrapper | no — pure compile-time |

Net new in v7: three runtime vocab tables (`sum_ops`, `sp_cb_ops`,
`future_callback_ops`), one new core for sstream, one for valarray
cold-path, one for filesystem, plus two deferred (`<regex>` and chrono
formatting). Six families produce no cores at all. The big-ticket "no"s
are the iterator/ranges layer (because compile-time fusion is the entire
performance model) and locale (because the virtuals are the public ABI).

---

## 1. The carried rule (and one v7 sharpening)

v5/v6 rule, restated:

> A core earns its keep when:
> 1. Its body is **typeless byte work** (size in bytes, not count of `T`).
> 2. **More than one caller** consumes the same body.
> 3. The **trivial fast path** collapses to a `memcpy`/`memmove`/`memset`/
>    `memcmp`/skip/atomic-builtin that the linker can fold across consumers.
> 4. The compile-time alternative is **not already the COMDAT optimum**.

v7 adds, after looking at iostreams and locale:

> **A virtual-function API is itself a runtime vocab table.** When a
> facility's public ABI is already "user-supplied subclass of a virtual
> base" (`streambuf`, `memory_resource`, `locale::facet`), the vtable is
> the vocab table — there is no opportunity to "introduce" one. The cores
> question for these subsystems is whether the *library-supplied
> subclasses* share their byte work; usually they don't, because each
> ships exactly one implementation. The exception is `sstream`, where the
> overflow/underflow loops do typeless buffer ops that all `CharT`
> instantiations share (§2.4.3).

This is why `<streambuf>` / `<locale>` / `<memory_resource>` get no
cores: the type erasure is already at the language boundary, and the
implementations behind it are single-instance.

---

## 2. Walking the families

### 2.1 Sum types — `optional`, `variant`, `expected`

#### 2.1.1 `optional` — no core

**Current shape.** `optional<T>` holds an `engaged_` bool plus a payload
union of `{ char nothing; T value; }`. Modifiers (`emplace`, `reset`, copy
ctor, move ctor, `operator=`) are tiny: an `if (engaged_) destroy/copy
T; else nothing`. Per-instantiation cost is essentially the size of `T`'s
copy/move/destroy bodies, which the user has to instantiate anyway.

**Why no core.** Two reasons.

1. **There is no shared body to factor.** The four modifier shapes
   (`copy_construct`, `move_construct`, `copy_assign`, `move_assign`) each
   compile to ~10 instructions: a bool test, a branch, and a delegation
   to `T`'s leaf. The body is per-`T`-shaped because it embeds the bool
   test before the leaf call. Pulling the bool test into a typeless core
   would mean writing `if (((const optional_base*)p)->engaged_) ...`, but
   the layout of `optional<T>::engaged_` depends on `alignof(T)`
   (gcc-10's `[[no_unique_address]]` packs `engaged_` into `T`'s padding
   slot for `T` like `bool` / `char`, but not for `T` like `int64_t`).
   A typeless core can't know where the bool lives without a slot in the
   ops table, and at that point the ops table costs more than the body
   it would share.
2. **No trivial fast path that ≠ memcpy(T).** When `T` is trivially
   copyable, `optional<T>` is trivially copyable, and the compiler
   already emits `memcpy(sizeof(optional<T>))` for the assignment. No
   slot is unlocked by a core.

**Status quo.** Per-`T` instantiation of the four lifecycle modifiers
stays. The leaves are already shared (they're `T`'s leaves) via COMDAT.

#### 2.1.2 `variant` — one new vocab table (`sum_ops`)

**Current shape.** `variant<Ts...>` holds an `index_` (uint, sized by
`sizeof...(Ts)`) plus a recursive union of `Ts...` (the `vbase` / `vdtor`
/ `vctor` / `vmove_ctor` chain in `include/variant`). Each modifier
(`emplace<I>`, copy ctor, move ctor, assign) dispatches through
`visitation_variant::visit_alt`, which builds an `fmatrix` of N function
pointers (one per alternative, per visitor instantiation) and indexes it
by `index_`. The fmatrix is `static constexpr` and COMDAT-folded across
all variant flavors that share the same `(Visitor, Vs...)`.

**The redundancy.** For the *lifecycle* modifiers — destroy, copy, move,
swap — the visitor in question is one of four trivial visitors that
collapse to "call the `Is`th alternative's destroy/copy/move/swap leaf".
That builds one fmatrix per `variant<Ts...>` instantiation per modifier,
i.e. ~`4 × N × sizeof(void*)` of `.rodata` per variant flavor (~128 B
for N=4), plus ~4 N tiny one-line function bodies that the compiler
generates as private symbols.

For `variant<int, string>`, that's ~256 B of dispatch infrastructure on
top of the actual leaves. Multiply by the number of distinct variant
flavors a program uses (json-style code is heavy with these) and the
fmatrix-table overhead dominates the cost of `variant` in `.text +
.rodata`.

**The proposed shape.** A `sum_ops` table per-`variant<Ts...>`:

```cpp
struct sum_ops {
    size_t   n_alternatives;
    size_t   size;            // sizeof(variant)
    size_t   align;
    unsigned flags;           // f_triv_destroy / f_triv_copy / f_triv_move
                              //   — the &-of-flags across alternatives
    // Per-alternative lifecycle leaves (one entry per Is).
    // The table has `n_alternatives` rows; each row is a small struct.
    const struct alt_ops* alts;
};

struct alt_ops {
    size_t offset;             // byte offset of the alternative from variant base
    void (*destroy)(void* alt);
    void (*copy_construct)(void* dst_alt, const void* src_alt);
    void (*move_construct)(void* dst_alt, void* src_alt);
};
```

Lifecycle cores then become:

```cpp
void sum_destroy(const sum_ops*, void* self, size_t index);
void sum_copy_construct(const sum_ops*, void* dst, const void* src,
                        size_t src_index);
void sum_move_construct(const sum_ops*, void* dst, void* src,
                        size_t src_index);
// Reads `src_index` (= ((variant*)src)->index_) once, indexes alts[],
// dispatches through the per-alternative slot. The whole-variant
// trivial-copy fast path is checked here.
```

Four cores in `src/support.cpp`, one `sum_ops` table per variant flavor.
Per-flavor cost drops from ~256 B of fmatrix + private bodies to a
~`(N+1) × 32` B table — for N=4, ~160 B. The COMDAT-folding across
flavors with the same triviality profile of alternatives gets noticeably
better than fmatrix-folding does today (fmatrix is keyed on the exact
type list).

**Why this is a `sum_ops`, not a `value_ops` per alternative.** v6 §2.5
introduced `value_ops` for `any` (one type-erased value). A variant has
N type-erased values with one index discriminating which one is live.
Reusing N `value_ops*` per variant would cost N pointer slots per
flavor; one `sum_ops*` with an inline array of `alt_ops` is half the
size and gives the core a single base pointer to walk from.

**The non-lifecycle dispatch (`std::visit`, comparisons, assignment
between different alternatives) stays template-instantiated.** Same
reasoning as v6 §2.6 — `visit`'s visitor signature is the type, and
type-erasing it loses the inlining. v7 only erases the four lifecycle
modifiers.

**Bench claim.** Programs with > 3 variant flavors should see −0.5 to
−1.5 KB in `.text + .rodata`. tomlplusplus uses variant heavily; json
does too if a `json::value`-like type is built on variant. The
`sum_ops` shape pairs with v6's `value_ops` — same authorship rules
(allocator-free, includes `f_triv_*` flags, separate from `type_ops`).

#### 2.1.3 `expected` — same shape as `optional`, no core

`expected<T, E>` is `optional<T>` with a non-monostate error arm.
Per-instantiation cost is two `T`/`E` leaf bundles plus a discriminator.
Same arguments as §2.1.1: no shared body to factor, no trivial fast
path that isn't already memcpy. Status quo.

If a future caller needs sum-style lifecycle erasure for `expected`,
the `sum_ops` table from §2.1.2 covers it (N=2). Not worth landing
until that caller exists.

### 2.2 Product types — `tuple`, `pair`

**Current shape.** `tuple<Ts...>` is an EBO chain (`tuple_leaf<Is, Ts>`
inheritance) and `pair<T1, T2>` is two members with `[[no_unique_address]]`.
All operations are compile-time recursion.

**Why no core.** There is no runtime byte work to share. `tuple`'s
lifecycle ops are inlined N-fold expansions of "construct/destroy
leaf I"; each leaf is `T`'s own leaf. The chain itself has no body —
COMDAT folding happens at the per-`T` leaf level, not at the chain
level. A typeless core would have to walk a table of `{offset, leaf}`
per `Is`, which is exactly the unrolled inline expansion the compiler
already produces, minus the indirect call. Pure regression.

**Status quo.** Compile-time only.

### 2.3 Smart pointers — `<memory>`

#### 2.3.1 `unique_ptr` — no core

`unique_ptr<T, D>` is a pointer + `[[no_unique_address]] D`. The deleter
is template-parameterized. `reset` is "call `del_(ptr_)`, store new
ptr"; copy is deleted; move is two pointer copies plus a deleter move.
The whole thing is ~4 inlined instructions per modifier.

There is nothing to factor. Even the array specialization is just
"call `del_[]`". Status quo.

#### 2.3.2 `shared_ptr` / `weak_ptr` — one new vocab (`sp_cb_ops`)

**Current shape.** `<memory>` ships a `sp_counted_base` virtual base
plus three concrete control-block classes:

```
sp_counted_base               : abstract (pure on_zero_shared/_weak)
sp_counted_ptr_deleter<P,D,A> : stores ptr, deleter, allocator
sp_counted_emplace<T,A>       : stores allocator + inline T storage
```

Each control block carries a vtable pointer (one per concrete class, but
the class is itself parameterized on `(Ptr, Deleter, Alloc)` for
`sp_counted_ptr_deleter` and on `(T, Alloc)` for `sp_counted_emplace`).
So each distinct `shared_ptr` construction path produces one new vtable
symbol + two non-trivial virtual method bodies (`on_zero_shared`,
`on_zero_shared_weak`).

The refcount manipulation itself (`add_shared`, `release_shared`,
`add_weak`, `release_weak`, `lock`) lives non-virtually in
`sp_counted_base` and is already shared across all flavors. The
work-to-share is in the destructors only.

**The redundancy.** `on_zero_shared` is "call `del_(ptr_)`, run
`del_.~D()`" or "call `alloc_traits::destroy(elem)`". `on_zero_shared_weak`
is "rebind allocator, deallocate the control block". Each instantiation
emits ~50 B of body plus a vtable slot pair. For programs that build
`shared_ptr<T>` for many distinct `T` (json-ish, AST-ish workloads,
~30 types), the control-block bodies are 30 × ~100 B ≈ 3 KB of
`shared_ptr`-only `.text` on top of the `make_shared` paths the user
asked for.

**The proposed shape.** Replace the vtable with a vocab table:

```cpp
struct sp_cb_ops {
    // Layout of the control block: where do `ptr`, `deleter`, `alloc`
    // live as byte offsets within the control block body?
    uint16_t ptr_offset;        // 0xffff = no separate ptr (emplace case)
    uint16_t deleter_offset;    // 0xffff = default delete
    uint16_t alloc_offset;      // 0xffff = ::operator new/delete
    uint16_t cb_size;           // total bytes of the control block
    unsigned flags;             // f_alloc_default, f_no_deleter,
                                //   f_inline_emplace (T storage inside the CB)
    // The two lifecycle ops. Take a `void* cb` pointing at the
    // sp_counted_base subobject; index through the offsets above.
    void (*on_zero_shared)(void* cb);
    void (*on_zero_shared_weak)(void* cb);
};

class sp_counted_base {
    long shared_owners_;
    long shared_weak_owners_;
    const sp_cb_ops* ops_;     // replaces vtable pointer
    // ... non-virtual refcount methods unchanged ...
};
```

`release_shared` becomes `if (...) ops_->on_zero_shared(this)` — one
indirect call, same cost as the virtual call it replaces. The wins are
not in *calls saved*; they're in *symbols shared*:

1. The four default-delete-and-`std::allocator<T>` shapes
   (`shared_ptr<T>(new T)`, `make_shared<T>`, `shared_ptr<T[]>`,
   `make_shared<T[]>`) all become one `sp_cb_ops` instance each —
   shared across all `T` with the same triviality profile, because the
   `on_zero_shared` body for "trivially-destructible T, default
   delete" is exactly `void(){}` plus deallocate.
2. The vtable itself goes away. Each control-block instantiation today
   costs `sizeof(vtable) = 2*sizeof(void*)` (offset-to-top + RTTI slot,
   both zero under `-fno-rtti` but still emitted on this target — see
   `sp_counted_base`'s `virtual ~sp_counted_base() = default`). Replacing
   with `const sp_cb_ops*` is one slot.

**Why this beats the current vtable.** Under `-fno-rtti` gcc still emits
the typeinfo pointer slot in the vtable as null, plus the vtable itself
is keyed on the concrete-class type (so
`sp_counted_ptr_deleter<P1,D1,A1>` and `sp_counted_ptr_deleter<P2,D2,A2>`
get separate vtables even if their `on_zero_shared` bodies fold). With
the vocab table, the symbol-deduping is at the body level — bodies fold
across template parameters as soon as their byte work matches. v6's
analysis of `function` (§2.6) is the same argument in a different
subsystem.

**Cost.** One `sp_cb_ops` constant per distinct *(deleter, allocator,
inline-emplace)* combination. For typical programs that's 2–4 tables
total (default-delete-default-alloc, default-delete-default-alloc-emplace,
maybe one or two custom).

**Bench claim.** Programs using `shared_ptr` for > 5 distinct `T` should
see −1 to −3 KB in `.text` (the vtables and per-`T` bodies). Programs
using `shared_ptr` for one `T` see a small loss (one `sp_cb_ops` constant
is ~32 B; the vtable it replaces is 16 B + the body). Break-even at
~3 distinct `T`.

**Why this isn't `value_ops` (from v6 §2.5).** Three differences:

- Carries refcount layout info (`ptr_offset` etc) that `value_ops`
  doesn't need.
- The "destroy" semantics are not `T`'s destructor — they're "deleter
  applied to ptr" which may differ (`unique_ptr<T[]>` calls `delete[]`).
- No `type_id` slot; `shared_ptr` doesn't carry runtime type identity
  (libcis omits the libc++ `get_deleter<D>` API that needed RTTI).

So `sp_cb_ops` is a sibling, same as `value_ops` and `callable_ops`.

#### 2.3.3 `<memory>` allocator machinery — no core

`allocator_traits`, `allocator`, `uses_allocator` are pure compile-time.
`allocator<T>::allocate` is one line. No byte work to factor.

### 2.4 PMR and iostreams — virtuals are the API

#### 2.4.1 `<memory_resource>` — no core

`memory_resource` is a virtual base with `do_allocate`, `do_deallocate`,
`do_is_equal`. Users (and the stdlib) ship subclasses. The vtable **is**
the vocab table for the allocation-resource axis. There is no
"library-side common body" to factor; each resource implementation
(`new_delete_resource`, `monotonic_buffer_resource`,
`unsynchronized_pool_resource`) is its own.

Status quo. (If `pool_resource` ever ships and shows duplication between
the synchronized/unsynchronized flavors, that's a §6.5-style "look
again", not a v7 commitment.)

#### 2.4.2 `<streambuf>` / `<istream>` / `<ostream>` / `<iostream>` — no core, but see §2.4.3

`basic_streambuf<CharT>` is the virtual-base API: `xsputn`, `xsgetn`,
`overflow`, `underflow`, `pbackfail`, `seekoff`, `seekpos`, `sync`. Same
shape as `memory_resource` — the vtable is the vocab.

The `basic_istream` / `basic_ostream` formatted-IO operators (`operator<<`
for arithmetic types, etc.) are template-instantiated per `CharT`
(typically only `char` and `wchar_t`). Each operator's body is "format
to a stack buffer, call `sputn`". The format step delegates to `<format>`
or to `to_chars` / `num_put`; the sputn step is one virtual call. There
is no shared byte work at the operator level that isn't already inside
`<format>` (§2.6) or inside the locale facet (§2.13).

Status quo.

#### 2.4.3 `<sstream>` — one new core (`sb_xsputn_bytes`)

**Current shape.** `basic_stringbuf<CharT>` overrides `xsputn` /
`overflow` to grow its internal `basic_string<CharT>` buffer. The
`xsputn(s, n)` body is "if room, memcpy n*sizeof(CharT) bytes and bump
pptr; else overflow". The `overflow` body is "grow the string by some
factor, retry". Both bodies are per-`CharT` instantiated and **byte
work**, not character-typed work — every operation is a memcpy or a
buffer grow.

**Why a core.** `basic_stringbuf<char>` and `basic_stringbuf<wchar_t>`
share the byte work exactly. The current per-`CharT` instantiation
duplicates ~200 B of buffer-management code. Pull the growth + memcpy
loop into one `sb_xsputn_bytes(string_storage*, const void* src,
size_t n_bytes)` core in `src/support.cpp`, parameterized on
`storage_ops` (because the underlying `basic_string`'s storage already
is). The CharT template instance becomes a trivial forwarder:
`bytes = n * sizeof(CharT); sb_xsputn_bytes(buf_, s, bytes);`.

This is exactly the v5 sandwich applied one level higher — the
streambuf modifier sits on top of `basic_string`, which already sits on
top of `storage_ops`. The new core just bridges them.

**Bench claim.** ~−0.4 KB per program that instantiates both
`stringstream` and `wstringstream`. Programs using only one see parity.

#### 2.4.4 `<fstream>` — no core

`basic_filebuf<CharT>` wraps a `FILE*` (or platform `int fd`). The
`xsputn` body is `fwrite(s, sizeof(CharT), n, fp)` or equivalent. The
byte work is in libc, not in libcis. Per-`CharT` is one body that
COMDAT-folds when CharT is `char` (the common case). Status quo.

The codecvt-conversion path is one-deep into the locale facets (§2.13);
that's where the bytes-vs-chars boundary lives.

### 2.5 `<chrono>` — duration math, tzdb, formatting

#### 2.5.1 Core chrono — no core

`duration<Rep, Period>` and `time_point<Clock, Duration>` are
compile-time arithmetic wrappers. Modifiers (add/sub/cast) are inline
math; no body to factor. Status quo.

#### 2.5.2 `bits/chrono_tzdb.h` and `bits/chrono_tzdb_data.h` — data, not code

The 7208-line `chrono_tzdb_data.h` is data tables. The lookup code in
`chrono_tzdb.h` (647 lines) is one body — already shared across all
callers because there's only one `tzdb`. Status quo.

#### 2.5.3 `bits/chrono_format.h` — deferred

The formatter dispatch is "for each format-spec char, run that
component's print body". The component bodies (`print_year`,
`print_hours_24h`, etc) are each one body, shared across all chrono
types (because `year_month_day` and `hh_mm_ss` decompose into the same
components before formatting). They're **already** shared at the body
level; the dispatch is per-`Char` instantiated.

The question is whether the dispatch loop should be a typeless core
taking `const chrono_fmt_op* ops` (one per format-spec char). Possibly
yes, but the saving is bounded by the dispatch loop size (~100 B per
`CharT`), times two `CharT` instantiations. Maximum ~200 B/program.
Marginal.

**Defer:** check after `<format>` settles (§2.6). If `<format>`'s arg
dispatch gets typeless-erased, chrono formatting should ride on the
same machinery for free.

### 2.6 `<format>` — no core (it's already type-erased)

**Current shape.** `<format>` is 8284 lines, but the bulk is
formatter specializations. The dispatch is already type-erased: each
`basic_format_arg<Context>` is a `(uint8_t arg_t, union value)` pair,
and `visit_format_arg` dispatches on the `arg_t` enum — exactly the
shape v6's `any` and `function` were moving *toward*.

The `arg_t` enum is the runtime tag; the union is the typed payload.
There is no per-`T` vtable to deduplicate; the type erasure is at the
language level. The per-`T` cost is one entry in the `arg_t`-tagged
union for each user formatter, which is unavoidable.

**Why no core.** The `<format>` design already mirrors the cores
philosophy: a fixed-shape runtime vocab (the `arg_t` enum + the union
member set) plus typeless dispatch over it. Adding a `cores`-style
indirection layer on top would duplicate the work the language type
erasure already does.

The one place there *might* be a saving is the format-spec parser
(parses `{:>10.3f}` etc into a `format_specs` struct). Today, gcc
instantiates the parser per `CharT` × per `formatter<T>` specialization
because `format_specs` is parameterized on `CharT`. If the parser body
is large and the CharT-difference is purely "compare against
`CharT('>')`", a typeless `parse_format_spec(const void* in, size_t
in_bytes, format_specs* out, int char_width)` core could share the
parser body across CharT instantiations. **Defer** until the bench
shows this is a measurable line on `format.cpp`-using programs;
predicted saving < 1 KB.

### 2.7 `<regex>` — deferred

**Current shape.** `basic_regex<CharT, Traits>` builds an NFA at
construct time and walks it at match time. NFA build and walk are
per-`(CharT, Traits)` instantiated. For `char` and `wchar_t` users (the
common case), that's two copies of a 2000+ line algorithm.

**Why this looks promising.** The NFA states are byte-encoded
(transition tables, character classes). The walk body is byte work
plus a few traits calls (`Traits::translate`, `Traits::isctype`).
Replacing the traits template parameter with a `regex_traits_ops`
vocab table at the walker boundary could share the walker across both
CharT instantiations.

**Why it's deferred, not committed.** Three risks.

1. **The traits dispatch is *inside* the walker's hot loop.** Every
   character compared goes through traits. Going from inlined to
   indirect is the same trade as v6 §2.8's `sort_bytes` — `.text`
   wins, runtime loses, and regex is a "users care about both" axis.
2. **The character-class tables are CharT-shaped.** A byte-encoded
   table only works for 8-bit CharT directly; for `wchar_t` the table
   has to be bigger or different. Sharing across CharT would mean
   either always paying the wide-table cost or maintaining two table
   formats with one walker.
3. **The bench doesn't probe regex.** Without a `regex_probe.cpp` in
   `bench/codesize.py`, the size claim is uncheckable.

**Decision.** Add a regex probe to the bench first. If the per-`CharT`
duplication exceeds 3 KB/program and the walker can be split from the
traits at the body level, then come back to this. Else, status quo.

### 2.8 Container adaptors — `<stack>`, `<queue>`, `<priority_queue>`

**Current shape.** `stack<T, Container = deque<T>>` is `Container c_`
plus thin forwarders (`push`/`pop`/`top` → `c_.push_back/pop_back/back`).
Same for `queue`. `priority_queue` adds heap algorithms (`push_heap`,
`pop_heap`) from `<algorithm>`.

**Why no core.** The forwarders are zero-cost — the compiler inlines
`stack::push(x)` to `c_.push_back(x)` directly. There's no body to
factor; the body lives in the underlying container, which is already
cored (vector v5, deque v6 §2.4).

`priority_queue`'s heap algorithms are part of `<algorithm>` and follow
that file's rules (v6 §2.8 deferred `sort_bytes`; heap algorithms are
the same trade in miniature).

Status quo.

### 2.9 Views and fixed-size — `<string_view>`, `<span>`, `<array>`, `<bitset>`, `<valarray>`

#### 2.9.1 `<string_view>`, `<span>` — no core

Both are `(ptr, size)` pairs by construction. No allocation, no
lifecycle to share. Member functions (`substr`, `find`, `compare`)
delegate to libc (`memcmp`, `memchr`, `memcpy`) or to `char_traits`.
`find` benefits from the v6 §2.8 `find_byte` core when it lands. No new
core.

#### 2.9.2 `<array>` — no core

`array<T, N>` is `T data[N]`. Aggregate. Lifecycle is `T`'s own,
N-unrolled by the compiler. Status quo.

#### 2.9.3 `<bitset>` — no core (but adjacent to one)

`bitset<N>` is `unsigned long words[(N+63)/64]`. Operations are inline
word-by-word bit ops. They're shared across `N` *only when the
word-count rounds the same way* — which the compiler already inlines.
The bench probably won't notice.

One marginal opportunity: `bitset<N>::to_string()` does a per-bit
loop building a `string`. The body is per-`N` instantiated by way of
the `N`-shaped loop bound. Pulling it into a `bitset_to_string_bytes(
char* out, const void* words, size_t n_bits)` core would share across
all bitset sizes. Saving ~50 B per `(N, CharT)` pair, probably ~3
pairs in a typical program = ~150 B. **Reject:** below the v5 floor
("a core earns its keep when …"); the body is too small.

#### 2.9.4 `<valarray>` — one cold-path core

**Current shape.** `valarray<T>` is a (ptr, size) heap-allocated array
of `T`. Modifiers are like a stripped-down vector (no capacity, no
push_back; the only resize is `resize(n)` reallocating). The
arithmetic operations (`+=`, `*=`, etc) are expression-templated.

**Where the core lives.** The non-template modifiers (`resize`,
`apply`, `swap`, copy ctor, copy assign) are the v5 sandwich without
the growth: allocate exact, copy n elements, free old. They already
share `type_ops` and `storage_ops` if we plug them in. There is one
small new core that earns its keep:

```cpp
void valarray_resize_exact(const type_ops*, const storage_ops*,
                           void* ctx, size_t new_n_bytes);
// allocate exactly new_n_bytes; destroy old elements; free old;
// install new pointer (with size_bytes = new_n_bytes); leave new
// elements default-initialized.
```

Trivial fast path: `f_triv_destroy & f_alloc_default_life` collapses
the destroy loop. This is **smaller than vector's `grow`** because
there's no capacity book-keeping, but it does the same byte work, and
it's called from `valarray::resize` *and* `valarray::operator=` (size
mismatch case) *and* the move-assign-from-shorter case. Three callers,
one body.

**Bench claim.** Per-`T` valarray instantiation drops by ~150 B. Total
program saving: bounded by valarray usage, typically < 0.5 KB.
valarray is a cold corner of the library; not a hot win, but a
clean one.

### 2.10 `<iterator>` and `<ranges>` — no core (compile-time fusion is the point)

`<ranges>` (3973 lines) is view adapters that pipe under
`operator|`. The performance model is: each view's iterator is a thin
wrapper around the underlying iterator, and the compiler inlines the
adapter chain into a single typed loop. Type-erasing any of these
through a vocab table would break the inlining, which is the entire
point.

`std::views::filter`, `std::views::transform`, `std::views::join`, etc.
have lifecycle bodies that are *zero*: their constructors store the
predicate/transform and a base view, and that's it. There is no body
to share.

The one place where type erasure could maybe live is
`views::common` / `views::all` / `std::ranges::ref_view` — but these
are even thinner.

**Status quo.** Compile-time fusion stays. If a future header introduces
`std::function`-shaped view storage (e.g. for runtime view-graph
construction), that's covered by `callable_ops<Sig>` from v6 §2.6.

### 2.11 Concurrency

#### 2.11.1 Atomic, semaphore, barrier, latch — no core

`<atomic>` is `__atomic_*` builtins. Per-`T` instantiation is exactly
the builtin call; no body to share. `<barrier>`, `<semaphore>`, `<latch>`
sit on top — small bodies, single implementation, no shared work.
Status quo.

#### 2.11.2 `<mutex>`, `<shared_mutex>`, `<condition_variable>`, `<stop_token>` — no core

These wrap pthreads / OS handles. The bodies are thin forwarders. Status
quo.

#### 2.11.3 `<thread>` — no core

`thread` stores a `pthread_t` and a callable. The callable is invoked
once at thread start; v6 §2.6's `callable_ops<Sig>` (if landed) would
cover the captured-`F` cost for the `function<void()>`-shaped
trampoline that `thread::start` builds. Otherwise status quo.

#### 2.11.4 `<future>` — one new vocab (`future_callback_ops`)

**Current shape.** `<future>` (1219 lines) builds a shared state
(`assoc_sub_state`) virtual base with derived classes for
`promise<T>::set_value`, `packaged_task<R(A...)>`, `async`'s deferred
function, etc. Each derived class carries the value `T` plus a vtable
with one virtual (`execute()` or `set_value_at_thread_exit`) per
derived class. The pattern is identical to `shared_ptr`'s control
block: virtual base + per-`(T, Callable)` derived class.

**Why a core.** Same argument as §2.3.2. The vtable carries CharT-RTTI-
shaped overhead even under `-fno-rtti`; pulling the callbacks into a
`future_callback_ops` table lets the bodies fold across `(T, Callable)`
pairs that share triviality / lifecycle.

```cpp
struct future_callback_ops {
    size_t value_size;
    size_t value_align;
    unsigned flags;
    void (*destroy_value)(void* state);
    // Set when packaged_task: invoke the stored callable, store result
    // in the value slot, signal the cv.
    void (*invoke)(void* state);   // signature-erased through a void* args slot
};
```

The `invoke` slot is signature-erased the same way `callable_ops<Sig>`
*almost was* in v6 §2.6 — but `<future>`'s callback is always a
`R(void)` (the args have been bound at `packaged_task::operator()`
time). One signature, no per-`Sig` table. So this is more like
`value_ops` than `callable_ops`.

**Bench claim.** `<future>` is less heavily used than `shared_ptr` in
the bench, but programs using `std::async` heavily can save ~0.5 KB.
Borderline. Land only if the shared_ptr-side (§2.3.2) lands and the
shared `sp_cb_ops`-shaped infrastructure can be reused (it can't, but
the v7 author should sanity-check that conclusion).

### 2.12 `<filesystem>` — one core (`path_lexical_normal`)

**Current shape.** `<filesystem>` (2961 lines). `path` holds a
`basic_string<value_type>` and methods do textual manipulation
(append, lexically_normal, parent_path, extension). `directory_iterator`
wraps OS calls.

**Where the core lives.** `path::lexically_normal()` (and friends:
`lexically_relative`, `lexically_proximate`) does a fairly involved
state-machine walk over the path string — split on separators, fold
`.` and `..`, rebuild. The body is per-`CharT` instantiated (path's
char type is platform-dependent: `char` on POSIX, `wchar_t` on
Windows; libcis is POSIX-only so `char`). But the body is ~150 lines
of state-machine code that's pure byte work.

For libcis (POSIX, `char` only), there's only one instantiation —
*no win from a vocab*. The case for a core is therefore weaker than
on a multi-CharT target. **Land it anyway** as `void
path_lexical_normal_bytes(char* buf, size_t* len_in_out)` because:

1. It's >100 B of body, called from at least four `path` methods that
   each currently embed it.
2. It belongs in `src/support.cpp` more cleanly than in the user
   header (it's a self-contained state machine, not a template).
3. Future Windows support (out of scope today, but plausible) would
   need only a CharT-shaped wrapper, not a body rewrite.

**Bench claim.** ~0.3 KB saved per program using `<filesystem>`. Small
but clean.

`directory_iterator`'s OS-call bodies are single-instance; no core.

### 2.13 `<locale>` — no core (facets are the API)

`<locale>` (6830 lines). Same argument as `<streambuf>` and
`<memory_resource>`: each facet is a virtual base with user-derivable
subclasses. The library ships one implementation of each facet
(`num_get`, `num_put`, `time_get`, etc); there's no library-side body
duplication to factor out. The user's subclasses are theirs to optimize.

If `num_put`-style facets shared body work with `<charconv>` and
`<format>`, that would be worth factoring — but they don't today;
`num_put` predates `<charconv>` and does its own conversion. Reviving
the question is a v8 task ("can we route `num_put` through `<charconv>`
and drop the duplicate conversion code"). Out of scope for v7.

### 2.14 `<random>` — no core (template arithmetic)

`<random>` (3935 lines) is engines (`mersenne_twister_engine`,
`linear_congruential_engine`) and distributions (`uniform_int`,
`normal`, `discrete`). All are template-parameterized on `Result_t` /
`UIntType`. The engine bodies are arithmetic with no byte work;
distribution bodies are math on samples.

There's no shared body across distributions (each is a different
algorithm), no shared body across engines (different state shapes).
Per-`T` cost is irreducible. Status quo.

### 2.15 `<typeinfo>`, `<typeindex>` — out of scope under `-fno-rtti`

Same reasoning as v6 §2.7 for exceptions. The whole subsystem is dead
at this target. Status quo.

### 2.16 `<system_error>`, `<stdexcept>` — no core

`<system_error>` uses `error_category` as a virtual base; same shape as
`memory_resource`. No core.

`<stdexcept>` exception classes (`runtime_error`, `logic_error`, etc.)
inherit from `std::exception`. Under `-fno-exceptions`, the throw sites
are replaced with `__builtin_trap()` (see e.g.
`<memory>`'s `throw_bad_weak_ptr`). The exception classes themselves are
mostly inert — they exist to satisfy ABI requirements, not to be
thrown. No core needed; the relevant body is the trap, which is one
instruction.

### 2.17 `<charconv>` — no core

`<charconv>` (1116 lines). `to_chars` / `from_chars` are per-
arithmetic-type instantiated. Each instantiation is an algorithm
(Ryu / Dragon for float, base conversion for int). Bodies are large
but distinct per type — no shared byte work across `(int)` and
`(double)` to factor. COMDAT-folding within a type already happens.
Status quo.

If the bench shows that the `int`/`long` integer-format bodies
COMDAT-fold poorly under `-Os`, *that's* a bug to file; not a v7 cores
proposal.

### 2.18 `<numeric>` — deferred, same as `std::sort`

`accumulate`, `reduce`, `inner_product`, `partial_sum`, `iota`,
`adjacent_difference` are template-parameterized on iterator pair and
binary op. Same trade as v6 §2.8 `sort_bytes` — the typeless version
saves `.text` but pays per-element indirect call. The typed version
inlines the op.

**Decision:** deferred along with `sort_bytes` until the bench
measures runtime. Same conditional: if `.text` win > 1 KB and runtime
hit < 50%, ship.

### 2.19 `<coroutine>`, `<source_location>` — no library code to share

`<coroutine>` is compiler intrinsics + `coroutine_handle` /
`coroutine_traits`. The intrinsics are codegen-side; the library wrappers
are zero. No body. Status quo.

`<source_location>` is one struct + `current()` which is a builtin
intrinsic. Same. Status quo.

### 2.20 `<scoped_allocator>` — no core

`scoped_allocator_adaptor` is an EBO chain of inner+outer allocators.
Pure compile-time. Status quo.

---

## 3. Where things live (the v7 header map)

Adding to v6's header map:

```
  bits/cores.h              (v5/v6 decls)
  bits/segment_map_ops.h    (v6 — deque)
  bits/value_ops.h          (v6 — any/function)
  bits/algo_cores.h         (v6 — equal_bytes, find_byte)
  bits/sum_ops.h            (NEW v7 — variant lifecycle vocab)
  bits/sp_cb_ops.h          (NEW v7 — shared_ptr control-block vocab)
  bits/future_cb_ops.h      (NEW v7 — future shared-state vocab)
  bits/path_normal.h        (NEW v7 — path lexical_normal core decl)
```

`src/support.cpp` grows from v6's ~1100 lines to ~1500. New cores:

```cpp
// (v7, sum types)
void sum_destroy        (const sum_ops*, void* self, size_t index);
void sum_copy_construct (const sum_ops*, void* dst,  const void* src, size_t src_index);
void sum_move_construct (const sum_ops*, void* dst,  void* src,       size_t src_index);

// (v7, sstream)
void sb_xsputn_bytes(const storage_ops*, void* sb_ctx,
                     const void* src, size_t n_bytes);

// (v7, valarray)
void valarray_resize_exact(const type_ops*, const storage_ops*,
                           void* ctx, size_t new_n_bytes);

// (v7, filesystem)
void path_lexical_normal_bytes(char* buf, size_t* len_in_out);
```

Each new core ships in its own `.text.fn_name` section so
`--gc-sections` drops it for programs that never call it.

`sp_cb_ops` and `future_callback_ops` are vocab tables; their bodies
already live inside `<memory>` and `<future>` (the existing virtual
methods become non-virtual functions whose addresses go in the table).
No new lines in `src/support.cpp` for those two — they're a per-flavor
constant `.rodata` saving plus a vtable removal.

---

## 4. Migration order (the recommended commit sequence)

Stage v7 in seven commits, each independently measurable. Order chosen
so each stage's bench probe exists before the stage lands.

1. **Land `sum_ops`, port `variant` lifecycle modifiers.** Bench claim:
   programs with > 3 variant flavors see −0.5 to −1.5 KB. Needs a
   variant-heavy probe in `bench/codesize.py` first
   (`variant_probe.cpp` — three flavors of `variant<int, string, double,
   vector<int>>`-shaped types).
2. **Land `sp_cb_ops`, port `shared_ptr` control blocks.** Bench claim:
   programs with > 5 `shared_ptr<T>` flavors see −1 to −3 KB. Probe:
   `shared_ptr_probe.cpp` with eight `T` flavors and `make_shared` /
   `shared_ptr(new T)` / `shared_ptr(new T[], default_delete<T[]>)`.
3. **Land `sb_xsputn_bytes`, port sstream.** Bench claim: programs
   using both `stringstream` and `wstringstream` see −0.4 KB. Probe:
   `sstream_probe.cpp` exercising both CharT instantiations.
4. **Land `valarray_resize_exact`, port valarray.** Bench claim:
   programs using valarray for > 2 `T` see −0.3 KB. Probe needed;
   may be a wash on programs that don't use valarray (most). Land
   only if a downstream user (`numeric_probe.cpp`?) measurably uses
   valarray.
5. **Land `path_lexical_normal_bytes`, port `<filesystem>`.** Bench
   claim: ~0.3 KB saved per program using `<filesystem>` non-trivially.
   Probe: `filesystem_probe.cpp` exercising `lexically_normal` /
   `lexically_relative` / `parent_path` / `extension` on 50 paths.
6. **Land `future_callback_ops`, port `<future>` shared state.** Bench
   claim: ~0.5 KB per program using `std::async` for > 3 result types.
   Probe: `async_probe.cpp` with five `async<T>` calls.
7. **Add a `regex_probe.cpp` to the bench.** Don't port `<regex>` yet
   — just measure the current cost so v8 can decide. The bench entry
   for this stage has no `.text` delta; it's a measurement-only
   commit.

The `<format>` parser core, the chrono format dispatch core, the
`<numeric>` / `<algorithm>` runtime-erased cores, and the regex
walker port are **explicitly deferred** to v8, gated on bench probes
that don't yet exist.

Each stage is revertable on its own. Each stage has a bench-claim; if
the claim doesn't hold (within 50% — v5 §13's rule), the stage is
rejected, not papered over. Per CLAUDE.md, negative results are
signal: a stage that fails to deliver is a documented "this didn't
pay off" that protects v8 from trying again.

---

## 5. What v7 does **not** propose

Mirroring v5 §13 and v6 §5:

### 5.1 No `optional_ops` vocab

§2.1.1. `optional<T>` has no shared body to factor; the bool test is
layout-shaped and not vocab-shaped.

### 5.2 No `tuple_ops` / `pair_ops`

§2.2. Product types are compile-time only. The EBO chain is the
optimum.

### 5.3 No `unique_ptr` core

§2.3.1. EBO of the deleter already produces the minimum body.

### 5.4 No vocab over `memory_resource` / `streambuf` / `locale::facet`

§2.4.1, §2.4.2, §2.13. The virtuals are the public ABI. Adding a
parallel vocab table would be a sibling type erasure on top of an
existing one — pure duplication.

### 5.5 No core in `<atomic>` / `<barrier>` / `<semaphore>` / `<latch>`

§2.11.1. The bodies are builtins; there is no library code to share.

### 5.6 No core in `<random>`

§2.14. Engine / distribution algorithms are mutually distinct; no
body folds across them.

### 5.7 No runtime erasure of `views::filter` / `views::transform` predicates

§2.10. Compile-time fusion is the entire performance model for
`<ranges>`. Erasing the predicate through `callable_ops<Sig>` would
break the inlining of the adapter chain.

If a user genuinely wants a runtime-pluggable view, they wrap the
predicate in a `std::function` themselves; that's already covered by
v6 §2.6's `callable_ops<Sig>`.

### 5.8 No core in `<charconv>`

§2.17. Each per-type body is a distinct algorithm; no shared work.

### 5.9 No port of `<numeric>` / `<algorithm>` reductions

§2.18 + v6 §2.8. Same deferred-runtime trade as `sort_bytes`. Gate is
the runtime+size bench, which doesn't exist yet.

### 5.10 No port of `<regex>` engine

§2.7. Probe first (stage 7); decide in v8.

### 5.11 No revival of `<exception>` / `<typeinfo>` / `<typeindex>`

§2.15 and v6 §2.7. Dead at `-fno-exceptions -fno-rtti`.

### 5.12 No cross-subsystem fusion (`num_put` ↔ `<charconv>` ↔ `<format>`)

§2.13 closing paragraph. A worthwhile question, but it's about routing
existing logic through one implementation, not about introducing a
core. v8 task.

---

## 6. The invariants v7 adds

Carried from v5 §12 and v6 §6, with two more:

- **Vtables that already exist count as the vocab table.** A virtual
  base in a public API (`memory_resource`, `streambuf`, `locale::facet`,
  `error_category`) is already a runtime vocab. Don't propose a parallel
  vocab on top of it. The cores question for these subsystems is whether
  the *library's* concrete subclasses share byte work with each other —
  not whether we can re-erase what the language already erased.
- **Vtables that exist *because* the library couldn't share bodies are
  fair game.** When the library ships a virtual hierarchy (`shared_ptr`'s
  `sp_counted_base`, `<future>`'s `assoc_sub_state`) whose subclasses
  fold pairwise under COMDAT *if you remove the vtable*, replacing the
  vtable with a vocab table is the v5/v6/v7 sandwich.
  This is the inversion of the previous invariant: user-facing virtuals
  are vocab tables, library-internal virtuals are missed cores.

---

## 7. Bench claims (the numbers v7 commits to)

Per stage in §4. Stated against the v6 baseline (`bench/record.py`
entry "v6-freeze" — assuming v6 lands first; if a v6 stage is reverted
under negative results, restate against the latest accepted baseline).

| stage | program shape | predicted `.text + .rodata` delta |
|---|---|---|
| 1 | variant_probe (3 flavors) | −0.6 KB |
| 1 | tomlplusplus (uses variant) | −0.4 KB |
| 1 | json (uses variant for value) | −0.8 KB |
| 2 | shared_ptr_probe (8 flavors) | −2.1 KB |
| 2 | tomlplusplus (uses shared_ptr) | −1.3 KB |
| 2 | json (uses shared_ptr) | −1.1 KB |
| 3 | sstream_probe (both CharT) | −0.4 KB |
| 3 | every other program | parity |
| 4 | valarray_probe | −0.3 KB |
| 4 | every other program | parity |
| 5 | filesystem_probe | −0.3 KB |
| 5 | other programs | parity (filesystem usage is rare) |
| 6 | async_probe | −0.5 KB |
| 6 | other programs | parity |
| 7 | regex_probe (measure only) | n/a (recorded baseline) |

Numbers are predictions, not measurements. The recorded entry that
lands each stage carries the actual measurement and the diff against
the stage above. A stage whose actual delta misses prediction by more
than 50% is rejected (per CLAUDE.md's "negative results are signal"
rule).

Three probes need to be added to `bench/codesize.py` before their
stages land: `variant_probe.cpp` (stage 1), `shared_ptr_probe.cpp`
(stage 2), `regex_probe.cpp` (stage 7). `sstream_probe.cpp`,
`valarray_probe.cpp`, `filesystem_probe.cpp`, `async_probe.cpp` are
useful but the stages can land against existing programs if the
authors prefer to avoid probe expansion (the measurement just has to
be against *something* that hits the affected code paths).

---

## 8. The asks for the next agent

Carried from v6 §8 with two additions:

1. **State which stage you're on**, predicted vs measured delta. The
   bench journal entry has the measurement; the commit message has
   the diff against prediction.
2. **Don't expand `type_ops` or `storage_ops`.** They're v5-frozen.
   Don't expand `value_ops`, `callable_ops`, or `segment_map_ops`
   either — they're v6-frozen. v7's new tables (`sum_ops`,
   `sp_cb_ops`, `future_callback_ops`) become frozen the moment their
   stage lands.
3. **Don't fold stages.** Each stage's bench claim has to be checkable
   on its own.
4. **If a stage's claim doesn't hold, report negative.**
5. **Read the v5 and v6 invariants.** v5 §12 (five), v6 §6 (three), v7
   §6 (two). Ten invariants now. If you find yourself wanting to
   violate one, you've found a v8 question.
6. **The "user-facing virtual is already a vocab" rule (v7 §6) is the
   load-bearing addition.** Don't propose a `streambuf_ops` or a
   `facet_ops` table; the vtable *is* it. Programs would not benefit,
   and the proposal would re-erase work the language already erased.
7. **Defer questions belong in a v8 note, not in a v7 stage.** Today
   that means: `<format>` parser core, chrono format dispatch core,
   numeric / sort runtime-erased cores, regex walker port, and the
   cross-subsystem `num_put` ↔ `<charconv>` ↔ `<format>` fusion. Each
   is a measurement-pending question; v7 commits not to answer them
   without the missing measurements.

---

## 9. Closing — the asymptote

v5 built the sandwich for contiguous storage and listed the
five-cores set. v6 carried it through the structurally-different
containers, the value-erased wrappers, and the algorithm primitives.
v7 walked everything else and the **dominant verdict is "no"** —
because the rest of the library splits cleanly into:

- Subsystems whose type erasure is already at the language boundary
  (`streambuf`, `memory_resource`, `locale::facet`, `<format>`'s
  `arg_t`). Adding a vocab table on top duplicates the erasure.
- Subsystems whose performance model is compile-time fusion
  (`<ranges>`, `<iterator>`, `<tuple>`, `<chrono>` core, `<random>`).
  Erasing the leaves through a runtime vocab breaks the inlining that
  is the entire point.
- Subsystems whose bodies don't share work across instantiations
  (`<charconv>`, `<atomic>`, `<numeric>` algorithm bodies). No
  duplication to factor.

What's left — three vocab tables (`sum_ops`, `sp_cb_ops`,
`future_callback_ops`) and three small new cores (sstream, valarray,
filesystem path-normal) — is the residue, and the residue is small.
v7's predicted total saving across the eight bench programs is in the
3–7 KB range, mostly from `sp_cb_ops` for programs with heavy
`shared_ptr` use.

After v7 ships, the cores layer is approximately complete. v8's
unfinished questions are all measurement-pending: regex, the
runtime-vs-size trade for `<numeric>` / `<algorithm>`, the
`<format>`/`<charconv>`/`num_put` fusion. None of them are "we forgot
a family"; they're "we didn't have the bench probe yet".

Five cores in v5 (contiguous). Three more + two vocab tables in v6
(segmented + value-erased + algorithm). Three more cores + three more
vocab tables in v7 (sum types + smart pointers + futures + cold corners).
That's the whole library, with the explicit rejections of about
fifteen tempting-but-wrong proposals catalogued so v8 doesn't pay
for the same look twice.

The sandwich was the right shape. Most of the library doesn't need
another one.
