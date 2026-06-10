// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_exchange).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_exchange; file-scope helpers isolated in anon namespace.
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
// shared_ptr<T>
// atomic_exchange(shared_ptr<T>* p, shared_ptr<T> r)

// UNSUPPORTED: c++03

#include <memory>
#include <cassert>

#include "test_macros.h"

void test_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_exchange()
{
    {
        std::shared_ptr<int> p(new int(4));
        std::shared_ptr<int> r(new int(3));
        r = std::atomic_exchange(&p, r);
        assert(*p == 3);
        assert(*r == 4);
    }

  return;
}
