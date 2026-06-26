// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_time_locale_time_put_types_1d69e34b).
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

namespace libcis_ns_localization_locale_categories_category_time_locale_time_put_types_1d69e34b { // libcis
int main(int, char**)
{
    static_assert((std::is_base_of<std::locale::facet, std::time_put<char> >::value), "");
    static_assert((std::is_base_of<std::locale::facet, std::time_put<wchar_t> >::value), "");
    static_assert((std::is_same<std::time_put<char>::char_type, char>::value), "");
    static_assert((std::is_same<std::time_put<wchar_t>::char_type, wchar_t>::value), "");
    static_assert((std::is_same<std::time_put<char>::iter_type, std::ostreambuf_iterator<char> >::value), "");
    static_assert((std::is_same<std::time_put<wchar_t>::iter_type, std::ostreambuf_iterator<wchar_t> >::value), "");

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_time_locale_time_put_types_1d69e34b (libcis)

