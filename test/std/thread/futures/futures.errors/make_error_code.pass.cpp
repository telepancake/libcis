// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_errors_make_error_code).
// main -> test_thread_futures_futures_errors_make_error_code; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: no-threads

// <future>

// class error_code

// error_code make_error_code(future_errc e);

#include <future>
#include <cassert>

#include "test_macros.h"

void test_thread_futures_futures_errors_make_error_code()
{
    {
        std::error_code ec = make_error_code(std::future_errc::broken_promise);
        assert(ec.value() == static_cast<int>(std::future_errc::broken_promise));
        assert(ec.category() == std::future_category());
    }

  return;
}
