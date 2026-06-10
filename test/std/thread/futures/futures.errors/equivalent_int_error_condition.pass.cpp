// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_errors_equivalent_int_error_condition).
// main -> test_thread_futures_futures_errors_equivalent_int_error_condition; file-scope helpers isolated in anon namespace.
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

// const error_category& future_category();

// virtual bool equivalent(int code, const error_condition& condition) const;

#include <future>
#include <cassert>

#include "test_macros.h"

void test_thread_futures_futures_errors_equivalent_int_error_condition()
{
    const std::error_category& e_cat = std::future_category();
    std::error_condition e_cond = e_cat.default_error_condition(5);
    assert(e_cat.equivalent(5, e_cond));
    assert(!e_cat.equivalent(6, e_cond));

  return;
}
