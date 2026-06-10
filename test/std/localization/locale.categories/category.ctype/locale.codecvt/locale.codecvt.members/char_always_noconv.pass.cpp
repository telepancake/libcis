// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_always_noconv).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_always_noconv; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class codecvt<char, char, mbstate_t>

// bool always_noconv() const throw();

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_always_noconv { // libcis: isolate file-scope helpers
typedef std::codecvt<char, char, std::mbstate_t> F;
} using namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_always_noconv; // libcis


void test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_always_noconv()
{
    std::locale l = std::locale::classic();
    const F& f = std::use_facet<F>(l);
    assert(f.always_noconv());

  return;
}
