// AST-transferred from libc++ by tools/transfer.py (slug=strings_basic_string_string_nonmembers_string_io_stream_insert).
// main -> test_strings_basic_string_string_nonmembers_string_io_stream_insert; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <string>

// template<class charT, class traits, class Allocator>
//   basic_ostream<charT, traits>&
//   operator<<(basic_ostream<charT, traits>& os,
//              const basic_string<charT,traits,Allocator>& str);

#include <string>
#include <sstream>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"

namespace libcis_ns_strings_basic_string_string_nonmembers_string_io_stream_insert { // libcis: isolate file-scope helpers
template <template <class> class Alloc>
void test() {
  using S  = std::basic_string<char, std::char_traits<char>, Alloc<char> >;
  using OS = std::basic_ostringstream<char, std::char_traits<char>, Alloc<char> >;
  {
    OS out;
    S s("some text");
    out << s;
    assert(out.good());
    assert(s == out.str());
  }
  {
    OS out;
    S s("some text");
    out.width(12);
    out << s;
    assert(out.good());
    assert("   " + s == out.str());
  }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  using WS  = std::basic_string<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t> >;
  using WOS = std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, Alloc<wchar_t> >;
  {
    WOS out;
    WS s(L"some text");
    out << s;
    assert(out.good());
    assert(s == out.str());
  }
  {
    WOS out;
    WS s(L"some text");
    out.width(12);
    out << s;
    assert(out.good());
    assert(L"   " + s == out.str());
  }
#endif
}
} using namespace libcis_ns_strings_basic_string_string_nonmembers_string_io_stream_insert; // libcis


void test_strings_basic_string_string_nonmembers_string_io_stream_insert() {
  test<std::allocator>();

#if TEST_STD_VER >= 11
  test<min_allocator>();
#endif

  return;
}
