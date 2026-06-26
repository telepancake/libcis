// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_alg_re_alg_search_no_update_pos_6bceebe4).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator, class Allocator, class charT, class traits>
//     bool
//     regex_search(BidirectionalIterator first, BidirectionalIterator last,
//                  match_results<BidirectionalIterator, Allocator>& m,
//                  const basic_regex<charT, traits>& e,
//                  regex_constants::match_flag_type flags = regex_constants::match_default);

#include <regex>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_re_re_alg_re_alg_search_no_update_pos_6bceebe4 { // libcis
int main(int, char**)
{
    // Iterating over /^a/ should yield one instance at the beginning
    // of the text.

    const char *text = "aaa\naa";
    std::regex re("^a");
    std::cregex_iterator it(text, text+6, re);
    std::cregex_iterator end = std::cregex_iterator();

    assert(it->str() == "a");
    assert(it->position(0) == 0);
    assert(it->length(0) == 1);

    ++it;
    assert(it == end);

  return 0;

    return 0;
}
} // libcis_ns_re_re_alg_re_alg_search_no_update_pos_6bceebe4 (libcis)

