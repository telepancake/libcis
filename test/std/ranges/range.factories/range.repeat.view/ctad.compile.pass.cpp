// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_factories_range_repeat_view_ctad).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// template<class T, class Bound>
//    repeat_view(T, Bound) -> repeat_view<T, Bound>;

#include <concepts>
#include <ranges>
#include <utility>

namespace libcis_ns_ranges_range_factories_range_repeat_view_ctad { // libcis: isolate file-scope helpers
struct Empty {};

// clang-format off
static_assert(std::same_as<decltype(std::ranges::repeat_view(Empty())), std::ranges::repeat_view<Empty>>);
static_assert(std::same_as<decltype(std::ranges::repeat_view(std::declval<Empty&>())), std::ranges::repeat_view<Empty>>);
static_assert(std::same_as<decltype(std::ranges::repeat_view(std::declval<Empty&&>())), std::ranges::repeat_view<Empty>>);
static_assert(std::same_as<decltype(std::ranges::repeat_view(10, 1)), std::ranges::repeat_view<int, int>>);
static_assert(std::same_as<decltype(std::ranges::repeat_view(10, 1U)), std::ranges::repeat_view<int, unsigned>>);
static_assert(std::same_as<decltype(std::ranges::repeat_view(10, 1UL)), std::ranges::repeat_view<int, unsigned long>>);

using RPV = std::ranges::repeat_view<const char*>;
static_assert(std::same_as<decltype(std::ranges::repeat_view("foo", std::unreachable_sentinel)), RPV>);  // OK
static_assert(std::same_as<decltype(std::ranges::repeat_view(+"foo", std::unreachable_sentinel)), RPV>); // OK
static_assert(std::same_as<decltype(std::ranges::repeat_view("foo")), RPV>);                             // OK since LWG4053
static_assert(std::same_as<decltype(std::ranges::repeat_view(+"foo")), RPV>);
} using namespace libcis_ns_ranges_range_factories_range_repeat_view_ctad; // libcis
                            // OK
// clang-format on
