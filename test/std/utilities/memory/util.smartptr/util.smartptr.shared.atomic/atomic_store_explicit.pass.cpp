// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_store_explicit).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_store_explicit; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <memory>

// shared_ptr

// template <class T>
// void
// atomic_store_explicit(shared_ptr<T>* p, shared_ptr<T> r, memory_order mo)

// UNSUPPORTED: c++03

#include <memory>

#include <atomic>
#include <cassert>

#include "test_macros.h"

void test_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_store_explicit()
{
    {
        std::shared_ptr<int> p;
        std::shared_ptr<int> r(new int(3));
        std::atomic_store_explicit(&p, r, std::memory_order_seq_cst);
        assert(*p == *r);
    }

  return;
}
