// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_cons_proxy).
// main -> test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_cons_proxy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// istreambuf_iterator

// istreambuf_iterator(const proxy& p) throw();

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_cons_proxy()
{
    {
        std::istringstream inf("abc");
        std::istreambuf_iterator<char> j(inf);
        std::istreambuf_iterator<char> i = j++;
        assert(i != std::istreambuf_iterator<char>());
        assert(*i == 'b');
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wistringstream inf(L"abc");
        std::istreambuf_iterator<wchar_t> j(inf);
        std::istreambuf_iterator<wchar_t> i = j++;
        assert(i != std::istreambuf_iterator<wchar_t>());
        assert(*i == L'b');
    }
#endif

  return;
}
