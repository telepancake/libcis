// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_threads_thread_thread_this_sleep_until_d5f9636a).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <thread>

// template <class Clock, class Duration>
//   void sleep_until(const chrono::time_point<Clock, Duration>& abs_time);

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <thread>

#include "test_macros.h"

namespace libcis_ns_thread_thread_threads_thread_thread_this_sleep_until_d5f9636a { // libcis
int main(int, char**)
{
  typedef std::chrono::system_clock Clock;
  typedef Clock::time_point time_point;
  std::chrono::milliseconds ms(500);
  time_point t0 = Clock::now();
  std::this_thread::sleep_until(t0 + ms);
  time_point t1 = Clock::now();
  // NOTE: Operating systems are (by default) best effort and therefore we may
  // have slept longer, perhaps much longer than we requested.
  assert(t1 - t0 >= ms);

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_threads_thread_thread_this_sleep_until_d5f9636a (libcis)

