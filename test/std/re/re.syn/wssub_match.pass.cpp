// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_syn_wssub_match_9c096333).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef sub_match<wstring::const_iterator>   wssub_match;

// XFAIL: no-wide-characters

#include <regex>
#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_re_re_syn_wssub_match_9c096333 { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::sub_match<std::wstring::const_iterator>, std::wssub_match>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_re_re_syn_wssub_match_9c096333 (libcis)

