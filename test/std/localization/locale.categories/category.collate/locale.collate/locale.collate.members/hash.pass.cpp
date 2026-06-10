// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_collate_locale_collate_locale_collate_members_hash).
// main -> test_localization_locale_categories_category_collate_locale_collate_locale_collate_members_hash; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// https://llvm.org/PR41018
// XFAIL: windows-dll && msvc

// <locale>

// template <class charT> class collate;

// long hash(const charT* low, const charT* high) const;

//   This test is not portable

#include <locale>
#include <string>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_category_collate_locale_collate_locale_collate_members_hash()
{
    std::locale l = std::locale::classic();
    {
        std::string x1("1234");
        std::string x2("12345");
        const std::collate<char>& f = std::use_facet<std::collate<char> >(l);
        assert(f.hash(x1.data(), x1.data() + x1.size())
            != f.hash(x2.data(), x2.data() + x2.size()));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wstring x1(L"1234");
        std::wstring x2(L"12345");
        const std::collate<wchar_t>& f = std::use_facet<std::collate<wchar_t> >(l);
        assert(f.hash(x1.data(), x1.data() + x1.size())
            != f.hash(x2.data(), x2.data() + x2.size()));
    }
#endif

  return;
}
