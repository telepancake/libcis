// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopsource_cons_nostopstate).
// main -> test_thread_thread_stoptoken_stopsource_cons_nostopstate; file-scope helpers isolated in anon namespace.
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

// explicit stop_source(nostopstate_t) noexcept;

#include <cassert>
#include <stop_token>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stopsource_cons_nostopstate { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_constructible_v<std::stop_source, std::nostopstate_t>);
// explicit
static_assert(!std::is_convertible_v<std::nostopstate_t, std::stop_source>);
} using namespace libcis_ns_thread_thread_stoptoken_stopsource_cons_nostopstate; // libcis


void test_thread_thread_stoptoken_stopsource_cons_nostopstate() {
  {
    std::stop_source ss(std::nostopstate);
    assert(!ss.stop_possible());
    assert(!ss.stop_requested());
  }

  return;
}
