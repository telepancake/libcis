// AST-transferred from libc++ by tools/transfer.py (slug=re_re_regex_re_regex_locale_imbue).
// main -> test_re_re_regex_re_regex_locale_imbue; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// locale_type imbue(locale_type loc);

#include <regex>
#include <locale>
#include <cassert>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

void test_re_re_regex_re_regex_locale_imbue()
{
    std::regex r;
    std::locale loc = r.imbue(std::locale(LOCALE_en_US_UTF_8));
    assert(loc.name() == "C");
    assert(r.getloc().name() == LOCALE_en_US_UTF_8);
    loc = r.imbue(std::locale("C"));
    assert(loc.name() == LOCALE_en_US_UTF_8);
    assert(r.getloc().name() == "C");

  return;
}
