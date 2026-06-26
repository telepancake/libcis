// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_split_sentinel_ctor_default_59157935).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// sentinel() = default;

#include <cassert>
#include <ranges>

namespace libcis_ns_ranges_range_adaptors_range_split_sentinel_ctor_default_59157935 { // libcis
struct PODSentinel {
  int i;

  friend constexpr bool operator==(int*, const PODSentinel& p) { return p.i == 0; }
};

struct Range : std::ranges::view_base {
  int* begin() const;
  PODSentinel end();
};

constexpr bool test() {
  using SplitView = std::ranges::split_view<Range, Range>;
  using SplitIter = std::ranges::iterator_t<SplitView>;
  using SplitSent = std::ranges::sentinel_t<SplitView>;
  static_assert(!std::is_same_v<SplitSent, SplitIter>);

  {
    SplitIter it;
    SplitSent s;
    assert(s == it); // to make sure that s.__end_.i is initialised to 0;
  }

  {
    SplitIter it;
    SplitSent s = {};
    assert(s == it); // to make sure that s.__end_.i is initialised to 0;
  }
  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_split_sentinel_ctor_default_59157935 (libcis)

