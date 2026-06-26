// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock_b9537c61).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock(mutex_type& m, defer_lock_t);

#include <cassert>
#include <memory>
#include <mutex>
#include <type_traits>

#include "checking_mutex.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock_b9537c61 { // libcis
#if TEST_STD_VER >= 11
static_assert(
    std::is_nothrow_constructible<std::unique_lock<checking_mutex>, checking_mutex&, std::defer_lock_t>::value, "");
#endif

int main(int, char**) {
  checking_mutex m;
  std::unique_lock<checking_mutex> lk(m, std::defer_lock_t());
  assert(m.last_try == checking_mutex::none);
  assert(lk.mutex() == std::addressof(m));
  assert(lk.owns_lock() == false);

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock_b9537c61 (libcis)

