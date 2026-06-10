// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_trans_meta_trans_arr_remove_all_extents).
// main -> test_utilities_meta_meta_trans_meta_trans_arr_remove_all_extents; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// remove_all_extents

#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_trans_meta_trans_arr_remove_all_extents { // libcis: isolate file-scope helpers
enum Enum {zero, one_};

template <class T, class U>
void test_remove_all_extents()
{
    ASSERT_SAME_TYPE(U, typename std::remove_all_extents<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U,        std::remove_all_extents_t<T>);
#endif
}
} using namespace libcis_ns_utilities_meta_meta_trans_meta_trans_arr_remove_all_extents; // libcis


void test_utilities_meta_meta_trans_meta_trans_arr_remove_all_extents()
{
    test_remove_all_extents<int, int> ();
    test_remove_all_extents<const Enum, const Enum> ();
    test_remove_all_extents<int[], int> ();
    test_remove_all_extents<const int[], const int> ();
    test_remove_all_extents<int[3], int> ();
    test_remove_all_extents<const int[3], const int> ();
    test_remove_all_extents<int[][3], int> ();
    test_remove_all_extents<const int[][3], const int> ();
    test_remove_all_extents<int[2][3], int> ();
    test_remove_all_extents<const int[2][3], const int> ();
    test_remove_all_extents<int[1][2][3], int> ();
    test_remove_all_extents<const int[1][2][3], const int> ();

  return;
}
