// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_transform_iterator_requirements).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// The requirements for transform_view::<iterator>'s members.

#include <ranges>

#include "test_macros.h"
#include "../types.h"

namespace libcis_ns_ranges_range_adaptors_range_transform_iterator_requirements { // libcis: isolate file-scope helpers
static_assert(std::ranges::bidirectional_range<std::ranges::transform_view<BidirectionalView, PlusOne>>);
static_assert(!std::ranges::bidirectional_range<std::ranges::transform_view<ForwardView, PlusOne>>);

static_assert(std::ranges::random_access_range<std::ranges::transform_view<RandomAccessView, PlusOne>>);
static_assert(!std::ranges::random_access_range<std::ranges::transform_view<BidirectionalView, PlusOne>>);
} using namespace libcis_ns_ranges_range_adaptors_range_transform_iterator_requirements; // libcis

