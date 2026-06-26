// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_increment_aa6df218).
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

namespace libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_increment_aa6df218 { // libcis
int main(int, char**)
{
    std::ostringstream os;
    std::ostream_iterator<int> i(os);
    std::ostream_iterator<int>& iref1 = ++i;
    assert(&iref1 == &i);
    std::ostream_iterator<int>& iref2 = i++;
    assert(&iref2 == &i);

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_increment_aa6df218 (libcis)

