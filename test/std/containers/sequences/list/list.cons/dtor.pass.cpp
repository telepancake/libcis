// AST-transferred from libc++ by tools/transfer.py (slug=containers_sequences_list_list_cons_dtor).
// main -> test_containers_sequences_list_list_cons_dtor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// ~list()

// no emplace_back in C++03
// UNSUPPORTED: c++03

#include <list>
#include <cassert>
#include <set>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_list_list_cons_dtor { // libcis: isolate file-scope helpers
std::set<int> destroyed;

struct Foo {
  explicit Foo(int i) : value(i) {}
  ~Foo() { destroyed.insert(value); }
  int value;
};
} using namespace libcis_ns_containers_sequences_list_list_cons_dtor; // libcis


void test_containers_sequences_list_list_cons_dtor() {
  {
    std::list<Foo> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);
    assert(destroyed.empty());
  }
  assert(destroyed.count(1) == 1);
  assert(destroyed.count(2) == 1);
  assert(destroyed.count(3) == 1);

  return;
}
