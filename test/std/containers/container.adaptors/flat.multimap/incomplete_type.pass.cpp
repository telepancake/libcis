// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_flat_multimap_incomplete_type).
// main -> test_containers_container_adaptors_flat_multimap_incomplete_type; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_map>

// Check that std::flat_multimap and its iterators can be instantiated with an incomplete
// type.

#include <flat_map>
#include <vector>

namespace libcis_ns_containers_container_adaptors_flat_multimap_incomplete_type { // libcis: isolate file-scope helpers
struct A {
  using Map = std::flat_multimap<A, A>;
  int data;
  Map m;
  Map::iterator it;
  Map::const_iterator cit;
};

// Implement the operator< required in order to instantiate flat_multimap<A, X>
bool operator<(A const& L, A const& R) { return L.data < R.data; }
} using namespace libcis_ns_containers_container_adaptors_flat_multimap_incomplete_type; // libcis


void test_containers_container_adaptors_flat_multimap_incomplete_type() {
  A a;
  return;
}
