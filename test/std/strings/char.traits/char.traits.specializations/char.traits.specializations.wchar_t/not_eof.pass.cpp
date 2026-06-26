// transferred+adapted from libc++ by tools/transfer.py (slug=strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_48e62e78).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<> struct char_traits<wchar_t>

// static constexpr int_type not_eof(int_type c);

// UNSUPPORTED: no-wide-characters

#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_48e62e78 { // libcis
int main(int, char**) {
  assert(std::char_traits<wchar_t>::not_eof(L'a') == L'a');
  assert(std::char_traits<wchar_t>::not_eof(L'A') == L'A');
  assert(std::char_traits<wchar_t>::not_eof(0) == 0);
  assert(std::char_traits<wchar_t>::not_eof(std::char_traits<wchar_t>::eof()) != std::char_traits<wchar_t>::eof());

  return 0;

    return 0;
}
} // libcis_ns_strings_char_traits_char_traits_specializations_char_traits_specializations_wchar_t_not_eof_48e62e78 (libcis)

