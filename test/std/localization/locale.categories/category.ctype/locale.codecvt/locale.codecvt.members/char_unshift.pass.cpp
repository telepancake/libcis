// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_unshift).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_unshift; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class codecvt<char, char, mbstate_t>

// result unshift(stateT& state,
//                externT* to, externT* to_end, externT*& to_next) const;

#include <locale>
#include <string>
#include <vector>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_unshift { // libcis: isolate file-scope helpers
typedef std::codecvt<char, char, std::mbstate_t> F;
} using namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_unshift; // libcis


void test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char_unshift()
{
    std::locale l = std::locale::classic();
    std::vector<char> to(3);
    const F& f = std::use_facet<F>(l);
    std::mbstate_t mbs = {};
    char* to_next = 0;
    assert(f.unshift(mbs, to.data(), to.data() + to.size(), to_next) == F::noconv);
    assert(to_next == to.data());

  return;
}
