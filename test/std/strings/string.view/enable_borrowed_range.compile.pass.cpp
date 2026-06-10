// AST-transferred from libc++ by tools/transfer.py (slug=strings_string_view_enable_borrowed_range).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <string_view>

// template<class charT, class traits>
// inline constexpr bool ranges::enable_borrowed_range<
//     basic_string_view<charT, traits>> = true;

#include <string_view>

#include "test_macros.h"

namespace libcis_ns_strings_string_view_enable_borrowed_range { // libcis: isolate file-scope helpers
void test() {
  using std::ranges::enable_borrowed_range;
  static_assert(enable_borrowed_range<std::basic_string_view<char> >);
  static_assert(enable_borrowed_range<std::basic_string_view<wchar_t> >);
  static_assert(enable_borrowed_range<std::basic_string_view<char8_t> >);
}
} using namespace libcis_ns_strings_string_view_enable_borrowed_range; // libcis

