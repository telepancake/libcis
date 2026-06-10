// AST-transferred from libc++ by tools/transfer.py (slug=containers_associative_set_extract_iterator).
// main -> test_containers_associative_set_extract_iterator; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <set>

// class set

// node_type extract(const_iterator);

#include <set>
#include "test_macros.h"
#include "min_allocator.h"
#include "Counter.h"

namespace libcis_ns_containers_associative_set_extract_iterator { // libcis: isolate file-scope helpers
template <class Container>
void test(Container& c) {
  std::size_t sz = c.size();

  for (auto first = c.cbegin(); first != c.cend();) {
    auto key_value                  = *first;
    typename Container::node_type t = c.extract(first++);
    --sz;
    assert(t.value() == key_value);
    assert(t.get_allocator() == c.get_allocator());
    assert(sz == c.size());
  }

  assert(c.size() == 0);
}
} using namespace libcis_ns_containers_associative_set_extract_iterator; // libcis


void test_containers_associative_set_extract_iterator() {
  {
    using set_type = std::set<int>;
    set_type m     = {1, 2, 3, 4, 5, 6};
    test(m);
  }

  {
    std::set<Counter<int>> m = {1, 2, 3, 4, 5, 6};
    assert(Counter_base::gConstructed == 6);
    test(m);
    assert(Counter_base::gConstructed == 0);
  }

  {
    using min_alloc_set = std::set<int, std::less<int>, min_allocator<int>>;
    min_alloc_set m     = {1, 2, 3, 4, 5, 6};
    test(m);
  }

  return;
}
