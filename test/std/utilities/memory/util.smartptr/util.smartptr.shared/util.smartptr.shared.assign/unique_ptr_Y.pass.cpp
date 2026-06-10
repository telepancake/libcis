// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

#include <memory>
#include <type_traits>
#include <cassert>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y; // libcis


int A::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y { // libcis: isolate file-scope helpers
template <class T>
struct StatefulArrayDeleter {
  int state = 0;

  StatefulArrayDeleter(int val = 0) : state(val) {}
  StatefulArrayDeleter(StatefulArrayDeleter const&) { assert(false); }

  void operator()(T* ptr) {
    assert(state == 42);
    delete []ptr;
  }
};

// https://llvm.org/PR53368
// Bogus unique_ptr-to-shared_ptr conversions should be forbidden
#if TEST_STD_VER >= 17
static_assert( std::is_assignable<std::shared_ptr<A>&,   std::unique_ptr<A>&&>::value, "");
static_assert( std::is_assignable<std::shared_ptr<A[]>&, std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_assignable<std::shared_ptr<A>&,   std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_assignable<std::shared_ptr<B[]>&, std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_assignable<std::shared_ptr<B>&,   std::unique_ptr<A[]>&&>::value, "");
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y; // libcis

#endif

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y()
{
    {
        std::unique_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 1);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::unique_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            assert(B::count == 0);
            assert(A::count == 0);
//          assert(pB.use_count() == 1); // no longer true due to LWG 2415
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::unique_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            assert(B::count == 1);
            assert(A::count == 1);
            assert(pB.use_count() == 1);
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);
    {
        std::unique_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            assert(B::count == 0);
            assert(A::count == 0);
//          assert(pB.use_count() == 1); // no longer true due to LWG 2415
            assert(pA.get() == 0);
            assert(pB.get() == ptrA);
        }
        assert(B::count == 0);
        assert(A::count == 0);
    }
    assert(B::count == 0);
    assert(A::count == 0);

#if TEST_STD_VER > 14
    {
      StatefulArrayDeleter<A> d;
      std::unique_ptr<A[], StatefulArrayDeleter<A>&> u(new A[4], d);
      std::shared_ptr<A[]> p;
      p = std::move(u);
      d.state = 42;
      assert(A::count == 4);
    }
    assert(A::count == 0);
    assert(B::count == 0);

    {
      std::unique_ptr<A[]> ptr(new A[8]);
      A* raw_ptr = ptr.get();
      std::shared_ptr<A[]> p;
      p = std::move(ptr);
      assert(A::count == 8);
      assert(p.use_count() == 1);
      assert(p.get() == raw_ptr);
      assert(ptr.get() == 0);
    }
    assert(A::count == 0);

    {
      std::unique_ptr<int[]> ptr(new int[8]);
      std::shared_ptr<int[]> p;
      p = std::move(ptr);
    }
#endif // TEST_STD_VER >= 14

  return;
}
