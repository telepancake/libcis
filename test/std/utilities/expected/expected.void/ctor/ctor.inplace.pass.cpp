// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_void_ctor_ctor_inplace).
// main -> test_utilities_expected_expected_void_ctor_ctor_inplace; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr explicit expected(in_place_t) noexcept;

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>

// test explicit
namespace libcis_ns_utilities_expected_expected_void_ctor_ctor_inplace { // libcis: isolate file-scope helpers
static_assert(std::is_constructible_v<std::expected<void, int>, std::in_place_t>);
static_assert(!std::is_convertible_v<std::in_place_t, std::expected<void, int>>);

// test noexcept
static_assert(std::is_nothrow_constructible_v<std::expected<void, int>, std::in_place_t>);

constexpr bool test() {
  std::expected<void, int> e(std::in_place);
  assert(e.has_value());

  return true;
}
} using namespace libcis_ns_utilities_expected_expected_void_ctor_ctor_inplace; // libcis


void test_utilities_expected_expected_void_ctor_ctor_inplace() {
  test();
  static_assert(test());
  return;
}
