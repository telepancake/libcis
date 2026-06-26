// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_8a376c42).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// explicit unique_lock(mutex_type& m);

// template<class _Mutex> unique_lock(unique_lock<_Mutex>)
//     -> unique_lock<_Mutex>;  // C++17

#include <cassert>
#include <mutex>

#include "checking_mutex.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_8a376c42 { // libcis
int main(int, char**) {
  checking_mutex mux;

  {
    std::unique_lock<checking_mutex> lock(mux);
    assert(mux.current_state == checking_mutex::locked_via_lock);
  }
  assert(mux.current_state == checking_mutex::unlocked);

#if TEST_STD_VER >= 17
  static_assert(std::is_same_v<std::unique_lock<checking_mutex>, decltype(std::unique_lock{mux})>, "");
#endif

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_8a376c42 (libcis)

