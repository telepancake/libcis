// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_semaphore_binary).
// main -> test_thread_thread_semaphore_binary; file-scope helpers isolated in anon namespace.
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

// <semaphore>

#include <semaphore>
#include <chrono>
#include <thread>
#include <type_traits>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_semaphore_binary { // libcis: isolate file-scope helpers
static_assert(std::is_same<std::binary_semaphore, std::counting_semaphore<1>>::value, "");
} using namespace libcis_ns_thread_thread_semaphore_binary; // libcis


void test_thread_thread_semaphore_binary()
{
  std::binary_semaphore s(1);

  auto l = [&](){
    for(int i = 0; i < 1024; ++i) {
        s.acquire();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        s.release();
    }
  };

  std::thread t = support::make_test_thread(l);
  l();

  t.join();

  return;
}
