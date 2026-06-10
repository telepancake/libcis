// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_jthread_request_stop).
// main -> test_thread_thread_jthread_request_stop; file-scope helpers isolated in anon namespace.
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

// [[nodiscard]] bool request_stop() noexcept;

#include <cassert>
#include <concepts>
#include <stop_token>
#include <thread>
#include <type_traits>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_jthread_request_stop { // libcis: isolate file-scope helpers
static_assert(noexcept(std::declval<std::jthread&>().request_stop()));
} using namespace libcis_ns_thread_thread_jthread_request_stop; // libcis


void test_thread_thread_jthread_request_stop() {
  // Represents a thread
  {
    std::jthread jt = support::make_test_jthread([] {});
    auto st         = jt.get_stop_token();
    assert(!st.stop_requested());
    std::same_as<bool> decltype(auto) result = jt.request_stop();
    assert(result);
    assert(st.stop_requested());
  }

  // Does not represent a thread
  {
    std::jthread jt{};
    std::same_as<bool> decltype(auto) result = jt.request_stop();
    assert(!result);
  }

  return;
}
