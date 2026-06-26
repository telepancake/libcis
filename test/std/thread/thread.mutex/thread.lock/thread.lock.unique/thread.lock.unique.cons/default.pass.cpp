// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default_c37661fe).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock();

#include <cassert>
#include <mutex>
#include <type_traits>

#include "checking_mutex.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default_c37661fe { // libcis
#if TEST_STD_VER >= 11
static_assert(std::is_nothrow_default_constructible<std::unique_lock<checking_mutex>>::value, "");
#endif

int main(int, char**) {
  std::unique_lock<checking_mutex> ul;
  assert(!ul.owns_lock());
  assert(ul.mutex() == nullptr);

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default_c37661fe (libcis)

