// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_swap).
// main -> test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_swap; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// weak_ptr

// void swap(weak_ptr& r);

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_swap { // libcis: isolate file-scope helpers
struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_swap; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_swap()
{
    {
        A* ptr1 = new A;
        A* ptr2 = new A;
        std::shared_ptr<A> p1(ptr1);
        std::weak_ptr<A> w1(p1);
        {
            std::shared_ptr<A> p2(ptr2);
            std::weak_ptr<A> w2(p2);
            w1.swap(w2);
            assert(w1.use_count() == 1);
            assert(w1.lock().get() == ptr2);
            assert(w2.use_count() == 1);
            assert(w2.lock().get() == ptr1);
            assert(A::count == 2);
        }
    }
    assert(A::count == 0);

  return;
}
