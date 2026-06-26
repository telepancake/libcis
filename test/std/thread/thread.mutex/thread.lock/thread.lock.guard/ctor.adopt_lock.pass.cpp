// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_guard_ctor_adopt_lock_81f9eee7).
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

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_ctor_adopt_lock_81f9eee7 { // libcis
int main(int, char**) {
  MyMutex m;
  {
    m.lock();
    std::lock_guard<MyMutex> lg(m, std::adopt_lock);
    assert(m.locked);
  }
  assert(!m.locked);

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_guard_ctor_adopt_lock_81f9eee7 (libcis)

