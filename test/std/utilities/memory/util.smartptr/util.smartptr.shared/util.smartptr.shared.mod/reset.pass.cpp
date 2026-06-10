// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// void reset();

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset { // libcis: isolate file-scope helpers
struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset; // libcis


int B::count = 0;

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset { // libcis: isolate file-scope helpers
struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset()
{
    {
        std::shared_ptr<B> p(new B);
        p.reset();
        assert(A::count == 0);
        assert(B::count == 0);
        assert(p.use_count() == 0);
        assert(p.get() == 0);
    }
    assert(A::count == 0);
    {
        std::shared_ptr<B> p;
        p.reset();
        assert(A::count == 0);
        assert(B::count == 0);
        assert(p.use_count() == 0);
        assert(p.get() == 0);
    }
    assert(A::count == 0);

  return;
}
