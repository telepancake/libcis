// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_falsename_a98a4195).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class numpunct;

// string_type falsename() const;

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_falsename_a98a4195 { // libcis
int main(int, char**)
{
    std::locale l = std::locale::classic();
    {
        typedef char C;
        const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
        assert(np.falsename() == std::string("false"));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef wchar_t C;
        const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
        assert(np.falsename() == std::wstring(L"false"));
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_falsename_a98a4195 (libcis)

