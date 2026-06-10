// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_prop_query_rank).
// main -> test_utilities_meta_meta_unary_prop_query_rank; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// rank

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_prop_query_rank { // libcis: isolate file-scope helpers
template <class T, unsigned A>
void test_rank()
{
    static_assert( std::rank<T>::value == A, "");
    static_assert( std::rank<const T>::value == A, "");
    static_assert( std::rank<volatile T>::value == A, "");
    static_assert( std::rank<const volatile T>::value == A, "");
#if TEST_STD_VER > 14
    static_assert( std::rank_v<T> == A, "");
    static_assert( std::rank_v<const T> == A, "");
    static_assert( std::rank_v<volatile T> == A, "");
    static_assert( std::rank_v<const volatile T> == A, "");
#endif
}

class Class
{
public:
    ~Class();
};
} using namespace libcis_ns_utilities_meta_meta_unary_prop_query_rank; // libcis


void test_utilities_meta_meta_unary_prop_query_rank()
{
    test_rank<void, 0>();
    test_rank<int&, 0>();
    test_rank<Class, 0>();
    test_rank<int*, 0>();
    test_rank<const int*, 0>();
    test_rank<int, 0>();
    test_rank<double, 0>();
    test_rank<bool, 0>();
    test_rank<unsigned, 0>();

    test_rank<char[3], 1>();
    test_rank<char[][3], 2>();
    test_rank<char[][4][3], 3>();

  return;
}
