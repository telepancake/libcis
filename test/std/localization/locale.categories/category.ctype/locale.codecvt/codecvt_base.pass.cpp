// AST-transferred from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_codecvt_base).
// main -> test_localization_locale_categories_category_ctype_locale_codecvt_codecvt_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <locale>

// class codecvt_base
// {
// public:
//     enum result {ok, partial, error, noconv};
// };

#include <locale>
#include <cassert>

#include "test_macros.h"

void test_localization_locale_categories_category_ctype_locale_codecvt_codecvt_base()
{
    assert(std::codecvt_base::ok == 0);
    assert(std::codecvt_base::partial == 1);
    assert(std::codecvt_base::error == 2);
    assert(std::codecvt_base::noconv == 3);

  return;
}
