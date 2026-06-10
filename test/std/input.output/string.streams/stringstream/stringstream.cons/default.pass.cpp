// AST-transferred from libc++ by tools/transfer.py (slug=input_output_string_streams_stringstream_stringstream_cons_default).
// main -> test_input_output_string_streams_stringstream_stringstream_cons_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <sstream>

// template <class charT, class traits = char_traits<charT>, class Allocator = allocator<charT> >
// class basic_stringstream

// explicit basic_stringstream(ios_base::openmode which = ios_base::out | ios_base::in); // before C++20
// basic_stringstream() : basic_stringstream(ios_base::out | ios_base::in) {}            // C++20
// explicit basic_stringstream(ios_base::openmode which);                                // C++20

#include <sstream>
#include <cassert>

#include "test_macros.h"
#include "operator_hijacker.h"
#if TEST_STD_VER >= 11
#include "test_convertible.h"

namespace libcis_ns_input_output_string_streams_stringstream_stringstream_cons_default { // libcis: isolate file-scope helpers
template <typename S>
void test() {
  static_assert(test_convertible<S>(), "");
  static_assert(!test_convertible<S, std::ios_base::openmode>(), "");
}
} using namespace libcis_ns_input_output_string_streams_stringstream_stringstream_cons_default; // libcis

#endif

void test_input_output_string_streams_stringstream_stringstream_cons_default()
{
    {
        std::stringstream ss;
        assert(ss.rdbuf() != nullptr);
        assert(ss.good());
        assert(ss.str() == "");
    }
    {
      std::basic_stringstream<char, std::char_traits<char>, operator_hijacker_allocator<char> > ss;
      assert(ss.rdbuf() != nullptr);
      assert(ss.good());
      assert(ss.str() == "");
    }
    {
        std::stringstream ss(std::ios_base::in);
        assert(ss.rdbuf() != nullptr);
        assert(ss.good());
        assert(ss.str() == "");
    }
    {
      std::basic_stringstream<char, std::char_traits<char>, operator_hijacker_allocator<char> > ss(std::ios_base::in);
      assert(ss.rdbuf() != nullptr);
      assert(ss.good());
      assert(ss.str() == "");
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wstringstream ss;
        assert(ss.rdbuf() != nullptr);
        assert(ss.good());
        assert(ss.str() == L"");
    }
    {
        std::wstringstream ss(std::ios_base::in);
        assert(ss.rdbuf() != nullptr);
        assert(ss.good());
        assert(ss.str() == L"");
    }
#endif

#if TEST_STD_VER >= 11
    test<std::stringstream>();
#  ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<std::wstringstream>();
#   endif
#endif

    return;
}
