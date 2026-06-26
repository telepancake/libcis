// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_latch_count_down_e0fbe464).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14, c++17

// XFAIL: availability-synchronization_library-missing

// <latch>

#include <latch>
#include <thread>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_latch_count_down_e0fbe464 { // libcis
int main(int, char**)
{
  std::latch l(2);

  l.count_down();
  std::thread t = support::make_test_thread([&](){
    l.count_down();
  });
  l.wait();
  t.join();

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_latch_count_down_e0fbe464 (libcis)

