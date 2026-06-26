// transferred+adapted from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator_f3d68e55).
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

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator_f3d68e55 { // libcis
int g(int) {return 0;}

int main(int, char**)
{
    {
    std::function<int(int)> f;
    assert(f == nullptr);
    assert(nullptr == f);
    f = g;
    assert(f != nullptr);
    assert(nullptr != f);
    }

  return 0;

    return 0;
}
} // libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator_f3d68e55 (libcis)

