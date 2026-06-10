// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_cons_ctor_rcontainer).
// main -> test_containers_container_adaptors_stack_stack_cons_ctor_rcontainer; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <stack>

// explicit stack(Container&&= Container());  // before C++20
// stack() : stack(Container()) {}            // C++20
// explicit stack(Container&&);               // C++20

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#if TEST_STD_VER >= 11
#  include "test_convertible.h"
#endif

namespace libcis_ns_containers_container_adaptors_stack_stack_cons_ctor_rcontainer { // libcis: isolate file-scope helpers
template <class C>
C make(int n) {
  C c;
  for (int i = 0; i < n; ++i)
    c.push_back(MoveOnly(i));
  return c;
}
} using namespace libcis_ns_containers_container_adaptors_stack_stack_cons_ctor_rcontainer; // libcis


void test_containers_container_adaptors_stack_stack_cons_ctor_rcontainer() {
  typedef std::deque<MoveOnly> Container;
  typedef std::stack<MoveOnly> Q;
  Q q(make<Container>(5));
  assert(q.size() == 5);

#if TEST_STD_VER >= 11
  static_assert(!test_convertible<Q, Container&&>(), "");
#endif

  return;
}
