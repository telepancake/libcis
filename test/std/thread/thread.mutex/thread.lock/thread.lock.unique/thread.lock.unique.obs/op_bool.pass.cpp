// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_op_bool).
// main -> test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_op_bool; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// explicit operator bool() const noexcept;

#include <cassert>
#include <mutex>
#include <type_traits>

#include "checking_mutex.h"
#include "test_macros.h"

#if TEST_STD_VER >= 11
namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_op_bool { // libcis: isolate file-scope helpers
static_assert(noexcept(static_cast<bool>(std::declval<std::unique_lock<checking_mutex>&>())), "");
} using namespace libcis_ns_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_op_bool; // libcis

#endif

void test_thread_thread_mutex_thread_lock_thread_lock_unique_thread_lock_unique_obs_op_bool() {
  static_assert(std::is_constructible<bool, std::unique_lock<checking_mutex> >::value, "");
  static_assert(!std::is_convertible<std::unique_lock<checking_mutex>, bool>::value, "");

  checking_mutex mux;
  const std::unique_lock<checking_mutex> lk0; // Make sure `operator bool()` is `const`
  assert(!static_cast<bool>(lk0));
  std::unique_lock<checking_mutex> lk1(mux);
  assert(static_cast<bool>(lk1));
  lk1.unlock();
  assert(!static_cast<bool>(lk1));

  ASSERT_NOEXCEPT(static_cast<bool>(lk0));

  return;
}
