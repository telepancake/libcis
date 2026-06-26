// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_collate_locale_collate_byname_types_02974edb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// REQUIRES: locale.en_US.UTF-8

// <locale>

// template <class charT>
// class collate_byname
//     : public collate<charT>
// {
// public:
//     typedef basic_string<charT> string_type;
//     explicit collate_byname(const char*, size_t refs = 0);
//     explicit collate_byname(const string&, size_t refs = 0);
// protected:
//     ~collate_byname();
// };

#include <locale>
#include <string>
#include <cassert>

#include <stdio.h>

#include "test_macros.h"
#include "platform_support.h" // locale name macros

namespace libcis_ns_localization_locale_categories_category_collate_locale_collate_byname_types_02974edb { // libcis
int main(int, char**)
{
    std::locale l(LOCALE_en_US_UTF_8);
    {
        assert(std::has_facet<std::collate_byname<char> >(l));
        assert(&std::use_facet<std::collate<char> >(l)
            == &std::use_facet<std::collate_byname<char> >(l));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        assert(std::has_facet<std::collate_byname<wchar_t> >(l));
        assert(&std::use_facet<std::collate<wchar_t> >(l)
            == &std::use_facet<std::collate_byname<wchar_t> >(l));
    }
#endif

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_collate_locale_collate_byname_types_02974edb (libcis)

