// AST-transferred from libc++ by tools/transfer.py (slug=utilities_allocator_adaptor_allocator_adaptor_members_allocate_size).
// main -> test_utilities_allocator_adaptor_allocator_adaptor_members_allocate_size; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// pointer allocate(size_type n);

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"

void test_utilities_allocator_adaptor_allocator_adaptor_members_allocate_size() {
  {
    typedef std::scoped_allocator_adaptor<A1<int>> A;
    A a;
    A1<int>::allocate_called = false;
    assert(a.allocate(10) == (int*)10);
    assert(A1<int>::allocate_called == true);
  }
  {
    typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
    A a;
    A1<int>::allocate_called = false;
    assert(a.allocate(10) == (int*)10);
    assert(A1<int>::allocate_called == true);
  }
  {
    typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
    A a;
    A1<int>::allocate_called = false;
    assert(a.allocate(10) == (int*)10);
    assert(A1<int>::allocate_called == true);
  }

  return;
}
