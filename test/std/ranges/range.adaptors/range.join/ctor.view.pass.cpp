// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_join_ctor_view_8bd8865e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr explicit join_view(V base);

#include <cassert>
#include <ranges>

#include "test_macros.h"
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_join_ctor_view_8bd8865e { // libcis
constexpr bool test() {
  int buffer[4][4] = {{1111, 2222, 3333, 4444}, {555, 666, 777, 888}, {99, 1010, 1111, 1212}, {13, 14, 15, 16}};

  {
    ChildView children[4] = {ChildView(buffer[0]), ChildView(buffer[1]), ChildView(buffer[2]), ChildView(buffer[3])};
    auto jv = std::ranges::join_view(ParentView{children});
    assert(std::move(jv).base().ptr_ == children);
  }

  {
    std::ranges::join_view jv(buffer);
    assert(jv.base().base() == buffer + 0);
  }

  {
    // Test explicitness.
    static_assert( std::is_constructible_v<std::ranges::join_view<ParentView<ChildView>>, ParentView<ChildView>>);
    static_assert(!std::is_convertible_v<std::ranges::join_view<ParentView<ChildView>>, ParentView<ChildView>>);
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_join_ctor_view_8bd8865e (libcis)

