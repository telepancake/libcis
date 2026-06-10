// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_ctor_char16_t).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_ctor_char16_t; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <> class codecvt<char16_t, char, mbstate_t>

// explicit codecvt(size_t refs = 0);

// This test runs in C++20, but we have deprecated codecvt<char(16|32), char, mbstate_t> in C++20.
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_ctor_char16_t { // libcis: isolate file-scope helpers
typedef std::codecvt<char16_t, char, std::mbstate_t> F;

class my_facet
    : public F
{
public:
    static int count;

    explicit my_facet(std::size_t refs = 0)
        : F(refs) {++count;}

    ~my_facet() {--count;}
};
} using namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_ctor_char16_t; // libcis


int my_facet::count = 0;

void test_localization_locale_categories_category_ctype_locale_codecvt_ctor_char16_t()
{
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
