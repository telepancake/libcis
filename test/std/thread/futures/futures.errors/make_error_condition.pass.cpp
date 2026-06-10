// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_errors_make_error_condition).
// main -> test_thread_futures_futures_errors_make_error_condition; file-scope helpers isolated in anon namespace.
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

// class error_condition

// error_condition make_error_condition(future_errc e);

#include <future>
#include <cassert>

#include "test_macros.h"

void test_thread_futures_futures_errors_make_error_condition()
{
    {
        const std::error_condition ec1 =
          std::make_error_condition(std::future_errc::future_already_retrieved);
        assert(ec1.value() ==
                  static_cast<int>(std::future_errc::future_already_retrieved));
        assert(ec1.category() == std::future_category());
    }

  return;
}
