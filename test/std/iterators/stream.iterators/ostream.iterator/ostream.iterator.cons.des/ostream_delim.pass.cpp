// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_delim).
// main -> test_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_delim; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostream_iterator

// ostream_iterator(ostream_type& s, const charT* delimiter);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"


namespace libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_delim { // libcis: isolate file-scope helpers
struct MyTraits : std::char_traits<char> {};

typedef std::basic_ostringstream<char, MyTraits> StringStream;
typedef std::basic_ostream<char, MyTraits> BasicStream;

void operator&(BasicStream const&) {}
} using namespace libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_delim; // libcis


void test_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_delim()
{
    {
        std::ostringstream outf;
        std::ostream_iterator<int> i(outf, ", ");
        assert(outf.good());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wostringstream outf;
        std::ostream_iterator<double, wchar_t> i(outf, L", ");
        assert(outf.good());
    }
#endif
    {
        StringStream outf;
        std::ostream_iterator<int, char, MyTraits> i(outf, ", ");
        assert(outf.good());
    }

  return;
}
