// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_prop_is_trivially_copyable).
// main -> test_utilities_meta_meta_unary_meta_unary_prop_is_trivially_copyable; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_trivially_copyable

#include <type_traits>
#include <cassert>
#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_trivially_copyable { // libcis: isolate file-scope helpers
template <class T>
void test_is_trivially_copyable()
{
    static_assert( std::is_trivially_copyable<T>::value, "");
    static_assert( std::is_trivially_copyable<const T>::value, "");
    static_assert( std::is_trivially_copyable<volatile T>::value, "");
    static_assert( std::is_trivially_copyable<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_trivially_copyable_v<T>, "");
    static_assert( std::is_trivially_copyable_v<const T>, "");
    static_assert( std::is_trivially_copyable_v<volatile T>, "");
    static_assert( std::is_trivially_copyable_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_trivially_copyable()
{
    static_assert(!std::is_trivially_copyable<T>::value, "");
    static_assert(!std::is_trivially_copyable<const T>::value, "");
    static_assert(!std::is_trivially_copyable<volatile T>::value, "");
    static_assert(!std::is_trivially_copyable<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_trivially_copyable_v<T>, "");
    static_assert(!std::is_trivially_copyable_v<const T>, "");
    static_assert(!std::is_trivially_copyable_v<volatile T>, "");
    static_assert(!std::is_trivially_copyable_v<const volatile T>, "");
#endif
}

struct A
{
    int i_;
};

struct B
{
    int i_;
    ~B() {assert(i_ == 0);}
};

class C
{
public:
    C();
};
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_trivially_copyable; // libcis


void test_utilities_meta_meta_unary_meta_unary_prop_is_trivially_copyable()
{
    test_is_trivially_copyable<int> ();
    test_is_trivially_copyable<const int> ();
    test_is_trivially_copyable<A> ();
    test_is_trivially_copyable<const A> ();
    test_is_trivially_copyable<C> ();

    test_is_not_trivially_copyable<int&> ();
    test_is_not_trivially_copyable<const A&> ();
    test_is_not_trivially_copyable<B> ();

  return;
}
