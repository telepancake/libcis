// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_trans_meta_trans_cv_remove_cv).
// main -> test_utilities_meta_meta_trans_meta_trans_cv_remove_cv; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// remove_cv

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_trans_meta_trans_cv_remove_cv { // libcis: isolate file-scope helpers
template <class T, class U>
void test_remove_cv_imp()
{
    ASSERT_SAME_TYPE(U, typename std::remove_cv<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U,        std::remove_cv_t<T>);
#endif
}

template <class T>
void test_remove_cv()
{
    test_remove_cv_imp<T, T>();
    test_remove_cv_imp<const T, T>();
    test_remove_cv_imp<volatile T, T>();
    test_remove_cv_imp<const volatile T, T>();
}
} using namespace libcis_ns_utilities_meta_meta_trans_meta_trans_cv_remove_cv; // libcis


void test_utilities_meta_meta_trans_meta_trans_cv_remove_cv()
{
    test_remove_cv<void>();
    test_remove_cv<int>();
    test_remove_cv<int[3]>();
    test_remove_cv<int&>();
    test_remove_cv<const int&>();
    test_remove_cv<int*>();
    test_remove_cv<const int*>();

  return;
}
