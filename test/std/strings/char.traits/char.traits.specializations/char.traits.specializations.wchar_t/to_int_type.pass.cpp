// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_315ebb20).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<wchar_t>

// static constexpr int_type to_int_type(char_type c);

// UNSUPPORTED: no-wide-characters

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_315ebb20 { // libcis
int main(int, char**) {
  assert(std::char_traits<wchar_t>::to_int_type(L'a') == L'a');
  assert(std::char_traits<wchar_t>::to_int_type(L'A') == L'A');
  assert(std::char_traits<wchar_t>::to_int_type(0) == 0);

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_to_int_type_315ebb20 (libcis)

