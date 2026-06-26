// transferred+adapted from libc++ by tools/transfer.py (slug=iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_assign_t_0e4ba3e2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <iterator>

// class ostream_iterator

// ostream_iterator& operator=(const T& value);

#include <iterator>
#include <sstream>
#include <cassert>

#include "test_macros.h"

TEST_CLANG_DIAGNOSTIC_IGNORED("-Wliteral-conversion")
TEST_MSVC_DIAGNOSTIC_IGNORED(4244) // conversion from 'X' to 'Y', possible loss of data

namespace libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_assign_t_0e4ba3e2 { // libcis
int main(int, char**)
{
    {
        std::ostringstream outf;
        std::ostream_iterator<int> i(outf);
        i = 2.4;
        assert(outf.str() == "2");
    }
    {
        std::ostringstream outf;
        std::ostream_iterator<int> i(outf, ", ");
        i = 2.4;
        assert(outf.str() == "2, ");
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wostringstream outf;
        std::ostream_iterator<int, wchar_t> i(outf);
        i = 2.4;
        assert(outf.str() == L"2");
    }
    {
        std::wostringstream outf;
        std::ostream_iterator<int, wchar_t> i(outf, L", ");
        i = 2.4;
        assert(outf.str() == L"2, ");
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_iterators_stream_iterators_ostream_iterator_ostream_iterator_ops_assign_t_0e4ba3e2 (libcis)

