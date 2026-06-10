// AST-transferred from libc++ by tools/transfer.py (slug=language_support_support_exception_propagation_exception_ptr).
// main -> test_language_support_support_exception_propagation_exception_ptr; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <exception>

// typedef unspecified exception_ptr;

// exception_ptr shall satisfy the requirements of NullablePointer.

#include <exception>
#include <cassert>

#include "test_macros.h"

void test_language_support_support_exception_propagation_exception_ptr()
{
    std::exception_ptr p;
    assert(p == nullptr);
    std::exception_ptr p2 = p;
    assert(nullptr == p);
    assert(!p);
    assert(p2 == p);
    p2 = p;
    assert(p2 == p);
    assert(p2 == nullptr);
    std::exception_ptr p3 = nullptr;
    assert(p3 == nullptr);
    p3 = nullptr;
    assert(p3 == nullptr);

    return;
}
