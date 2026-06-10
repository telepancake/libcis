// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_condition_cv_status).
// main -> test_thread_thread_condition_cv_status; file-scope helpers isolated in anon namespace.
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

// enum class cv_status { no_timeout, timeout };

#include <condition_variable>
#include <cassert>

#include "test_macros.h"

void test_thread_thread_condition_cv_status()
{
    assert(static_cast<int>(std::cv_status::no_timeout) == 0);
    assert(static_cast<int>(std::cv_status::timeout)    == 1);

  return;
}
