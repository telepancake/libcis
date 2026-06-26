// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_nonmemb_re_regex_nmswap_swap_d589a566).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// template <class charT, class traits>
//   void swap(basic_regex<charT, traits>& lhs, basic_regex<charT, traits>& rhs);

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_regex_re_regex_nonmemb_re_regex_nmswap_swap_d589a566 { // libcis
int main(int, char**)
{
    std::regex r1("(a([bc]))");
    std::regex r2;
    swap(r2, r1);
    assert(r1.flags() == std::regex::ECMAScript);
    assert(r1.mark_count() == 0);
    assert(r2.flags() == std::regex::ECMAScript);
    assert(r2.mark_count() == 2);

  return 0;

    return 0;
}
} // libcis_ns_re_re_regex_re_regex_nonmemb_re_regex_nmswap_swap_d589a566 (libcis)

