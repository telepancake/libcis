// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_dereference_6ed5853e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostream_iterator

// ostream_iterator& operator*() const;

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_dereference_6ed5853e { // libcis
int main(int, char**)
{
    std::ostringstream os;
    std::ostream_iterator<int> i(os);
    std::ostream_iterator<int>& iref = *i;
    assert(&iref == &i);

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_dereference_6ed5853e (libcis)

