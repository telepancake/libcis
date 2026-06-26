// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_facet_numpunct_locale_numpunct_byname_decimal_point_c3e45f0f).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8
// REQUIRES: locale.fr_FR.UTF-8

// <locale>

// template <class charT> class numpunct_byname;

// char_type decimal_point() const;

#include <locale>
#include <cassert>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

namespace libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_byname_decimal_point_c3e45f0f { // libcis
int main(int, char**)
{
    {
        std::locale l("C");
        {
            typedef char C;
            const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
            assert(np.decimal_point() == '.');
        }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
        {
            typedef wchar_t C;
            const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
            assert(np.decimal_point() == L'.');
        }
#endif
    }
    {
        std::locale l(LOCALE_en_US_UTF_8);
        {
            typedef char C;
            const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
            assert(np.decimal_point() == '.');
        }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
        {
            typedef wchar_t C;
            const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
            assert(np.decimal_point() == L'.');
        }
#endif
    }
    {
        std::locale l(LOCALE_fr_FR_UTF_8);
        {
            typedef char C;
            const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
            assert(np.decimal_point() == ',');
        }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
        {
            typedef wchar_t C;
            const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
            assert(np.decimal_point() == L',');
        }
#endif
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_byname_decimal_point_c3e45f0f (libcis)

