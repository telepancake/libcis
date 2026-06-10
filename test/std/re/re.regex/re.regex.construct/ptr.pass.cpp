// AST-transferred from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_construct_ptr).
// main -> test_re_re_regex_re_regex_construct_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// basic_regex(const charT* p);

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_regex_re_regex_construct_ptr { // libcis: isolate file-scope helpers
template <class CharT>
void
test(const CharT* p, unsigned mc)
{
    std::basic_regex<CharT> r(p);
    assert(r.flags() == std::regex_constants::ECMAScript);
    assert(r.mark_count() == mc);
}
} using namespace libcis_ns_re_re_regex_re_regex_construct_ptr; // libcis


void test_re_re_regex_re_regex_construct_ptr()
{
    test("\\(a\\)", 0);
    test("\\(a[bc]\\)", 0);
    test("\\(a\\([bc]\\)\\)", 0);
    test("(a([bc]))", 2);

  return;
}
