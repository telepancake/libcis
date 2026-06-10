// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_expected_observers_error).
// main -> test_utilities_expected_expected_expected_observers_error; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr const E& error() const & noexcept;
// constexpr E& error() & noexcept;
// constexpr E&& error() && noexcept;
// constexpr const E&& error() const && noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"

// Test noexcept
namespace libcis_ns_utilities_expected_expected_expected_observers_error { // libcis: isolate file-scope helpers
template <class T>
concept ErrorNoexcept =
    requires(T t) {
      { std::forward<T>(t).error() } noexcept;
    };

static_assert(!ErrorNoexcept<int>);

static_assert(ErrorNoexcept<std::expected<int, int>&>);
static_assert(ErrorNoexcept<const std::expected<int, int>&>);
static_assert(ErrorNoexcept<std::expected<int, int>&&>);
static_assert(ErrorNoexcept<const std::expected<int, int>&&>);

constexpr bool test() {
  // non-const &
  {
    std::expected<int, int> e(std::unexpect, 5);
    decltype(auto) x = e.error();
    static_assert(std::same_as<decltype(x), int&>);
    assert(x == 5);
  }

  // const &
  {
    const std::expected<int, int> e(std::unexpect, 5);
    decltype(auto) x = e.error();
    static_assert(std::same_as<decltype(x), const int&>);
    assert(x == 5);
  }

  // non-const &&
  {
    std::expected<int, int> e(std::unexpect, 5);
    decltype(auto) x = std::move(e).error();
    static_assert(std::same_as<decltype(x), int&&>);
    assert(x == 5);
  }

  // const &&
  {
    const std::expected<int, int> e(std::unexpect, 5);
    decltype(auto) x = std::move(e).error();
    static_assert(std::same_as<decltype(x), const int&&>);
    assert(x == 5);
  }

  return true;
}
} using namespace libcis_ns_utilities_expected_expected_expected_observers_error; // libcis


void test_utilities_expected_expected_expected_observers_error() {
  test();
  static_assert(test());
  return;
}
