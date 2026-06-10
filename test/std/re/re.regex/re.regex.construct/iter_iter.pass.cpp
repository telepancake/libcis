// AST-transferred from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_construct_iter_iter).
// main -> test_re_re_regex_re_regex_construct_iter_iter; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// template <class ForwardIterator>
//    basic_regex(ForwardIterator first, ForwardIterator last);

#include <regex>
#include <cassert>

#include "test_iterators.h"
#include "test_macros.h"

namespace libcis_ns_re_re_regex_re_regex_construct_iter_iter { // libcis: isolate file-scope helpers
template <class Iter>
void
test(Iter first, Iter last, unsigned mc)
{
    std::basic_regex<typename std::iterator_traits<Iter>::value_type> r(first, last);
    assert(r.flags() == std::regex_constants::ECMAScript);
    assert(r.mark_count() == mc);
}
} using namespace libcis_ns_re_re_regex_re_regex_construct_iter_iter; // libcis


void test_re_re_regex_re_regex_construct_iter_iter()
{
    typedef forward_iterator<std::string::const_iterator> F;
    std::string s1("\\(a\\)");
    std::string s2("\\(a[bc]\\)");
    std::string s3("\\(a\\([bc]\\)\\)");
    std::string s4("(a([bc]))");

    test(F(s1.begin()), F(s1.end()), 0);
    test(F(s2.begin()), F(s2.end()), 0);
    test(F(s3.begin()), F(s3.end()), 0);
    test(F(s4.begin()), F(s4.end()), 2);

  return;
}
