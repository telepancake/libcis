// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_req_range_range_range_size_t).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<sized_range R>
// using range_size_t = decltype(ranges::size(declval<R&>()));

#include <ranges>
#include <concepts>
#include <cstddef>

#include "test_iterators.h"

namespace libcis_ns_ranges_range_req_range_range_range_size_t { // libcis: isolate file-scope helpers
template<class T>
concept has_range_size_t = requires { typename std::ranges::range_size_t<T>; };

struct A { int *begin(); int *end(); short size(); };
static_assert(std::same_as<std::ranges::range_size_t<A>, short>);
static_assert(std::same_as<std::ranges::range_size_t<A&>, short>);
static_assert(std::same_as<std::ranges::range_size_t<A&&>, short>);
static_assert(!has_range_size_t<const A>);
static_assert(!has_range_size_t<const A&>);
static_assert(!has_range_size_t<const A&&>);

struct B { int *begin(); int *end(); };
static_assert(std::same_as<std::ranges::range_size_t<B>, std::size_t>);
static_assert(std::same_as<std::ranges::range_size_t<B&>, std::size_t>);
static_assert(std::same_as<std::ranges::range_size_t<B&&>, std::size_t>);
static_assert(!has_range_size_t<const B>);
static_assert(!has_range_size_t<const B&>);
static_assert(!has_range_size_t<const B&&>);

struct C { bidirectional_iterator<int*> begin(); bidirectional_iterator<int*> end(); };
static_assert(!has_range_size_t<C>);
} using namespace libcis_ns_ranges_range_req_range_range_range_size_t; // libcis

