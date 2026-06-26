// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_recursive_ctor_default_5ce99e58).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// <mutex>

// class recursive_timed_mutex;

// recursive_timed_mutex();

#include <mutex>
#include <cassert>

namespace libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_recursive_ctor_default_5ce99e58 { // libcis
int main(int, char**) {
  // The mutex is unlocked after default construction
  {
    std::recursive_timed_mutex m;
    assert(m.try_lock());
    m.unlock();
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_mutex_requirements_thread_timedmutex_requirements_thread_timedmutex_recursive_ctor_default_5ce99e58 (libcis)

