// AST-transferred from libc++ by tools/transfer.py (slug=re_re_alg_re_alg_search_invert_neg_word_search).
// main -> test_re_re_alg_re_alg_search_invert_neg_word_search; file-scope helpers isolated in anon namespace.
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
void test_re_re_alg_re_alg_search_invert_neg_word_search()
{
  assert(std::regex_search("HelloWorld", std::regex("[^\\W]")));
  assert(std::regex_search("_", std::regex("[^\\W]")));
  return;
}
