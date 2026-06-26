// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_4aaa2037).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char16_t>

// static size_t length(const char_type* s);
// constexpr in C++17

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_4aaa2037 { // libcis
#if TEST_STD_VER > 14
constexpr bool test_constexpr() {
  return std::char_traits<char16_t>::length(u"") == 0 && std::char_traits<char16_t>::length(u"abcd") == 4;
}
#endif

int main(int, char**) {
#if TEST_STD_VER >= 11
  assert(std::char_traits<char16_t>::length(u"") == 0);
  assert(std::char_traits<char16_t>::length(u"a") == 1);
  assert(std::char_traits<char16_t>::length(u"aa") == 2);
  assert(std::char_traits<char16_t>::length(u"aaa") == 3);
  assert(std::char_traits<char16_t>::length(u"aaaa") == 4);
#endif

#if TEST_STD_VER > 14
  static_assert(test_constexpr(), "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_length_4aaa2037 (libcis)

