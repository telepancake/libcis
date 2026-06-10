// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_this_sleep_for).
// main -> test_thread_thread_threads_thread_thread_this_sleep_for; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// ALLOW_RETRIES: 3

// <thread>

// template <class Rep, class Period>
//   void sleep_for(const chrono::duration<Rep, Period>& rel_time);

#include <thread>
#include <cassert>
#include <chrono>

void test_thread_thread_threads_thread_thread_this_sleep_for()
{
  typedef std::chrono::system_clock Clock;
  typedef Clock::time_point time_point;
  std::chrono::milliseconds ms(500);
  time_point t0 = Clock::now();
  std::this_thread::sleep_for(ms);
  time_point t1 = Clock::now();
  // NOTE: Operating systems are (by default) best effort and therefore we may
  // have slept longer, perhaps much longer than we requested.
  assert(t1 - t0 >= ms);

  return;
}
