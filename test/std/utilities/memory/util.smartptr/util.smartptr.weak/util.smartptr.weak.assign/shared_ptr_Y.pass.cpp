// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y).
// main -> test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// weak_ptr

// template<class Y> weak_ptr& operator=(const shared_ptr<Y>& r);

#include <memory>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y()
{
    {
        const std::shared_ptr<A> pA(new A);
        assert(pA.use_count() == 1);
        {
            std::weak_ptr<B> pB;
            pB = pA;
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 1);
            assert(pA.use_count() == 1);
        }
        assert(pA.use_count() == 1);
        assert(B::count == 1);
        assert(A::count == 1);
    }
    assert(B::count == 0);
    assert(A::count == 0);

#if TEST_STD_VER > 14
    {
        const std::shared_ptr<A[]> p1(new A[8]);
        assert(p1.use_count() == 1);
        {
            std::weak_ptr<const A[]> p2;
            p2 = p1;
            assert(A::count == 8);
            assert(p2.use_count() == 1);
            assert(p1.use_count() == 1);
        }
        assert(p1.use_count() == 1);
        assert(A::count == 8);
    }
    assert(A::count == 0);
#endif

  return;
}
