// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_numeric_locale_num_get_types_5fbf1d50).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class charT, class InputIterator = istreambuf_iterator<charT> >
// class num_get
//     : public locale::facet
// {
// public:
//     typedef charT char_type;
//     typedef InputIterator iter_type;

#include <locale>
#include <iterator>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_numeric_locale_num_get_types_5fbf1d50 { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::locale::facet, std::num_get<char> >::value), "");
    static_assert((std::is_base_of<std::locale::facet, std::num_get<wchar_t> >::value), "");
    static_assert((std::is_same<std::num_get<char>::char_type, char>::value), "");
    static_assert((std::is_same<std::num_get<wchar_t>::char_type, wchar_t>::value), "");
    static_assert((std::is_same<std::num_get<char>::iter_type, std::istreambuf_iterator<char> >::value), "");
    static_assert((std::is_same<std::num_get<wchar_t>::iter_type, std::istreambuf_iterator<wchar_t> >::value), "");

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_numeric_locale_num_get_types_5fbf1d50 (libcis)

