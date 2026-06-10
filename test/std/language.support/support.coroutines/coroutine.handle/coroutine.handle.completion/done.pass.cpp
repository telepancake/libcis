// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_coroutines_coroutine_handle_coroutine_handle_completion_done).
// main -> test_language_support_support_coroutines_coroutine_handle_coroutine_handle_completion_done; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <coroutine>

// template <class Promise = void>
// struct coroutine_handle;

// bool done() const

#include <coroutine>
#include <type_traits>
#include <memory>
#include <utility>
#include <cstdint>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_coroutines_coroutine_handle_coroutine_handle_completion_done { // libcis: isolate file-scope helpers
template <class Promise>
void do_test(std::coroutine_handle<Promise> const& H) {
  // FIXME Add a runtime test
  {
    ASSERT_SAME_TYPE(decltype(H.done()), bool);
    LIBCPP_ASSERT_NOT_NOEXCEPT(H.done());
  }
}
} using namespace libcis_ns_language_support_support_coroutines_coroutine_handle_coroutine_handle_completion_done; // libcis


void test_language_support_support_coroutines_coroutine_handle_coroutine_handle_completion_done()
{
  do_test(std::coroutine_handle<>{});
  do_test(std::coroutine_handle<int>{});

  return;
}
