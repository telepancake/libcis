// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_length).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_length; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class codecvt<char, char, mbstate_t>

// int length(stateT& state, const externT* from, const externT* from_end, size_t max) const;

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_length { // libcis: isolate file-scope helpers
typedef std::codecvt<char, char, std::mbstate_t> F;
} using namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_length; // libcis


void test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_length()
{
    std::locale l = std::locale::classic();
    const F& f = std::use_facet<F>(l);
    std::mbstate_t mbs = {};
    const char from[10]= {0};
    assert(f.length(mbs, from, from+10, 0) == 0);
    assert(f.length(mbs, from, from+10, 9) == 9);
    assert(f.length(mbs, from, from+10, 10) == 10);
    assert(f.length(mbs, from, from+10, 11) == 10);
    assert(f.length(mbs, from, from+10, 100) == 10);

  return;
}
