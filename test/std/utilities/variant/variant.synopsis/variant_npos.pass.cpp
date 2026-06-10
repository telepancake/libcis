// AST-transferred from libc++ by tools/transfer.py (slug=utilities_variant_variant_synopsis_variant_npos).
// main -> test_utilities_variant_variant_synopsis_variant_npos; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// constexpr size_t variant_npos = -1;

#include <variant>

#include "test_macros.h"

void test_utilities_variant_variant_synopsis_variant_npos() {
  static_assert(std::variant_npos == static_cast<std::size_t>(-1), "");

  return;
}
