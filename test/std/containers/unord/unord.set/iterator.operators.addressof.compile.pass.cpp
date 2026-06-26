// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_set_iterator_operators_addressof_51ee0b50).
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

#include <unordered_set>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_set_iterator_operators_addressof_51ee0b50 { // libcis
template <class ToIterator, class FromIterator>
void test() {
  FromIterator from;
  ToIterator copy(from);
  copy = from;

  ToIterator move(std::move(from));
  from = FromIterator();
  move = std::move(from);
}

void test() {
  {
    using I = std::unordered_set<operator_hijacker>::iterator;
    using CI = std::unordered_set<operator_hijacker>::const_iterator;
    test<I, I>();
    test<CI, I>();
    test<CI, CI>();
  }
  {
    using IL = std::unordered_set<operator_hijacker>::local_iterator;
    using CIL = std::unordered_set<operator_hijacker>::const_local_iterator;
    test<IL, IL>();
    test<CIL, IL>();
    test<CIL, CIL>();
  }
}
} // libcis_ns_containers_unord_unord_set_iterator_operators_addressof_51ee0b50 (libcis)

