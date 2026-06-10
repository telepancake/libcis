// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_base).
// main -> test_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_base; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// constexpr const iterator_t<Base>& inner-iterator::base() const& noexcept;
//
// constexpr iterator_t<Base> inner-iterator::base() &&
//   requires forward_range<View>;

#include <ranges>

#include <cassert>
#include <concepts>
#include <utility>
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_base { // libcis: isolate file-scope helpers
static_assert( noexcept(std::declval<InnerIterForward&>().base()));
static_assert( noexcept(std::declval<InnerIterForward const &>().base()));
static_assert( noexcept(std::declval<InnerIterForward const &&>().base()));
static_assert( noexcept(std::declval<InnerIterInput&>().base()));
static_assert( noexcept(std::declval<InnerIterInput const &>().base()));
static_assert( noexcept(std::declval<InnerIterInput const &&>().base()));

constexpr bool test() {
  // `base` works with a forward view (two different overloads based on ref-qualification of the `inner-iterator`).
  {
    using BaseIter = std::ranges::iterator_t<CopyableView>;
    CopyableView input("abc def");
    std::ranges::lazy_split_view<CopyableView, ForwardView> v(input, " ");
    auto i = (*v.begin()).begin();
    const auto ci = i;

    // Note: some macOS platforms seem to have trouble deducing the type when using `std::same_as` -- use the equivalent
    // `ASSERT_SAME_TYPE` instead.
    {
      decltype(auto) b = i.base();
      ASSERT_SAME_TYPE(decltype(b), const BaseIter&);
      assert(b == input.begin());
    }

    {
      decltype(auto) b = ci.base();
      ASSERT_SAME_TYPE(decltype(b), const BaseIter&);
      assert(b == input.begin());
    }

    {
      decltype(auto) b = std::move(i).base();
      ASSERT_SAME_TYPE(decltype(b), BaseIter);
      assert(b == input.begin());
    }

    {
      decltype(auto) b = std::move(ci).base();
      ASSERT_SAME_TYPE(decltype(b), const BaseIter&);
      assert(b == input.begin());
    }
  }

  // `base` works with an input view (no overloads).
  {
    using BaseIter = std::ranges::iterator_t<InputView>;
    InputView input("abc def");
    std::ranges::lazy_split_view<InputView, ForwardTinyView> v(input, ' ');
    auto i = (*v.begin()).begin();
    const auto ci = i;

    {
      decltype(auto) b = i.base();
      ASSERT_SAME_TYPE(decltype(b), const BaseIter&);
    }

    {
      decltype(auto) b = ci.base();
      ASSERT_SAME_TYPE(decltype(b), const BaseIter&);
    }

    {
      decltype(auto) b = std::move(i).base();
      ASSERT_SAME_TYPE(decltype(b), const BaseIter&);
    }

    {
      decltype(auto) b = std::move(ci).base();
      ASSERT_SAME_TYPE(decltype(b), const BaseIter&);
    }
  }

  return true;
}
} using namespace libcis_ns_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_base; // libcis


void test_ranges_range_adaptors_range_lazy_split_range_lazy_split_inner_base() {
  test();
  static_assert(test());

  return;
}
