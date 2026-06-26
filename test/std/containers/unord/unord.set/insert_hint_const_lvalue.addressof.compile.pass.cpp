// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_set_insert_hint_const_lvalue_addressof_4c2e2b00).
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
// class unordered_set

// iterator insert(const_iterator p, const value_type& x);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <unordered_set>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_set_insert_hint_const_lvalue_addressof_4c2e2b00 { // libcis
void test() {
  std::unordered_set<operator_hijacker> s;
  const operator_hijacker v;
  s.insert(s.cbegin(), v);
}
} // libcis_ns_containers_unord_unord_set_insert_hint_const_lvalue_addressof_4c2e2b00 (libcis)

