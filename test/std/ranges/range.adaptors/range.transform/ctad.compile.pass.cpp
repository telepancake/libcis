// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_transform_ctad).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class R, class F>
//   transform_view(R&&, F) -> transform_view<views::all_t<R>, F>;

#include <cassert>
#include <concepts>
#include <ranges>
#include <utility>

namespace libcis_ns_ranges_range_adaptors_range_transform_ctad { // libcis: isolate file-scope helpers
struct PlusOne {
    int operator()(int x) const;
};

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
} using namespace libcis_ns_ranges_range_adaptors_range_transform_ctad; // libcis

template<>
inline constexpr bool std::ranges::enable_borrowed_range<BorrowedRange> = true;

namespace libcis_ns_ranges_range_adaptors_range_transform_ctad { // libcis: isolate file-scope helpers
void testCTAD() {
    View v;
    Range r;
    BorrowedRange br;
    PlusOne f;

    static_assert(std::same_as<
        decltype(std::ranges::transform_view(v, f)),
        std::ranges::transform_view<View, PlusOne>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::transform_view(std::move(v), f)),
        std::ranges::transform_view<View, PlusOne>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::transform_view(r, f)),
        std::ranges::transform_view<std::ranges::ref_view<Range>, PlusOne>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::transform_view(std::move(r), f)),
        std::ranges::transform_view<std::ranges::owning_view<Range>, PlusOne>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::transform_view(br, f)),
        std::ranges::transform_view<std::ranges::ref_view<BorrowedRange>, PlusOne>
    >);
    static_assert(std::same_as<
        decltype(std::ranges::transform_view(std::move(br), f)),
        std::ranges::transform_view<std::ranges::owning_view<BorrowedRange>, PlusOne>
    >);
}
} using namespace libcis_ns_ranges_range_adaptors_range_transform_ctad; // libcis

