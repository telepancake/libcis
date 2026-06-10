// AST-transferred from libc++ by tools/transfer.py (slug=utilities_utility_forward_forward_like).
// main -> test_utilities_utility_forward_forward_like; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// test forward_like

#include <cassert>
#include <type_traits>
#include <utility>

namespace libcis_ns_utilities_utility_forward_forward_like { // libcis: isolate file-scope helpers
struct U {}; // class type so const-qualification is not stripped from a prvalue
using CU = const U;
using T  = int;
using CT = const T;

U u{};
const U& cu = u;

static_assert(std::is_same_v<decltype(std::forward_like<T>(U{})), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(u)), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(std::move(u))), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(std::move(cu))), CU&&>);

static_assert(std::is_same_v<decltype(std::forward_like<CT>(U{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(u)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(std::move(u))), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(std::move(cu))), CU&&>);

static_assert(std::is_same_v<decltype(std::forward_like<T&>(U{})), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(CU{})), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(u)), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(cu)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(std::move(u))), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(std::move(cu))), CU&>);

static_assert(std::is_same_v<decltype(std::forward_like<CT&>(U{})), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(CU{})), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(u)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(cu)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(std::move(u))), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(std::move(cu))), CU&>);

static_assert(std::is_same_v<decltype(std::forward_like<T&&>(U{})), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(u)), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(std::move(u))), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(std::move(cu))), CU&&>);

static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(U{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(u)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(std::move(u))), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(std::move(cu))), CU&&>);

static_assert(noexcept(std::forward_like<T>(u)));

static_assert(std::is_same_v<decltype(std::forward_like<U&>(u)), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<CU&>(cu)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<U&&>(std::move(u))), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CU&&>(std::move(cu))), CU&&>);

struct NoCtorCopyMove {
  NoCtorCopyMove() = delete;
  NoCtorCopyMove(const NoCtorCopyMove&) = delete;
  NoCtorCopyMove(NoCtorCopyMove&&) = delete;
};

static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(std::declval<NoCtorCopyMove>())), const NoCtorCopyMove&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(std::declval<NoCtorCopyMove>())), const NoCtorCopyMove&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(std::declval<NoCtorCopyMove>())), NoCtorCopyMove&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(std::declval<NoCtorCopyMove>())), NoCtorCopyMove&>);

static_assert(noexcept(std::forward_like<T>(std::declval<NoCtorCopyMove>())));

constexpr bool test() {
  {
    int val       = 1729;
    auto&& result = std::forward_like<const double&>(val);
    static_assert(std::is_same_v<decltype(result), const int&>);
    assert(&result == &val);
  }
  {
    int val       = 1729;
    auto&& result = std::forward_like<double&>(val);
    static_assert(std::is_same_v<decltype(result), int&>);
    assert(&result == &val);
  }
  {
    int val       = 1729;
    auto&& result = std::forward_like<const double&&>(val);
    static_assert(std::is_same_v<decltype(result), const int&&>);
    assert(&result == &val);
  }
  {
    int val       = 1729;
    auto&& result = std::forward_like<double&&>(val);
    static_assert(std::is_same_v<decltype(result), int&&>);
    assert(&result == &val);
  }
  return true;
}
} using namespace libcis_ns_utilities_utility_forward_forward_like; // libcis


void test_utilities_utility_forward_forward_like() {
  test();
  static_assert(test());

  return;
}
