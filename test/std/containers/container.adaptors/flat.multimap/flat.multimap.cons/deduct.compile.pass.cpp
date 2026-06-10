// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_flat_multimap_flat_multimap_cons_deduct).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_map>

// Test CTAD on cases where deduction should fail.

#include <flat_map>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace libcis_ns_containers_container_adaptors_flat_multimap_flat_multimap_cons_deduct { // libcis: isolate file-scope helpers
struct NotAnAllocator {
  friend bool operator<(NotAnAllocator, NotAnAllocator) { return false; }
};

using P  = std::pair<int, long>;
using PC = std::pair<const int, long>;

template <class... Args>
concept CanDeductFlatMultimap = requires { std::flat_multimap{std::declval<Args>()...}; };

static_assert(CanDeductFlatMultimap<std::vector<int>, std::vector<int>>);

// cannot deduce Key and T from nothing
static_assert(!CanDeductFlatMultimap<>);

// cannot deduce Key and T from just (KeyContainer), even if it's a container of pairs
static_assert(!CanDeductFlatMultimap<std::vector<std::pair<int, int>>>);

// cannot deduce Key and T from just (KeyContainer, Allocator)
static_assert(!CanDeductFlatMultimap<std::vector<int>, std::allocator<std::pair<const int, int>>>);

// cannot deduce Key and T from just (Compare)
static_assert(!CanDeductFlatMultimap<std::less<int>>);

// cannot deduce Key and T from just (Compare, Allocator)
static_assert(!CanDeductFlatMultimap<std::less<int>, std::allocator<PC>>);

// cannot deduce Key and T from just (Allocator)
static_assert(!CanDeductFlatMultimap<std::allocator<PC>>);

// cannot convert from some arbitrary unrelated type
static_assert(!CanDeductFlatMultimap<NotAnAllocator>);
} using namespace libcis_ns_containers_container_adaptors_flat_multimap_flat_multimap_cons_deduct; // libcis

