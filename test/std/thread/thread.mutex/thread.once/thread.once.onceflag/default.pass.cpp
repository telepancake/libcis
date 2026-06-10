// AST-transferred from libc++ by tools/transfer.py (slug=thread_thread_mutex_thread_once_thread_once_onceflag_default).
// main -> test_thread_thread_mutex_thread_once_thread_once_onceflag_default; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: no-threads

// <mutex>

// struct once_flag;

// constexpr once_flag() noexcept;

#include <mutex>
#include "test_macros.h"

void test_thread_thread_mutex_thread_once_thread_once_onceflag_default()
{
    {
    std::once_flag f;
    (void)f;
    }
#if TEST_STD_VER >= 11
    {
    constexpr std::once_flag f;
    (void)f;
    }
#endif

  return;
}
