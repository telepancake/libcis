// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_static_pointer_cast).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_static_pointer_cast; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template<class T, class U> shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) noexcept;
// template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U>&& r) noexcept;

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_static_pointer_cast { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_static_pointer_cast; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_static_pointer_cast { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_static_pointer_cast; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_cast_static_pointer_cast()
{
    {
        const std::shared_ptr<A> pA(new A);
        ASSERT_NOEXCEPT(std::static_pointer_cast<B>(pA));
        std::shared_ptr<B> pB = std::static_pointer_cast<B>(pA);
        assert(pB.get() == pA.get());
        assert(!pB.owner_before(pA) && !pA.owner_before(pB));
    }
    {
        const std::shared_ptr<B> pA(new A);
        std::shared_ptr<A> pB = std::static_pointer_cast<A>(pA);
        assert(pB.get() == pA.get());
        assert(!pB.owner_before(pA) && !pA.owner_before(pB));
    }
    {
        const std::shared_ptr<A> pA;
        std::shared_ptr<B> pB = std::static_pointer_cast<B>(pA);
        assert(pB.get() == pA.get());
        assert(!pB.owner_before(pA) && !pA.owner_before(pB));
    }
    {
        const std::shared_ptr<B> pA;
        std::shared_ptr<A> pB = std::static_pointer_cast<A>(pA);
        assert(pB.get() == pA.get());
        assert(!pB.owner_before(pA) && !pA.owner_before(pB));
    }
#if TEST_STD_VER > 14
    {
      const std::shared_ptr<A[8]> pA;
      std::shared_ptr<B[8]> pB = std::static_pointer_cast<B[8]>(pA);
      assert(pB.get() == pA.get());
      assert(!pB.owner_before(pA) && !pA.owner_before(pB));
    }
    {
      const std::shared_ptr<B[8]> pA;
      std::shared_ptr<A[8]> pB = std::static_pointer_cast<A[8]>(pA);
      assert(pB.get() == pA.get());
      assert(!pB.owner_before(pA) && !pA.owner_before(pB));
    }
#endif // TEST_STD_VER > 14
#if TEST_STD_VER > 20
    {
      A* pA_raw = new A;
      std::shared_ptr<A> pA(pA_raw);
      ASSERT_NOEXCEPT(std::static_pointer_cast<B>(std::move(pA)));
      std::shared_ptr<B> pB = std::static_pointer_cast<B>(std::move(pA));
      assert(pA.get() == nullptr);
      assert(pB.get() == pA_raw);
      assert(pB.use_count() == 1);
    }
#endif // TEST_STD_VER > 20

    return;
}
