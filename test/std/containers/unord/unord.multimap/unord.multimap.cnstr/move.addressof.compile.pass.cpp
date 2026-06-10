// AST-transferred from libc++ by tools/transfer.py (slug=containers_unord_unord_multimap_unord_multimap_cnstr_move_addressof).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
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
// class unordered_multimap

// unordered_multimap(unordered_multimap&&)
//   noexcept(
//     is_nothrow_move_constructible<hasher>::value &&
//     is_nothrow_move_constructible<key_equal>::value &&
//     is_nothrow_move_constructible<allocator_type>::value);

// Validate whether the operation properly guards against ADL-hijacking operator&

#include <unordered_map>

#include "test_macros.h"
#include "operator_hijacker.h"

namespace libcis_ns_containers_unord_unord_multimap_unord_multimap_cnstr_move_addressof { // libcis: isolate file-scope helpers
void test() {
  std::unordered_multimap<operator_hijacker, operator_hijacker> mo;
  std::unordered_multimap<operator_hijacker, operator_hijacker> m(std::move(mo));
}
} using namespace libcis_ns_containers_unord_unord_multimap_unord_multimap_cnstr_move_addressof; // libcis

