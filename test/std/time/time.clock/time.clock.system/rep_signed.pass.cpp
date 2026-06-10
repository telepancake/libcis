// AST-transferred from libc++ by tools/transfer.py (slug=time_time_clock_time_clock_system_rep_signed).
// main -> test_time_time_clock_time_clock_system_rep_signed; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <chrono>

// system_clock

// rep should be signed

#include <chrono>
#include <cassert>

#include "test_macros.h"

void test_time_time_clock_time_clock_system_rep_signed()
{
    assert(std::chrono::system_clock::duration::min() <
           std::chrono::system_clock::duration::zero());

  return;
}
