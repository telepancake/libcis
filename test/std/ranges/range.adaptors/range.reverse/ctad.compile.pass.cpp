// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_reverse_ctad).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class R>
//   reverse_view(R&&) -> reverse_view<views::all_t<R>>;

#include <ranges>

#include <concepts>
#include <utility>

#include "test_iterators.h"

namespace libcis_ns_ranges_range_adaptors_range_reverse_ctad { // libcis: isolate file-scope helpers
struct View : std::ranges::view_base {
  int *begin() const;
  int *end() const;
};

struct Range {
  int *begin() const;
  int *end() const;
};

struct BorrowedRange {
  int *begin() const;
  int *end() const;
};
} using namespace libcis_ns_ranges_range_adaptors_range_reverse_ctad; // libcis

template<>
inline constexpr bool std::ranges::enable_borrowed_range<BorrowedRange> = true;

namespace libcis_ns_ranges_range_adaptors_range_reverse_ctad { // libcis: isolate file-scope helpers
void testCTAD() {
    View v;
    Range r;
    BorrowedRange br;

    static_assert(std::same_as<
        decltype(std::ranges::reverse_view(v)),
        std::ranges::reverse_view<View>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::reverse_view(std::move(v))),
        std::ranges::reverse_view<View>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::reverse_view(r)),
        std::ranges::reverse_view<std::ranges::ref_view<Range>>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::reverse_view(std::move(r))),
        std::ranges::reverse_view<std::ranges::owning_view<Range>>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::reverse_view(br)),
        std::ranges::reverse_view<std::ranges::ref_view<BorrowedRange>>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::reverse_view(std::move(br))),
        std::ranges::reverse_view<std::ranges::owning_view<BorrowedRange>>
    >);
}
} using namespace libcis_ns_ranges_range_adaptors_range_reverse_ctad; // libcis

