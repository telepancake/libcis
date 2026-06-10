// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_op_dereference).
// main -> test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_op_dereference; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// istreambuf_iterator

// charT operator*() const

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_op_dereference()
{
    {
        std::istringstream inf("abc");
        std::istreambuf_iterator<char> i(inf);
        assert(*i == 'a');
        ++i;
        assert(*i == 'b');
        ++i;
        assert(*i == 'c');
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wistringstream inf(L"abc");
        std::istreambuf_iterator<wchar_t> i(inf);
        assert(*i == L'a');
        ++i;
        assert(*i == L'b');
        ++i;
        assert(*i == L'c');
    }
#endif

  return;
}
