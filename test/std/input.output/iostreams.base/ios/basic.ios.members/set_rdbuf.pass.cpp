// AST-transferred from libc++ by tools/transfer.py (slug=input_output_iostreams_base_ios_basic_ios_members_set_rdbuf).
// main -> test_input_output_iostreams_base_ios_basic_ios_members_set_rdbuf; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <ios>

// template <class charT, class traits> class basic_ios

// void set_rdbuf(basic_streambuf<charT, traits>* sb);

#include <ios>
#include <streambuf>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_input_output_iostreams_base_ios_basic_ios_members_set_rdbuf { // libcis: isolate file-scope helpers
struct testbuf
    : public std::streambuf
{
};

struct testios
    : public std::ios
{
    testios(std::streambuf* p) : std::ios(p) {}
    void set_rdbuf(std::streambuf* x) {std::ios::set_rdbuf(x);}
};
} using namespace libcis_ns_input_output_iostreams_base_ios_basic_ios_members_set_rdbuf; // libcis


void test_input_output_iostreams_base_ios_basic_ios_members_set_rdbuf()
{
    testbuf sb1;
    testbuf sb2;
    testios ios(&sb1);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        ios.setstate(std::ios::badbit);
        ios.exceptions(std::ios::badbit);
        assert(false);
    }
    catch (...)
    {
    }
#endif
    ios.set_rdbuf(&sb2);
    assert(ios.rdbuf() == &sb2);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        ios.setstate(std::ios::badbit);
        ios.exceptions(std::ios::badbit);
    }
    catch (...)
    {
    }
#endif
    ios.set_rdbuf(0);
    assert(ios.rdbuf() == 0);

  return;
}
