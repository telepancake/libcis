// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_elements_borrowed).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
//
// template<class T, size_t N>
//   inline constexpr bool enable_borrowed_range<elements_view<T, N>> =
//     enable_borrowed_range<T>;

#include <ranges>
#include <tuple>

namespace libcis_ns_ranges_range_adaptors_range_elements_borrowed { // libcis: isolate file-scope helpers
struct NonBorrowed : std::ranges::view_base {
  std::tuple<int>* begin();
  std::tuple<int>* end();
};

struct Borrowed : std::ranges::view_base {
  std::tuple<int>* begin();
  std::tuple<int>* end();
};
} using namespace libcis_ns_ranges_range_adaptors_range_elements_borrowed; // libcis


template <>
inline constexpr bool std::ranges::enable_borrowed_range<Borrowed> = true;

namespace libcis_ns_ranges_range_adaptors_range_elements_borrowed { // libcis: isolate file-scope helpers
static_assert(!std::ranges::borrowed_range<std::ranges::elements_view<NonBorrowed, 0>>);
static_assert(std::ranges::borrowed_range<std::ranges::elements_view<Borrowed, 0>>);
} using namespace libcis_ns_ranges_range_adaptors_range_elements_borrowed; // libcis

