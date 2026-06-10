// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_latch_ctor).
// main -> test_thread_thread_latch_ctor; file-scope helpers isolated in anon namespace.
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

// <latch>

// inline constexpr explicit latch(ptrdiff_t __expected);

// Make sure that the ctor of latch is constexpr and explicit.

#include <latch>

#include "test_convertible.h"

namespace libcis_ns_thread_thread_latch_ctor { // libcis: isolate file-scope helpers
static_assert(!test_convertible<std::latch, std::ptrdiff_t>(), "This constructor must be explicit");

constexpr bool test() {
  std::latch l(5);
  (void)l;
  return true;
}
} using namespace libcis_ns_thread_thread_latch_ctor; // libcis


void test_thread_thread_latch_ctor() {
  test();
  static_assert(test(), "");

  return;
}
