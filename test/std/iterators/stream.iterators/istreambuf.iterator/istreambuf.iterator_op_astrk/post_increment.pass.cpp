// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_op_astrk_post_increment).
// main -> test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_op_astrk_post_increment; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// istreambuf_iterator

// proxy istreambuf_iterator<charT,traits>::operator++(int);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_op_astrk_post_increment()
{
    {
        std::istringstream inf("abc");
        std::istreambuf_iterator<char> i(inf);
        assert(*i++ == 'a');
        assert(*i++ == 'b');
        assert(*i++ == 'c');
        assert(i == std::istreambuf_iterator<char>());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wistringstream inf(L"abc");
        std::istreambuf_iterator<wchar_t> i(inf);
        assert(*i++ == L'a');
        assert(*i++ == L'b');
        assert(*i++ == L'c');
        assert(i == std::istreambuf_iterator<wchar_t>());
    }
#endif

  return;
}
