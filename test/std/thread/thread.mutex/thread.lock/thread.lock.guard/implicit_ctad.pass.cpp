// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_guard_implicit_ctad).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_guard_implicit_ctad; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <mutex>

// template <class Mutex> class lock_guard;

// Make sure that the implicitly-generated CTAD works.

#include <mutex>

#include "test_macros.h"
#include "types.h"

void test_thread_thread_mutex_thread_lock_thread_lock_guard_implicit_ctad() {
  MyMutex m;
  {
    std::lock_guard lg(m);
    ASSERT_SAME_TYPE(decltype(lg), std::lock_guard<MyMutex>);
  }

  return;
}

