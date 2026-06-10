// AST-transferred from libc++ by tools/transfer.py (slug=containers_views_views_span_span_elem_back).
// main -> test_containers_views_views_span_span_elem_back; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <span>

// constexpr reference back() const noexcept;
//   Expects: empty() is false.
//   Effects: Equivalent to: return *(data() + (size() - 1));
//

#include <span>
#include <cassert>
#include <string>

#include "test_macros.h"

namespace libcis_ns_containers_views_views_span_span_elem_back { // libcis: isolate file-scope helpers
template <typename Span>
constexpr bool testConstexprSpan(Span sp) {
  LIBCPP_ASSERT(noexcept(sp.back()));
  return std::addressof(sp.back()) == sp.data() + sp.size() - 1;
}

template <typename Span>
void testRuntimeSpan(Span sp) {
  LIBCPP_ASSERT(noexcept(sp.back()));
  assert(std::addressof(sp.back()) == sp.data() + sp.size() - 1);
}

template <typename Span>
void testEmptySpan(Span sp) {
  if (!sp.empty()) [[maybe_unused]]
    auto res = sp.back();
}

struct A {};
constexpr int iArr1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int iArr2[]           = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
} using namespace libcis_ns_containers_views_views_span_span_elem_back; // libcis


void test_containers_views_views_span_span_elem_back() {
  static_assert(testConstexprSpan(std::span<const int>(iArr1, 1)), "");
  static_assert(testConstexprSpan(std::span<const int>(iArr1, 2)), "");
  static_assert(testConstexprSpan(std::span<const int>(iArr1, 3)), "");
  static_assert(testConstexprSpan(std::span<const int>(iArr1, 4)), "");

  static_assert(testConstexprSpan(std::span<const int, 1>(iArr1, 1)), "");
  static_assert(testConstexprSpan(std::span<const int, 2>(iArr1, 2)), "");
  static_assert(testConstexprSpan(std::span<const int, 3>(iArr1, 3)), "");
  static_assert(testConstexprSpan(std::span<const int, 4>(iArr1, 4)), "");

  testRuntimeSpan(std::span<int>(iArr2, 1));
  testRuntimeSpan(std::span<int>(iArr2, 2));
  testRuntimeSpan(std::span<int>(iArr2, 3));
  testRuntimeSpan(std::span<int>(iArr2, 4));

  testRuntimeSpan(std::span<int, 1>(iArr2, 1));
  testRuntimeSpan(std::span<int, 2>(iArr2, 2));
  testRuntimeSpan(std::span<int, 3>(iArr2, 3));
  testRuntimeSpan(std::span<int, 4>(iArr2, 4));

  std::string s;
  testRuntimeSpan(std::span<std::string>(&s, 1));
  testRuntimeSpan(std::span<std::string, 1>(&s, 1));

  std::span<int, 0> sp;
  testEmptySpan(sp);

  return;
}
