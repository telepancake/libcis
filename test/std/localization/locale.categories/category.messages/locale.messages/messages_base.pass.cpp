// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_messages_locale_messages_messages_base).
// main -> test_localization_locale_categories_category_messages_locale_messages_messages_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class messages_base
// {
// public:
//     typedef unspecified catalog;
// };

#include <cstdint>
#include <locale>
#include <type_traits>

#include "assert_macros.h"

#ifdef _LIBCPP_VERSION
namespace libcis_ns_localization_locale_categories_category_messages_locale_messages_messages_base { // libcis: isolate file-scope helpers
ASSERT_SAME_TYPE(std::messages_base::catalog, std::intptr_t);
#endif

// Check that we implement LWG2028
static_assert(std::is_signed<std::messages_base::catalog>::value, "");
static_assert(std::is_integral<std::messages_base::catalog>::value, "");
} using namespace libcis_ns_localization_locale_categories_category_messages_locale_messages_messages_base; // libcis


void test_localization_locale_categories_category_messages_locale_messages_messages_base() {
  std::messages_base mb;

  return;
}
