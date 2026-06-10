// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_dynamic_destroying_delete_t_declaration).
// main -> test_language_support_support_dynamic_destroying_delete_t_declaration; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// struct destroying_delete_t {
//   explicit destroying_delete_t() = default;
// };
// inline constexpr destroying_delete_t destroying_delete{};

// UNSUPPORTED: c++03, c++11, c++14, c++17

// Test only the library parts of destroying delete in this test.
// Verify that it's properly declared after C++17 and that it's constexpr.
//
// Other tests will check the language side of things -- but those are
// limited to newer compilers.

#include <new>

#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "test_convertible.h"

#ifdef __cpp_impl_destroying_delete
# ifndef __cpp_lib_destroying_delete
#   error "Expected __cpp_lib_destroying_delete to be defined"
#   elif __cpp_lib_destroying_delete < 201806L
#     error "Unexpected value of __cpp_lib_destroying_delete"
#   endif
#else
# ifdef __cpp_lib_destroying_delete
#   error "__cpp_lib_destroying_delete should not be defined unless the compiler supports it"
# endif
#endif

namespace libcis_ns_language_support_support_dynamic_destroying_delete_t_declaration { // libcis: isolate file-scope helpers
constexpr bool test_constexpr(std::destroying_delete_t) {
  return true;
}
} using namespace libcis_ns_language_support_support_dynamic_destroying_delete_t_declaration; // libcis


void test_language_support_support_dynamic_destroying_delete_t_declaration() {
  static_assert(std::is_default_constructible<std::destroying_delete_t>::value, "");
  static_assert(!test_convertible<std::destroying_delete_t>(), "");
  constexpr std::destroying_delete_t dd{};
  static_assert(&dd != &std::destroying_delete, "");
  static_assert(test_constexpr(std::destroying_delete), "");
  return;
}
