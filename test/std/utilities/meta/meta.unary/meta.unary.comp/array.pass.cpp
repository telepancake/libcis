// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_comp_array).
// main -> test_utilities_meta_meta_unary_meta_unary_comp_array; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// array

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_comp_array { // libcis: isolate file-scope helpers
template <class T>
void test_array_imp()
{
    static_assert(!std::is_reference<T>::value, "");
    static_assert(!std::is_arithmetic<T>::value, "");
    static_assert(!std::is_fundamental<T>::value, "");
    static_assert( std::is_object<T>::value, "");
    static_assert(!std::is_scalar<T>::value, "");
    static_assert( std::is_compound<T>::value, "");
    static_assert(!std::is_member_pointer<T>::value, "");
}

template <class T>
void test_array()
{
    test_array_imp<T>();
    test_array_imp<const T>();
    test_array_imp<volatile T>();
    test_array_imp<const volatile T>();
}

typedef char array[3];
typedef const char const_array[3];
typedef char incomplete_array[];

class incomplete_type;
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_comp_array; // libcis


void test_utilities_meta_meta_unary_meta_unary_comp_array()
{
    test_array<array>();
    test_array<const_array>();
    test_array<incomplete_array>();
    test_array<incomplete_type[]>();

  return;
}
