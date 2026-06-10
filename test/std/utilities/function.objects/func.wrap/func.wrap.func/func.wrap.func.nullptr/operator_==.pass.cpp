// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator).
// main -> test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator; file-scope helpers isolated in anon namespace.
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

// template <MoveConstructible R, MoveConstructible ... ArgTypes>
//   bool operator==(const function<R(ArgTypes...)>&, nullptr_t);
//
// template <MoveConstructible R, MoveConstructible ... ArgTypes>
//   bool operator==(nullptr_t, const function<R(ArgTypes...)>&);
//
// template <MoveConstructible R, MoveConstructible ... ArgTypes>
//   bool operator!=(const function<R(ArgTypes...)>&, nullptr_t);
//
// template <MoveConstructible  R, MoveConstructible ... ArgTypes>
//   bool operator!=(nullptr_t, const function<R(ArgTypes...)>&);

#include <functional>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator { // libcis: isolate file-scope helpers
int g(int) {return 0;}
} using namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator; // libcis


void test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator()
{
    {
    std::function<int(int)> f;
    assert(f == nullptr);
    assert(nullptr == f);
    f = g;
    assert(f != nullptr);
    assert(nullptr != f);
    }

  return;
}
