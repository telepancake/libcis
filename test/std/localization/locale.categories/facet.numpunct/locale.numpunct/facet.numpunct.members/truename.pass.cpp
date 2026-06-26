// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_truename_f9b9dfa4).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class numpunct;

// string_type truename() const;

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_truename_f9b9dfa4 { // libcis
int main(int, char**)
{
    std::locale l = std::locale::classic();
    {
        typedef char C;
        const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
        assert(np.truename() == std::string("true"));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef wchar_t C;
        const std::numpunct<C>& np = std::use_facet<std::numpunct<C> >(l);
        assert(np.truename() == std::wstring(L"true"));
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_facet_numpunct_members_truename_f9b9dfa4 (libcis)

