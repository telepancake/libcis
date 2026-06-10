// AST-transferred from libc++ by tools/transfer.py (slug=re_re_syn_csub_match).
// main -> test_re_re_syn_csub_match; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef sub_match<const char*>   csub_match;

#include <regex>
#include <type_traits>
#include "test_macros.h"

void test_re_re_syn_csub_match()
{
    static_assert((std::is_same<std::sub_match<const char*>, std::csub_match>::value), "");

  return;
}
