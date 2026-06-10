// AST-transferred from libc++ by tools/transfer.py (slug=re_re_submatch_re_submatch_members_operator_string).
// main -> test_re_re_submatch_re_submatch_members_operator_string; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator> class sub_match;

// operator string_type() const;

#include <regex>
#include <cassert>
#include "test_macros.h"

void test_re_re_submatch_re_submatch_members_operator_string()
{
    {
        typedef char CharT;
        typedef std::sub_match<const CharT*> SM;
        SM sm = SM();
        SM::string_type str = sm;
        assert(str.empty());
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        str = sm;
        assert(str == std::string("123"));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef wchar_t CharT;
        typedef std::sub_match<const CharT*> SM;
        SM sm = SM();
        SM::string_type str = sm;
        assert(str.empty());
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        str = sm;
        assert(str == std::wstring(L"123"));
    }
#endif

  return;
}
