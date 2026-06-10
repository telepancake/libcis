// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template<class Y> void reset(Y* p);

#include <memory>
#include <cassert>
#include <utility>

#include "reset_helper.h"
#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer; // libcis


int A::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer { // libcis: isolate file-scope helpers
struct Derived : A {};

static_assert( HasReset<std::shared_ptr<int>,  int*>::value, "");
static_assert( HasReset<std::shared_ptr<A>,  Derived*>::value, "");
static_assert(!HasReset<std::shared_ptr<A>,  int*>::value, "");

#if TEST_STD_VER >= 17
static_assert( HasReset<std::shared_ptr<int[]>,  int*>::value, "");
static_assert(!HasReset<std::shared_ptr<int[]>,  int(*)[]>::value, "");
static_assert( HasReset<std::shared_ptr<int[5]>, int*>::value, "");
static_assert(!HasReset<std::shared_ptr<int[5]>, int(*)[5]>::value, "");
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer; // libcis

#endif

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer()
{
    {
        std::shared_ptr<B> p(new B);
        A* ptr = new A;
        p.reset(ptr);
        assert(A::count == 1);
        assert(B::count == 1);
        assert(p.use_count() == 1);
        assert(p.get() == ptr);
    }
    assert(A::count == 0);
    {
        std::shared_ptr<B> p;
        A* ptr = new A;
        p.reset(ptr);
        assert(A::count == 1);
        assert(B::count == 1);
        assert(p.use_count() == 1);
        assert(p.get() == ptr);
    }
    assert(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<const A[]> p;
        A* ptr = new A[8];
        p.reset(ptr);
        assert(A::count == 8);
        assert(p.use_count() == 1);
        assert(p.get() == ptr);
    }
    assert(A::count == 0);
#endif

  return;
}
