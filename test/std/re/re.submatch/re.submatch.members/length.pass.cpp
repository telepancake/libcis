// AST-transferred from libc++ by tools/transfer.py (slug=re_re_submatch_re_submatch_members_length).
// main -> test_re_re_submatch_re_submatch_members_length; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator> class sub_match;

// difference_type length() const;

#include <regex>
#include <cassert>
#include "test_macros.h"

void test_re_re_submatch_re_submatch_members_length()
{
    {
        typedef char CharT;
        typedef std::sub_match<const CharT*> SM;
        SM sm = SM();
        assert(sm.length() == 0);
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        assert(sm.length() == 3);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef wchar_t CharT;
        typedef std::sub_match<const CharT*> SM;
        SM sm = SM();
        assert(sm.length() == 0);
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        assert(sm.length() == 3);
    }
#endif

  return;
}
