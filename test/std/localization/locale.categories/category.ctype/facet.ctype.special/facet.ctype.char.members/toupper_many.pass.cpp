// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_toupper_many_84d7bc0b).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class ctype<char>;

// const char* toupper(char* low, const char* high) const;

#include <locale>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_toupper_many_84d7bc0b { // libcis
int main(int, char**)
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<char> F;
        const F& f = std::use_facet<F>(l);
        std::string in(" A\x07.a1");

        assert(f.toupper(&in[0], in.data() + in.size()) == in.data() + in.size());
        assert(in[0] == ' ');
        assert(in[1] == 'A');
        assert(in[2] == '\x07');
        assert(in[3] == '.');
        assert(in[4] == 'A');
        assert(in[5] == '1');
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_toupper_many_84d7bc0b (libcis)

