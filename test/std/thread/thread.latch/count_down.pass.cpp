// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_latch_count_down).
// main -> test_thread_thread_latch_count_down; file-scope helpers isolated in anon namespace.
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

// <latch>

#include <latch>
#include <thread>

#include "make_test_thread.h"
#include "test_macros.h"

void test_thread_thread_latch_count_down()
{
  std::latch l(2);

  l.count_down();
  std::thread t = support::make_test_thread([&](){
    l.count_down();
  });
  l.wait();
  t.join();

  return;
}
