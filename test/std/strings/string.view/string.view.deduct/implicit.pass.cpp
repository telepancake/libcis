// transferred+adapted from libc++ by tools/transfer.py (slug=strings_string_view_string_view_deduct_implicit_37d8ac92).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <string_view>

// basic_string_view

// Make sure that the implicitly-generated CTAD works.

#include <string_view>

#include "test_macros.h"

namespace libcis_ns_strings_string_view_string_view_deduct_implicit_37d8ac92 { // libcis
int main(int, char**) {
  {
    char const* str = "hello world";
    std::basic_string_view sv(str);
    ASSERT_SAME_TYPE(decltype(sv), std::basic_string_view<char>);
  }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  {
    wchar_t const* str = L"hello world";
    std::basic_string_view sv(str);
    ASSERT_SAME_TYPE(decltype(sv), std::basic_string_view<wchar_t>);
  }
#endif

  return 0;

    return 0;
}
} // libcis_ns_strings_string_view_string_view_deduct_implicit_37d8ac92 (libcis)

