// AST-transferred from libc++ by tools/transfer.py (slug=utilities_function_objects_logical_operations_transparent).
// main -> test_utilities_function_objects_logical_operations_transparent; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11
#include <functional>
#include <string>

#include "test_macros.h"

namespace libcis_ns_utilities_function_objects_logical_operations_transparent { // libcis: isolate file-scope helpers
template <class T>
struct is_transparent
{
private:
    struct two {char lx; char lxx;};
    template <class U> static two test(...);
    template <class U> static char test(typename U::is_transparent* = 0);
public:
    static const bool value = sizeof(test<T>(0)) == 1;
};
} using namespace libcis_ns_utilities_function_objects_logical_operations_transparent; // libcis



void test_utilities_function_objects_logical_operations_transparent()
{
    static_assert ( !is_transparent<std::logical_and<int>>::value, "" );
    static_assert ( !is_transparent<std::logical_and<std::string>>::value, "" );
    static_assert (  is_transparent<std::logical_and<void>>::value, "" );
    static_assert (  is_transparent<std::logical_and<>>::value, "" );

    static_assert ( !is_transparent<std::logical_or<int>>::value, "" );
    static_assert ( !is_transparent<std::logical_or<std::string>>::value, "" );
    static_assert (  is_transparent<std::logical_or<void>>::value, "" );
    static_assert (  is_transparent<std::logical_or<>>::value, "" );

    static_assert ( !is_transparent<std::logical_not<int>>::value, "" );
    static_assert ( !is_transparent<std::logical_not<std::string>>::value, "" );
    static_assert (  is_transparent<std::logical_not<void>>::value, "" );
    static_assert (  is_transparent<std::logical_not<>>::value, "" );

    return;
}
