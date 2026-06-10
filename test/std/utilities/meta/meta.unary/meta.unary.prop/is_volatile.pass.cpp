// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_prop_is_volatile).
// main -> test_utilities_meta_meta_unary_meta_unary_prop_is_volatile; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_volatile

#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_volatile { // libcis: isolate file-scope helpers
template <class T>
void test_is_volatile()
{
    static_assert(!std::is_volatile<T>::value, "");
    static_assert(!std::is_volatile<const T>::value, "");
    static_assert( std::is_volatile<volatile T>::value, "");
    static_assert( std::is_volatile<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_volatile_v<T>, "");
    static_assert(!std::is_volatile_v<const T>, "");
    static_assert( std::is_volatile_v<volatile T>, "");
    static_assert( std::is_volatile_v<const volatile T>, "");
#endif
}

struct A;
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_volatile; // libcis
 // incomplete

void test_utilities_meta_meta_unary_meta_unary_prop_is_volatile()
{
    test_is_volatile<void>();
    test_is_volatile<int>();
    test_is_volatile<double>();
    test_is_volatile<int*>();
    test_is_volatile<const int*>();
    test_is_volatile<char[3]>();
    test_is_volatile<char[]>();

    test_is_volatile<A>();

    static_assert(!std::is_volatile<int&>::value, "");
    static_assert(!std::is_volatile<volatile int&>::value, "");

  return;
}
