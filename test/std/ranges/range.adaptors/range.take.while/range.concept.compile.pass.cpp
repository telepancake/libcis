// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_take_while_range_concept).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// concept checking
// template<view V, class Pred>
//     requires input_range<V> && is_object_v<Pred> &&
//              indirect_unary_predicate<const Pred, iterator_t<V>>
//   class take_while_view;

#include <array>
#include <ranges>

#include "test_iterators.h"

namespace libcis_ns_ranges_range_adaptors_range_take_while_range_concept { // libcis: isolate file-scope helpers
template <class It>
using Range = std::ranges::subrange<It, sentinel_wrapper<It>>;

template <class Val = int>
struct Pred {
  bool operator()(const Val&) const;
};

template <class V, class Pred>
concept HasTakeWhileView = requires { typename std::ranges::take_while_view<V, Pred>; };

static_assert(HasTakeWhileView<Range<int*>, bool (*)(int)>);
static_assert(HasTakeWhileView<Range<int*>, Pred<int>>);

// !view<V>
static_assert(!HasTakeWhileView<std::array<int, 5>, Pred<int>>);

// !input_range
static_assert(!HasTakeWhileView<Range<cpp20_output_iterator<int*>>, bool (*)(int)>);

// !is_object_v<Pred>
static_assert(!HasTakeWhileView<Range<int*>, Pred<int>&>);

// !indirect_unary_predicate<const Pred, iterator_t<V>>
static_assert(!HasTakeWhileView<Range<int*>, int>);
static_assert(!HasTakeWhileView<Range<int**>, Pred<int>>);
} using namespace libcis_ns_ranges_range_adaptors_range_take_while_range_concept; // libcis

