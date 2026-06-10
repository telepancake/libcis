// AST-transferred from libc++ by tools/transfer.py (slug=re_re_syn_wregex).
// main -> test_re_re_syn_wregex; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef basic_regex<wchar_t> wregex;

// XFAIL: no-wide-characters

#include <regex>
#include <type_traits>
#include "test_macros.h"

void test_re_re_syn_wregex()
{
    static_assert((std::is_same<std::basic_regex<wchar_t>, std::wregex>::value), "");

  return;
}
