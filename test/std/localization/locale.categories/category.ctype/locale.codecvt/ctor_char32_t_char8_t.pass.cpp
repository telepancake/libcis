// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_ctor_char32_t_char8_t).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_ctor_char32_t_char8_t; file-scope helpers isolated in anon namespace.
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

// explicit codecvt(size_t refs = 0);

#include <cassert>
#include <locale>

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_ctor_char32_t_char8_t { // libcis: isolate file-scope helpers
using F = std::codecvt<char32_t, char8_t, std::mbstate_t>;

struct my_facet : F {
  static int count;

  explicit my_facet(std::size_t refs = 0) : F(refs) { ++count; }

  ~my_facet() { --count; }
};
} using namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_ctor_char32_t_char8_t; // libcis


int my_facet::count = 0;

void test_localization_locale_categories_category_ctype_locale_codecvt_ctor_char32_t_char8_t() {
  {
    std::locale l(std::locale::classic(), new my_facet);
    assert(my_facet::count == 1);
  }
  assert(my_facet::count == 0);
  {
    my_facet f(1);
    assert(my_facet::count == 1);
    {
      std::locale l(std::locale::classic(), &f);
      assert(my_facet::count == 1);
    }
    assert(my_facet::count == 1);
  }
  assert(my_facet::count == 0);

  return;
}
