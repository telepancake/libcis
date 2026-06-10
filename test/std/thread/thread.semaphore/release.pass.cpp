// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_semaphore_release).
// main -> test_thread_thread_semaphore_release; file-scope helpers isolated in anon namespace.
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
#include <thread>

#include "make_test_thread.h"
#include "test_macros.h"

void test_thread_thread_semaphore_release()
{
  std::counting_semaphore<> s(0);

  s.release();
  s.acquire();

  std::thread t = support::make_test_thread([&](){
    s.acquire();
  });
  s.release(2);
  t.join();
  s.acquire();

  return;
}
