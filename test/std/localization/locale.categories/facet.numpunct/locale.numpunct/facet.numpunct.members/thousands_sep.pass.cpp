// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_thousands_sep).
// main -> test_localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_thousands_sep; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class numpunct;

// char_type thousands_sep() const;

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_thousands_sep()
{
    std::locale l = std::locale::classic();
    {
        typedef char C;
        const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
        assert(np.thousands_sep() == ',');
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef wchar_t C;
        const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
        assert(np.thousands_sep() == L',');
    }
#endif

  return;
}
