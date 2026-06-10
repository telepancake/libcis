// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_addressof).
// main -> test_utilities_function_objects_func_wrap_func_wrap_func_addressof; file-scope helpers isolated in anon namespace.
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

// Make sure we can use std::function with a type that has a hostile overload
// of operator&().

#include <functional>
#include <cassert>

#include "operator_hijacker.h"

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_addressof { // libcis: isolate file-scope helpers
struct TrapAddressof : operator_hijacker {
    int operator()() const { return 1; }
};
} using namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_addressof; // libcis


void test_utilities_function_objects_func_wrap_func_wrap_func_addressof() {
    std::function<int()> f = TrapAddressof();
    assert(f() == 1);
    return;
}
