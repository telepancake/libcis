// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_assign_75283128).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock& operator=(unique_lock&& u);

#include <cassert>
#include <memory>
#include <mutex>

#include "checking_mutex.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_assign_75283128 { // libcis
int main(int, char**) {
  checking_mutex m0;
  checking_mutex m1;
  std::unique_lock<checking_mutex> lk0(m0);
  std::unique_lock<checking_mutex> lk1(m1);

  auto& result = (lk1 = std::move(lk0));

  assert(&result == &lk1);
  assert(lk1.mutex() == std::addressof(m0));
  assert(lk1.owns_lock());
  assert(lk0.mutex() == nullptr);
  assert(!lk0.owns_lock());

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_assign_75283128 (libcis)

