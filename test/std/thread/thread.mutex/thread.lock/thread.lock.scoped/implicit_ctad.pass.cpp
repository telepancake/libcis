// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_scoped_implicit_ctad).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_scoped_implicit_ctad; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14

// <mutex>

// scoped_lock

// Make sure that the implicitly-generated CTAD works.

#include <mutex>

#include "test_macros.h"

void test_thread_thread_mutex_thread_lock_thread_lock_scoped_implicit_ctad() {
  std::mutex m1;
  std::recursive_mutex m2;
  std::recursive_timed_mutex m3;
  {
    std::scoped_lock lock(m1);
    ASSERT_SAME_TYPE(decltype(lock), std::scoped_lock<std::mutex>);
  }
  {
    std::scoped_lock lock(m1, m2);
    ASSERT_SAME_TYPE(decltype(lock), std::scoped_lock<std::mutex, std::recursive_mutex>);
  }
  {
    std::scoped_lock lock(m1, m2, m3);
    ASSERT_SAME_TYPE(decltype(lock), std::scoped_lock<std::mutex, std::recursive_mutex, std::recursive_timed_mutex>);
  }

  return;
}

