// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_lock).
// main -> test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_lock; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// weak_ptr

// shared_ptr<T> lock() const;

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_lock { // libcis: isolate file-scope helpers
struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_lock; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_lock()
{
    {
        std::weak_ptr<A> wp;
        std::shared_ptr<A> sp = wp.lock();
        assert(sp.use_count() == 0);
        assert(sp.get() == 0);
        assert(A::count == 0);
    }
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        std::shared_ptr<A> sp = wp.lock();
        assert(sp.use_count() == 2);
        assert(sp.get() == sp0.get());
        assert(A::count == 1);
    }
    assert(A::count == 0);
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        sp0.reset();
        std::shared_ptr<A> sp = wp.lock();
        assert(sp.use_count() == 0);
        assert(sp.get() == 0);
        assert(A::count == 0);
    }
    assert(A::count == 0);

  return;
}
