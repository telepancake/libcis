// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_jthread_get_id).
// main -> test_thread_thread_jthread_get_id; file-scope helpers isolated in anon namespace.
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

// [[nodiscard]] id get_id() const noexcept;

#include <cassert>
#include <concepts>
#include <thread>
#include <type_traits>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_jthread_get_id { // libcis: isolate file-scope helpers
static_assert(noexcept(std::declval<const std::jthread&>().get_id()));
} using namespace libcis_ns_thread_thread_jthread_get_id; // libcis


void test_thread_thread_jthread_get_id() {
  // Does not represent a thread
  {
    const std::jthread jt;
    std::same_as<std::jthread::id> decltype(auto) result = jt.get_id();
    assert(result == std::jthread::id());
  }

  // Represents a thread
  {
    const std::jthread jt                                = support::make_test_jthread([] {});
    std::same_as<std::jthread::id> decltype(auto) result = jt.get_id();
    assert(result != std::jthread::id());
  }

  return;
}
