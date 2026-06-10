// AST-transferred from libc++ by tools/transfer.py (slug=localization_locales_locale_global_templates_has_facet).
// main -> test_localization_locales_locale_global_templates_has_facet; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// template <class Facet> bool has_facet(const locale& loc) throw();

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locales_locale_global_templates_has_facet { // libcis: isolate file-scope helpers
struct my_facet
    : public std::locale::facet
{
    static std::locale::id id;
};
} using namespace libcis_ns_localization_locales_locale_global_templates_has_facet; // libcis


std::locale::id my_facet::id;

void test_localization_locales_locale_global_templates_has_facet()
{
    std::locale loc;
    assert(std::has_facet<std::ctype<char> >(loc));
    assert(!std::has_facet<my_facet>(loc));
    std::locale loc2(loc, new my_facet);
    assert(std::has_facet<my_facet>(loc2));

  return;
}
