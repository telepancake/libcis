// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_facet_numpunct_locale_numpunct_ctor).
// main -> test_localization_locale_categories_facet_numpunct_locale_numpunct_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class numpunct;

// explicit numpunct(size_t refs = 0);

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_ctor { // libcis: isolate file-scope helpers
template <class C>
class my_facet
    : public std::numpunct<C>
{
public:
    static int count;

    explicit my_facet(std::size_t refs = 0)
        : std::numpunct<C>(refs) {++count;}

    ~my_facet() {--count;}
};
} using namespace libcis_ns_localization_locale_categories_facet_numpunct_locale_numpunct_ctor; // libcis


template <class C> int my_facet<C>::count = 0;

void test_localization_locale_categories_facet_numpunct_locale_numpunct_ctor()
{
    {
        std::locale l(std::locale::classic(), new my_facet<char>);
        assert(my_facet<char>::count == 1);
    }
    assert(my_facet<char>::count == 0);
    {
        my_facet<char> f(1);
        assert(my_facet<char>::count == 1);
        {
            std::locale l(std::locale::classic(), &f);
            assert(my_facet<char>::count == 1);
        }
        assert(my_facet<char>::count == 1);
    }
    assert(my_facet<char>::count == 0);

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::locale l(std::locale::classic(), new my_facet<wchar_t>);
        assert(my_facet<wchar_t>::count == 1);
    }
    assert(my_facet<wchar_t>::count == 0);
    {
        my_facet<wchar_t> f(1);
        assert(my_facet<wchar_t>::count == 1);
        {
            std::locale l(std::locale::classic(), &f);
            assert(my_facet<wchar_t>::count == 1);
        }
        assert(my_facet<wchar_t>::count == 1);
    }
    assert(my_facet<wchar_t>::count == 0);
#endif // TEST_HAS_NO_WIDE_CHARACTERS

  return;
}
