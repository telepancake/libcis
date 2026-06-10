// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_reverse_end).
// main -> test_ranges_range_adaptors_range_reverse_end; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr reverse_iterator<iterator_t<V>> end();
// constexpr auto end() const requires common_range<const V>;

#include <ranges>

#include <cassert>
#include <utility>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_reverse_end { // libcis: isolate file-scope helpers
constexpr bool test() {
  int buffer[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  // Common bidirectional range.
  {
    auto rev = std::ranges::reverse_view(BidirRange{buffer, buffer + 8});
    assert(base(rev.end().base()) == buffer);
    assert(base(std::move(rev).end().base()) == buffer);

    ASSERT_SAME_TYPE(decltype(rev.end()), std::reverse_iterator<bidirectional_iterator<int*>>);
    ASSERT_SAME_TYPE(decltype(std::move(rev).end()), std::reverse_iterator<bidirectional_iterator<int*>>);
  }
  // Const common bidirectional range.
  {
    const auto rev = std::ranges::reverse_view(BidirRange{buffer, buffer + 8});
    assert(base(rev.end().base()) == buffer);
    assert(base(std::move(rev).end().base()) == buffer);

    ASSERT_SAME_TYPE(decltype(rev.end()), std::reverse_iterator<bidirectional_iterator<const int*>>);
    ASSERT_SAME_TYPE(decltype(std::move(rev).end()), std::reverse_iterator<bidirectional_iterator<const int*>>);
  }
  // Non-common, non-const (move only) bidirectional range.
  {
    auto rev = std::ranges::reverse_view(BidirSentRange<MoveOnly>{buffer, buffer + 8});
    assert(base(std::move(rev).end().base()) == buffer);

    ASSERT_SAME_TYPE(decltype(std::move(rev).end()), std::reverse_iterator<bidirectional_iterator<int*>>);
  }
  // Non-common, const bidirectional range.
  {
    auto rev = std::ranges::reverse_view(BidirSentRange<Copyable>{buffer, buffer + 8});
    assert(base(rev.end().base()) == buffer);
    assert(base(std::move(rev).end().base()) == buffer);

    ASSERT_SAME_TYPE(decltype(rev.end()), std::reverse_iterator<bidirectional_iterator<int*>>);
    ASSERT_SAME_TYPE(decltype(std::move(rev).end()), std::reverse_iterator<bidirectional_iterator<int*>>);
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_reverse_end; // libcis


void test_ranges_range_adaptors_range_reverse_end() {
  test();
  static_assert(test());

  return;
}
