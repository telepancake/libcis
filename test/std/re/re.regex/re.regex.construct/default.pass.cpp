// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_construct_default_c93940d4).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// basic_regex();

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_regex_re_regex_construct_default_c93940d4 { // libcis
template <class CharT>
void
test()
{
    std::basic_regex<CharT> r;
    assert(r.flags() == std::regex_constants::ECMAScript);
    assert(r.mark_count() == 0);
}

int main(int, char**)
{
    test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    test<wchar_t>();
#endif

  return 0;

    return 0;
}
} // libcis_ns_re_re_regex_re_regex_construct_default_c93940d4 (libcis)

