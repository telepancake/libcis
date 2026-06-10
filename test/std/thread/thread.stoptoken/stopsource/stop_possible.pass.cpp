// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopsource_stop_possible).
// main -> test_thread_thread_stoptoken_stopsource_stop_possible; file-scope helpers isolated in anon namespace.
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

// [[nodiscard]] bool stop_possible() const noexcept;
// Returns: true if *this has ownership of a stop state; otherwise, false.

#include <cassert>
#include <stop_token>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stopsource_stop_possible { // libcis: isolate file-scope helpers
template <class T>
concept IsStopPossibleNoexcept = requires(const T& t) {
  { t.stop_possible() } noexcept;
};

static_assert(IsStopPossibleNoexcept<std::stop_source>);
} using namespace libcis_ns_thread_thread_stoptoken_stopsource_stop_possible; // libcis


void test_thread_thread_stoptoken_stopsource_stop_possible() {
  // no state
  {
    const std::stop_source st{std::nostopstate};
    assert(!st.stop_possible());
  }

  // with state
  {
    const std::stop_source st;
    assert(st.stop_possible());
  }

  return;
}
