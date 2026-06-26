// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_empty_borrowing_81f8b747).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
//   inline constexpr bool enable_borrowed_range<empty_view<T>> = true;

#include <ranges>

#include "test_range.h"

namespace libcis_ns_ranges_range_adaptors_range_empty_borrowing_81f8b747 { // libcis
static_assert(std::ranges::borrowed_range<std::ranges::empty_view<int>>);
static_assert(std::ranges::borrowed_range<std::ranges::empty_view<int*>>);
static_assert(std::ranges::borrowed_range<std::ranges::empty_view<BorrowedView>>);
static_assert(std::ranges::borrowed_range<std::ranges::empty_view<NonBorrowedView>>);
} // libcis_ns_ranges_range_adaptors_range_empty_borrowing_81f8b747 (libcis)

