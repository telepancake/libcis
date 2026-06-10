// AST-transferred from libc++ by tools/transfer.py (slug=iterators_predef_iterators_move_iterators_sized_sentinel).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <cstddef>
#include <iterator>

#include "test_iterators.h"

namespace libcis_ns_iterators_predef_iterators_move_iterators_sized_sentinel { // libcis: isolate file-scope helpers
using sized_it = random_access_iterator<int*>;
static_assert(std::sized_sentinel_for<sized_it, sized_it>);
static_assert(std::sized_sentinel_for<std::move_iterator<sized_it>, std::move_iterator<sized_it>>);

struct unsized_it {
  using value_type      = int;
  using difference_type = std::ptrdiff_t;

  value_type& operator*() const;
  unsized_it& operator++();
  bool operator==(const unsized_it&) const;
  difference_type operator-(const unsized_it&) const { return 0; }
};
} using namespace libcis_ns_iterators_predef_iterators_move_iterators_sized_sentinel; // libcis


template <>
inline constexpr bool std::disable_sized_sentinel_for<unsized_it, unsized_it> = true;

namespace libcis_ns_iterators_predef_iterators_move_iterators_sized_sentinel { // libcis: isolate file-scope helpers
static_assert(!std::sized_sentinel_for<unsized_it, unsized_it>);
static_assert(!std::sized_sentinel_for<std::move_iterator<unsized_it>, std::move_iterator<unsized_it>>);
} using namespace libcis_ns_iterators_predef_iterators_move_iterators_sized_sentinel; // libcis

