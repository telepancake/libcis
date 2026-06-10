// AST-transferred from libc++ by tools/transfer.py (slug=thread_futures_futures_errors_equivalent_error_code_int).
// main -> test_thread_futures_futures_errors_equivalent_error_code_int; file-scope helpers isolated in anon namespace.
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

// virtual bool equivalent(const error_code& code, int condition) const;

#include <future>
#include <cassert>

#include "test_macros.h"

void test_thread_futures_futures_errors_equivalent_error_code_int()
{
    const std::error_category& e_cat = std::future_category();
    assert(e_cat.equivalent(std::error_code(5, e_cat), 5));
    assert(!e_cat.equivalent(std::error_code(5, e_cat), 6));

  return;
}
