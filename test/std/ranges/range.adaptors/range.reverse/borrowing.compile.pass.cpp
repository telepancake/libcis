// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_reverse_borrowing).
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
//   inline constexpr bool enable_borrowed_range<reverse_view<T>> = enable_borrowed_range<T>;

#include <ranges>

#include "test_range.h"

namespace libcis_ns_ranges_range_adaptors_range_reverse_borrowing { // libcis: isolate file-scope helpers
static_assert( std::ranges::borrowed_range<std::ranges::reverse_view<BorrowedView>>);
static_assert(!std::ranges::borrowed_range<std::ranges::reverse_view<NonBorrowedView>>);
} using namespace libcis_ns_ranges_range_adaptors_range_reverse_borrowing; // libcis

