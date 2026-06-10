// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_latch_max).
// main -> test_thread_thread_latch_max; file-scope helpers isolated in anon namespace.
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

#include <latch>
#include <cassert>

#include "test_macros.h"

void test_thread_thread_latch_max()
{
  static_assert(std::latch::max() > 0, "");
  return;
}
