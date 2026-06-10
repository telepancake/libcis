// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_encoding).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_encoding; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class codecvt<char32_t, char, mbstate_t>

// int encoding() const throw();

// This test runs in C++20, but we have deprecated codecvt<char(16|32), char, mbstate_t> in C++20.
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_encoding { // libcis: isolate file-scope helpers
typedef std::codecvt<char32_t, char, std::mbstate_t> F;
} using namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_encoding; // libcis


void test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_encoding()
{
    std::locale l = std::locale::classic();
    const F& f = std::use_facet<F>(l);
    assert(f.encoding() == 0);

  return;
}
