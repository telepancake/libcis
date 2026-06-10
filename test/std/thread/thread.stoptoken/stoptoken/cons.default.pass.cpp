// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stoptoken_cons_default).
// main -> test_thread_thread_stoptoken_stoptoken_cons_default; file-scope helpers isolated in anon namespace.
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

// stop_token() noexcept;

#include <cassert>
#include <stop_token>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stoptoken_cons_default { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_default_constructible_v<std::stop_token>);
} using namespace libcis_ns_thread_thread_stoptoken_stoptoken_cons_default; // libcis


void test_thread_thread_stoptoken_stoptoken_cons_default() {
  {
    std::stop_token st = {}; // implicit
    assert(!st.stop_possible());
    assert(!st.stop_requested());
  }

  return;
}
