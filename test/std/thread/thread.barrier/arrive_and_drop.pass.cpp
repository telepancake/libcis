// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_barrier_arrive_and_drop).
// main -> test_thread_thread_barrier_arrive_and_drop; file-scope helpers isolated in anon namespace.
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

void test_thread_thread_barrier_arrive_and_drop()
{
  std::barrier<> b(2);

  std::thread t = support::make_test_thread([&](){
    b.arrive_and_drop();
  });

  b.arrive_and_wait();
  b.arrive_and_wait();
  t.join();
  return;
}
