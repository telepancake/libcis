// AST-transferred from libc++ by tools/transfer.py (slug=utilities_tuple_tuple_tuple_tuple_helper_tuple_include_array).
// main -> test_utilities_tuple_tuple_tuple_tuple_helper_tuple_include_array; file-scope helpers isolated in anon namespace.
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <tuple>

// template <class... Types> class tuple;

// template <size_t I, class... Types>
// struct tuple_element<I, tuple<Types...> >
// {
//     typedef Ti type;
// };
//
//  LWG #2212 says that tuple_size and tuple_element must be
//     available after including <utility>

#include <array>
#include <type_traits>

#include "test_macros.h"

namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_helper_tuple_include_array { // libcis: isolate file-scope helpers
template <class T, std::size_t N, class U, std::size_t idx>
void test()
{
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<const T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<volatile T> >::value), "");
    static_assert((std::is_base_of<std::integral_constant<std::size_t, N>,
                                   std::tuple_size<const volatile T> >::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, T>::type, U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, const T>::type, const U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, volatile T>::type, volatile U>::value), "");
    static_assert((std::is_same<typename std::tuple_element<idx, const volatile T>::type, const volatile U>::value), "");
}
} using namespace libcis_ns_utilities_tuple_tuple_tuple_tuple_helper_tuple_include_array; // libcis


void test_utilities_tuple_tuple_tuple_tuple_helper_tuple_include_array()
{
    test<std::array<int, 5>, 5, int, 0>();
    test<std::array<int, 5>, 5, int, 1>();
    test<std::array<const char *, 4>, 4, const char *, 3>();
    test<std::array<volatile int, 4>, 4, volatile int, 3>();
    test<std::array<char *, 3>, 3, char *, 1>();
    test<std::array<char *, 3>, 3, char *, 2>();

  return;
}
