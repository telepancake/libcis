// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopsource_get_token).
// main -> test_thread_thread_stoptoken_stopsource_get_token; file-scope helpers isolated in anon namespace.
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

// [[nodiscard]] stop_token get_token() const noexcept;

#include <cassert>
#include <concepts>
#include <stop_token>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stopsource_get_token { // libcis: isolate file-scope helpers
template <class T>
concept IsGetTokenNoexcept = requires(const T& t) {
  { t.get_token() } noexcept;
};

static_assert(IsGetTokenNoexcept<std::stop_source>);
} using namespace libcis_ns_thread_thread_stoptoken_stopsource_get_token; // libcis


void test_thread_thread_stoptoken_stopsource_get_token() {
  // no state
  {
    std::stop_source ss{std::nostopstate};
    std::same_as<std::stop_token> decltype(auto) st = ss.get_token();
    assert(!st.stop_possible());
    assert(!st.stop_requested());
  }

  // with state
  {
    std::stop_source ss;
    std::same_as<std::stop_token> decltype(auto) st = ss.get_token();
    assert(st.stop_possible());
    assert(!st.stop_requested());

    ss.request_stop();
    assert(st.stop_requested());
  }

  return;
}
