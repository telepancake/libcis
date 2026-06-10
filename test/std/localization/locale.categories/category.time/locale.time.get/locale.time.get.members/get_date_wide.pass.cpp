// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_time_locale_time_get_locale_time_get_members_get_date_wide).
// main -> test_localization_locale_categories_category_time_locale_time_get_locale_time_get_members_get_date_wide; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class time_get<charT, InputIterator>

// iter_type
// get_date(iter_type s, iter_type end, ios_base& str,
//          ios_base::iostate& err, tm* t) const;

// XFAIL: no-wide-characters

#include <locale>
#include <cassert>
#include <ios>
#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_localization_locale_categories_category_time_locale_time_get_locale_time_get_members_get_date_wide { // libcis: isolate file-scope helpers
typedef cpp17_input_iterator<const wchar_t*> I;

typedef std::time_get<wchar_t, I> F;

class my_facet
    : public F
{
public:
    explicit my_facet(std::size_t refs = 0)
        : F(refs) {}
};
} using namespace libcis_ns_localization_locale_categories_category_time_locale_time_get_locale_time_get_members_get_date_wide; // libcis


void test_localization_locale_categories_category_time_locale_time_get_locale_time_get_members_get_date_wide()
{
    const my_facet f(1);
    std::ios ios(0);
    std::ios_base::iostate err;
    std::tm t;
    {
        const wchar_t in[] = L"5/5/5";
        err = std::ios_base::goodbit;
        t = std::tm();
        I i = f.get_date(I(in), I(in+sizeof(in)/sizeof(in[0])-1), ios, err, &t);
        assert(base(i) == in+sizeof(in)/sizeof(in[0])-1);
        assert(t.tm_mon == 4);
        assert(t.tm_mday == 5);
        assert(t.tm_year == 105);
        assert(err == std::ios_base::eofbit);
    }

  return;
}
