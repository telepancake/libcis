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
