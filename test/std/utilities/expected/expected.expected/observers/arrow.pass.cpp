// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_expected_observers_arrow).
// main -> test_utilities_expected_expected_expected_observers_arrow; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr const T* operator->() const noexcept;
// constexpr T* operator->() noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"

// Test noexcept
namespace libcis_ns_utilities_expected_expected_expected_observers_arrow { // libcis: isolate file-scope helpers
template <class T>
concept ArrowNoexcept =
    requires(T t) {
      { t.operator->() } noexcept;
    };

static_assert(!ArrowNoexcept<int>);

static_assert(ArrowNoexcept<std::expected<int, int>>);
static_assert(ArrowNoexcept<const std::expected<int, int>>);

constexpr bool test() {
  // const
  {
    const std::expected<int, int> e(5);
    std::same_as<const int*> decltype(auto) x = e.operator->();
    assert(x == &(e.value()));
    assert(*x == 5);
  }

  // non-const
  {
    std::expected<int, int> e(5);
    std::same_as<int*> decltype(auto) x = e.operator->();
    assert(x == &(e.value()));
    assert(*x == 5);
  }

  return true;
}
} using namespace libcis_ns_utilities_expected_expected_expected_observers_arrow; // libcis


void test_utilities_expected_expected_expected_observers_arrow() {
  test();
  static_assert(test());
  return;
}
