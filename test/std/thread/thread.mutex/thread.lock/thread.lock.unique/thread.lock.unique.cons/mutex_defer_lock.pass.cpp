// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock; file-scope helpers isolated in anon namespace.
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

#if TEST_STD_VER >= 11
namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock { // libcis: isolate file-scope helpers
static_assert(
    std::is_nothrow_constructible<std::unique_lock<checking_mutex>, checking_mutex&, std::defer_lock_t>::value, "");
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock; // libcis

#endif

void test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_mutex_defer_lock() {
  checking_mutex m;
  std::unique_lock<checking_mutex> lk(m, std::defer_lock_t());
  assert(m.last_try == checking_mutex::none);
  assert(lk.mutex() == std::addressof(m));
  assert(lk.owns_lock() == false);

  return;
}
