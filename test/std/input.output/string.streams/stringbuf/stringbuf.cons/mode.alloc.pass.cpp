// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_string_streams_stringbuf_stringbuf_cons_mode_alloc_1554ac30).
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

// basic_stringbuf(ios_base::openmode which, const Allocator& a);

#include <sstream>
#include <cassert>

#include "test_allocator.h"
#include "test_macros.h"

namespace libcis_ns_input_output_string_streams_stringbuf_stringbuf_cons_mode_alloc_1554ac30 { // libcis
template <class CharT>
static void test() {
  const test_allocator<CharT> a(2);
  const std::basic_stringbuf<CharT, std::char_traits<CharT>, test_allocator<CharT>> buf(std::ios_base::in, a);
  assert(buf.get_allocator() == a);
  assert(buf.view().empty());
}

int main(int, char**) {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
  return 0;

    return 0;
}
} // libcis_ns_input_output_string_streams_stringbuf_stringbuf_cons_mode_alloc_1554ac30 (libcis)

