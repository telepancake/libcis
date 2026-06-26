// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_multimap_unord_multimap_cnstr_assign_copy_addressof_920bb9a7).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// unordered_multimap& operator=(const unordered_multimap& u);

// Validate whether the container can be copy-assigned with an ADL-hijacking operator&

#include <unordered_map>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_multimap_unord_multimap_cnstr_assign_copy_addressof_920bb9a7 { // libcis
void test() {
  {
    std::unordered_multimap<int, operator_hijacker> mo;
    std::unordered_multimap<int, operator_hijacker> m;
    m = mo;
  }
  {
    std::unordered_multimap<operator_hijacker, int> mo;
    std::unordered_multimap<operator_hijacker, int> m;
    m = mo;
  }
}
} // libcis_ns_containers_unord_unord_multimap_unord_multimap_cnstr_assign_copy_addressof_920bb9a7 (libcis)

