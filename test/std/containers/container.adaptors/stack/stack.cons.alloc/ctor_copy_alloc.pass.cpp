// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_cons_alloc_ctor_copy_alloc).
// main -> test_containers_container_adaptors_stack_stack_cons_alloc_ctor_copy_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// template <class Alloc>
//   stack(const stack& q, const Alloc& a);

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_cons_alloc_ctor_copy_alloc { // libcis: isolate file-scope helpers
template <class C>
C make(int n) {
  C c;
  for (int i = 0; i < n; ++i)
    c.push_back(int(i));
  return c;
}

typedef std::deque<int, test_allocator<int> > C;

template <class T>
struct test : public std::stack<T, C> {
  typedef std::stack<T, C> base;
  typedef test_allocator<int> allocator_type;
  typedef typename base::container_type container_type;

  explicit test(const allocator_type& a) : base(a) {}
  test(const container_type& cont, const allocator_type& a) : base(cont, a) {}
  test(const test& q, const allocator_type& a) : base(q, a) {}
  allocator_type get_allocator() { return this->c.get_allocator(); }
};
} using namespace libcis_ns_containers_container_adaptors_stack_stack_cons_alloc_ctor_copy_alloc; // libcis


void test_containers_container_adaptors_stack_stack_cons_alloc_ctor_copy_alloc() {
  test<int> q(make<C>(5), test_allocator<int>(4));
  test<int> q2(q, test_allocator<int>(5));
  assert(q2.get_allocator() == test_allocator<int>(5));
  assert(q2.size() == 5);

  return;
}
