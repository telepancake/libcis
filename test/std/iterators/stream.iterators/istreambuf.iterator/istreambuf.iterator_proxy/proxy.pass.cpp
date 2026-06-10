// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_proxy_proxy).
// main -> test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_proxy_proxy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// template<class charT, class traits = char_traits<charT> >
// class istreambuf_iterator {
// public:
//     ...
//     proxy operator++(int);

// class proxy
// {
// public:
//     charT operator*();
// };

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_istreambuf_iterator_istreambuf_iterator_proxy_proxy()
{
    {
        std::istringstream inf("abc");
        std::istreambuf_iterator<char> i(inf);
        assert(*i++ == 'a');
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wistringstream inf(L"abc");
        std::istreambuf_iterator<wchar_t> i(inf);
        assert(*i++ == L'a');
    }
#endif

  return;
}
