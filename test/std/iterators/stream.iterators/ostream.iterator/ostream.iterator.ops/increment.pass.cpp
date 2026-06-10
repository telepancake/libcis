// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_increment).
// main -> test_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_increment; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostream_iterator

// ostream_iterator& operator++();
// ostream_iterator& operator++(int);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_increment()
{
    std::ostringstream os;
    std::ostream_iterator<int> i(os);
    std::ostream_iterator<int>& iref1 = ++i;
    assert(&iref1 == &i);
    std::ostream_iterator<int>& iref2 = i++;
    assert(&iref2 == &i);

  return;
}
