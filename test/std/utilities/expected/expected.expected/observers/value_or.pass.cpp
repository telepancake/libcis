// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_expected_observers_value_or).
// main -> test_utilities_expected_expected_expected_observers_value_or; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// template<class U> constexpr T value_or(U&& v) const &;
// template<class U> constexpr T value_or(U&& v) &&;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "MoveOnly.h"
#include "test_macros.h"

namespace libcis_ns_utilities_expected_expected_expected_observers_value_or { // libcis: isolate file-scope helpers
constexpr bool test() {
  // const &, has_value()
  {
    const std::expected<int, int> e(5);
    std::same_as<int> decltype(auto) x = e.value_or(10);
    assert(x == 5);
  }

  // const &, !has_value()
  {
    const std::expected<int, int> e(std::unexpect, 5);
    std::same_as<int> decltype(auto) x = e.value_or(10);
    assert(x == 10);
  }

  // &&, has_value()
  {
    std::expected<MoveOnly, int> e(std::in_place, 5);
    std::same_as<MoveOnly> decltype(auto) x = std::move(e).value_or(10);
    assert(x == 5);
  }

  // &&, !has_value()
  {
    std::expected<MoveOnly, int> e(std::unexpect, 5);
    std::same_as<MoveOnly> decltype(auto) x = std::move(e).value_or(10);
    assert(x == 10);
  }

  return true;
}
} using namespace libcis_ns_utilities_expected_expected_expected_observers_value_or; // libcis


void test_utilities_expected_expected_expected_observers_value_or() {
  test();
  static_assert(test());
  return;
}
