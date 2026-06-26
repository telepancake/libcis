// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_submatch_re_submatch_op_stream_bee99575).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator> class sub_match;

// template <class charT, class ST, class BiIter>
//     basic_ostream<charT, ST>&
//     operator<<(basic_ostream<charT, ST>& os, const sub_match<BiIter>& m);

#include <regex>
#include <sstream>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_submatch_re_submatch_op_stream_bee99575 { // libcis
template <class CharT>
void
test(const std::basic_string<CharT>& s)
{
    typedef std::basic_string<CharT> string;
    typedef std::sub_match<typename string::const_iterator> SM;
    typedef std::basic_ostringstream<CharT> ostringstream;
    SM sm;
    sm.first = s.begin();
    sm.second = s.end();
    sm.matched = true;
    ostringstream os;
    os << sm;
    assert(os.str() == s);
}

int main(int, char**)
{
    test(std::string("123"));
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test(std::wstring(L"123"));
#endif

  return 0;

    return 0;
}
} // libcis_ns_re_re_submatch_re_submatch_op_stream_bee99575 (libcis)

