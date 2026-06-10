// AST-transferred from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_construct_copy).
// main -> test_re_re_regex_re_regex_construct_copy; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// basic_regex(const basic_regex& e);

#include <regex>
#include <cassert>
#include "test_macros.h"

void test_re_re_regex_re_regex_construct_copy()
{
    std::regex r1("(a([bc]))");
    std::regex r2 = r1;
    assert(r2.flags() == std::regex::ECMAScript);
    assert(r2.mark_count() == 2);

  return;
}
