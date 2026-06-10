// AST-transferred from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_copy).
// main -> test_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostream_iterator

// ostream_iterator(const ostream_iterator& x);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

void test_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_copy()
{
    std::ostringstream outf;
    std::ostream_iterator<int> i(outf);
    std::ostream_iterator<int> j = i;
    assert(outf.good());
    ((void)j);

  return;
}
