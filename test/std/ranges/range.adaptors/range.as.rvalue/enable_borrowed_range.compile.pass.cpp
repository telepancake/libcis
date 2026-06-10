// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_adaptors_range_as_rvalue_enable_borrowed_range).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <ranges>
#include <vector>

namespace libcis_ns_ranges_range_adaptors_range_as_rvalue_enable_borrowed_range { // libcis: isolate file-scope helpers
static_assert(std::ranges::enable_borrowed_range<std::ranges::as_rvalue_view<std::ranges::empty_view<int>>>);
static_assert(std::ranges::enable_borrowed_range<std::ranges::as_rvalue_view<std::views::all_t<std::vector<int>&>>>);
static_assert(!std::ranges::enable_borrowed_range<std::ranges::as_rvalue_view<std::views::all_t<std::vector<int>>>>);
} using namespace libcis_ns_ranges_range_adaptors_range_as_rvalue_enable_borrowed_range; // libcis

