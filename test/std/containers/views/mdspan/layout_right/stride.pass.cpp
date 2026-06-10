// AST-transferred from libc++ by tools/transfer.py (slug=containers_views_mdspan_layout_right_stride).
// main -> test_containers_views_mdspan_layout_right_stride; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <mdspan>

// constexpr index_type stride(rank_type i) const noexcept;
//
//   Constraints: extents_type::rank() > 0 is true.
//
//   Preconditions: i < extents_type::rank() is true.
//
//   Returns: extents().rev-prod-of-extents(i).

#include <mdspan>
#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>

#include "test_macros.h"

namespace libcis_ns_containers_views_mdspan_layout_right_stride { // libcis: isolate file-scope helpers
template <class E, class... Args>
constexpr void test_stride(std::array<typename E::index_type, E::rank()> strides, Args... args) {
  using M = std::layout_right::mapping<E>;
  M m(E(args...));

  ASSERT_NOEXCEPT(m.stride(0));
  for (size_t r = 0; r < E::rank(); r++)
    assert(strides[r] == m.stride(r));
}

constexpr bool test() {
  constexpr size_t D = std::dynamic_extent;
  test_stride<std::extents<unsigned, D>>(std::array<unsigned, 1>{1}, 7);
  test_stride<std::extents<unsigned, 7>>(std::array<unsigned, 1>{1});
  test_stride<std::extents<unsigned, 7, 8>>(std::array<unsigned, 2>{8, 1});
  test_stride<std::extents<int64_t, D, 8, D, D>>(std::array<int64_t, 4>{720, 90, 10, 1}, 7, 9, 10);
  return true;
}
} using namespace libcis_ns_containers_views_mdspan_layout_right_stride; // libcis


void test_containers_views_mdspan_layout_right_stride() {
  test();
  static_assert(test());
  return;
}
