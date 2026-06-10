// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopcallback_ctad).
// no entry point (compile-only); all file-scope decls isolated in anon namespace.
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

//   template<class Callback>
//   stop_callback(stop_token, Callback) -> stop_callback<Callback>;

#include <stop_token>
#include <type_traits>
#include <utility>

namespace libcis_ns_thread_thread_stoptoken_stopcallback_ctad { // libcis: isolate file-scope helpers
void test() {
  std::stop_token st;
  auto a = [] {};
  static_assert(std::is_same_v<decltype(std::stop_callback(st, a)), std::stop_callback<decltype(a)>>);
  static_assert(std::is_same_v<decltype(std::stop_callback(st, std::as_const(a))), std::stop_callback<decltype(a)>>);
  static_assert(std::is_same_v<decltype(std::stop_callback(st, std::move(a))), std::stop_callback<decltype(a)>>);
  static_assert(
      std::is_same_v<decltype(std::stop_callback(st, std::move(std::as_const(a)))), std::stop_callback<decltype(a)>>);
}
} using namespace libcis_ns_thread_thread_stoptoken_stopcallback_ctad; // libcis

