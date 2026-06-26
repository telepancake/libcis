// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_5ce499e5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<char16_t>

// static constexpr bool eq(char_type c1, char_type c2);

// UNSUPPORTED: c++03

#include <string>
#include <cassert>

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_5ce499e5 { // libcis
int main(int, char**) {
  assert(std::char_traits<char16_t>::eq(u'a', u'a'));
  assert(!std::char_traits<char16_t>::eq(u'a', u'A'));
  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_char16_t_eq_5ce499e5 (libcis)

