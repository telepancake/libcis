// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_default_a810642c).
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

// function();

#include <functional>
#include <cassert>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_default_a810642c { // libcis
int main(int, char**)
{
    std::function<int(int)> f;
    assert(!f);

  return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_default_a810642c (libcis)

