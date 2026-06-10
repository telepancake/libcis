// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template<class Y> shared_ptr(const shared_ptr<Y>& r, T *p);

#include <memory>
#include <cassert>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer { // libcis: isolate file-scope helpers
struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer()
{
    {
        std::shared_ptr<A> pA(new A);
        assert(pA.use_count() == 1);

        {
            B b;
            std::shared_ptr<B> pB(pA, &b);
            assert(A::count == 1);
            assert(B::count == 1);
            assert(pA.use_count() == 2);
            assert(pB.use_count() == 2);
            assert(pB.get() == &b);
        }
        assert(pA.use_count() == 1);
        assert(A::count == 1);
        assert(B::count == 0);
    }
    assert(A::count == 0);
    assert(B::count == 0);

    {
        std::shared_ptr<A const> pA(new A);
        assert(pA.use_count() == 1);

        {
            B const b;
            std::shared_ptr<B const> pB(pA, &b);
            assert(A::count == 1);
            assert(B::count == 1);
            assert(pA.use_count() == 2);
            assert(pB.use_count() == 2);
            assert(pB.get() == &b);
        }
        assert(pA.use_count() == 1);
        assert(A::count == 1);
        assert(B::count == 0);
    }
    assert(A::count == 0);
    assert(B::count == 0);

    int *pi = new int;
    {
      std::shared_ptr<int> p1(nullptr);
      std::shared_ptr<int> p2(p1, pi);
      assert(p2.get() == pi);
    }
    delete pi;
    {
      std::shared_ptr<int> p1(new int);
      std::shared_ptr<int> p2(p1, nullptr);
      assert(p2.get() == nullptr);
    }

#if TEST_STD_VER > 17 && defined(_LIBCPP_VERSION)
    {
      std::shared_ptr<A> pA(new A);
      assert(pA.use_count() == 1);

#  if TEST_STD_VER >= 20
      // LWG-2996 is only implemented in c++20 and beyond.
      // We don't backport because it is an evolutionary change.
      {
        B b;
        std::shared_ptr<B> pB(std::move(pA), &b);
        assert(A::count == 1);
        assert(B::count == 1);
        assert(pA.use_count() == 0);
        assert(pB.use_count() == 1);
        assert(pB.get() == &b);
      }
#  endif // TEST_STD_VER > 20
    }
    assert(A::count == 0);
    assert(B::count == 0);
#endif

    return;
}
