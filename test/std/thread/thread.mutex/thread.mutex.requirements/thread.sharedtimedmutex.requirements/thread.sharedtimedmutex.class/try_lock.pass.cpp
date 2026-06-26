// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_try_lock_5119f49c).
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

// bool try_lock();

#include <shared_mutex>
#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

#include "make_test_thread.h"

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_try_lock_5119f49c { // libcis
int main(int, char**) {
  // Try to exclusive-lock a mutex that is not locked yet. This should succeed.
  {
    std::shared_timed_mutex m;
    bool succeeded = m.try_lock();
    assert(succeeded);
    m.unlock();
  }

  // Try to exclusive-lock a mutex that is already locked exclusively. This should fail.
  {
    std::shared_timed_mutex m;
    m.lock();

    std::thread t = support::make_test_thread([&] {
      bool succeeded = m.try_lock();
      assert(!succeeded);
    });
    t.join();

    m.unlock();
  }

  // Try to exclusive-lock a mutex that is already share-locked. This should fail.
  {
    std::shared_timed_mutex m;
    m.lock_shared();

    std::thread t = support::make_test_thread([&] {
      bool succeeded = m.try_lock();
      assert(!succeeded);
    });
    t.join();

    m.unlock_shared();
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_sharedtimedmutex_requirements_thread_sharedtimedmutex_class_try_lock_5119f49c (libcis)

