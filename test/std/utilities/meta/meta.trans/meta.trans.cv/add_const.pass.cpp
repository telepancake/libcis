// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_trans_meta_trans_cv_add_const).
// main -> test_utilities_meta_meta_trans_meta_trans_cv_add_const; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// add_const

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_trans_meta_trans_cv_add_const { // libcis: isolate file-scope helpers
template <class T, class U>
void test_add_const_imp()
{
    ASSERT_SAME_TYPE(const U, typename std::add_const<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(const U,        std::add_const_t<T>);
#endif
}

template <class T>
void test_add_const()
{
    test_add_const_imp<T, const T>();
    test_add_const_imp<const T, const T>();
    test_add_const_imp<volatile T, volatile const T>();
    test_add_const_imp<const volatile T, const volatile T>();
}
} using namespace libcis_ns_utilities_meta_meta_trans_meta_trans_cv_add_const; // libcis


void test_utilities_meta_meta_trans_meta_trans_cv_add_const()
{
    test_add_const<void>();
    test_add_const<int>();
    test_add_const<int[3]>();
    test_add_const<int&>();
    test_add_const<const int&>();
    test_add_const<int*>();
    test_add_const<const int*>();

  return;
}
