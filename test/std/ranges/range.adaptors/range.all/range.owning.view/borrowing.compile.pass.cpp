// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_all_range_owning_view_borrowing_b81879e3).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
//   inline constexpr bool enable_borrowed_range<owning_view<T>> = enable_borrowed_range<T>;

#include <ranges>

#include "test_range.h"

namespace libcis_ns_ranges_range_adaptors_range_all_range_owning_view_borrowing_b81879e3 { // libcis
static_assert( std::ranges::borrowed_range<std::ranges::owning_view<BorrowedView>>);
static_assert(!std::ranges::borrowed_range<std::ranges::owning_view<NonBorrowedView>>);
} // libcis_ns_ranges_range_adaptors_range_all_range_owning_view_borrowing_b81879e3 (libcis)

