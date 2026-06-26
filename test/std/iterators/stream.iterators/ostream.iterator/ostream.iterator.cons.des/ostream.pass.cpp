// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_11fd7892).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostream_iterator

// ostream_iterator(ostream_type& s);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_11fd7892 { // libcis
struct MyTraits : std::char_traits<char> {};

typedef std::basic_ostringstream<char, MyTraits> StringStream;
typedef std::basic_ostream<char, MyTraits> BasicStream;

void operator&(BasicStream const&) {}

int main(int, char**)
{
    {
        std::ostringstream outf;
        std::ostream_iterator<int> i(outf);
        assert(outf.good());
    }
    {
        StringStream outf;
        std::ostream_iterator<int, char, MyTraits> i(outf);
        assert(outf.good());
    }

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_cons_des_ostream_11fd7892 (libcis)

