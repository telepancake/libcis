// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_ctype_locale_ctype_members_toupper_many_8ea5ae7a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class ctype;

// const charT* toupper(charT* low, const charT* high) const;

// XFAIL: no-wide-characters

#include <locale>
#include <string>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_locale_ctype_locale_ctype_members_toupper_many_8ea5ae7a { // libcis
int main(int, char**)
{
    std::locale l = std::locale::classic();
    {
        typedef std::ctype<wchar_t> F;
        const F& f = std::use_facet<F>(l);
        std::wstring in(L" A\x07.a1");

        assert(f.toupper(&in[0], in.data() + in.size()) == in.data() + in.size());
        assert(in[0] == L' ');
        assert(in[1] == L'A');
        assert(in[2] == L'\x07');
        assert(in[3] == L'.');
        assert(in[4] == L'A');
        assert(in[5] == L'1');
    }

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_ctype_locale_ctype_locale_ctype_members_toupper_many_8ea5ae7a (libcis)

