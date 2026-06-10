// AST-transferred from libc++ by tools/transfer.py (slug=containers_container_adaptors_stack_stack_cons_alloc_ctor_alloc).
// main -> test_containers_container_adaptors_stack_stack_cons_alloc_ctor_alloc; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <stack>

// template <class Alloc>
//   explicit stack(const Alloc& a);

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"

namespace libcis_ns_containers_container_adaptors_stack_stack_cons_alloc_ctor_alloc { // libcis: isolate file-scope helpers
struct test : private std::stack<int, std::deque<int, test_allocator<int> > > {
  typedef std::stack<int, std::deque<int, test_allocator<int> > > base;

  explicit test(const test_allocator<int>& a) : base(a) {}
  test(const container_type& cont, const test_allocator<int>& a) : base(cont, a) {}
#if TEST_STD_VER >= 11
  test(container_type&& cont, const test_allocator<int>& a) : base(std::move(cont), a) {}
  test(test&& q, const test_allocator<int>& a) : base(std::move(q), a) {}
#endif
  test_allocator<int> get_allocator() { return c.get_allocator(); }
};
} using namespace libcis_ns_containers_container_adaptors_stack_stack_cons_alloc_ctor_alloc; // libcis


void test_containers_container_adaptors_stack_stack_cons_alloc_ctor_alloc() {
  test q(test_allocator<int>(3));
  assert(q.get_allocator() == test_allocator<int>(3));

  return;
}
