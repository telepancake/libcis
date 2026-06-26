// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_syn_cregex_iterator_6f332a38).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef regex_iterator<const char*>   cregex_iterator;

#include <regex>
#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_re_re_syn_cregex_iterator_6f332a38 { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::regex_iterator<const char*>, std::cregex_iterator>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_re_re_syn_cregex_iterator_6f332a38 (libcis)

