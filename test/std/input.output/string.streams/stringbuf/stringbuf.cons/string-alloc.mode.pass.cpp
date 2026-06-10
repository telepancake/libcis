// AST-transferred from libc++ by tools/transfer.py (slug=input_output_string_streams_stringbuf_stringbuf_cons_string_alloc_mode).
// main -> test_input_output_string_streams_stringbuf_stringbuf_cons_string_alloc_mode; file-scope helpers isolated in anon namespace.
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
// class basic_stringbuf

// template <class SAlloc>
// explicit basic_stringbuf(const basic_string<charT, traits, SAlloc>& s,
//                          ios_base::openmode which = ios_base::in | ios_base::out);

#include <sstream>
#include <cassert>

#include "make_string.h"
#include "test_allocator.h"
#include "test_macros.h"

#define STR(S) MAKE_STRING(CharT, S)
#define SV(S) MAKE_STRING_VIEW(CharT, S)

namespace libcis_ns_input_output_string_streams_stringbuf_stringbuf_cons_string_alloc_mode { // libcis: isolate file-scope helpers
template <class CharT>
static void test() {
  const std::basic_string<CharT, std::char_traits<CharT>, test_allocator<CharT>> s(STR("testing"));
  const std::basic_stringbuf<CharT> buf(s, std::ios_base::in);
  assert(buf.view() == SV("testing"));
}
} using namespace libcis_ns_input_output_string_streams_stringbuf_stringbuf_cons_string_alloc_mode; // libcis


void test_input_output_string_streams_stringbuf_stringbuf_cons_string_alloc_mode() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
  return;
}
