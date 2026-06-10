// AST-transferred from libc++ by tools/transfer.py (slug=utilities_variant_variant_monostate_relops_relops).
// main -> test_utilities_variant_variant_monostate_relops_relops; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// constexpr bool operator<(monostate, monostate) noexcept { return false; }
// constexpr bool operator>(monostate, monostate) noexcept { return false; }
// constexpr bool operator<=(monostate, monostate) noexcept { return true; }
// constexpr bool operator>=(monostate, monostate) noexcept { return true; }
// constexpr bool operator==(monostate, monostate) noexcept { return true; }
// constexpr bool operator!=(monostate, monostate) noexcept { return false; }
// constexpr strong_ordering operator<=>(monostate, monostate) noexcept { return strong_ordering::equal; } // since C++20

#include <cassert>
#include <variant>

#include "test_comparisons.h"
#include "test_macros.h"

namespace libcis_ns_utilities_variant_variant_monostate_relops_relops { // libcis: isolate file-scope helpers
constexpr bool test() {
  using M = std::monostate;
  constexpr M m1{};
  constexpr M m2{};
  assert(testComparisons(m1, m2, /*isEqual*/ true, /*isLess*/ false));
  AssertComparisonsAreNoexcept<M>();

#if TEST_STD_VER > 17
  assert(testOrder(m1, m2, std::strong_ordering::equal));
  AssertOrderAreNoexcept<M>();
#endif // TEST_STD_VER > 17

  return true;
}
} using namespace libcis_ns_utilities_variant_variant_monostate_relops_relops; // libcis


void test_utilities_variant_variant_monostate_relops_relops() {
  test();
  static_assert(test());

  return;
}
