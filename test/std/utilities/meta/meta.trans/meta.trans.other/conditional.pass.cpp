// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_meta_meta_trans_meta_trans_other_conditional_5e43a149).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// conditional

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_trans_meta_trans_other_conditional_5e43a149 { // libcis
int main(int, char**)
{
    ASSERT_SAME_TYPE(char, std::conditional<true, char, int>::type);
    ASSERT_SAME_TYPE(int,  std::conditional<false, char, int>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(char, std::conditional_t<true, char, int>);
    ASSERT_SAME_TYPE(int,  std::conditional_t<false, char, int>);
#endif

  return 0;

    return 0;
}
} // libcis_ns_utilities_meta_meta_trans_meta_trans_other_conditional_5e43a149 (libcis)

