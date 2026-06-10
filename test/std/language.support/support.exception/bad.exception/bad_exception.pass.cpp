// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_exception_bad_exception_bad_exception).
// main -> test_language_support_support_exception_bad_exception_bad_exception; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// test bad_exception

#include <exception>
#include <type_traits>
#include <cassert>

#include "test_macros.h"

void test_language_support_support_exception_bad_exception_bad_exception()
{
    static_assert((std::is_base_of<std::exception, std::bad_exception>::value),
                 "std::is_base_of<std::exception, std::bad_exception>::value");
    static_assert(std::is_polymorphic<std::bad_exception>::value,
                 "std::is_polymorphic<std::bad_exception>::value");
    std::bad_exception b;
    std::bad_exception b2 = b;
    b2 = b;
    const char* w = b2.what();
    assert(w);

  return;
}
