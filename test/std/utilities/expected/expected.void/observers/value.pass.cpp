// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_void_observers_value).
// main -> test_utilities_expected_expected_void_observers_value; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr void value() const &;
// constexpr void value() &&;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "MoveOnly.h"
#include "test_macros.h"

namespace libcis_ns_utilities_expected_expected_void_observers_value { // libcis: isolate file-scope helpers
constexpr bool test() {
  // const &
  {
    const std::expected<void, int> e;
    e.value();
    static_assert(std::is_same_v<decltype(e.value()), void>);
  }

  // &
  {
    std::expected<void, int> e;
    e.value();
    static_assert(std::is_same_v<decltype(e.value()), void>);
  }

  // &&
  {
    std::expected<void, int> e;
    std::move(e).value();
    static_assert(std::is_same_v<decltype(std::move(e).value()), void>);
  }

  // const &&
  {
    const std::expected<void, int> e;
    std::move(e).value();
    static_assert(std::is_same_v<decltype(std::move(e).value()), void>);
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS

  // int
  {
    const std::expected<void, int> e(std::unexpect, 5);
    try {
      e.value();
      assert(false);
    } catch (const std::bad_expected_access<int>& ex) {
      assert(ex.error() == 5);
    }
  }

#endif // TEST_HAS_NO_EXCEPTIONS
}
} using namespace libcis_ns_utilities_expected_expected_void_observers_value; // libcis


void test_utilities_expected_expected_void_observers_value() {
  test();
  static_assert(test());
  testException();
  return;
}
