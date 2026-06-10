// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y_rv).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y_rv; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <memory>

// shared_ptr

// template<class Y> shared_ptr& operator=(shared_ptr<Y>&& r);

#include <memory>
#include <type_traits>
#include <utility>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y_rv { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y_rv; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y_rv { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y_rv; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y_rv()
{
    {
        std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 1);
            assert(pA.use_count() == 0);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 0);
            assert(pA.use_count() == 0);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 1);
            assert(pA.use_count() == 0);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 0);
            assert(pA.use_count() == 0);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> p1(new A[8]);
        A* ptr = p1.get();
        assert(A::count == 8);
        {
            std::shared_ptr<const A[]> p2;
            p2 = std::move(p1);
            assert(A::count == 8);
            assert(p2.use_count() == 1);
            assert(p1.use_count() == 0);
            assert(p1.get() == nullptr);
            assert(p2.get() == ptr);
        }
        assert(p1.use_count() == 0);
        assert(A::count == 0);
    }
    assert(A::count == 0);
#endif

  return;
}
