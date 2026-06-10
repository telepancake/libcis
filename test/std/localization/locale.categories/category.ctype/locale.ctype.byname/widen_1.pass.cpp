// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_ctype_byname_widen_1).
// main -> test_localization_locale_categories_category_ctype_locale_ctype_byname_widen_1; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8
// XFAIL: no-wide-characters

// <locale>

// template <class charT> class ctype_byname;

// charT widen(char c) const;

// I doubt this test is portable


#include <locale>
#include <cassert>
#include <limits.h>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

void test_localization_locale_categories_category_ctype_locale_ctype_byname_widen_1()
{
    {
        std::locale l;
        {
            typedef std::ctype_byname<wchar_t> F;
            std::locale ll(l, new F(LOCALE_en_US_UTF_8));
            const F& f = std::use_facet<F>(ll);

            assert(f.widen(' ') == L' ');
            assert(f.widen('A') == L'A');
            assert(f.widen('\x07') == L'\x07');
            assert(f.widen('.') == L'.');
            assert(f.widen('a') == L'a');
            assert(f.widen('1') == L'1');
            assert(f.widen(char(-5)) == wchar_t(-1));
        }
    }
    {
        std::locale l;
        {
            typedef std::ctype_byname<wchar_t> F;
            std::locale ll(l, new F("C"));
            const F& f = std::use_facet<F>(ll);

            assert(f.widen(' ') == L' ');
            assert(f.widen('A') == L'A');
            assert(f.widen('\x07') == L'\x07');
            assert(f.widen('.') == L'.');
            assert(f.widen('a') == L'a');
            assert(f.widen('1') == L'1');
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(_WIN32) || defined(_AIX)
            assert(f.widen(char(-5)) == L'\u00fb');
#else
            assert(f.widen(char(-5)) == wchar_t(-1));
#endif
        }
    }

  return;
}
