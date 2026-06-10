// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_semaphore_lost_wakeup).
// main -> test_thread_thread_semaphore_lost_wakeup; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14, c++17

// This is a regression test for https://llvm.org/PR47013.

// <semaphore>

#include <barrier>
#include <semaphore>
#include <thread>
#include <vector>

#include "make_test_thread.h"

namespace libcis_ns_thread_thread_semaphore_lost_wakeup { // libcis: isolate file-scope helpers
static std::counting_semaphore<> s(0);
static std::barrier<> b(8 + 1);

void acquire() {
  for (int i = 0; i < 10'000; ++i) {
    s.acquire();
    b.arrive_and_wait();
  }
}

void release() {
  for (int i = 0; i < 10'000; ++i) {
    s.release(1);
    s.release(1);
    s.release(1);
    s.release(1);

    s.release(1);
    s.release(1);
    s.release(1);
    s.release(1);

    b.arrive_and_wait();
  }
}
} using namespace libcis_ns_thread_thread_semaphore_lost_wakeup; // libcis


void test_thread_thread_semaphore_lost_wakeup() {
  for (int run = 0; run < 20; ++run) {
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i)
      threads.push_back(support::make_test_thread(acquire));

    threads.push_back(support::make_test_thread(release));

    for (auto& thread : threads)
      thread.join();
  }

  return;
}
