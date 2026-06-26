// transferred+adapted from libc++ by tools/transfer.py (slug=localization_locale_categories_category_ctype_locale_codecvt_codecvt_base_ef6aa648).
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

namespace libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_codecvt_base_ef6aa648 { // libcis
int main(int, char**)
{
    assert(std::codecvt_base::ok == 0);
    assert(std::codecvt_base::partial == 1);
    assert(std::codecvt_base::error == 2);
    assert(std::codecvt_base::noconv == 3);

  return 0;

    return 0;
}
} // libcis_ns_localization_locale_categories_category_ctype_locale_codecvt_codecvt_base_ef6aa648 (libcis)

