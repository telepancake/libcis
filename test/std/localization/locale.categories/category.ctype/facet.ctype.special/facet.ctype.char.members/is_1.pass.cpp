// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_is_1).
// main -> test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_is_1; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class ctype<char>;

// bool is(mask m, char c) const;

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_is_1()
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<char> F;
        const F& f = std::use_facet<F>(l);

        assert(f.is(F::space, ' '));
        assert(!f.is(F::space, 'A'));

        assert(f.is(F::print, ' '));
        assert(!f.is(F::print, '\x07'));

        assert(f.is(F::cntrl, '\x07'));
        assert(!f.is(F::cntrl, ' '));

        assert(f.is(F::upper, 'A'));
        assert(!f.is(F::upper, 'a'));

        assert(f.is(F::lower, 'a'));
        assert(!f.is(F::lower, 'A'));

        assert(f.is(F::alpha, 'a'));
        assert(!f.is(F::alpha, '1'));

        assert(f.is(F::digit, '1'));
        assert(!f.is(F::digit, 'a'));

        assert(f.is(F::punct, '.'));
        assert(!f.is(F::punct, 'a'));

        assert(f.is(F::xdigit, 'a'));
        assert(!f.is(F::xdigit, 'g'));

        assert(f.is(F::alnum, 'a'));
        assert(!f.is(F::alnum, '.'));

        assert(f.is(F::graph, '.'));
        assert(!f.is(F::graph,  '\x07'));
    }

  return;
}
