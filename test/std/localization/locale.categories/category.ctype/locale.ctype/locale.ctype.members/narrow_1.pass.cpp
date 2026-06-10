// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_ctype_locale_ctype_members_narrow_1).
// main -> test_localization_locale_categories_category_ctype_locale_ctype_locale_ctype_members_narrow_1; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class ctype;

// char narrow(charT c, char dfault) const;

// XFAIL: no-wide-characters

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_category_ctype_locale_ctype_locale_ctype_members_narrow_1()
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<wchar_t> F;
        const F& f = std::use_facet<F>(l);

        assert(f.narrow(L' ', '*') == ' ');
        assert(f.narrow(L'A', '*') == 'A');
        assert(f.narrow(L'\x07', '*') == '\x07');
        assert(f.narrow(L'.', '*') == '.');
        assert(f.narrow(L'a', '*') == 'a');
        assert(f.narrow(L'1', '*') == '1');
    }

  return;
}
