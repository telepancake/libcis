// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_owns_lock).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_owns_lock; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// bool owns_lock() const;

#include <cassert>
#include <mutex>

#include "checking_mutex.h"
#include "test_macros.h"

#if TEST_STD_VER >= 11
namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_owns_lock { // libcis: isolate file-scope helpers
static_assert(noexcept(std::declval<std::unique_lock<checking_mutex>&>().owns_lock()), "");
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_owns_lock; // libcis

#endif

void test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_owns_lock() {
  {
    checking_mutex mux;
    const std::unique_lock<checking_mutex> lock0; // Make sure `owns_lock()` is `const`
    assert(!lock0.owns_lock());
    std::unique_lock<checking_mutex> lock1(mux);
    assert(lock1.owns_lock());
    lock1.unlock();
    assert(!lock1.owns_lock());
  }

  return;
}
