// AST-transferred from libc++ by tools/transfer.py (slug=depr_depr_c_headers_assert_h).
// main -> test_depr_depr_c_headers_assert_h; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test <assert.h>

#include <assert.h>

#ifndef assert
#error assert not defined
#endif

void test_depr_depr_c_headers_assert_h() {
  assert(true);

  return;
}
