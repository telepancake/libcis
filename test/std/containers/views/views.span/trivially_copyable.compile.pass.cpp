// transferred+adapted from libc++ by tools/transfer.py (slug=containers_views_views_span_trivially_copyable_58496a25).
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// P2251 was voted into C++23, but is supported even in C++20 mode by all vendors.

// <span>

#include <span>
#include <type_traits>

namespace libcis_ns_containers_views_views_span_trivially_copyable_58496a25 { // libcis
static_assert(std::is_trivially_copyable_v<std::span<int>>);
static_assert(std::is_trivially_copyable_v<std::span<int, 3>>);
} // libcis_ns_containers_views_views_span_trivially_copyable_58496a25 (libcis)

