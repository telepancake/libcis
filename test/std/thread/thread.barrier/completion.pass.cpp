// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_barrier_completion).
// main -> test_thread_thread_barrier_completion; file-scope helpers isolated in anon namespace.
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

// <barrier>

#include <barrier>
#include <thread>
#include <cassert>

#include "make_test_thread.h"
#include "test_macros.h"

void test_thread_thread_barrier_completion()
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
  return;
}
