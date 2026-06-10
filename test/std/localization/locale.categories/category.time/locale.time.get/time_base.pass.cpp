// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_time_locale_time_get_time_base).
// main -> test_localization_locale_categories_category_time_locale_time_get_time_base; file-scope helpers isolated in anon namespace.
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

void test_localization_locale_categories_category_time_locale_time_get_time_base()
{
    std::time_base::dateorder d = std::time_base::no_order;
    ((void)d); // Prevent unused warning
    assert(std::time_base::no_order == 0);
    assert(std::time_base::dmy == 1);
    assert(std::time_base::mdy == 2);
    assert(std::time_base::ymd == 3);
    assert(std::time_base::ydm == 4);

  return;
}
