// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool_96c2e522).
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

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool_96c2e522 { // libcis
int g(int) {return 0;}

int main(int, char**)
{
    static_assert(std::is_constructible<bool, std::function<void()> >::value, "");
    static_assert(!std::is_convertible<std::function<void()>, bool>::value, "");

    {
    std::function<int(int)> f;
    assert(!f);
    f = g;
    assert(f);
    }

  return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool_96c2e522 (libcis)

