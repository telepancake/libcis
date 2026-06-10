// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_comp_enum).
// main -> test_utilities_meta_meta_unary_meta_unary_comp_enum; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// enum

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_comp_enum { // libcis: isolate file-scope helpers
template <class T>
void test_enum_imp()
{
    static_assert(!std::is_reference<T>::value, "");
    static_assert(!std::is_arithmetic<T>::value, "");
    static_assert(!std::is_fundamental<T>::value, "");
    static_assert( std::is_object<T>::value, "");
    static_assert( std::is_scalar<T>::value, "");
    static_assert( std::is_compound<T>::value, "");
    static_assert(!std::is_member_pointer<T>::value, "");
}

template <class T>
void test_enum()
{
    test_enum_imp<T>();
    test_enum_imp<const T>();
    test_enum_imp<volatile T>();
    test_enum_imp<const volatile T>();
}

enum Enum {zero, one};
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_comp_enum; // libcis


void test_utilities_meta_meta_unary_meta_unary_comp_enum()
{
    test_enum<Enum>();

  return;
}
