// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_syn_wcmatch_43eb7079).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef match_results<const wchar_t*>   wcmatch;

// XFAIL: no-wide-characters

#include <regex>
#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_re_re_syn_wcmatch_43eb7079 { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::match_results<const wchar_t*>, std::wcmatch>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_re_re_syn_wcmatch_43eb7079 (libcis)

