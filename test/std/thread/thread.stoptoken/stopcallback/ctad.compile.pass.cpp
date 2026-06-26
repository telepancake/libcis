// transferred+adapted from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopcallback_ctad_eb9a079e).
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
// XFAIL: availability-synchronization_library-missing

//   template<class Callback>
//   stop_callback(stop_token, Callback) -> stop_callback<Callback>;

#include <stop_token>
#include <type_traits>
#include <utility>

namespace libcis_ns_thread_thread_stoptoken_stopcallback_ctad_eb9a079e { // libcis
void test() {
  std::stop_token st;
  auto a = [] {};
  static_assert(std::is_same_v<decltype(std::stop_callback(st, a)), std::stop_callback<decltype(a)>>);
  static_assert(std::is_same_v<decltype(std::stop_callback(st, std::as_const(a))), std::stop_callback<decltype(a)>>);
  static_assert(std::is_same_v<decltype(std::stop_callback(st, std::move(a))), std::stop_callback<decltype(a)>>);
  static_assert(
      std::is_same_v<decltype(std::stop_callback(st, std::move(std::as_const(a)))), std::stop_callback<decltype(a)>>);
}
} // libcis_ns_thread_thread_stoptoken_stopcallback_ctad_eb9a079e (libcis)

