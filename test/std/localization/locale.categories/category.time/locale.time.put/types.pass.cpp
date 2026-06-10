// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_time_locale_time_put_types).
// main -> test_localization_locale_categories_category_time_locale_time_put_types; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT, class OutputIterator = ostreambuf_iterator<charT> >
// class time_put
//     : public locale::facet
// {
// public:
//     typedef charT          char_type;
//     typedef OutputIterator iter_type;

#include <locale>
#include <iterator>
#include <type_traits>

#include "test_macros.h"

void test_localization_locale_categories_category_time_locale_time_put_types()
{
    static_assert((std::is_base_of<std::locale::facet, std::time_put<char> >::value), "");
    static_assert((std::is_base_of<std::locale::facet, std::time_put<wchar_t> >::value), "");
    static_assert((std::is_same<std::time_put<char>::char_type, char>::value), "");
    static_assert((std::is_same<std::time_put<wchar_t>::char_type, wchar_t>::value), "");
    static_assert((std::is_same<std::time_put<char>::iter_type, std::ostreambuf_iterator<char> >::value), "");
    static_assert((std::is_same<std::time_put<wchar_t>::iter_type, std::ostreambuf_iterator<wchar_t> >::value), "");

  return;
}
