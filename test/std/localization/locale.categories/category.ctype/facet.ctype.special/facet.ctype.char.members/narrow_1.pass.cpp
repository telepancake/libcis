// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_narrow_1).
// main -> test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_narrow_1; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class ctype<char>;

// char narrow(char c, char dfault) const;

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_narrow_1()
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<char> F;
        const F& f = std::use_facet<F>(l);

        assert(f.narrow(' ', '*') == ' ');
        assert(f.narrow('A', '*') == 'A');
        assert(f.narrow('\x07', '*') == '\x07');
        assert(f.narrow('.', '*') == '.');
        assert(f.narrow('a', '*') == 'a');
        assert(f.narrow('1', '*') == '1');
    }

  return;
}
