// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_common_view_ctor_view_47c5a23a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit common_view(V r);

#include <ranges>

#include <cassert>
#include <utility>

#include "test_iterators.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_common_view_ctor_view_47c5a23a { // libcis
constexpr bool test() {
  int buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};

  {
    MoveOnlyView view{buf, buf + 8};
    std::ranges::common_view<MoveOnlyView> common(std::move(view));
    assert(std::move(common).base().begin_ == buf);
  }

  {
    CopyableView const view{buf, buf + 8};
    std::ranges::common_view<CopyableView> const common(view);
    assert(common.base().begin_ == buf);
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  // Can't compare common_iterator inside constexpr
  {
    int buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    MoveOnlyView view{buf, buf + 8};
    std::ranges::common_view<MoveOnlyView> const common(std::move(view));
    assert(common.begin() == buf);
  }

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_common_view_ctor_view_47c5a23a (libcis)

