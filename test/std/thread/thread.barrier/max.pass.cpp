// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_barrier_max_53730708).
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

#include "test_macros.h"

namespace libcis_ns_thread_thread_barrier_max_53730708 { // libcis
int main(int, char**)
{
  static_assert(std::barrier<>::max() > 0, "");
  auto l = []() noexcept {};
  static_assert(std::barrier<decltype(l)>::max() > 0, "");
  return 0;

    return 0;
}
} // libcis_ns_thread_thread_barrier_max_53730708 (libcis)

