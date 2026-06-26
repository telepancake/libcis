// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_construct_string_1fa5cb71).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// template <class ST, class SA>
//    basic_regex(const basic_string<charT, ST, SA>& s);

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_regex_re_regex_construct_string_1fa5cb71 { // libcis
template <class String>
void
test(const String& p, unsigned mc)
{
    std::basic_regex<typename String::value_type> r(p);
    assert(r.flags() == std::regex_constants::ECMAScript);
    assert(r.mark_count() == mc);
}

int main(int, char**)
{
    test(std::string("\\(a\\)"), 0);
    test(std::string("\\(a[bc]\\)"), 0);
    test(std::string("\\(a\\([bc]\\)\\)"), 0);
    test(std::string("(a([bc]))"), 2);

  return 0;

    return 0;
}
} // libcis_ns_re_re_regex_re_regex_construct_string_1fa5cb71 (libcis)

