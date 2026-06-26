// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_widen_1_28c77609).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class ctype<char>;

// char widen(char c) const;

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_widen_1_28c77609 { // libcis
int main(int, char**)
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<char> F;
        const F& f = std::use_facet<F>(l);

        assert(f.widen(' ') == ' ');
        assert(f.widen('A') == 'A');
        assert(f.widen('\x07') == '\x07');
        assert(f.widen('.') == '.');
        assert(f.widen('a') == 'a');
        assert(f.widen('1') == '1');
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_widen_1_28c77609 (libcis)

