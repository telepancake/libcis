// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_all_range_owning_view_implicit_ctad_26624343).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// owning_view

// Make sure that the implicitly-generated CTAD works.

#include <ranges>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_ranges_range_adaptors_range_all_range_owning_view_implicit_ctad_26624343 { // libcis
struct Range {
  int* begin();
  int* end();
};

int main(int, char**) {
  Range r;
  std::ranges::owning_view view{std::move(r)};
  ASSERT_SAME_TYPE(decltype(view), std::ranges::owning_view<Range>);

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_all_range_owning_view_implicit_ctad_26624343 (libcis)

