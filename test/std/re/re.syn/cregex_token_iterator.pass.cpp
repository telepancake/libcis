// transferred+adapted from libc++ by tools/transfer.py (slug=re_re_syn_cregex_token_iterator_7e1b22bc).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// typedef regex_token_iterator<const char*>   cregex_token_iterator;

#include <regex>
#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_re_re_syn_cregex_token_iterator_7e1b22bc { // libcis
int main(int, char**)
{
    static_assert((std::is_same<std::regex_token_iterator<const char*>, std::cregex_token_iterator>::value), "");

  return 0;

    return 0;
}
} // libcis_ns_re_re_syn_cregex_token_iterator_7e1b22bc (libcis)

