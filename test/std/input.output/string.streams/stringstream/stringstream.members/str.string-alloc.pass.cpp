// AST-transferred from libc++ by tools/transfer.py (slug=input_output_string_streams_stringstream_stringstream_members_str_string_alloc).
// main -> test_input_output_string_streams_stringstream_stringstream_members_str_string_alloc; file-scope helpers isolated in anon namespace.
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
// class basic_stringstream

// template <class SAlloc>
// void str(const basic_string<charT, traits, SAlloc>& s);

#include <sstream>
#include <cassert>

#include "make_string.h"
#include "test_allocator.h"
#include "test_macros.h"

#define STR(S) MAKE_STRING(CharT, S)

namespace libcis_ns_input_output_string_streams_stringstream_stringstream_members_str_string_alloc { // libcis: isolate file-scope helpers
template <class CharT>
static void test() {
  {
    const test_allocator<CharT> a(6);
    const std::basic_string<CharT, std::char_traits<CharT>, test_allocator<CharT>> s(STR("testing"), a);
    std::basic_stringstream<CharT> ss;
    ss.str(s);
    assert(ss.str() == STR("testing"));
  }
}
} using namespace libcis_ns_input_output_string_streams_stringstream_stringstream_members_str_string_alloc; // libcis


void test_input_output_string_streams_stringstream_stringstream_members_str_string_alloc() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
  return;
}
