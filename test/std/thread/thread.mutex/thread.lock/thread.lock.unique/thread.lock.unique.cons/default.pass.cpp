// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default; file-scope helpers isolated in anon namespace.
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

#if TEST_STD_VER >= 11
namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_default_constructible<std::unique_lock<checking_mutex>>::value, "");
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default; // libcis

#endif

void test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_default() {
  std::unique_lock<checking_mutex> ul;
  assert(!ul.owns_lock());
  assert(ul.mutex() == nullptr);

  return;
}
