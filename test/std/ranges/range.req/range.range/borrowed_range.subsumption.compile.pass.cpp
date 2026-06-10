// AST-transferred from libc++ by tools/transfer.py (slug=ranges_range_req_range_range_borrowed_range_subsumption).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
// concept borrowed_range;

#include <ranges>

namespace libcis_ns_ranges_range_req_range_range_borrowed_range_subsumption { // libcis: isolate file-scope helpers
template <std::ranges::range R>
consteval bool check_subsumption() {
  return false;
}

template <std::ranges::borrowed_range R>
consteval bool check_subsumption() {
  return true;
}

static_assert(check_subsumption<int (&)[8]>());
} using namespace libcis_ns_ranges_range_req_range_range_borrowed_range_subsumption; // libcis

