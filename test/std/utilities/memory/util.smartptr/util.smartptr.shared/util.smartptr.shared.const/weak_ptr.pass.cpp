// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);

#include <cassert>
#include <memory>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr; // libcis


int A::count = 0;

// https://llvm.org/PR60258
// Invalid constructor SFINAE for std::shared_ptr's array ctors
namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr { // libcis: isolate file-scope helpers
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::weak_ptr<long>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<B>,       const std::weak_ptr<A>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<const A>, const std::weak_ptr<A>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<A>,       const std::weak_ptr<const A>&>::value, "");

#if TEST_STD_VER >= 17
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::weak_ptr<int[]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::weak_ptr<int[5]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[]>,   const std::weak_ptr<int>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<int[]>,   const std::weak_ptr<int[5]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  const std::weak_ptr<int>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  const std::weak_ptr<int[]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[7]>,  const std::weak_ptr<int[5]>&>::value, "");
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr; // libcis

#endif

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr()
{
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        std::weak_ptr<A> wp;
        try
        {
            std::shared_ptr<A> sp(wp);
            assert(false);
        }
        catch (std::bad_weak_ptr&)
        {
        }
        assert(A::count == 0);
    }
#endif
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        std::shared_ptr<A> sp(wp);
        assert(sp.use_count() == 2);
        assert(sp.get() == sp0.get());
        assert(A::count == 1);
    }
    assert(A::count == 0);
    {
        std::shared_ptr<A const> sp0(new A);
        std::weak_ptr<A const> wp(sp0);
        std::shared_ptr<A const> sp(wp);
        assert(sp.use_count() == 2);
        assert(sp.get() == sp0.get());
        assert(A::count == 1);
    }
    assert(A::count == 0);
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        sp0.reset();
        try
        {
            std::shared_ptr<A> sp(wp);
            assert(false);
        }
        catch (std::bad_weak_ptr&)
        {
        }
    }
    assert(A::count == 0);
#endif

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> sp0(new A[8]);
        std::weak_ptr<A[]> wp(sp0);
        std::shared_ptr<const A[]> sp(wp);
        assert(sp.use_count() == 2);
        assert(sp.get() == sp0.get());
        assert(A::count == 8);
    }
    assert(A::count == 0);
#endif

  return;
}
