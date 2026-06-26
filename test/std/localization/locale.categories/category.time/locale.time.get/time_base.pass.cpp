// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_time_locale_time_get_time_base_82d960f0).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class time_base
// {
// public:
//     enum dateorder {no_order, dmy, mdy, ymd, ydm};
// };

#include <locale>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_localization_locale_categories_category_time_locale_time_get_time_base_82d960f0 { // libcis
int main(int, char**)
{
    std::time_base::dateorder d = std::time_base::no_order;
    ((void)d); // Prevent unused warning
    assert(std::time_base::no_order == 0);
    assert(std::time_base::dmy == 1);
    assert(std::time_base::mdy == 2);
    assert(std::time_base::ymd == 3);
    assert(std::time_base::ydm == 4);

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_time_locale_time_get_time_base_82d960f0 (libcis)

