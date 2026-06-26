// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_istream_iterator_istream_iterator_ops_pre_increment_1ebdb9cf).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class istream_iterator

// istream_iterator& operator++();

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_iterators_stream_iterators_istream_iterator_istream_iterator_ops_pre_increment_1ebdb9cf { // libcis
int main(int, char**)
{
    std::istringstream inf(" 1 23");
    std::istream_iterator<int> i(inf);
    std::istream_iterator<int>& iref = ++i;
    assert(&iref == &i);
    int j = 0;
    j = *i;
    assert(j == 23);

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_istream_iterator_istream_iterator_ops_pre_increment_1ebdb9cf (libcis)

