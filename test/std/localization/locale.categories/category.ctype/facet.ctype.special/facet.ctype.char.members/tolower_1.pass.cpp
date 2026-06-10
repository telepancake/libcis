// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_tolower_1).
// main -> test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_tolower_1; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class ctype<char>;

// char tolower(char) const;

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_tolower_1()
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<char> F;
        const F& f = std::use_facet<F>(l);

        assert(f.tolower(' ') == ' ');
        assert(f.tolower('A') == 'a');
        assert(f.tolower('\x07') == '\x07');
        assert(f.tolower('.') == '.');
        assert(f.tolower('a') == 'a');
        assert(f.tolower('1') == '1');
    }

  return;
}
