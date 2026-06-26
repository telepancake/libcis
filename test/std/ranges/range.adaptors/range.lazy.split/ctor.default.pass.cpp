// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_ctor_default_a1d347b2).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

//  lazy_split_view() requires default_initializable<V> && default_initializable<P> = default;

#include <ranges>

#include <cassert>
#include "types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_ctor_default_a1d347b2 { // libcis
struct ThrowingDefaultCtorForwardView : std::ranges::view_base {
  ThrowingDefaultCtorForwardView() noexcept(false);
  forward_iterator<int*> begin() const;
  forward_iterator<int*> end() const;
};

struct NoDefaultCtorForwardView : std::ranges::view_base {
  NoDefaultCtorForwardView() = delete;
  forward_iterator<int*> begin() const;
  forward_iterator<int*> end() const;
};

static_assert( std::is_default_constructible_v<std::ranges::lazy_split_view<ForwardView, ForwardView>>);
static_assert(!std::is_default_constructible_v<std::ranges::lazy_split_view<NoDefaultCtorForwardView, ForwardView>>);
static_assert(!std::is_default_constructible_v<std::ranges::lazy_split_view<ForwardView, NoDefaultCtorForwardView>>);

static_assert( std::is_nothrow_default_constructible_v<std::ranges::lazy_split_view<ForwardView, ForwardView>>);
static_assert(!std::is_nothrow_default_constructible_v<ThrowingDefaultCtorForwardView>);
static_assert(!std::is_nothrow_default_constructible_v<
    std::ranges::lazy_split_view<ThrowingDefaultCtorForwardView, ForwardView>>);

constexpr bool test() {
  {
    std::ranges::lazy_split_view<CopyableView, ForwardView> v;
    assert(v.base() == CopyableView());
  }

  {
    std::ranges::lazy_split_view<CopyableView, ForwardView> v = {};
    assert(v.base() == CopyableView());
  }

  return true;
}

int main(int, char**) {
  test();
  static_assert(test());

  return 0;

    return 0;
}
} // libcis_ns_ranges_range_adaptors_range_lazy_split_ctor_default_a1d347b2 (libcis)

