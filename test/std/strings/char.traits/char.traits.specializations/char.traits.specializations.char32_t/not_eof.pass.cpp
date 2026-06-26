// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_e8f4b524).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char32_t>

// static constexpr int_type not_eof(int_type c);

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_e8f4b524 { // libcis
int main(int, char**) {
#if TEST_STD_VER >= 11
  assert(std::char_traits<char32_t>::not_eof(U'a') == U'a');
  assert(std::char_traits<char32_t>::not_eof(U'A') == U'A');
#endif
  assert(std::char_traits<char32_t>::not_eof(0) == 0);
  assert(std::char_traits<char32_t>::not_eof(std::char_traits<char32_t>::eof()) != std::char_traits<char32_t>::eof());

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char32_t_not_eof_e8f4b524 (libcis)

