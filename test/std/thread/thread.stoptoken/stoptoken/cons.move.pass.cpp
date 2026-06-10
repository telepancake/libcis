// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stoptoken_cons_move).
// main -> test_thread_thread_stoptoken_stoptoken_cons_move; file-scope helpers isolated in anon namespace.
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

// stop_token(stop_token&&) noexcept;

#include <cassert>
#include <stop_token>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stoptoken_cons_move { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_move_constructible_v<std::stop_token>);
} using namespace libcis_ns_thread_thread_stoptoken_stoptoken_cons_move; // libcis


void test_thread_thread_stoptoken_stoptoken_cons_move() {
  {
    std::stop_source source;
    auto st = source.get_token();

    assert(st.stop_possible());
    assert(!st.stop_requested());

    std::stop_token st2{std::move(st)};

    assert(!st.stop_possible());
    assert(!st.stop_requested());

    assert(st2.stop_possible());
    assert(!st2.stop_requested());

    source.request_stop();

    assert(!st.stop_possible());
    assert(!st.stop_requested());

    assert(st2.stop_possible());
    assert(st2.stop_requested());

  }

  return;
}
