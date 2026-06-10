// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_types_byteops_enum_direct_init).
// main -> test_language_support_support_types_byteops_enum_direct_init; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <cstddef>
#include <test_macros.h>

// UNSUPPORTED: c++03, c++11, c++14

void test_language_support_support_types_byteops_enum_direct_init() {
  constexpr std::byte b{42};
  static_assert(std::to_integer<int>(b) == 42, "");

  return;
}
