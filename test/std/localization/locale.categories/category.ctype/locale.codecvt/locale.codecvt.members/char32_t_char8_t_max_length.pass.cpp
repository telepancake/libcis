// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_char8_t_max_length).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_char8_t_max_length; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// XFAIL: availability-char8_t_support-missing

// <locale>

// template <> class codecvt<char32_t, char8_t, mbstate_t>

// int max_length() const noexcept;

#include <cassert>
#include <locale>

void test_localization_locale_categories_category_ctype_locale_codecvt_locale_codecvt_members_char32_t_char8_t_max_length() {
  using F = std::codecvt<char32_t, char8_t, std::mbstate_t>;
  const F& f = std::use_facet<F>(std::locale::classic());
  assert(f.max_length() == 4);
  static_assert(noexcept(f.max_length()));
  return;
}
