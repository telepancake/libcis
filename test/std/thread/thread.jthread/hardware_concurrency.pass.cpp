// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_jthread_hardware_concurrency_ed3bfbd2).
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

// [[nodiscard]] static unsigned int hardware_concurrency() noexcept;

#include <cassert>
#include <concepts>
#include <thread>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_jthread_hardware_concurrency_ed3bfbd2 { // libcis
static_assert(noexcept(std::jthread::hardware_concurrency()));

int main(int, char**) {
  std::same_as<unsigned int> decltype(auto) result = std::jthread::hardware_concurrency();
  assert(result == std::thread::hardware_concurrency());

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_jthread_hardware_concurrency_ed3bfbd2 (libcis)

