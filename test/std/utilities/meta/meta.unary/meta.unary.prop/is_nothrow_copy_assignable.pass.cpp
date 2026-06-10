// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_prop_is_nothrow_copy_assignable).
// main -> test_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_copy_assignable; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_nothrow_copy_assignable

#include <type_traits>
#include "test_macros.h"

#include "common.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_copy_assignable { // libcis: isolate file-scope helpers
template <class T>
void test_has_nothrow_assign()
{
    static_assert( std::is_nothrow_copy_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_nothrow_copy_assignable_v<T>, "");
#endif
}

template <class T>
void test_has_not_nothrow_assign()
{
    static_assert(!std::is_nothrow_copy_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_nothrow_copy_assignable_v<T>, "");
#endif
}
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_copy_assignable; // libcis


void test_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_copy_assignable()
{
    test_has_nothrow_assign<int&>();
    test_has_nothrow_assign<Union>();
    test_has_nothrow_assign<Empty>();
    test_has_nothrow_assign<int>();
    test_has_nothrow_assign<double>();
    test_has_nothrow_assign<int*>();
    test_has_nothrow_assign<const int*>();
    test_has_nothrow_assign<NotEmpty>();
    test_has_nothrow_assign<bit_zero>();

// TODO: enable the test for GCC once https://gcc.gnu.org/bugzilla/show_bug.cgi?id=106611 is resolved
#if TEST_STD_VER >= 11 && !defined(TEST_COMPILER_GCC)
    test_has_not_nothrow_assign<TrivialNotNoexcept>();
#endif
    test_has_not_nothrow_assign<const int>();
    test_has_not_nothrow_assign<void>();
    test_has_not_nothrow_assign<A>();


  return;
}
