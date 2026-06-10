// AST-transferred from libc++ by tools/transfer.py (slug=containers_views_views_span_trivially_copyable).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
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

namespace libcis_ns_containers_views_views_span_trivially_copyable { // libcis: isolate file-scope helpers
static_assert(std::is_trivially_copyable_v<std::span<int>>);
static_assert(std::is_trivially_copyable_v<std::span<int, 3>>);
} using namespace libcis_ns_containers_views_views_span_trivially_copyable; // libcis

