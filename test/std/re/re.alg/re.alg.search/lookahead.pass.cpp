// AST-transferred from libc++ by tools/transfer.py (slug=re_re_alg_re_alg_search_lookahead).
// main -> test_re_re_alg_re_alg_search_lookahead; file-scope helpers isolated in anon namespace.
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

// https://llvm.org/PR11118

#include <regex>
#include <cassert>
#include "test_macros.h"

void test_re_re_alg_re_alg_search_lookahead()
{
    assert(!std::regex_search("ab", std::regex("(?=^)b")));
    assert(!std::regex_search("ab", std::regex("a(?=^)b")));

  return;
}
