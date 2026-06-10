// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_semaphore_lost_wakeup_timed).
// main -> test_thread_thread_semaphore_lost_wakeup_timed; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <semaphore>

// Test that counting_semaphore::try_acquire_for does not suffer from lost wakeup
// under stress testing.

#include <chrono>
#include <functional>
#include <semaphore>
#include <thread>
#include <vector>

#include "make_test_thread.h"

namespace libcis_ns_thread_thread_semaphore_lost_wakeup_timed { // libcis: isolate file-scope helpers
constexpr auto num_acquirer   = 100;
constexpr auto num_iterations = 5000;

void acquire(std::counting_semaphore<>& s) {
  for (int i = 0; i < num_iterations; ++i) {
    while (!s.try_acquire_for(std::chrono::seconds(1))) {
    }
  }
}

void release(std::counting_semaphore<>& s) {
  for (int i = 0; i < num_iterations; ++i) {
    s.release(num_acquirer);
  }
}
} using namespace libcis_ns_thread_thread_semaphore_lost_wakeup_timed; // libcis


void test_thread_thread_semaphore_lost_wakeup_timed() {
  std::vector<std::thread> threads;
  std::counting_semaphore<> s(0);
  for (int i = 0; i < num_acquirer; ++i)
    threads.push_back(support::make_test_thread(acquire, std::ref(s)));

  threads.push_back(support::make_test_thread(release, std::ref(s)));
  for (auto& thread : threads)
    thread.join();

  return;
}
