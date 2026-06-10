// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_numeric_locale_nm_put_ctor).
// main -> test_localization_locale_categories_category_numeric_locale_nm_put_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class num_put<charT, OutputIterator>

// explicit num_put(size_t refs = 0);

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_numeric_locale_nm_put_ctor { // libcis: isolate file-scope helpers
typedef std::num_put<char, char*> F;

class my_facet : public F {
public:
  static int count;

  explicit my_facet(std::size_t refs = 0) : F(refs) { ++count; }

  ~my_facet() { --count; }
};
} using namespace libcis_ns_localization_locale_categories_category_numeric_locale_nm_put_ctor; // libcis


int my_facet::count = 0;

void test_localization_locale_categories_category_numeric_locale_nm_put_ctor() {
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
