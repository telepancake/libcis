// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_reset).
// main -> test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_reset; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <memory>

// weak_ptr

// void swap(weak_ptr& r);

#include <memory>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_reset { // libcis: isolate file-scope helpers
struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};
} using namespace libcis_ns_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_reset; // libcis


int A::count = 0;

void test_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_reset()
{
    {
        std::shared_ptr<A> p1(new A);
        std::weak_ptr<A> w1(p1);
        assert(w1.use_count() == 1);
        w1.reset();
        assert(w1.use_count() == 0);
        assert(p1.use_count() == 1);
    }
    assert(A::count == 0);

  return;
}
