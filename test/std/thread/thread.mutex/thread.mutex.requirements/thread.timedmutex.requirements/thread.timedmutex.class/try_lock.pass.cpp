// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_class_try_lock_f0a41658).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03
// UNSUPPORTED: no-threads

// <mutex>

// class timed_mutex;

// bool try_lock();

#include <mutex>
#include <cassert>
#include <thread>

#include "make_test_thread.h"

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_class_try_lock_f0a41658 { // libcis
int main(int, char**) {
  // Try to lock a mutex that is not locked yet. This should succeed.
  {
    std::timed_mutex m;
    bool succeeded = m.try_lock();
    assert(succeeded);
    m.unlock();
  }

  // Try to lock a mutex that is already locked. This should fail.
  {
    std::timed_mutex m;
    m.lock();

    std::thread t = support::make_test_thread([&] {
      for (int i = 0; i != 10; ++i) {
        bool succeeded = m.try_lock();
        assert(!succeeded);
      }
    });
    t.join();

    m.unlock();
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_class_try_lock_f0a41658 (libcis)

