// transferred+adapted from libc++ by tools/transfer.py (slug=input_output_string_streams_stringstream_stringstream_cons_string_alloc_mode_2500f2c3).
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
// explicit basic_stringstream(const basic_string<char_type, traits_type, SAlloc>& s, ios_base::openmode which = ios_base::out | ios_base::in)

#include <sstream>
#include <cassert>

#include "make_string.h"
#include "test_allocator.h"
#include "operator_hijacker.h"
#include "test_macros.h"

#define STR(S) MAKE_STRING(CharT, S)
#define SV(S) MAKE_STRING_VIEW(CharT, S)

namespace libcis_ns_input_output_string_streams_stringstream_stringstream_cons_string_alloc_mode_2500f2c3 { // libcis
template <class CharT, class Allocator>
static void test() {
  {
    const std::basic_string<CharT> s(STR("testing"));
    const std::basic_stringstream<CharT, std::char_traits<CharT>, Allocator> ss(s);
    assert(ss.view() == SV("testing"));
  }
  {
    const std::basic_string<CharT> s(STR("testing"));
    const std::basic_stringstream<CharT, std::char_traits<CharT>, Allocator> ss(s, std::ios_base::in);
    assert(ss.view() == SV("testing"));
  }
}

int main(int, char**) {
  test<char, test_allocator<char>>();
  test<char, operator_hijacker_allocator<char>>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t, test_allocator<wchar_t>>();
  test<wchar_t, operator_hijacker_allocator<wchar_t>>();
#endif
  return 0;

    return 0;
}
} // libcis_ns_input_output_string_streams_stringstream_stringstream_cons_string_alloc_mode_2500f2c3 (libcis)

