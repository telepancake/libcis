// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_map_unord_map_swap_swap_addressof_2ad58f32).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <unordered_map>

// void swap(unordered_map& c)
//     noexcept(allocator_traits<Allocator>::is_always_equal::value &&
//               noexcept(swap(declval<Hash&>(), declval<Hash&>())) &&
//               noexcept(swap(declval<Pred&>(), declval<Pred&>())));

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <unordered_map>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_map_unord_map_swap_swap_addressof_2ad58f32 { // libcis
void test() {
  std::unordered_map<operator_hijacker, operator_hijacker> m1;
  std::unordered_map<operator_hijacker, operator_hijacker> m2;
  std::swap(m1, m2);
}
} // libcis_ns_containers_unord_unord_map_unord_map_swap_swap_addressof_2ad58f32 (libcis)

