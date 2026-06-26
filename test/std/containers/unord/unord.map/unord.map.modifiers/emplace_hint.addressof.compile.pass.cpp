// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_map_unord_map_modifiers_emplace_hint_addressof_f6ed251d).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// template <class... Args>
//     iterator emplace_hint(const_iterator position, Args&&... args);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <unordered_map>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_map_unord_map_modifiers_emplace_hint_addressof_f6ed251d { // libcis
void test() {
  std::unordered_map<operator_hijacker, operator_hijacker> m;
  m.emplace_hint(m.cbegin());
}
} // libcis_ns_containers_unord_unord_map_unord_map_modifiers_emplace_hint_addressof_f6ed251d (libcis)

