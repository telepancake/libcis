// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_locale_members_name).
// main -> test_localization_locales_locale_locale_members_name; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8

// <locale>

// basic_string<char> name() const;

#include <locale>
#include <cassert>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

void test_localization_locales_locale_locale_members_name()
{
    {
        std::locale loc;
        assert(loc.name() == "C");
    }
    {
        std::locale loc(LOCALE_en_US_UTF_8);
        assert(loc.name() == LOCALE_en_US_UTF_8);
    }

  return;
}
