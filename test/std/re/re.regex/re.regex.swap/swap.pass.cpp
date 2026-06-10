// AST-transferred from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_swap_swap).
// main -> test_re_re_regex_re_regex_swap_swap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// void swap(basic_regex& e);

#include <regex>
#include <cassert>
#include "test_macros.h"

void test_re_re_regex_re_regex_swap_swap()
{
    std::regex r1("(a([bc]))");
    std::regex r2;
    r2.swap(r1);
    assert(r1.flags() == std::regex::ECMAScript);
    assert(r1.mark_count() == 0);
    assert(r2.flags() == std::regex::ECMAScript);
    assert(r2.mark_count() == 2);

  return;
}
