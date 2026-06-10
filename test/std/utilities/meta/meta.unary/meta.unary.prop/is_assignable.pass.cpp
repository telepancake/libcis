// AST-transferred from libc++ by tools/transfer.py (slug=utilities_meta_meta_unary_meta_unary_prop_is_assignable).
// main -> test_utilities_meta_meta_unary_meta_unary_prop_is_assignable; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// type_traits

// is_assignable

#include <type_traits>
#include "test_macros.h"

namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_assignable { // libcis: isolate file-scope helpers
struct A
{
};

struct B
{
    void operator=(A);
};

template <class T, class U>
void test_is_assignable()
{
    static_assert(( std::is_assignable<T, U>::value), "");
#if TEST_STD_VER > 14
    static_assert(  std::is_assignable_v<T, U>, "");
#endif
}

template <class T, class U>
void test_is_not_assignable()
{
    static_assert((!std::is_assignable<T, U>::value), "");
#if TEST_STD_VER > 14
    static_assert( !std::is_assignable_v<T, U>, "");
#endif
}

struct D;

#if TEST_STD_VER >= 11
struct C
{
    template <class U>
    D operator,(U&&);
};

struct E
{
    C operator=(int);
};
#endif

template <typename T>
struct X { T t; };
} using namespace libcis_ns_utilities_meta_meta_unary_meta_unary_prop_is_assignable; // libcis


void test_utilities_meta_meta_unary_meta_unary_prop_is_assignable()
{
    test_is_assignable<int&, int&> ();
    test_is_assignable<int&, int> ();
    test_is_assignable<int&, double> ();
    test_is_assignable<B, A> ();
    test_is_assignable<void*&, void*> ();

#if TEST_STD_VER >= 11
    test_is_assignable<E, int> ();

    test_is_not_assignable<int, int&> ();
    test_is_not_assignable<int, int> ();
#endif
    test_is_not_assignable<A, B> ();
    test_is_not_assignable<void, const void> ();
    test_is_not_assignable<const void, const void> ();
    test_is_not_assignable<int(), int> ();

//  pointer to incomplete template type
    test_is_assignable<X<D>*&, X<D>*> ();

  return;
}
