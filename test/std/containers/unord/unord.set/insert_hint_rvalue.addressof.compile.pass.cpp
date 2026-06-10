// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_set_insert_hint_rvalue_addressof).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
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

// iterator insert(const_iterator p, value_type&& x);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <unordered_set>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_set_insert_hint_rvalue_addressof { // libcis: isolate file-scope helpers
void test() {
  std::unordered_set<operator_hijacker> s;
  s.insert(s.cbegin(), operator_hijacker());
}
} using namespace libcis_ns_containers_unord_unord_set_insert_hint_rvalue_addressof; // libcis

