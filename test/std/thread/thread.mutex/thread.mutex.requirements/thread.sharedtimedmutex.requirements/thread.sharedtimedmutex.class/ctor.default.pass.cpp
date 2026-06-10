// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_ctor_default).
// main -> test_thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_ctor_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11

// <shared_mutex>

// class shared_timed_mutex;

// shared_timed_mutex();

#include <shared_mutex>

#include "test_macros.h"

void test_thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_ctor_default() {
  std::shared_timed_mutex m;
  (void)m;

  return;
}
