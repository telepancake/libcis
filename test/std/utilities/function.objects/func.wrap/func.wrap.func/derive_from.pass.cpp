// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_func_wrap_func_wrap_func_derive_from).
// main -> test_utilities_function_objects_func_wrap_func_wrap_func_derive_from; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// <functional>

// See https://llvm.org/PR20002

#include <functional>
#include <memory>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_derive_from { // libcis: isolate file-scope helpers
using Fn = std::function<void()>;
struct S : public std::function<void()> { using function::function; };
} using namespace libcis_ns_utilities_function_objects_func_wrap_func_wrap_func_derive_from; // libcis


void test_utilities_function_objects_func_wrap_func_wrap_func_derive_from() {
    S s( [](){} );
    S f1( s );
#if TEST_STD_VER <= 14
    S f2(std::allocator_arg, std::allocator<int>{}, s);
#endif

  return;
}
