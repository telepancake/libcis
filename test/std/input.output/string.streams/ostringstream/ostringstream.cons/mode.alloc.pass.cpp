// AST-transferred from libc++ by tools/transfer.py (slug=input_output_string_streams_ostringstream_ostringstream_cons_mode_alloc).
// main -> test_input_output_string_streams_ostringstream_ostringstream_cons_mode_alloc; file-scope helpers isolated in anon namespace.
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

// basic_ostringstream(ios_base::openmode which, const Allocator& a);

#include <sstream>
#include <cassert>

#include "test_allocator.h"
#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_input_output_string_streams_ostringstream_ostringstream_cons_mode_alloc { // libcis: isolate file-scope helpers
template <class CharT, class Allocator>
static void test(const Allocator& a) {
  const std::basic_ostringstream<CharT, std::char_traits<CharT>, Allocator> ss(std::ios_base::binary, a);
  assert(ss.rdbuf()->get_allocator() == a);
  assert(ss.view().empty());
}
} using namespace libcis_ns_input_output_string_streams_ostringstream_ostringstream_cons_mode_alloc; // libcis


void test_input_output_string_streams_ostringstream_ostringstream_cons_mode_alloc() {
  test<char>(test_allocator<char>(2));
  test<char>(operator_hijacker_allocator<char>());
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>(test_allocator<wchar_t>(2));
  test<wchar_t>(operator_hijacker_allocator<wchar_t>());
#endif

  return;
}
