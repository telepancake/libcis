// AST-transferred from libc++ by tools/transfer.py (slug=utilities_format_format_arguments_format_arg_store_make_wformat_args).
// main -> test_utilities_format_format_arguments_format_arg_store_make_wformat_args; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17
// XFAIL: no-wide-characters

// <format>

// template<class... Args>
//   format-arg-store<wformat_context, Args...>
//   make_wformat_args(Args&... args);

#include <cassert>
#include <format>

#include "test_basic_format_arg.h"
#include "test_macros.h"

namespace libcis_ns_utilities_format_format_arguments_format_arg_store_make_wformat_args { // libcis: isolate file-scope helpers
template <class... Args>
concept can_make_wformat_args = requires(Args&&... args) { std::make_wformat_args(std::forward<Args>(args)...); };

static_assert(can_make_wformat_args<int&>);
static_assert(!can_make_wformat_args<int>);
static_assert(!can_make_wformat_args<int&&>);
} using namespace libcis_ns_utilities_format_format_arguments_format_arg_store_make_wformat_args; // libcis


void test_utilities_format_format_arguments_format_arg_store_make_wformat_args() {
  int i                       = 1;
  char c                      = 'c';
  nullptr_t p                 = nullptr;
  bool b                      = false;
  [[maybe_unused]] auto store = std::make_wformat_args(i, p, b, c);

  LIBCPP_STATIC_ASSERT(
      std::same_as<decltype(store), std::__format_arg_store<std::wformat_context, int, nullptr_t, bool, char>>);

  return;
}
