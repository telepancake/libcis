// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// shared_ptr& operator=(const shared_ptr& r);

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <memory>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr()
{
    {
        const std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<A> pB(new A);
            pB = pA;
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 2);
            assert(pA.use_count() == 2);
            assert(pA.get() == pB.get());
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 1);
        assert(B::count == 1);
        assert(A::count == 1);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        const std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<A> pB(new A);
            pB = pA;
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 0);
            assert(pA.use_count() == 0);
            assert(pA.get() == pB.get());
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        const std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<A> pB;
            pB = pA;
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 2);
            assert(pA.use_count() == 2);
            assert(pA.get() == pB.get());
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 1);
        assert(B::count == 1);
        assert(A::count == 1);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        const std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<A> pB;
            pB = pA;
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 0);
            assert(pA.use_count() == 0);
            assert(pA.get() == pB.get());
            assert(pB.get() == ptrA);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);

  return;
}
