// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_semaphore_acquire_c60c4547).
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

// <semaphore>

#include <semaphore>
#include <thread>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_semaphore_acquire_c60c4547 { // libcis
int main(int, char**)
{
  std::counting_semaphore<> s(2);

  std::thread t = support::make_test_thread([&](){
    s.acquire();
  });
  t.join();

  s.acquire();

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_semaphore_acquire_c60c4547 (libcis)

