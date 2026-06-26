// transferred+adapted from libc++ by tools/transfer.py (slug=containers_unord_unord_set_key_eq_10be6a8d).
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

// key_equal key_eq() const;

#include <unordered_set>
#include <cassert>

namespace libcis_ns_containers_unord_unord_set_key_eq_10be6a8d { // libcis
int main(int, char**) {
  typedef std::unordered_set<int> set_type;

  set_type s;

  std::pair<set_type::iterator, bool> p1 = s.insert(1);
  std::pair<set_type::iterator, bool> p2 = s.insert(2);

  const set_type& cs = s;

  assert(cs.key_eq()(*p1.first, *p1.first));
  assert(cs.key_eq()(*p2.first, *p2.first));

  assert(!cs.key_eq()(*p1.first, *p2.first));
  assert(!cs.key_eq()(*p2.first, *p1.first));

  return 0;

    return 0;
}
} // libcis_ns_containers_unord_unord_set_key_eq_10be6a8d (libcis)

