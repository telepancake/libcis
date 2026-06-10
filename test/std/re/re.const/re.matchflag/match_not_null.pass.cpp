// AST-transferred from libc++ by tools/transfer.py (slug=re_re_const_re_matchflag_match_not_null).
// main -> test_re_re_const_re_matchflag_match_not_null; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// match_not_null:
//     The regular expression shall not match an empty sequence.

#include "test_macros.h"
#include <cassert>
#include <regex>

void test_re_re_const_re_matchflag_match_not_null()
{
  // When match_not_null is on, the regex engine should reject empty matches and
  // move on to try other solutions.
  std::cmatch m;
  assert(!std::regex_search("a", m, std::regex("b*"),
                            std::regex_constants::match_not_null));
  assert(std::regex_search("aa", m, std::regex("a*?"),
                           std::regex_constants::match_not_null));
  assert(m[0].length() == 1);
  assert(!std::regex_search("a", m, std::regex("b*", std::regex::extended),
                            std::regex_constants::match_not_null));
  assert(!std::regex_search(
      "a", m,
      std::regex("b*", std::regex::extended | std::regex_constants::nosubs),
      std::regex_constants::match_not_null));

  assert(!std::regex_match("", m, std::regex("a*"),
                           std::regex_constants::match_not_null));
  assert(!std::regex_match("", m, std::regex("a*", std::regex::extended),
                           std::regex_constants::match_not_null));
  assert(!std::regex_match(
      "", m,
      std::regex("a*", std::regex::extended | std::regex_constants::nosubs),
      std::regex_constants::match_not_null));

  return;
}
