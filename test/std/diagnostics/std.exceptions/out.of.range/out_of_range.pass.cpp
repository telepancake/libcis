// AST-transferred from libc++ by tools/transfer.py (slug=diagnostics_std_exceptions_out_of_range_out_of_range).
// main -> test_diagnostics_std_exceptions_out_of_range_out_of_range; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test out_of_range

#include <stdexcept>
#include <type_traits>
#include <cstring>
#include <string>
#include <cassert>

#include "test_macros.h"

void test_diagnostics_std_exceptions_out_of_range_out_of_range()
{
    static_assert((std::is_base_of<std::logic_error, std::out_of_range>::value),
                 "std::is_base_of<std::logic_error, std::out_of_range>::value");
    static_assert(std::is_polymorphic<std::out_of_range>::value,
                 "std::is_polymorphic<std::out_of_range>::value");
    {
    const char* msg = "out_of_range message";
    std::out_of_range e(msg);
    assert(std::strcmp(e.what(), msg) == 0);
    std::out_of_range e2(e);
    assert(std::strcmp(e2.what(), msg) == 0);
    e2 = e;
    assert(std::strcmp(e2.what(), msg) == 0);
    }
    {
    std::string msg("another out_of_range message");
    std::out_of_range e(msg);
    assert(e.what() == msg);
    std::out_of_range e2(e);
    assert(e2.what() == msg);
    e2 = e;
    assert(e2.what() == msg);
    }

  return;
}
