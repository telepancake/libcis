// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_all_range_owning_view_data).
// main -> test_ranges_range_adaptors_range_all_range_owning_view_data; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr auto data() requires contiguous_range<R>
// constexpr auto data() const requires contiguous_range<const R>

#include <ranges>

#include <array>
#include <cassert>
#include <concepts>
#include <utility>

#include "test_iterators.h"
#include "test_macros.h"

namespace libcis_ns_ranges_range_adaptors_range_all_range_owning_view_data { // libcis: isolate file-scope helpers
template <class T>
concept HasData = requires (T t) {
  t.data();
};

constexpr bool test()
{
  {
    struct ContiguousIters {
      contiguous_iterator<int*> begin();
      sentinel_wrapper<contiguous_iterator<int*>> end();
    };
    using OwningView = std::ranges::owning_view<ContiguousIters>;
    static_assert(std::ranges::contiguous_range<OwningView&>);
    static_assert(!std::ranges::range<const OwningView&>); // no begin/end
    static_assert(HasData<OwningView&>);
    static_assert(HasData<OwningView&&>);
    static_assert(!HasData<const OwningView&>);
    static_assert(!HasData<const OwningView&&>);
  }
  {
    struct NoData {
      random_access_iterator<int*> begin();
      random_access_iterator<int*> end();
    };
    using OwningView = std::ranges::owning_view<NoData>;
    static_assert(!HasData<OwningView&>);
    static_assert(!HasData<OwningView&&>);
    static_assert(!HasData<const OwningView&>);
    static_assert(!HasData<const OwningView&&>);
  }
  {
    // Test a view.
    int a[] = {1};
    auto ov = std::ranges::owning_view(std::ranges::subrange(a, a+1));
    assert(ov.data() == a);
    assert(std::as_const(ov).data() == a);
  }
  {
    // Test a non-view.
    std::array<int, 2> a = {1, 2};
    auto ov = std::ranges::owning_view(std::move(a));
    assert(ov.data() != a.data()); // because it points into the copy
    assert(std::as_const(ov).data() != a.data());
  }
  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_all_range_owning_view_data; // libcis


void test_ranges_range_adaptors_range_all_range_owning_view_data() {
  test();
  static_assert(test());

  return;
}
