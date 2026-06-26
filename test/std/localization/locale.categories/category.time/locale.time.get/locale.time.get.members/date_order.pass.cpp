// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_time_locale_time_get_locale_time_get_members_date_order_ed2e31cb).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class time_get<charT, InputIterator>

// dateorder date_order() const;

#include <locale>
#include <cassert>
#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_localization_locale_categories_category_time_locale_time_get_locale_time_get_members_date_order_ed2e31cb { // libcis
typedef std::time_get<char, cpp17_input_iterator<const char*> > F;

class my_facet
    : public F
{
public:
    explicit my_facet(std::size_t refs = 0)
        : F(refs) {}
};

int main(int, char**)
{
    const my_facet f(1);
    assert(f.date_order() == std::time_base::mdy);

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_time_locale_time_get_locale_time_get_members_date_order_ed2e31cb (libcis)

