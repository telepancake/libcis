// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_factories_range_single_view_range_concept_conformance_238fced5).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// Test that single_view conforms to range and view concepts.

#include <ranges>

#include <cassert>
#include <concepts>

#include "test_iterators.h"

namespace libcis_ns_ranges_range_factories_range_single_view_range_concept_conformance_238fced5 { // libcis
struct Empty {};

static_assert(std::ranges::contiguous_range<std::ranges::single_view<Empty>>);
static_assert(std::ranges::contiguous_range<const std::ranges::single_view<Empty>>);
static_assert(std::ranges::view<std::ranges::single_view<Empty>>);
static_assert(std::ranges::view<std::ranges::single_view<const Empty>>);
static_assert(std::ranges::contiguous_range<const std::ranges::single_view<const Empty>>);
static_assert(std::ranges::view<std::ranges::single_view<int>>);
static_assert(std::ranges::view<std::ranges::single_view<const int>>);
static_assert(std::ranges::contiguous_range<const std::ranges::single_view<const int>>);
} // libcis_ns_ranges_range_factories_range_single_view_range_concept_conformance_238fced5 (libcis)

