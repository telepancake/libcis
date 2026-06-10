// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_trans_meta_trans_ptr_remove_pointer).
// main -> test_utilities_meta_meta_trans_meta_trans_ptr_remove_pointer; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// remove_pointer

#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_trans_meta_trans_ptr_remove_pointer { // libcis: isolate file-scope helpers
template <class T, class U>
void test_remove_pointer()
{
    ASSERT_SAME_TYPE(U, typename std::remove_pointer<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U, std::remove_pointer_t<T>);
#endif
}
} using namespace libcis_ns_utilities_meta_meta_trans_meta_trans_ptr_remove_pointer; // libcis


void test_utilities_meta_meta_trans_meta_trans_ptr_remove_pointer()
{
    test_remove_pointer<void, void>();
    test_remove_pointer<int, int>();
    test_remove_pointer<int[3], int[3]>();
    test_remove_pointer<int*, int>();
    test_remove_pointer<const int*, const int>();
    test_remove_pointer<int**, int*>();
    test_remove_pointer<int** const, int*>();
    test_remove_pointer<int*const * , int* const>();
    test_remove_pointer<const int** , const int*>();

    test_remove_pointer<int&, int&>();
    test_remove_pointer<const int&, const int&>();
    test_remove_pointer<int(&)[3], int(&)[3]>();
    test_remove_pointer<int(*)[3], int[3]>();
    test_remove_pointer<int*&, int*&>();
    test_remove_pointer<const int*&, const int*&>();

  return;
}
