// transferred+adapted from libc++ by tools/transfer.py (slug=ranges_range_req_range_sized_subsumption_37d5f942).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// template<class T>
// concept sized_range;

#include <ranges>

namespace libcis_ns_ranges_range_req_range_sized_subsumption_37d5f942 { // libcis
template <std::ranges::range R>
consteval bool check_subsumption() {
  return false;
}

template <std::ranges::sized_range R>
consteval bool check_subsumption() {
  return true;
}

static_assert(check_subsumption<int[5]>());
} // libcis_ns_ranges_range_req_range_sized_subsumption_37d5f942 (libcis)

