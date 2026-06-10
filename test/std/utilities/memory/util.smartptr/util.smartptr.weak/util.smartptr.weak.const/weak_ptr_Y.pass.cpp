// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y).
// main -> test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y; file-scope helpers isolated in anon namespace.
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

#include <memory>
#include <type_traits>
#include <utility>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y; // libcis


int A::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y { // libcis: isolate file-scope helpers
struct C
{
    static int count;

    C() {++count;}
    C(const C&) {++count;}
    virtual ~C() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y; // libcis


int C::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y { // libcis: isolate file-scope helpers
template <class T>
std::weak_ptr<T> source (std::shared_ptr<T> p) { return std::weak_ptr<T>(p); }
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y; // libcis


void test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y()
{
    static_assert(( std::is_convertible<std::weak_ptr<A>, std::weak_ptr<B> >::value), "");
    static_assert((!std::is_convertible<std::weak_ptr<B>, std::weak_ptr<A> >::value), "");
    static_assert((!std::is_convertible<std::weak_ptr<A>, std::weak_ptr<C> >::value), "");
    {
        const std::weak_ptr<A> pA(std::shared_ptr<A>(new A));
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
        {
            std::weak_ptr<B> pB(pA);
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 0);
            assert(pA.use_count() == 0);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::weak_ptr<A> pA;
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
        {
            std::weak_ptr<B> pB(pA);
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 0);
            assert(pA.use_count() == 0);
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);

    {
        std::shared_ptr<A> ps(new A);
        std::weak_ptr<A> pA = source(ps);
        std::weak_ptr<B> pB(std::move(pA));
        assert(pB.use_count() == 1);
    }
    assert(B::count == 0);
    assert(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> ps(new A[8]);
        std::weak_ptr<A[]> p1 = source(ps);
        std::weak_ptr<const A[]> p2(p1);
        assert(p2.use_count() == 1);
    }
    assert(A::count == 0);

    {
        std::shared_ptr<A[]> ps(new A[8]);
        std::weak_ptr<A[]> p1 = source(ps);
        std::weak_ptr<const A[]> p2(std::move(p1));
        assert(p2.use_count() == 1);
    }
    assert(A::count == 0);
#endif

  return;
}
