// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_coroutines_coroutine_handle_void_handle).
// main -> test_language_support_support_coroutines_coroutine_handle_void_handle; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <coroutine>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_language_support_support_coroutines_coroutine_handle_void_handle { // libcis: isolate file-scope helpers
struct A {
  using promise_type = A*;
};

struct B {};
struct C {};
} using namespace libcis_ns_language_support_support_coroutines_coroutine_handle_void_handle; // libcis


template <>
struct std::coroutine_traits<A, int> {
  using promise_type = int*;
};
template <class ...Args>
struct std::coroutine_traits<B, Args...> {
  using promise_type = B*;
};
template <>
struct std::coroutine_traits<C> {
  using promise_type = void;
};

namespace libcis_ns_language_support_support_coroutines_coroutine_handle_void_handle { // libcis: isolate file-scope helpers
template <class Expect, class T, class ...Args>
void check_type() {
  using P = typename std::coroutine_traits<T, Args...>::promise_type ;
  static_assert(std::is_same<P, Expect>::value, "");
};
} using namespace libcis_ns_language_support_support_coroutines_coroutine_handle_void_handle; // libcis


void test_language_support_support_coroutines_coroutine_handle_void_handle()
{
  check_type<A*, A>();
  check_type<int*, A, int>();
  check_type<B*, B>();
  check_type<void, C>();

  return;
}
