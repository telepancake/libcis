// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_prop_has_virtual_destructor).
// main -> test_utilities_meta_meta_unary_meta_unary_prop_has_virtual_destructor; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// has_virtual_destructor

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_has_virtual_destructor { // libcis: isolate file-scope helpers
template <class T>
void test_has_virtual_destructor()
{
    static_assert( std::has_virtual_destructor<T>::value, "");
    static_assert( std::has_virtual_destructor<const T>::value, "");
    static_assert( std::has_virtual_destructor<volatile T>::value, "");
    static_assert( std::has_virtual_destructor<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::has_virtual_destructor_v<T>, "");
    static_assert( std::has_virtual_destructor_v<const T>, "");
    static_assert( std::has_virtual_destructor_v<volatile T>, "");
    static_assert( std::has_virtual_destructor_v<const volatile T>, "");
#endif
}

template <class T>
void test_has_not_virtual_destructor()
{
    static_assert(!std::has_virtual_destructor<T>::value, "");
    static_assert(!std::has_virtual_destructor<const T>::value, "");
    static_assert(!std::has_virtual_destructor<volatile T>::value, "");
    static_assert(!std::has_virtual_destructor<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::has_virtual_destructor_v<T>, "");
    static_assert(!std::has_virtual_destructor_v<const T>, "");
    static_assert(!std::has_virtual_destructor_v<volatile T>, "");
    static_assert(!std::has_virtual_destructor_v<const volatile T>, "");
#endif
}

class Empty
{
};

class NotEmpty
{
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
    virtual ~Abstract() = 0;
};

struct A
{
    ~A();
};
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_has_virtual_destructor; // libcis


void test_utilities_meta_meta_unary_meta_unary_prop_has_virtual_destructor()
{
    test_has_not_virtual_destructor<void>();
    test_has_not_virtual_destructor<A>();
    test_has_not_virtual_destructor<int&>();
    test_has_not_virtual_destructor<Union>();
    test_has_not_virtual_destructor<Empty>();
    test_has_not_virtual_destructor<int>();
    test_has_not_virtual_destructor<double>();
    test_has_not_virtual_destructor<int*>();
    test_has_not_virtual_destructor<const int*>();
    test_has_not_virtual_destructor<char[3]>();
    test_has_not_virtual_destructor<char[]>();
    test_has_not_virtual_destructor<bit_zero>();

    test_has_virtual_destructor<Abstract>();
    test_has_virtual_destructor<NotEmpty>();

  return;
}
