// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_utility_range_subrange_borrowing).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class I, class S, subrange_kind K>
//   inline constexpr bool enable_borrowed_range<subrange<I, S, K>> = true;

#include <ranges>

namespace libcis_ns_ranges_range_utility_range_subrange_borrowing { // libcis: isolate file-scope helpers
static_assert(std::ranges::borrowed_range<std::ranges::subrange<int*, const int*, std::ranges::subrange_kind::sized>>);
static_assert(std::ranges::borrowed_range<std::ranges::subrange<int*, std::unreachable_sentinel_t, std::ranges::subrange_kind::sized>>);
static_assert(std::ranges::borrowed_range<std::ranges::subrange<int*, std::unreachable_sentinel_t, std::ranges::subrange_kind::unsized>>);
} using namespace libcis_ns_ranges_range_utility_range_subrange_borrowing; // libcis

