// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_split_constraints).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<forward_range V, forward_range Pattern>
//   requires view<V> && view<Pattern> &&
//            indirectly_comparable<iterator_t<V>, iterator_t<Pattern>, ranges::equal_to>
// class split_view;

#include <ranges>

#include "test_macros.h"
#include "test_iterators.h"

namespace libcis_ns_ranges_range_adaptors_range_split_constraints { // libcis: isolate file-scope helpers
template <class It>
using Range = std::ranges::subrange<It, sentinel_wrapper<It>>;

template <class View, class Pattern>
concept HasSplitView = requires { typename std::ranges::split_view<View, Pattern>; };

static_assert(HasSplitView<Range<int*>, Range<int*>>);

// !forward_range<V>
static_assert(!HasSplitView<Range<cpp20_input_iterator<int*>>, Range<int*>>);

// !forward_range<Pattern>
static_assert(!HasSplitView<Range<int*>, Range<cpp20_input_iterator<int*>>>);

struct NotAView {
  int* begin() const;
  int* end() const;
};

// !view<V>
static_assert(!HasSplitView<NotAView, Range<int*>>);

// !view<Pattern>
static_assert(!HasSplitView<Range<int*>, NotAView>);

// indirectly_comparable<iterator_t<V>, iterator_t<Pattern>, ranges::equal_to
struct Foo {};
static_assert(!HasSplitView<Range<int*>, Range<Foo*>>);
} using namespace libcis_ns_ranges_range_adaptors_range_split_constraints; // libcis

