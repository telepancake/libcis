// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_pr40459).
// main -> test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_pr40459; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// weak_ptr

// template<class Y> weak_ptr(const weak_ptr<Y>& r);
// template<class Y> weak_ptr(weak_ptr<Y>&& r);
//
// Regression test for https://llvm.org/PR41114
// Verify that these constructors never attempt a derived-to-virtual-base
// conversion on a dangling weak_ptr.

#include <cassert>
#include <cstring>
#include <memory>
#include <new>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_pr40459 { // libcis: isolate file-scope helpers
struct A {
  int i;
  virtual ~A() {}
};
struct B : public virtual A {
  int j;
};
struct Deleter {
  void operator()(void*) const {
    // do nothing
  }
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_pr40459; // libcis


void test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_pr40459() {
  TEST_ALIGNAS(TEST_ALIGNOF(B)) char buffer[sizeof(B)];
  B* pb                 = ::new ((void*)&buffer) B();
  std::shared_ptr<B> sp = std::shared_ptr<B>(pb, Deleter());
  std::weak_ptr<B> wp   = sp;
  sp                    = nullptr;
  assert(wp.expired());

  // Overwrite the B object with junk.
  std::memset(&buffer, '*', sizeof(buffer));

  std::weak_ptr<A> wq = wp;
  assert(wq.expired());
  std::weak_ptr<A> wr = std::move(wp);
  assert(wr.expired());

  return;
}
