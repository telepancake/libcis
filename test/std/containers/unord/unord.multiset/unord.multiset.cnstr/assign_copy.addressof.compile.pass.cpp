// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multiset_unord_multiset_cnstr_assign_copy_addressof_1603633e).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// unordered_multiset& operator=(const unordered_multiset& u);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <unordered_set>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_multiset_unord_multiset_cnstr_assign_copy_addressof_1603633e { // libcis
void test() {
  std::unordered_multiset<operator_hijacker> so;
  std::unordered_multiset<operator_hijacker> s;
  s = so;
}
} // libcis_ns_containers_unord_unord_multiset_unord_multiset_cnstr_assign_copy_addressof_1603633e (libcis)

