// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_prop_query_alignment_of).
// main -> test_utilities_meta_meta_unary_prop_query_alignment_of; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// alignment_of

#include <type_traits>
#include <cstdint>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_prop_query_alignment_of { // libcis: isolate file-scope helpers
template <class T, unsigned A>
void test_alignment_of()
{
    const unsigned AlignofResult = TEST_ALIGNOF(T);
    static_assert( AlignofResult == A, "Golden value does not match result of alignof keyword");
    static_assert( std::alignment_of<T>::value == AlignofResult, "");
    static_assert( std::alignment_of<T>::value == A, "");
    static_assert( std::alignment_of<const T>::value == A, "");
    static_assert( std::alignment_of<volatile T>::value == A, "");
    static_assert( std::alignment_of<const volatile T>::value == A, "");
#if TEST_STD_VER > 14
    static_assert( std::alignment_of_v<T> == A, "");
    static_assert( std::alignment_of_v<const T> == A, "");
    static_assert( std::alignment_of_v<volatile T> == A, "");
    static_assert( std::alignment_of_v<const volatile T> == A, "");
#endif
}

class Class
{
public:
    ~Class();
};
} using namespace libcis_ns_utilities_meta_meta_unary_prop_query_alignment_of; // libcis


void test_utilities_meta_meta_unary_prop_query_alignment_of()
{
    test_alignment_of<int&, 4>();
    test_alignment_of<Class, 1>();
    test_alignment_of<int*, sizeof(std::intptr_t)>();
    test_alignment_of<const int*, sizeof(std::intptr_t)>();
    test_alignment_of<char[3], 1>();
    test_alignment_of<int, 4>();
    test_alignment_of<double, TEST_ALIGNOF(double)>();
    test_alignment_of<bool, 1>();
    test_alignment_of<unsigned, 4>();

  return;
}
