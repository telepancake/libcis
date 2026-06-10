// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_multimap_multimap_ops_equal_range_transparent).
// main -> test_containers_associative_multimap_multimap_ops_equal_range_transparent; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <map>

// class multimap

// template<typename K>
//         pair<iterator,iterator>             equal_range(const K& x); // C++14
// template<typename K>
//         pair<const_iterator,const_iterator> equal_range(const K& x) const;
//         // C++14

#include <cassert>
#include <map>
#include <utility>

namespace libcis_ns_containers_associative_multimap_multimap_ops_equal_range_transparent { // libcis: isolate file-scope helpers
struct Comp {
  using is_transparent = void;

  bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const { return lhs < rhs; }

  bool operator()(const std::pair<int, int>& lhs, int rhs) const { return lhs.first < rhs; }

  bool operator()(int lhs, const std::pair<int, int>& rhs) const { return lhs < rhs.first; }
};
} using namespace libcis_ns_containers_associative_multimap_multimap_ops_equal_range_transparent; // libcis


void test_containers_associative_multimap_multimap_ops_equal_range_transparent() {
  std::multimap<std::pair<int, int>, int, Comp> s{{{2, 1}, 1}, {{1, 1}, 2}, {{1, 1}, 3}, {{1, 1}, 4}, {{2, 2}, 5}};

  auto er   = s.equal_range(1);
  long nels = 0;

  for (auto it = er.first; it != er.second; it++) {
    assert(it->first.first == 1);
    nels++;
  }

  assert(nels == 3);

  return;
}
