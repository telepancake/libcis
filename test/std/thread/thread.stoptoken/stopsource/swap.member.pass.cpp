// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopsource_swap_member).
// main -> test_thread_thread_stoptoken_stopsource_swap_member; file-scope helpers isolated in anon namespace.
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

// void swap(stop_source& rhs) noexcept;

#include <cassert>
#include <concepts>
#include <stop_token>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stopsource_swap_member { // libcis: isolate file-scope helpers
template <class T>
concept IsNoThrowMemberSwappable = requires(T& t) {
  { t.swap(t) } noexcept;
};

static_assert(IsNoThrowMemberSwappable<std::stop_source>);
} using namespace libcis_ns_thread_thread_stoptoken_stopsource_swap_member; // libcis


void test_thread_thread_stoptoken_stopsource_swap_member() {
  {
    std::stop_source ss1;
    std::stop_source ss2;

    assert(ss1 != ss2);

    ss2.request_stop();

    assert(!ss1.stop_requested());
    assert(ss2.stop_requested());

    ss1.swap(ss2);

    assert(ss1 != ss2);
    assert(ss1.stop_requested());
    assert(!ss2.stop_requested());
  }

  return;
}
