// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_set_unord_set_cnstr_move_addressof_20acf320).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// unordered_set(unordered_set&& u);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <unordered_set>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_set_unord_set_cnstr_move_addressof_20acf320 { // libcis
void test() {
  std::unordered_set<operator_hijacker> so;
  std::unordered_set<operator_hijacker> s(std::move(so));
}
} // libcis_ns_containers_unord_unord_set_unord_set_cnstr_move_addressof_20acf320 (libcis)

