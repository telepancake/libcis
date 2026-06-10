// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostreambuf_iterator_ostreambuf_iter_ops_assign_c).
// main -> test_iterators_stream_iterators_ostreambuf_iterator_ostreambuf_iter_ops_assign_c; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostreambuf_iterator

// ostreambuf_iterator<charT,traits>&
//   operator=(charT c);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_ostreambuf_iterator_ostreambuf_iter_ops_assign_c()
{
    {
        std::ostringstream outf;
        std::ostreambuf_iterator<char> i(outf);
        i = 'a';
        assert(outf.str() == "a");
        i = 'b';
        assert(outf.str() == "ab");
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wostringstream outf;
        std::ostreambuf_iterator<wchar_t> i(outf);
        i = L'a';
        assert(outf.str() == L"a");
        i = L'b';
        assert(outf.str() == L"ab");
    }
#endif

  return;
}
