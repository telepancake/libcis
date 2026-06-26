// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_iter_re_regiter_re_regiter_cnstr_default_70a4d5b0).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// class regex_iterator<BidirectionalIterator, charT, traits>

// regex_iterator();

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_iter_re_regiter_re_regiter_cnstr_default_70a4d5b0 { // libcis
template <class CharT>
void
test()
{
    typedef std::regex_iterator<const CharT*> I;
    I i1;
    assert(i1 == I());
}

int main(int, char**)
{
    test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>();
#endif

  return 0;

    return 0;
}
} // libcis_ns_re_re_iter_re_regiter_re_regiter_cnstr_default_70a4d5b0 (libcis)

