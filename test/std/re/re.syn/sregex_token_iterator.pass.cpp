// AST-transferred from libc++ by tools/transfer.py (slug=re_re_syn_sregex_token_iterator).
// main -> test_re_re_syn_sregex_token_iterator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef regex_token_iterator<string::const_iterator>   sregex_token_iterator;

#include <regex>
#include <type_traits>
#include "test_macros.h"

void test_re_re_syn_sregex_token_iterator()
{
    static_assert((std::is_same<std::regex_token_iterator<std::string::const_iterator>, std::sregex_token_iterator>::value), "");

  return;
}
