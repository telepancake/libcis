// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_set_contains).
// main -> test_containers_associative_set_contains; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <cassert>
#include <set>

// <set>

// bool contains(const key_type& x) const;

namespace libcis_ns_containers_associative_set_contains { // libcis: isolate file-scope helpers
template <typename T, typename V, typename B, typename... Vals>
void test(B bad, Vals... args) {
  T set;
  V vals[] = {args...};

  for (auto& v : vals)
    set.insert(v);
  for (auto& v : vals)
    assert(set.contains(v));

  assert(!set.contains(bad));
}

struct E {
  int a    = 1;
  double b = 1;
  char c   = 1;
};
} using namespace libcis_ns_containers_associative_set_contains; // libcis


void test_containers_associative_set_contains() {
  {
    test<std::set<int>, int>(14, 10, 11, 12, 13);
    test<std::set<char>, char>('e', 'a', 'b', 'c', 'd');
  }
  {
    test<std::multiset<int>, int>(14, 10, 11, 12, 13);
    test<std::multiset<char>, char>('e', 'a', 'b', 'c', 'd');
  }

  return;
}
