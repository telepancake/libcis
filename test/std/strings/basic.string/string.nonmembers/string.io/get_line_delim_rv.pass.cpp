// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_nonmembers_string_io_get_line_delim_rv).
// main -> test_strings_basic_string_string_nonmembers_string_io_get_line_delim_rv; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<class charT, class traits, class Allocator>
//   basic_istream<charT,traits>&
//   getline(basic_istream<charT,traits>&& is,
//           basic_string<charT,traits,Allocator>& str, charT delim);

#include <string>
#include <sstream>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_strings_basic_string_string_nonmembers_string_io_get_line_delim_rv { // libcis: isolate file-scope helpers
template <template <class> class Alloc>
void test() {
  {
    using S = std::basic_string<char, std::char_traits<char>, Alloc<char> >;
    S s("initial text");
    std::getline(std::istringstream(" abc*  def*   ghij"), s, '*');
    assert(s == " abc");
  }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  {
    using WS = std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t> >;
    WS s(L"initial text");
    std::getline(std::wistringstream(L" abc*  def*   ghij"), s, L'*');
    assert(s == L" abc");
  }
#endif
}
} using namespace libcis_ns_strings_basic_string_string_nonmembers_string_io_get_line_delim_rv; // libcis


void test_strings_basic_string_string_nonmembers_string_io_get_line_delim_rv() {
  test<std::allocator>();
#if TEST_STD_VER >= 11
  test<min_allocator>();
#endif

  return;
}
