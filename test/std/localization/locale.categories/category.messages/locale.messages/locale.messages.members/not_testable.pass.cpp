// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_messages_locale_messages_locale_messages_members_not_testable).
// main -> test_localization_locale_categories_category_messages_locale_messages_locale_messages_members_not_testable; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class messages<charT>

// catalog open(const basic_string<char>& name, const locale&) const;

#include <locale>
#include <cassert>

#include "test_macros.h"

// As far as I can tell, the messages facet is untestable.  I have a best
// effort implementation in the hopes that in the future I will learn how
// to test it.

namespace libcis_ns_localization_locale_categories_category_messages_locale_messages_locale_messages_members_not_testable { // libcis: isolate file-scope helpers
template <class CharT>
class F
    : public std::messages<CharT>
{
public:
    explicit F(std::size_t refs = 0)
        : std::messages<CharT>(refs) {}
};
} using namespace libcis_ns_localization_locale_categories_category_messages_locale_messages_locale_messages_members_not_testable; // libcis


void test_localization_locale_categories_category_messages_locale_messages_locale_messages_members_not_testable()
{

  return;
}
