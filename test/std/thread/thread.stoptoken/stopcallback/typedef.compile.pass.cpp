// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_stoptoken_stopcallback_typedef).
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

// using callback_type = _Callback;

#include <stop_token>
#include <type_traits>

namespace libcis_ns_thread_thread_stoptoken_stopcallback_typedef { // libcis: isolate file-scope helpers
struct Callback {
  void operator()() const;
};

static_assert(std::is_same_v<std::stop_callback<Callback>::callback_type, Callback>);
static_assert(std::is_same_v<std::stop_callback<const Callback>::callback_type, const Callback>);
static_assert(std::is_same_v<std::stop_callback<Callback&>::callback_type, Callback&>);
static_assert(std::is_same_v<std::stop_callback<const Callback&>::callback_type, const Callback&>);
static_assert(std::is_same_v<std::stop_callback<Callback&&>::callback_type, Callback&&>);
static_assert(std::is_same_v<std::stop_callback<const Callback&&>::callback_type, const Callback&&>);
} using namespace libcis_ns_thread_thread_stoptoken_stopcallback_typedef; // libcis

