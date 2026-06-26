// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_barrier_completion_8b37a8d5).
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

// <barrier>

#include <barrier>
#include <thread>
#include <cassert>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_barrier_completion_8b37a8d5 { // libcis
int main(int, char**)
{
  int x = 0;
  auto comp = [&]() noexcept { x += 1; };
  std::barrier<decltype(comp)> b(2, comp);

  std::thread t = support::make_test_thread([&](){
      for(int i = 0; i < 10; ++i)
        b.arrive_and_wait();
  });

  for(int i = 0; i < 10; ++i)
    b.arrive_and_wait();

  assert(x == 10);
  t.join();
  return 0;

    return 0;
}
} // libcis_ns_thread_thread_barrier_completion_8b37a8d5 (libcis)

