// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_c9a6bc78).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char16_t>

// static constexpr int_type to_int_type(char_type c);

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_c9a6bc78 { // libcis
int main(int, char**) {
#if TEST_STD_VER >= 11
  assert(std::char_traits<char16_t>::to_int_type(u'a') == u'a');
  assert(std::char_traits<char16_t>::to_int_type(u'A') == u'A');
#endif
  assert(std::char_traits<char16_t>::to_int_type(0) == 0);

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_to_int_type_c9a6bc78 (libcis)

