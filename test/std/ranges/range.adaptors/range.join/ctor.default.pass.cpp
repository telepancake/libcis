// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_join_ctor_default).
// main -> test_ranges_range_adaptors_range_join_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// join_view() requires default_initializable<V> = default;

#include <cassert>
#include <ranges>

#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_join_ctor_default { // libcis: isolate file-scope helpers
struct DefaultView : std::ranges::view_base {
  int i; // deliberately uninitialised

  ChildView* begin() const;
  ChildView* end() const;
};

constexpr bool test() {
  {
    std::ranges::join_view<ParentView<ChildView>> jv;
    assert(std::move(jv).base().ptr_ == globalChildren);
  }

  // Default constructor should value initialise underlying view
  {
    std::ranges::join_view<DefaultView> jv;
    assert(jv.base().i == 0);
  }

  static_assert( std::default_initializable<std::ranges::join_view<ParentView<ChildView>>>);
  static_assert(!std::default_initializable<std::ranges::join_view<CopyableParent>>);

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_join_ctor_default; // libcis


void test_ranges_range_adaptors_range_join_ctor_default() {
  test();
  static_assert(test());

  return;
}
