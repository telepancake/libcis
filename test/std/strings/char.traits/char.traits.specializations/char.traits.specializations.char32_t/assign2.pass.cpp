// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_60f52d4b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char32_t>

// static void assign(char_type& c1, const char_type& c2);
// constexpr in C++17

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_60f52d4b { // libcis
#if TEST_STD_VER > 14
constexpr bool test_constexpr() {
  char32_t c = U'1';
  std::char_traits<char32_t>::assign(c, U'a');
  return c == U'a';
}
#endif

int main(int, char**) {
#if TEST_STD_VER >= 11
  char32_t c = U'\0';
  std::char_traits<char32_t>::assign(c, U'a');
  assert(c == U'a');
#endif

#if TEST_STD_VER > 14
  static_assert(test_constexpr(), "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_assign2_60f52d4b (libcis)

