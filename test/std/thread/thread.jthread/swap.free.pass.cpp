// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_jthread_swap_free).
// main -> test_thread_thread_jthread_swap_free; file-scope helpers isolated in anon namespace.
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

// friend void swap(jthread& x, jthread& y) noexcept;

#include <cassert>
#include <thread>
#include <type_traits>

#include "make_test_thread.h"
#include "test_macros.h"

namespace libcis_ns_thread_thread_jthread_swap_free { // libcis: isolate file-scope helpers
template <class T>
concept IsFreeSwapNoexcept = requires(T& a, T& b) {
  { swap(a, b) } noexcept;
};

static_assert(IsFreeSwapNoexcept<std::jthread>);
} using namespace libcis_ns_thread_thread_jthread_swap_free; // libcis


void test_thread_thread_jthread_swap_free() {
  // x is default constructed
  {
    std::jthread t1;
    std::jthread t2        = support::make_test_jthread([] {});
    const auto originalId2 = t2.get_id();
    swap(t1, t2);

    assert(t1.get_id() == originalId2);
    assert(t2.get_id() == std::jthread::id());
  }

  // y is default constructed
  {
    std::jthread t1 = support::make_test_jthread([] {});
    std::jthread t2{};
    const auto originalId1 = t1.get_id();
    swap(t1, t2);

    assert(t1.get_id() == std::jthread::id());
    assert(t2.get_id() == originalId1);
  }

  // both not default constructed
  {
    std::jthread t1        = support::make_test_jthread([] {});
    std::jthread t2        = support::make_test_jthread([] {});
    const auto originalId1 = t1.get_id();
    const auto originalId2 = t2.get_id();
    swap(t1, t2);

    assert(t1.get_id() == originalId2);
    assert(t2.get_id() == originalId1);
  }

  // both default constructed
  {
    std::jthread t1;
    std::jthread t2;
    swap(t1, t2);

    assert(t1.get_id() == std::jthread::id());
    assert(t2.get_id() == std::jthread::id());
  }

  return;
}
