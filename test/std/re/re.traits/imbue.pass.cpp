// AST-transferred from libc++ by tools/transfer.py (slug=re_re_traits_imbue).
// main -> test_re_re_traits_imbue; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8

// <regex>

// template <class charT> struct regex_traits;

// locale_type imbue(locale_type l);

#include <regex>
#include <locale>
#include <cassert>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

void test_re_re_traits_imbue()
{
    {
        std::regex_traits<char> t;
        std::locale loc = t.imbue(std::locale(LOCALE_en_US_UTF_8));
        assert(loc.name() == "C");
        assert(t.getloc().name() == LOCALE_en_US_UTF_8);
    }

  return;
}
