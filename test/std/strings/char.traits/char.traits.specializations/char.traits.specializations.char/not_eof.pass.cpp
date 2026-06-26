// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_a25e4bfc).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char>

// static constexpr int_type not_eof(int_type c);

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_a25e4bfc { // libcis
int main(int, char**) {
  assert(std::char_traits<char>::not_eof('a') == 'a');
  assert(std::char_traits<char>::not_eof('A') == 'A');
  assert(std::char_traits<char>::not_eof(0) == 0);
  assert(std::char_traits<char>::not_eof(std::char_traits<char>::eof()) != std::char_traits<char>::eof());

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char_not_eof_a25e4bfc (libcis)

