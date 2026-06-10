// AST-transferred from libc++ by tools/transfer.py (slug=re_re_submatch_re_submatch_members_compare_value_type_ptr).
// main -> test_re_re_submatch_re_submatch_members_compare_value_type_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator> class sub_match;

// int compare(const value_type* s) const;

#include <regex>
#include <cassert>
#include "test_macros.h"

void test_re_re_submatch_re_submatch_members_compare_value_type_ptr()
{
    {
        typedef char CharT;
        typedef std::sub_match<const CharT*> SM;
        SM sm = SM();
        assert(sm.compare("") == 0);
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        assert(sm.compare("") > 0);
        assert(sm.compare("123") == 0);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef wchar_t CharT;
        typedef std::sub_match<const CharT*> SM;
        SM sm = SM();
        assert(sm.compare(L"") == 0);
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        assert(sm.compare(L"") > 0);
        assert(sm.compare(L"123") == 0);
    }
#endif

  return;
}
