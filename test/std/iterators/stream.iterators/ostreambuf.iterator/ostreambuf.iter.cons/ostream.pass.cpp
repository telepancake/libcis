// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostreambuf_iterator_ostreambuf_iter_cons_ostream_10d503e3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostreambuf_iterator

// ostreambuf_iterator(ostream_type& s) throw();

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_iterators_stream_iterators_ostreambuf_iterator_ostreambuf_iter_cons_ostream_10d503e3 { // libcis
int main(int, char**)
{
    {
        std::ostringstream outf;
        std::ostreambuf_iterator<char> i(outf);
        assert(!i.failed());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wostringstream outf;
        std::ostreambuf_iterator<wchar_t> i(outf);
        assert(!i.failed());
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_ostreambuf_iterator_ostreambuf_iter_cons_ostream_10d503e3 (libcis)

