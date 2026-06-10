// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_guard_ctor_adopt_lock).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_guard_ctor_adopt_lock; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <mutex>

// template <class Mutex> class lock_guard;

// lock_guard(mutex_type& m, adopt_lock_t);

#include <mutex>
#include <cassert>

#include "types.h"

void test_thread_thread_mutex_thread_lock_thread_lock_guard_ctor_adopt_lock() {
  MyMutex m;
  {
    m.lock();
    std::lock_guard<MyMutex> lg(m, std::adopt_lock);
    assert(m.locked);
  }
  assert(!m.locked);

  return;
}
