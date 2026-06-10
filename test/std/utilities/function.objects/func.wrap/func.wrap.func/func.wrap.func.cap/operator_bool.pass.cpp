// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool).
// main -> test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool; file-scope helpers isolated in anon namespace.
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

// explicit operator bool() const

#include <functional>
#include <cassert>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool { // libcis: isolate file-scope helpers
int g(int) {return 0;}
} using namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool; // libcis


void test_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool()
{
    static_assert(std::is_constructible<bool, std::function<void()> >::value, "");
    static_assert(!std::is_convertible<std::function<void()>, bool>::value, "");

    {
    std::function<int(int)> f;
    assert(!f);
    f = g;
    assert(f);
    }

  return;
}
