// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_std_exceptions_underflow_error_underflow_error).
// main -> test_diagnostics_std_exceptions_underflow_error_underflow_error; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test underflow_error

#include <stdexcept>
#include <type_traits>
#include <cstring>
#include <string>
#include <cassert>

#include "test_macros.h"

void test_diagnostics_std_exceptions_underflow_error_underflow_error()
{
    static_assert((std::is_base_of<std::runtime_error, std::underflow_error>::value),
                 "std::is_base_of<std::runtime_error, std::underflow_error>::value");
    static_assert(std::is_polymorphic<std::underflow_error>::value,
                 "std::is_polymorphic<std::underflow_error>::value");
    {
    const char* msg = "underflow_error message";
    std::underflow_error e(msg);
    assert(std::strcmp(e.what(), msg) == 0);
    std::underflow_error e2(e);
    assert(std::strcmp(e2.what(), msg) == 0);
    e2 = e;
    assert(std::strcmp(e2.what(), msg) == 0);
    }
    {
    std::string msg("another underflow_error message");
    std::underflow_error e(msg);
    assert(e.what() == msg);
    std::underflow_error e2(e);
    assert(e2.what() == msg);
    e2 = e;
    assert(e2.what() == msg);
    }

  return;
}
