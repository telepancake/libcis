// AST-transferred from libc++ by tools/transfer.py (slug=re_re_iter_re_regiter_re_regiter_cnstr_default).
// main -> test_re_re_iter_re_regiter_re_regiter_cnstr_default; file-scope helpers isolated in anon namespace.
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

namespace libcis_ns_re_re_iter_re_regiter_re_regiter_cnstr_default { // libcis: isolate file-scope helpers
template <class CharT>
void
test()
{
    typedef std::regex_iterator<const CharT*> I;
    I i1;
    assert(i1 == I());
}
} using namespace libcis_ns_re_re_iter_re_regiter_re_regiter_cnstr_default; // libcis


void test_re_re_iter_re_regiter_re_regiter_cnstr_default()
{
    test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>();
#endif

  return;
}
