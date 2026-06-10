// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv; file-scope helpers isolated in anon namespace.
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

// template<class Y> shared_ptr(shared_ptr<Y>&& r);

#include <memory>
#include <type_traits>
#include <utility>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv; // libcis


int A::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv { // libcis: isolate file-scope helpers
struct C
{
    static int count;

    C() {++count;}
    C(const C&) {++count;}
    virtual ~C() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv; // libcis


int C::count = 0;

// https://llvm.org/PR60258
// Invalid constructor SFINAE for std::shared_ptr's array ctors
namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv { // libcis: isolate file-scope helpers
static_assert(!std::is_constructible<std::shared_ptr<int>,     std::shared_ptr<long>&&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<B>,       std::shared_ptr<A>&&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<const A>, std::shared_ptr<A>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<A>,       std::shared_ptr<const A>&&>::value, "");

#if TEST_STD_VER >= 17
static_assert(!std::is_constructible<std::shared_ptr<int>,     std::shared_ptr<int[]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int>,     std::shared_ptr<int[5]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[]>,   std::shared_ptr<int>&&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<int[]>,   std::shared_ptr<int[5]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  std::shared_ptr<int>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  std::shared_ptr<int[]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[7]>,  std::shared_ptr<int[5]>&&>::value, "");
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv; // libcis

#endif

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv()
{
    static_assert(( std::is_convertible<std::shared_ptr<A>, std::shared_ptr<B> >::value), "");
    static_assert((!std::is_convertible<std::shared_ptr<B>, std::shared_ptr<A> >::value), "");
    static_assert((!std::is_convertible<std::shared_ptr<A>, std::shared_ptr<C> >::value), "");
    {
        std::shared_ptr<A> pA(new A);
        assert(pA.use_count() == 1);
        assert(B::count == 1);
        assert(A::count == 1);
        {
            B* p = pA.get();
            std::shared_ptr<B> pB(std::move(pA));
            assert(B::count == 1);
            assert(A::count == 1);
#if TEST_STD_VER >= 11
            assert(pB.use_count() == 1);
            assert(pA.use_count() == 0);
#else
            assert(pB.use_count() == 2);
            assert(pA.use_count() == 2);
#endif
            assert(p == pB.get());
        }
#if TEST_STD_VER >= 11
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
#else
        assert(pA.use_count() == 1);
        assert(B::count == 1);
        assert(A::count == 1);
#endif
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::shared_ptr<A> pA;
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
        {
            std::shared_ptr<B> pB(std::move(pA));
            assert(B::count == 0);
            assert(A::count == 0);
            assert(pB.use_count() == 0);
            assert(pA.use_count() == 0);
            assert(pA.get() == pB.get());
        }
        assert(pA.use_count() == 0);
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> p1;
        assert(p1.use_count() == 0);
        assert(A::count == 0);
        {
            std::shared_ptr<const A[]> p2(p1);
            assert(A::count == 0);
            assert(p2.use_count() == 0);
            assert(p1.use_count() == 0);
            assert(p1.get() == p2.get());
        }
        assert(p1.use_count() == 0);
        assert(A::count == 0);
    }
    assert(A::count == 0);
#endif

    {
        std::shared_ptr<A const> pA(new A);
        B const* p = pA.get();
        std::shared_ptr<B const> pB(std::move(pA));
        assert(pB.get() == p);
    }

  return;
}
