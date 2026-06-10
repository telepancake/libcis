// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_badcall_bad_function_call).
// main -> test_utilities_function_objects_func_wrap_func_wrap_badcall_bad_function_call; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// Class bad_function_call

// class bad_function_call
//     : public exception
// {
// public:
//   // 20.7.16.1.1, constructor:
//   bad_function_call();
// };

#include <exception>
#include <functional>
#include <type_traits>

#include "test_macros.h"

void test_utilities_function_objects_func_wrap_func_wrap_badcall_bad_function_call()
{
    static_assert((std::is_base_of<std::exception, std::bad_function_call>::value), "");

  return;
}
