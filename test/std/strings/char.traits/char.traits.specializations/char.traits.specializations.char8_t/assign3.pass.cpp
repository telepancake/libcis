// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_assign3_312ecbe2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <string>

// template<> struct char_traits<char8_t>

// static char_type* assign(char_type* s, size_t n, char_type a);

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_assign3_312ecbe2 { // libcis
TEST_CONSTEXPR_CXX20 bool test() {
#ifndef TEST_HAS_NO_CHAR8_T
  char8_t s2[3] = {0};
  assert(std::char_traits<char8_t>::assign(s2, 3, char8_t(5)) == s2);
  assert(s2[0] == char8_t(5));
  assert(s2[1] == char8_t(5));
  assert(s2[2] == char8_t(5));
  assert(std::char_traits<char8_t>::assign(NULL, 0, char8_t(5)) == NULL);
#endif

  return true;
}

int main(int, char**) {
  test();

#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char8_t_assign3_312ecbe2 (libcis)

