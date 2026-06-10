// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_locale_types_locale_category_category).
// main -> test_localization_locales_locale_locale_types_locale_category_category; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// typedef int category;

#include <locale>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locales_locale_locale_types_locale_category_category { // libcis: isolate file-scope helpers
template <class T>
void test(const T &) {}
} using namespace libcis_ns_localization_locales_locale_locale_types_locale_category_category; // libcis



void test_localization_locales_locale_locale_types_locale_category_category()
{
    static_assert((std::is_same<std::locale::category, int>::value), "");
    assert(std::locale::none == 0);
    assert(std::locale::collate);
    assert(std::locale::ctype);
    assert(std::locale::monetary);
    assert(std::locale::numeric);
    assert(std::locale::time);
    assert(std::locale::messages);
    assert((std::locale::collate
          & std::locale::ctype
          & std::locale::monetary
          & std::locale::numeric
          & std::locale::time
          & std::locale::messages) == 0);
    assert((std::locale::collate
          | std::locale::ctype
          | std::locale::monetary
          | std::locale::numeric
          | std::locale::time
          | std::locale::messages)
         == std::locale::all);

    test(std::locale::none);
    test(std::locale::collate);
    test(std::locale::ctype);
    test(std::locale::monetary);
    test(std::locale::numeric);
    test(std::locale::time);
    test(std::locale::messages);
    test(std::locale::all);

  return;
}
