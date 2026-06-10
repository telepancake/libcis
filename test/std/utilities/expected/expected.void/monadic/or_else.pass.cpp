// AST-transferred from libc++ by tools/transfer.py (slug=utilities_expected_expected_void_monadic_or_else).
// main -> test_utilities_expected_expected_void_monadic_or_else; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <expected>

// template<class F> constexpr auto or_else(F&& f) &;
// template<class F> constexpr auto or_else(F&& f) const &;
// template<class F> constexpr auto or_else(F&& f) &&;
// template<class F> constexpr auto or_else(F&& f) const &&;

#include <expected>
#include <concepts>
#include <cassert>
#include <type_traits>
#include <utility>

namespace libcis_ns_utilities_expected_expected_void_monadic_or_else { // libcis: isolate file-scope helpers
constexpr void test_val_types() {
  // Test & overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(l);
    assert(val.has_value());
  }

  // Test const& overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    const std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(l);
    assert(val.has_value());
  }

  // Test && overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(l);
    assert(val.has_value());
  }

  // Test const&& overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    const std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(l);
    assert(val.has_value());
  }
}

constexpr void test_fail() {
  auto never_called = [](auto) -> std::expected<void, long> {
    assert(false);
    return std::expected<void, long>(std::unexpected<long>(5));
  };

  // Test & overload
  {
    std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(never_called);
    assert(val.has_value());
  }

  // Test const& overload
  {
    const std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(never_called);
    assert(val.has_value());
  }

  // Test && overload
  {
    std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(never_called);
    assert(val.has_value());
  }

  // Test const&& overload
  {
    const std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(never_called);
    assert(val.has_value());
  }
}

constexpr bool test() {
  test_fail();
  test_val_types();
  return true;
}
} using namespace libcis_ns_utilities_expected_expected_void_monadic_or_else; // libcis


void test_utilities_expected_expected_void_monadic_or_else() {
  test();
  static_assert(test());

  return;
}
