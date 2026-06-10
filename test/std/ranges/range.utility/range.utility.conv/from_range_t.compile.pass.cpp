// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_utility_range_utility_conv_from_range_t).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// struct from_range_t { explicit from_range_t() = default; };  // Since C++23
// inline constexpr from_range_t from_range{};                  // Since C++23

#include <ranges>

namespace libcis_ns_ranges_range_utility_range_utility_conv_from_range_t { // libcis: isolate file-scope helpers
template <class T>
void check(std::from_range_t);

template <class T>
concept IsCtrNonexplicit = requires {
  check<T>({});
};

// Verify that the constructor is `explicit`.
static_assert(!IsCtrNonexplicit<std::from_range_t>);
} using namespace libcis_ns_ranges_range_utility_range_utility_conv_from_range_t; // libcis

