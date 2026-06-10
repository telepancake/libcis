// AST-transferred from libc++ by tools/transfer.py (slug=input_output_string_streams_ostringstream_ostringstream_members_str_alloc).
// main -> test_input_output_string_streams_ostringstream_ostringstream_members_str_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <sstream>

// template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT>>
// class basic_ostringstream

// template <class SAlloc>
// basic_string<charT, traits, SAlloc> str(const SAlloc& sa) const;

#include <sstream>
#include <cassert>

#include "make_string.h"
#include "test_allocator.h"
#include "test_macros.h"

#define STR(S) MAKE_STRING(CharT, S)
#define SV(S) MAKE_STRING_VIEW(CharT, S)

namespace libcis_ns_input_output_string_streams_ostringstream_ostringstream_members_str_alloc { // libcis: isolate file-scope helpers
template <class CharT>
static void test() {
  const std::basic_ostringstream<CharT> ss(STR("testing"));
  const test_allocator<CharT> a(1);
  const std::basic_string<CharT, std::char_traits<CharT>, test_allocator<CharT>> s = ss.str(a);
  assert(s == SV("testing"));
  assert(s.get_allocator() == a);
}
} using namespace libcis_ns_input_output_string_streams_ostringstream_ostringstream_members_str_alloc; // libcis


void test_input_output_string_streams_ostringstream_ostringstream_members_str_alloc() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
  return;
}
