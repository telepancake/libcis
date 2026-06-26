// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_copy_131cb30c).
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

namespace libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_copy_131cb30c { // libcis
int main(int, char**)
{
    std::ostringstream outf;
    std::ostream_iterator<int> i(outf);
    std::ostream_iterator<int> j = i;
    assert(outf.good());
    ((void)j);

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_copy_131cb30c (libcis)

