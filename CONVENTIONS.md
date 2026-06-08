# libcis conventions

A faithful but minimal, easy-to-read reimplementation of LLVM's libc++.

## Target

- Compiler: gcc-10.2 with the highest available language standard (`-std=gnu++20`).
- Flags: `-fno-exceptions -fno-rtti`.
- Single build, maximally featureful. No compatibility shims for older compilers.
- Features that *require* exceptions or RTTI are omitted. Features with *optional*
  exception/RTTI support drop that support.

## Style

- Everything lives in `namespace std`.
- **No trailing or leading underscores on names.** Write `template<class T>`,
  `for (size_t i = 0; ...)`, `value`, `type` — never `__T`, `__i`, `_Tp`.
- None of libc++'s portability/ABI macros. Spell them out:
  - `_NOEXCEPT` -> `noexcept`
  - `_LIBCPP_HIDE_FROM_ABI`, `_LIBCPP_INLINE_VISIBILITY` -> nothing (or `inline`)
  - `_LIBCPP_CONSTEXPR`, `_LIBCPP_CONSTEXPR_SINCE_CXXNN` -> `constexpr`
  - `_LIBCPP_DEPRECATED_IN_CXX17` and friends -> nothing
  - `_VSTD::` -> `std::` (or nothing inside `namespace std`)
  - `_LIBCPP_TEMPLATE_VIS`, `_LIBCPP_EXPORTED_FROM_ABI` -> nothing
- Maximally use compiler builtins (`__is_class`, `__is_base_of`,
  `__underlying_type`, `__builtin_*`, ...) for things that need compiler magic.
- Prefer `requires`, `if constexpr`, and concepts over SFINAE. Reduce SFINAE to
  the cases where nothing else works.

## Header layout

- User-facing headers live directly in `include/` with no extension
  (`include/type_traits`, `include/memory`, ...).
- Implementation goes **in the user header itself**. `<memory>` contains
  `shared_ptr`, not a separate impl file.
- User headers `#include` each other directly.
- `include/bits/` holds internal headers **only** for things that genuinely
  cannot live in a user header (e.g. shared by several headers in a way that
  would cause a cycle, or compiler-runtime glue).

## Traits: `_v` is primary, the struct/`_t` wraps it

For boolean traits, the logic lives in the `_v` constexpr variable; the trait
struct just inherits `bool_constant<trait_v<...>>`:

```cpp
template<class T, class U>
inline constexpr bool is_same_v = __is_same(T, U);

template<class T, class U>
struct is_same : bool_constant<is_same_v<T, U>> {};
```

For type-transformation traits, the struct holds `::type` and the `_t` alias
wraps it: `using remove_const_t = typename remove_const<T>::type;`.

When a predicate would otherwise need SFINAE, compute it in a `constexpr`
function with `if constexpr` and assign to the `_v` variable.

## Tests

- One file per feature in `test/`, e.g. `test/type_traits.cpp`.
- Each test is a free function `void test_<something>()`. They take no arguments.
- The `main()` that calls every `test_` function is **auto-generated** by
  grepping for `test_` functions (see `tools/gen_main.sh`); never maintained by
  hand.
- No progress printing. A test **crashes** on failure so that gdb/valgrind show a
  backtrace at the failing line. Use `CHECK(expr)` from `test/test.h` (traps) for
  runtime checks and `static_assert` for compile-time checks.
- Build & run everything with `./build.sh`.

## Porting contract (for agents)

This is **not** a from-scratch implementation. You are mechanically transforming
the real libc++ source files into libcis. Do not invent code from memory; start
from the actual source.

- **Input:** `/home/user/llvm-project/libcxx/include/` — the user header and the
  `__detail/*.h` files it includes.
- **Output:** `/home/user/libcis/include/<header>` — fold every `__detail`
  fragment the header pulls in *directly into the user header*. Only put
  something in `include/bits/` if it genuinely cannot live in a user header
  (e.g. a true include cycle). User headers `#include` each other directly.
- **Target:** g++-10.2, `-std=gnu++20 -fno-exceptions -fno-rtti`.
- **Verify on the real compiler:** build with `CXX=g++-10 ./build.sh`.

### Mechanical substitutions

| libc++ | libcis |
|---|---|
| `_LIBCPP_BEGIN_NAMESPACE_STD` / `_LIBCPP_END_NAMESPACE_STD` (and the UNVERSIONED variants) | `namespace std {` / `}` |
| `_LIBCPP_HIDE_FROM_ABI`, `_LIBCPP_INLINE_VISIBILITY`, `_LIBCPP_TEMPLATE_VIS`, `_LIBCPP_EXPORTED_FROM_ABI`, `_LIBCPP_HIDDEN` | (delete) |
| `_LIBCPP_CONSTEXPR`, `_LIBCPP_CONSTEXPR_SINCE_CXXNN` | `constexpr` |
| `_NOEXCEPT`, `_NOEXCEPT_(x)` | `noexcept`, `noexcept(x)` |
| `_LIBCPP_DEPRECATED*`, `_LIBCPP_NODISCARD*` | (delete, or `[[nodiscard]]`) |
| `_VSTD::`, `_LIBCPP_STD_VER` checks | `std::`, resolve for C++20 |
| `__enable_if_t<...>` SFINAE | prefer a `requires` clause |
| names with leading/trailing underscores (`_Tp`, `__x`, `value_type_`) | strip underscores (`T`, `x`, `value_type`) |
| `#include <__config>`, `#pragma GCC system_header`, `_LIBCPP_USE_FROZEN_CXX03_HEADERS` blocks, `__has_no_*` guards | (delete) |

- Delete pre-C++20 branches; keep the newest standard's path. Delete
  exception/RTTI code paths and any optional exception/RTTI support.
- `_v` is primary (logic in the constexpr variable); the trait struct and `_t`
  alias wrap it. Prefer `requires`/`if constexpr` over SFINAE.
- Use `for (T i = n; ...)`, never `for (T __i = __n; ...)`.

### Builtins (CRITICAL — target is gcc-10.2, not gcc-13)

libc++ `main` uses many builtins added in gcc-13/14 that **do not exist in
gcc-10.2**. Where libc++ guards a builtin with `#if __has_builtin(...)` and
provides a non-builtin `#else` fallback, and the builtin is unavailable in
gcc-10.2, **keep the fallback**. Verify every builtin you use by compiling a
probe with `g++-10 -std=gnu++20`.

Known-available in gcc-10.2 (use freely): `__is_same` (CONFIRMED available),
`__is_class`, `__is_union`, `__is_enum`, `__is_base_of`, `__is_abstract`,
`__is_polymorphic`, `__is_final`, `__is_empty`, `__is_aggregate`,
`__is_standard_layout`, `__is_trivial`, `__is_trivially_copyable`, `__is_pod`,
`__has_virtual_destructor`, `__has_unique_object_representations`,
`__underlying_type`, `__is_constructible`, `__is_assignable`,
`__is_trivially_constructible`, `__is_trivially_assignable`,
`__has_trivial_destructor`, `__builtin_*` runtime helpers.

Known-MISSING in gcc-10.2 (implement in C++, do NOT use the builtin):
`__is_convertible`, `__is_nothrow_convertible`,
`__is_nothrow_constructible`, `__is_nothrow_assignable`, `__is_function`,
`__is_pointer`, `__is_array`, `__is_reference`, `__is_const`, `__is_volatile`,
`__is_integral`, `__is_arithmetic`, `__remove_cv`, `__remove_reference`,
`__remove_cvref`, `__remove_pointer`, `__remove_all_extents`,
`__remove_extent`, `__add_pointer`, `__add_lvalue_reference`,
`__add_rvalue_reference`, `__decay`, `__make_signed`, `__make_unsigned`,
`__is_destructible`, `__is_nothrow_destructible`, `__is_scalar`, `__is_object`,
`__is_fundamental`, `__is_compound`, `__is_null_pointer`, `__is_signed`,
`__is_unsigned`, `__is_member_pointer`, `__is_member_object_pointer`,
`__is_member_function_pointer`, `__is_scoped_enum`, `__is_bounded_array`,
`__is_unbounded_array`, `__array_extent`, `__array_rank`,
`__builtin_bit_cast` (gcc-11+), `__type_pack_element`. (Probed and verified
against gcc-10.2.)

### No duplication of std vocabulary or detail helpers (ODR)

All headers live in one `namespace std` and frequently meet in a single
translation unit. Therefore:

- NEVER redefine `move`, `forward`, `swap`, `exchange`, `declval`, etc. — include
  the owning user header (`<utility>`, `<type_traits>`) and use them.
- Before inventing a `namespace std::detail` (or `std::ranges::...`) helper,
  assume it may already exist in another header. Reuse the existing one by
  including its header. Do not create a second definition with the same name.
- If you hit a redefinition/ambiguity conflict between two headers, that is a
  REAL bug to report (or fix at its single source) — do NOT work around it by
  copying the definition locally or adding `#ifndef` guard macros. Duplicated
  definitions of the same template in one TU are an ODR violation and a latent
  build break.
