// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_coroutines_coroutine_handle_coroutine_handle_resumption_destroy).
// main -> test_language_support_support_coroutines_coroutine_handle_coroutine_handle_resumption_destroy; file-scope helpers isolated in anon namespace.
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

// void destroy() const

#include <coroutine>
#include <type_traits>
#include <memory>
#include <utility>
#include <cstdint>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_language_support_support_coroutines_coroutine_handle_coroutine_handle_resumption_destroy { // libcis: isolate file-scope helpers
template <class H>
auto has_destroy_imp(H&& h, int) -> decltype(h.destroy(), std::true_type{});
template <class H>
auto has_destroy_imp(H&&, long) -> std::false_type;

template <class H>
constexpr bool has_destroy() {
  return decltype(has_destroy_imp(std::declval<H>(), 0))::value;
}

template <class Promise>
void do_test(std::coroutine_handle<Promise>&& H) {
  using HType = std::coroutine_handle<Promise>;
  // FIXME Add a runtime test
  {
    ASSERT_SAME_TYPE(decltype(H.destroy()), void);
    LIBCPP_ASSERT_NOT_NOEXCEPT(H.destroy());
    static_assert(has_destroy<HType&>(), "");
    static_assert(has_destroy<HType&&>(), "");
  }
  {
    static_assert(has_destroy<HType const&>(), "");
    static_assert(has_destroy<HType const&&>(), "");
  }
}
} using namespace libcis_ns_language_support_support_coroutines_coroutine_handle_coroutine_handle_resumption_destroy; // libcis


void test_language_support_support_coroutines_coroutine_handle_coroutine_handle_resumption_destroy()
{
  do_test(std::coroutine_handle<>{});
  do_test(std::coroutine_handle<int>{});

  return;
}
