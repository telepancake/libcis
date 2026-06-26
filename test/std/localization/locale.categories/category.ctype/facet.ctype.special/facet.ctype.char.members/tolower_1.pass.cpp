// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_tolower_1_80a0b490).
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

namespace libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_tolower_1_80a0b490 { // libcis
int main(int, char**)
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

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_tolower_1_80a0b490 (libcis)

