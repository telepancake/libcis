// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stoptoken_assign_move).
// main -> test_thread_thread_stoptoken_stoptoken_assign_move; file-scope helpers isolated in anon namespace.
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

// stop_token& operator=(stop_token&& rhs) noexcept;

#include <cassert>
#include <concepts>
#include <stop_token>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stoptoken_assign_move { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_move_assignable_v<std::stop_token>);
} using namespace libcis_ns_thread_thread_stoptoken_stoptoken_assign_move; // libcis


void test_thread_thread_stoptoken_stoptoken_assign_move() {
  {
    std::stop_token st1;

    std::stop_source source;
    auto st2 = source.get_token();

    assert(st1 != st2);

    source.request_stop();

    assert(!st1.stop_requested());
    assert(st2.stop_requested());

    std::same_as<std::stop_token&> decltype(auto) ref = st1 = std::move(st2);
    assert(&ref == &st1);

    assert(st1 != st2);
    assert(st1.stop_requested());
    assert(!st2.stop_requested());
  }

  return;
}
