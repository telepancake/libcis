// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_trans_meta_trans_other_enable_if).
// main -> test_utilities_meta_meta_trans_meta_trans_other_enable_if; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// enable_if

#include <type_traits>

#include "test_macros.h"

void test_utilities_meta_meta_trans_meta_trans_other_enable_if()
{
    ASSERT_SAME_TYPE(void, std::enable_if<true>::type);
    ASSERT_SAME_TYPE(int,  std::enable_if<true, int>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(void, std::enable_if_t<true, void>);
    ASSERT_SAME_TYPE(int,  std::enable_if_t<true, int>);
#endif

  return;
}
