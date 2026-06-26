// transferred+adapted from libc++ by tools/transfer.py (slug=containers_sequences_array_array_tuple_tuple_element_c6e227a6).
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <array>

// tuple_element<I, array<T, N> >::type

#include <array>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_containers_sequences_array_array_tuple_tuple_element_c6e227a6 { // libcis
template <class T>
void test()
{
    {
    typedef T Exp;
    typedef std::array<T, 3> C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
    {
    typedef T const Exp;
    typedef std::array<T, 3> const C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
    {
    typedef T volatile Exp;
    typedef std::array<T, 3> volatile C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
    {
    typedef T const volatile Exp;
    typedef std::array<T, 3> const volatile C;
    static_assert((std::is_same<typename std::tuple_element<0, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<1, C>::type, Exp>::value), "");
    static_assert((std::is_same<typename std::tuple_element<2, C>::type, Exp>::value), "");
    }
}

int main(int, char**)
{
    test<double>();
    test<int>();

  return 0;

    return 0;
}
} // libcis_ns_containers_sequences_array_array_tuple_tuple_element_c6e227a6 (libcis)

