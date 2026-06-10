// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_set_count_transparent).
// main -> test_containers_associative_set_count_transparent; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11

// <set>

// class set

//    template<typename K>
//        size_type count(const K& x) const;        // C++14

#include <cassert>
#include <set>
#include <utility>

namespace libcis_ns_containers_associative_set_count_transparent { // libcis: isolate file-scope helpers
struct Comp {
  using is_transparent = void;

  bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const { return lhs < rhs; }

  bool operator()(const std::pair<int, int>& lhs, int rhs) const { return lhs.first < rhs; }

  bool operator()(int lhs, const std::pair<int, int>& rhs) const { return lhs < rhs.first; }
};
} using namespace libcis_ns_containers_associative_set_count_transparent; // libcis


void test_containers_associative_set_count_transparent() {
  std::set<std::pair<int, int>, Comp> s{{2, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 2}};

  auto cnt = s.count(1);
  assert(cnt == 3);

  return;
}
