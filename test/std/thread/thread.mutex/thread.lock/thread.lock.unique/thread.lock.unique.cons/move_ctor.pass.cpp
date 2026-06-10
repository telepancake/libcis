// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_ctor).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_ctor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// unique_lock(unique_lock&& u);

#include <cassert>
#include <memory>
#include <mutex>
#include <type_traits>

#include "checking_mutex.h"
#include "test_macros.h"

#if TEST_STD_VER >= 11
namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_ctor { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_move_constructible<std::unique_lock<checking_mutex>>::value, "");
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_ctor; // libcis

#endif

void test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_cons_move_ctor() {
  checking_mutex m;
  std::unique_lock<checking_mutex> lk0(m);
  std::unique_lock<checking_mutex> lk = std::move(lk0);

  assert(lk.mutex() == std::addressof(m));
  assert(lk.owns_lock());
  assert(lk0.mutex() == nullptr);
  assert(!lk0.owns_lock());

  return;
}
