// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopsource_cons_move).
// main -> test_thread_thread_stoptoken_stopsource_cons_move; file-scope helpers isolated in anon namespace.
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

// stop_source(stop_source&&) noexcept;

#include <cassert>
#include <stop_token>
#include <type_traits>
#include <utility>

#include "test_macros.h"

namespace libcis_ns_thread_thread_stoptoken_stopsource_cons_move { // libcis: isolate file-scope helpers
static_assert(std::is_nothrow_move_constructible_v<std::stop_source>);
} using namespace libcis_ns_thread_thread_stoptoken_stopsource_cons_move; // libcis


void test_thread_thread_stoptoken_stopsource_cons_move() {
  {
    std::stop_source source;

    assert(source.stop_possible());
    assert(!source.stop_requested());

    std::stop_source source2{std::move(source)};

    assert(!source.stop_possible());
    assert(!source.stop_requested());

    assert(source2.stop_possible());
    assert(!source2.stop_requested());

    source2.request_stop();

    assert(!source.stop_possible());
    assert(!source.stop_requested());

    assert(source2.stop_possible());
    assert(source2.stop_requested());
  }

  return;
}
