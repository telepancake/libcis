// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istream_iterator_istream_iterator_ops_post_increment).
// main -> test_iterators_stream_iterators_istream_iterator_istream_iterator_ops_post_increment; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class istream_iterator

// istream_iterator operator++(int);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_istream_iterator_istream_iterator_ops_post_increment()
{
    std::istringstream inf(" 1 23");
    std::istream_iterator<int> i(inf);
    std::istream_iterator<int> icopy = i++;
    assert(icopy == i);
    int j = 0;
    j = *i;
    assert(j == 23);
    j = 0;
    j = *icopy;
    assert(j == 1);

  return;
}
