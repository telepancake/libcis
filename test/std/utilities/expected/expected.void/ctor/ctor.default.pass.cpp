// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_void_ctor_ctor_default).
// main -> test_utilities_expected_expected_void_ctor_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr expected() noexcept;

#include <cassert>
#include <expected>
#include <type_traits>

#include "test_macros.h"

// Test noexcept

namespace libcis_ns_utilities_expected_expected_void_ctor_ctor_default { // libcis: isolate file-scope helpers
struct NoDefaultCtor {
  constexpr NoDefaultCtor() = delete;
};

static_assert(std::is_nothrow_default_constructible_v<std::expected<void, int>>);
static_assert(std::is_nothrow_default_constructible_v<std::expected<void, NoDefaultCtor>>);

struct MyInt {
  int i;
  friend constexpr bool operator==(const MyInt&, const MyInt&) = default;
};

constexpr bool test() {
  // default constructible
  {
    std::expected<void, int> e;
    assert(e.has_value());
  }

  // non-default constructible
  {
    std::expected<void, NoDefaultCtor> e;
    assert(e.has_value());
  }

  return true;
}
} using namespace libcis_ns_utilities_expected_expected_void_ctor_ctor_default; // libcis


void test_utilities_expected_expected_void_ctor_ctor_default() {
  test();
  static_assert(test());
  return;
}
