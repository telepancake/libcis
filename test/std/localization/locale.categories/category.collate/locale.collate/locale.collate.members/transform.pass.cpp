// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_collate_locale_collate_locale_collate_members_transform).
// main -> test_localization_locale_categories_category_collate_locale_collate_locale_collate_members_transform; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT> class collate;

// string_type transform(const charT* low, const charT* high) const;

#include <locale>
#include <string>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_category_collate_locale_collate_locale_collate_members_transform()
{
    std::locale l = std::locale::classic();
    {
        std::string x("1234");
        const std::collate<char>& f = std::use_facet<std::collate<char> >(l);
        assert(f.transform(x.data(), x.data() + x.size()) == x);
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wstring x(L"1234");
        const std::collate<wchar_t>& f = std::use_facet<std::collate<wchar_t> >(l);
        assert(f.transform(x.data(), x.data() + x.size()) == x);
    }
#endif

  return;
}
