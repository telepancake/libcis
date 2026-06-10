// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t).
// main -> test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// function(nullptr_t);

#include <functional>
#include <cassert>

#include "test_macros.h"

void test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t()
{
    std::function<int(int)> f(nullptr);
    assert(!f);

  return;
}
