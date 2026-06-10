// AST-transferred from libc++ by tools/transfer.py (slug=input_output_string_streams_istringstream_istringstream_members_view).
// main -> test_input_output_string_streams_istringstream_istringstream_members_view; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <sstream>

// template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT> >
// class basic_istringstream

// basic_string_view<charT, traits> view() const noexcept;

#include <sstream>
#include <cassert>
#include <type_traits>

#include "make_string.h"
#include "test_macros.h"

#define STR(S) MAKE_STRING(CharT, S)
#define SV(S) MAKE_STRING_VIEW(CharT, S)

namespace libcis_ns_input_output_string_streams_istringstream_istringstream_members_view { // libcis: isolate file-scope helpers
template <class CharT>
struct my_char_traits : public std::char_traits<CharT> {};

template <class CharT>
static void test() {
  std::basic_istringstream<CharT> ss(STR(" 123 456"));
  static_assert(noexcept(ss.view()));
  assert(ss.view() == SV(" 123 456"));
  ss.str(STR(" 789"));
  ss.clear();
  assert(ss.view() == SV(" 789"));

  const std::basic_istringstream<CharT> css(STR("abc"));
  static_assert(noexcept(css.view()));
  assert(css.view() == SV("abc"));

  std::basic_istringstream<CharT, my_char_traits<CharT>> tss;
  static_assert(std::is_same_v<decltype(tss.view()), std::basic_string_view<CharT, my_char_traits<CharT>>>);
}
} using namespace libcis_ns_input_output_string_streams_istringstream_istringstream_members_view; // libcis


void test_input_output_string_streams_istringstream_istringstream_members_view() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
  return;
}
