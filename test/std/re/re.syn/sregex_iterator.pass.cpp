// AST-transferred from libc++ by tools/transfer.py (slug=re_re_syn_sregex_iterator).
// main -> test_re_re_syn_sregex_iterator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef regex_iterator<string::const_iterator>   sregex_iterator;

#include <regex>
#include <type_traits>
#include "test_macros.h"

void test_re_re_syn_sregex_iterator()
{
    static_assert((std::is_same<std::regex_iterator<std::string::const_iterator>, std::sregex_iterator>::value), "");

  return;
}
