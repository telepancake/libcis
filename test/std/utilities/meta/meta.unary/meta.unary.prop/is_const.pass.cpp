// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_prop_is_const).
// main -> test_utilities_meta_meta_unary_meta_unary_prop_is_const; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_const

#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_const { // libcis: isolate file-scope helpers
template <class T>
void test_is_const()
{
    static_assert(!std::is_const<T>::value, "");
    static_assert( std::is_const<const T>::value, "");
    static_assert(!std::is_const<volatile T>::value, "");
    static_assert( std::is_const<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_const_v<T>, "");
    static_assert( std::is_const_v<const T>, "");
    static_assert(!std::is_const_v<volatile T>, "");
    static_assert( std::is_const_v<const volatile T>, "");
#endif
}

struct A;
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_const; // libcis
 // incomplete

void test_utilities_meta_meta_unary_meta_unary_prop_is_const()
{
    test_is_const<void>();
    test_is_const<int>();
    test_is_const<double>();
    test_is_const<int*>();
    test_is_const<const int*>();
    test_is_const<char[3]>();
    test_is_const<char[]>();

    test_is_const<A>();

    static_assert(!std::is_const<int&>::value, "");
    static_assert(!std::is_const<const int&>::value, "");

  return;
}
