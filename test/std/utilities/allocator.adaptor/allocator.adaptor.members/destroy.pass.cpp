// AST-transferred from libc++ by tools/transfer.py (slug=utilities_allocator_adaptor_allocator_adaptor_members_destroy).
// main -> test_utilities_allocator_adaptor_allocator_adaptor_members_destroy; file-scope helpers isolated in anon namespace.
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

// template <class T> void destroy(T* p);

#include <scoped_allocator>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "allocators.h"

namespace libcis_ns_utilities_allocator_adaptor_allocator_adaptor_members_destroy { // libcis: isolate file-scope helpers
struct B {
  static bool constructed;

  B() { constructed = true; }
  ~B() { constructed = false; }
};
} using namespace libcis_ns_utilities_allocator_adaptor_allocator_adaptor_members_destroy; // libcis


bool B::constructed = false;

void test_utilities_allocator_adaptor_allocator_adaptor_members_destroy() {
  {
    typedef std::scoped_allocator_adaptor<A1<B>> A;
    A a;
    char buf[100];
    typedef B S;
    S* s = (S*)buf;
    assert(!S::constructed);
    a.construct(s);
    assert(S::constructed);
    a.destroy(s);
    assert(!S::constructed);
  }

  {
    typedef std::scoped_allocator_adaptor<A3<B>, A1<B>> A;
    A a;
    char buf[100];
    typedef B S;
    S* s = (S*)buf;
    assert(!S::constructed);
    assert(!A3<S>::constructed);
    assert(!A3<S>::destroy_called);
    a.construct(s);
    assert(S::constructed);
    assert(A3<S>::constructed);
    assert(!A3<S>::destroy_called);
    a.destroy(s);
    assert(!S::constructed);
    assert(A3<S>::constructed);
    assert(A3<S>::destroy_called);
  }

  return;
}
