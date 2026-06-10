// AST-transferred from libc++ by tools/transfer.py (slug=utilities_variant_variant_monostate_monostate).
// main -> test_utilities_variant_variant_monostate_monostate; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// struct monostate {};

#include <type_traits>
#include <variant>

#include "test_macros.h"

void test_utilities_variant_variant_monostate_monostate() {
  using M = std::monostate;
  static_assert(std::is_trivially_default_constructible<M>::value, "");
  static_assert(std::is_trivially_copy_constructible<M>::value, "");
  static_assert(std::is_trivially_copy_assignable<M>::value, "");
  static_assert(std::is_trivially_destructible<M>::value, "");
  constexpr M m{};
  ((void)m);

  return;
}
