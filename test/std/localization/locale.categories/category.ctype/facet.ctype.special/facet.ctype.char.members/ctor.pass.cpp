// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_ctor).
// main -> test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class ctype;

// explicit ctype(const mask* tbl = 0, bool del = false, size_t refs = 0);

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_ctor { // libcis: isolate file-scope helpers
class my_facet
    : public std::ctype<char>
{
public:
    static int count;

    explicit my_facet(const mask* tbl = 0, bool del = false, std::size_t refs = 0)
        : std::ctype<char>(tbl, del, refs) {++count;}

    ~my_facet() {--count;}
};
} using namespace libcis_ns_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_ctor; // libcis


int my_facet::count = 0;

void test_localization_locale_categories_category_ctype_facet_ctype_special_facet_ctype_char_members_ctor()
{
    {
        std::locale l(std::locale::classic(), new my_facet);
        assert(my_facet::count == 1);
    }
    assert(my_facet::count == 0);
    {
        my_facet f(0, false, 1);
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
