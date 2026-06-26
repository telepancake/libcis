// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_try_lock_shared_f60f8549).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14

// <shared_mutex>

// class shared_mutex;

// bool try_lock_shared();

#include <shared_mutex>
#include <cassert>
#include <thread>
#include <vector>

#include "make_test_thread.h"

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_try_lock_shared_f60f8549 { // libcis
int main(int, char**) {
  // Try to lock-shared a mutex that is not locked yet. This should succeed.
  {
    std::shared_mutex m;
    std::vector<std::thread> threads;
    for (int i = 0; i != 5; ++i) {
      threads.push_back(support::make_test_thread([&] {
        bool succeeded = m.try_lock_shared();
        assert(succeeded);
        m.unlock_shared();
      }));
    }

    for (auto& t : threads)
      t.join();
  }

  // Try to lock-shared a mutex that is already exclusively locked. This should fail.
  {
    std::shared_mutex m;
    m.lock();

    std::vector<std::thread> threads;
    for (int i = 0; i != 5; ++i) {
      threads.push_back(support::make_test_thread([&] {
        bool succeeded = m.try_lock_shared();
        assert(!succeeded);
      }));
    }

    for (auto& t : threads)
      t.join();

    m.unlock();
  }

  // Try to lock-shared a mutex that is already lock-shared. This should succeed.
  {
    std::shared_mutex m;
    m.lock_shared();
    std::vector<std::thread> threads;
    for (int i = 0; i != 5; ++i) {
      threads.push_back(support::make_test_thread([&] {
        bool succeeded = m.try_lock_shared();
        assert(succeeded);
        m.unlock_shared();
      }));
    }
    m.unlock_shared();

    for (auto& t : threads)
      t.join();
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_shared_mutex_requirements_thread_shared_mutex_class_try_lock_shared_f60f8549 (libcis)

