// AST-transferred from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_assign_ptr).
// main -> test_re_re_regex_re_regex_assign_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// basic_regex& operator=(const charT* ptr);

#include <regex>
#include <cassert>
#include "test_macros.h"

void test_re_re_regex_re_regex_assign_ptr()
{
    std::regex r2;
    r2 = "(a([bc]))";
    assert(r2.flags() == std::regex::ECMAScript);
    assert(r2.mark_count() == 2);

  return;
}
