// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_alg_re_alg_search_invert_neg_word_search_e0968ce4).
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


// PR34310
namespace libcis_ns_re_re_alg_re_alg_search_invert_neg_word_search_e0968ce4 { // libcis
int main(int, char**)
{
  assert(std::regex_search("HelloWorld", std::regex("[^\\W]")));
  assert(std::regex_search("_", std::regex("[^\\W]")));
  return 0;

    return 0;
}
} // libcis_ns_re_re_alg_re_alg_search_invert_neg_word_search_e0968ce4 (libcis)

