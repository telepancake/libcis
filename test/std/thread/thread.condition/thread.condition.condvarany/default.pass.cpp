// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_condition_thread_condition_condvarany_default).
// main -> test_thread_thread_condition_thread_condition_condvarany_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <condition_variable>

// class condition_variable_any;

// condition_variable_any();

#include <condition_variable>
#include <cassert>

#include "test_macros.h"

void test_thread_thread_condition_thread_condition_condvarany_default()
{
    std::condition_variable_any cv;

  return;
}
