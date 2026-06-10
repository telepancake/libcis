// AST-transferred from libc++ by tools/transfer.py (slug=utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_is_lock_free).
// main -> test_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_is_lock_free; file-scope helpers isolated in anon namespace.
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

// template<class T>
// bool
// atomic_is_lock_free(const shared_ptr<T>* p);

// UNSUPPORTED: c++03

#include <memory>
#include <cassert>

#include "test_macros.h"

void test_utilities_memory_util_smartptr_util_smartptr_shared_atomic_atomic_is_lock_free()
{
    {
        const std::shared_ptr<int> p(new int(3));
        assert(std::atomic_is_lock_free(&p) == false);
    }

  return;
}
