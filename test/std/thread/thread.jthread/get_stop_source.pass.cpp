// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_jthread_get_stop_source_6cdce60d).
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

// [[nodiscard]] stop_source get_stop_source() noexcept;

#include <cassert>
#include <concepts>
#include <stop_token>
#include <thread>
#include <type_traits>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_jthread_get_stop_source_6cdce60d { // libcis
static_assert(noexcept(std::declval<std::jthread&>().get_stop_source()));

int main(int, char**) {
  // Represents a thread
  {
    std::jthread jt                                      = support::make_test_jthread([] {});
    std::same_as<std::stop_source> decltype(auto) result = jt.get_stop_source();
    assert(result.stop_possible());
  }

  // Does not represents a thread
  {
    std::jthread jt{};
    std::same_as<std::stop_source> decltype(auto) result = jt.get_stop_source();
    assert(!result.stop_possible());
  }

  return 0;

    return 0;
}
} // libcis_ns_thread_thread_jthread_get_stop_source_6cdce60d (libcis)

