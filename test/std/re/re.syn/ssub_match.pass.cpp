// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_syn_ssub_match_bf14a17c).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef sub_match<string::const_iterator>   ssub_match;

#include <regex>
#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_re_re_syn_ssub_match_bf14a17c { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::sub_match<std::string::const_iterator>, std::ssub_match>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_re_re_syn_ssub_match_bf14a17c (libcis)

