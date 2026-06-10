// AST-transferred from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_construct_ptr_size).
// main -> test_re_re_regex_re_regex_construct_ptr_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// basic_regex(const charT* p, size_t len);

#include <regex>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_re_re_regex_re_regex_construct_ptr_size { // libcis: isolate file-scope helpers
template <class CharT>
void
test(const CharT* p, std::size_t len, unsigned mc)
{
    std::basic_regex<CharT> r(p, len);
    assert(r.flags() == std::regex_constants::ECMAScript);
    assert(r.mark_count() == mc);
}
} using namespace libcis_ns_re_re_regex_re_regex_construct_ptr_size; // libcis


void test_re_re_regex_re_regex_construct_ptr_size()
{
    test("\\(a\\)", 5, 0);
    test("\\(a[bc]\\)", 9, 0);
    test("\\(a\\([bc]\\)\\)", 13, 0);
    test("(a([bc]))", 9, 2);

    test("(\0)(b)(c)(d)", 12, 4);
    test("(\0)(b)(c)(d)", 9, 3);
    test("(\0)(b)(c)(d)", 3, 1);
    test("(\0)(b)(c)(d)", 0, 0);

  return;
}
