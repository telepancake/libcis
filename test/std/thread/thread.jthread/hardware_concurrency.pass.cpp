// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_jthread_hardware_concurrency).
// main -> test_thread_thread_jthread_hardware_concurrency; file-scope helpers isolated in anon namespace.
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

// [[nodiscard]] static unsigned int hardware_concurrency() noexcept;

#include <cassert>
#include <concepts>
#include <thread>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_jthread_hardware_concurrency { // libcis: isolate file-scope helpers
static_assert(noexcept(std::jthread::hardware_concurrency()));
} using namespace libcis_ns_thread_thread_jthread_hardware_concurrency; // libcis


void test_thread_thread_jthread_hardware_concurrency() {
  std::same_as<unsigned int> decltype(auto) result = std::jthread::hardware_concurrency();
  assert(result == std::thread::hardware_concurrency());

  return;
}
