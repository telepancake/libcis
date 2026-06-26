// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_assign_assign_string_flag_7a6f0a37).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// template <class string_traits, class A>
//   basic_regex& assign(const basic_string<charT, string_traits, A>& s,
//                       flag_type f = regex_constants::ECMAScript);

#include <cassert>
#include <regex>
#include <string>

#include "test_macros.h"

namespace libcis_ns_re_re_regex_re_regex_assign_assign_string_flag_7a6f0a37 { // libcis
int main(int, char**)
{
    std::regex r2;
    r2.assign(std::string("(a([bc]))"));
    assert(r2.flags() == std::regex::ECMAScript);
    assert(r2.mark_count() == 2);

    r2.assign(std::string("(a([bc]))"), std::regex::extended);
    assert(r2.flags() == std::regex::extended);
    assert(r2.mark_count() == 2);

  return 0;

    return 0;
}
} // libcis_ns_re_re_regex_re_regex_assign_assign_string_flag_7a6f0a37 (libcis)

