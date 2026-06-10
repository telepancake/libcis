// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_dynamic_nothrow_t).
// main -> test_language_support_support_dynamic_nothrow_t; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// struct nothrow_t {
//   explicit nothrow_t() = default;
// };
// extern const nothrow_t nothrow;

#include <new>


void test_language_support_support_dynamic_nothrow_t() {
  std::nothrow_t x = std::nothrow;
  (void)x;

  return;
}
