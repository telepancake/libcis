// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_narrow_many_407287e8).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class ctype<char>;

// const char* narrow(const char* low, const char*, char dfault, char* to) const;

#include <locale>
#include <string>
#include <vector>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_narrow_many_407287e8 { // libcis
int main(int, char**)
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<char> F;
        const F& f = std::use_facet<F>(l);
        std::string in(" A\x07.a1");
        std::vector<char> v(in.size());

        assert(f.narrow(&in[0], in.data() + in.size(), '*', v.data()) == in.data() + in.size());
        assert(v[0] == ' ');
        assert(v[1] == 'A');
        assert(v[2] == '\x07');
        assert(v[3] == '.');
        assert(v[4] == 'a');
        assert(v[5] == '1');
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_narrow_many_407287e8 (libcis)

