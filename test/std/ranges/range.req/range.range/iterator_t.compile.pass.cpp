// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_req_range_range_iterator_t).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
// using iterator_t = decltype(ranges::begin(declval<T&>()));

#include <ranges>

#include <concepts>

#include "test_range.h"



namespace libcis_ns_ranges_range_req_range_range_iterator_t { // libcis: isolate file-scope helpers
static_assert(std::same_as<std::ranges::iterator_t<test_range<cpp17_input_iterator> >, cpp17_input_iterator<int*> >);
static_assert(std::same_as<std::ranges::iterator_t<test_range<cpp17_input_iterator> const>, cpp17_input_iterator<int const*> >);

static_assert(std::same_as<std::ranges::iterator_t<test_non_const_range<cpp17_input_iterator> >, cpp17_input_iterator<int*> >);

static_assert(std::same_as<std::ranges::iterator_t<test_common_range<cpp17_input_iterator> >, cpp17_input_iterator<int*> >);
static_assert(std::same_as<std::ranges::iterator_t<test_common_range<cpp17_input_iterator> const>, cpp17_input_iterator<int const*> >);

static_assert(std::same_as<std::ranges::iterator_t<test_non_const_common_range<cpp17_input_iterator> >, cpp17_input_iterator<int*> >);
} using namespace libcis_ns_ranges_range_req_range_range_iterator_t; // libcis

