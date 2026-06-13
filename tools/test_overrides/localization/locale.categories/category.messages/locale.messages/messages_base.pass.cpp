// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_messages_locale_messages_messages_base).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// libcis: the auto-transferred form opened the test namespace *inside*
// `#ifdef _LIBCPP_VERSION` but closed it unconditionally, so with _LIBCPP_VERSION
// undefined (this port) the braces are unbalanced AND the entry symbol
// libcis_ns_...::main is never declared.  This hand-adapted copy opens the
// namespace unconditionally and keeps the ASSERT_SAME_TYPE (catalog == intptr_t
// holds here) outside any _LIBCPP_VERSION guard.

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

namespace libcis_ns_localization_locale_categories_category_messages_locale_messages_messages_base { // libcis
ASSERT_SAME_TYPE(std::messages_base::catalog, std::intptr_t);

// Check that we implement LWG2028
static_assert(std::is_signed<std::messages_base::catalog>::value, "");
static_assert(std::is_integral<std::messages_base::catalog>::value, "");

int main(int, char**) {
  std::messages_base mb;

  return 0;
}
} // libcis_ns_localization_locale_categories_category_messages_locale_messages_messages_base (libcis)
