// AST-transferred from libc++ by tools/transfer.py (slug=time_time_clock_time_clock_file_file_time).
// main -> test_time_time_clock_time_clock_file_file_time; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <chrono>

// file_time

#include <chrono>

#include "test_macros.h"

namespace libcis_ns_time_time_clock_time_clock_file_file_time { // libcis: isolate file-scope helpers
template <class Dur>
void test() {
  ASSERT_SAME_TYPE(std::chrono::file_time<Dur>, std::chrono::time_point<std::chrono::file_clock, Dur>);
}
} using namespace libcis_ns_time_time_clock_time_clock_file_file_time; // libcis


void test_time_time_clock_time_clock_file_file_time() {
  test<std::chrono::nanoseconds>();
  test<std::chrono::minutes>();
  test<std::chrono::hours>();

  return;
}
